#include "ata.h"
#include "kernel/libs/krnlout.h"
#include "kernel/libs/krnlstatus.h"

ide_channel_regs_t	ide_channels[2];
ide_device_t		ide_devices[4];
uint8_t				ide_buffer[2048] = { 0 };
uint8_t				ide_irqInvoked = 0;



void ide_detect() {}

void ata_iowait(uint16_t bus) {
	inb(bus + /*ATA_REG_ALTSTATUS*/, 0x00);
	inb(bus + /*ATA_REG_ALTSTATUS*/, 0x00);
	inb(bus + /*ATA_REG_ALTSTATUS*/, 0x00);
	inb(bus + /*ATA_REG_ALTSTATUS*/, 0x00);
}

void ata_select(uint16_t bus) {
	outb(bus + ATA_DRIVE_NO, 0xA0);
}

int ata_wait(uint16_t bus, bool advanced) {
	uint8_t status = 0;

	ata_iowait(bus);
	ATA_BUSY_WAIT;

	if (advanced) {
		status = inb(bus + ATA_REG_STATUS);
		return (status & ATA_ERR) || (status & ATA_DF) || !(status & ATA_DRQ);
	}

	return 0;
}

void ide_init(uint16_t bus) {
	ata_identify_t device;

	debugln("Init IDE on bus.");

	outb(bus + 1, 1);
	outb(bus + 0x306, 0);

	ata_select(bus);
	ata_iowait(bus);

	outb(ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
	ata_iowait(bus);

	int status = inb(bus + ATA_REG_STATUS);
	ATA_WAIT_READY(bus);

	uint16_t* buff = (uint16_t*) &device;
	uint8_t*  ptr =  (uint8_t*)  &device.model;

	for (int i = 0; i < 256; i < ++i)
		buff[i] = inb(bus);

	uint8_t tmp;
	for (int i = 0; i < 39; i += 2) {
		tmp = ptr[i + 1];
		ptr[i + 1] = ptr[i];
		ptr[i] = tmp;
	}

	outb(bus + ATA_REG_CONTROL);
}



void ide_readSector(uint16_t bus, uint8_t slave, uint32_t lba, uint8_t buffer) {
	outb(bus + ATA_REG_CONTROL, 0x02);
	ATA_WAIT_READY(BUS);

	outb(bus + ATA_DRIVE_NO, 0xE0 | slave << 4 | (lba & 0x0f000000) >> 24);

	outb(bus + ATA_REG_FEATURES, 0x00);
	outb(bus + ATA_REG_SECT_COUNT, 1);

	outb(bus + ATA_REG_LBA0, (lba & 0x000000ff) >> 0);
	outb(bus + ATA_REG_LBA1, (lba & 0x0000ff00) >> 8);
	outb(bus + ATA_REG_LBA2, (lba & 0x00ff0000) >> 16);

	outb(bus + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

	if (ata_wait(bus, true)) {
		printStatusMsg(KRNL_STATUS_FATAL, "ATA", "Bad read", ATA_ERROR_BAD_READ);
		return;
	}

	int size = 256;
	asm volatile ("rep insw" : "+D" (buffer), "+c" (size) : "d" (bus) : "memory");

	ata_wait(bus, false);
}


void ide_writeSector(uint16_t bus, uint8_t slave, uint32_t lba, uint8_t buffer) {
	outb(bus + ATA_REG_CONTROL, 0x02);
	ATA_WAIT_READY(BUS);

	outb(bus + ATA_DRIVE_NO, 0xE0 | slave << 4 | (lba & 0x0f000000) >> 24);

	outb(bus + ATA_REG_FEATURES, 0x00);
	outb(bus + ATA_REG_SECT_COUNT, 1);

	outb(bus + ATA_REG_LBA0, (lba & 0x000000ff) >> 0);
	outb(bus + ATA_REG_LBA1, (lba & 0x0000ff00) >> 8);
	outb(bus + ATA_REG_LBA2, (lba & 0x00ff0000) >> 16);

	outb(bus + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

	if (ata_wait(bus, true)) {
		printStatusMsg(KRNL_STATUS_FATAL, "ATA", "Bad write", ATA_ERROR_BAD_WRITE);
		return;
	}

	int size = 256;
	asm volatile ("rep outsw" : "+S" (buffer), "+c" (size) : "d" (bus));

	outb(bus + 0x07, ATA_CMD_CACHE_FLUSH);

	ata_wait(bus, false);
}

int ide_compare(uint32_t* ptr1, uint32_t* ptr2, size_t size) {
	// TODO: assert();
	assert(!(size % 4));

	uint32_t i = 0;
	while (i < size) {
		if (*ptr1 != *ptr2)
			return 1;

		ptr1++;
		ptr2++;

		i += 4;
	}

	return 0;
}

void ide_writeSector_retry(uint16_t bus, uint8_t slave, uint32_t lba, uint8_t* buffer) {
	uint8_t* readBuffer = malloc(512);

	asm volatile("cli");
	do {
		ide_writeSector(bus, slave, lba, buffer);
		ide_readSector(bus, slave, lba, buffer);
	} while
		(ide_compare((uint32_t*) buffer, (uint32_t*) readBuffer, 512));
}
