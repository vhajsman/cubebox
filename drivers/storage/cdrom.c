#include "cubebox.h"
#include "ata/ata.h"

#define CDROM_REG_DATA					0
#define CDROM_REG_ERROR					1
#define CDROM_REG_SECTOR_COUNT			2
#define CDROM_REG_LBA_LOW				3
#define CDROM_REG_LBA_MID				4
#define CDROM_REG_LBA_HIGH				5
#define CDROM_REG_DRIVE_SELECT			6
#define CDROM_REG_COMMAND				7

#define CDROM_CMD_TEST_UNIT_READY		0x00
#define CDROM_CMD_REQUEST_SENSE			0x03
#define CDROM_CMD_FORMAT_UNIT			0x04
#define CDROM_CMD_INQUIRY				0x12
#define CDROM_CMD_PREVENT_REMOVAL		0x1E
#define CDROM_CMD_READ_FORMAT_C			0x23
#define CDROM_CMD_READ_CAPACITY			0x25
#define CDROM_CMD_READ10				0x28
#define CDROM_CMD_WRITE10				0x2A
#define CDROM_CMD_SEEK10				0x2B
#define CDROM_CMD_WRITE_VERIFY10		0x2E
#define CDROM_CMD_VERIFY10				0x2F
#define CDROM_CMD_SYNC_CACHE			0x35
#define CDROM_CMD_WRITE_BUFFER			0x3B
#define CDROM_CMD_READ_BUFFER			0x3C
#define CDROM_CMD_READ_TOC				0x43
#define CDROM_CMD_READ_PMA				0x43
#define CDROM_CMD_READ_ATIP				0x43
#define CDROM_CMD_GET_CONFIG			0x46
#define CDROM_CMD_GET_EVENT_STATUS		0x4A
#define CDROM_CMD_READ_DISC_INFO		0x51
#define CDROM_CMD_READ_TRACK_INFO		0x52
#define CDROM_CMD_RESERVE_TRACK			0x53
#define CDROM_CMD_SEND_OPC				0x54
#define CDROM_CMD_MODE_SELECT10			0x55
#define CDROM_CMD_REPAIR_TRACK			0x58
#define CDROM_CMD_MODE_SENSE			0x5A
#define CDROM_CMD_CLOSE_SECTION			0x5B
#define CDROM_CMD_READ_BUFFER_C			0x5C
#define CDROM_CMD_SEND_CUE_SHEET		0x5D
#define CDROM_CMD_REPORT_LUNS			0xA0
#define CDROM_CMD_BLANK					0x1A
#define CDROM_CMD_SEC_PROTOCOL_IN		0xA2
#define CDROM_CMD_SEND_KEY				0xA3
#define CDROM_CMD_REPORT_KEY			0xA4
#define CDROM_CMD_LOAD_MEDIUM			0xA6
#define CDROM_CMD_UNLOAD_MEDIUM			0xA6
#define CDROM_CMD_SET_READ_AHEAD		0xA7
#define CDROM_CMD_READ12				0xA8
#define CDROM_CMD_WRITE12				0xAA
#define CDROM_CMD_READ_SET_NO			0xAB
#define CDROM_CMD_SERVICE_ACTION_IN12	0x01
#define CDROM_CMD_GET_PERFORMANCE		0xAC
#define CDROM_CMD_READ_STRUCTURE		0xAD
#define CDROM_CMD_SEC_PROTOCOL_OUT		0xB5
#define CDROM_CMD_SET_STREAMING			0xB6
#define CDROM_CMD_READ_MSF				0xB9
#define CDROM_CMD_SET_SPEED				0xBB
#define CDROM_CMD_MECHANISM_STATUS		0xBD
#define CDROM_CMD_READ					0xBE
#define CDROM_SEND_DISC_STRUCTURE		0xBF

#define _SECTOR_COUNT 2
#define _LBA_LOW 3
#define _LBA_MID 4
#define _LBA_HIGH 5
#define _DRIVE_SELECT 6
#define _CMD_REG 7

static __inline void insw(uint16_t __port, void* __buf, unsigned long __n) {
	__asm__ __volatile__("cld; rep; insw"
		: "+D"(__buf), "+c"(__n)
		: "d"(__port));
}

static __inline__ void outsw(uint16_t __port, const void* __buf, unsigned long __n) {
	__asm__ __volatile__("cld; rep; outsw"
		: "+S"(__buf), "+c"(__n)
		: "d"(__port));
}

int cdrom_read(uint16_t port, bool slave, uint32_t lba, uint32_t sectors, uint16_t* buffer) {
	volatile atapi_packet packet = {
		0xA8, 0,

		(lba >> 0x18) & 0xFF,
		(lba >> 0x10) & 0xFF,
		(lba >> 0x08) & 0xFF,
		(lba >> 0x00) & 0xFF,

		(sectors >> 0x18) & 0xFF,
		(sectors >> 0x10) & 0xFF,
		(sectors >> 0x08) & 0xFF,
		(sectors >> 0x00) & 0xFF,

		0, 0
	};

	outb(port + CDROM_REG_SECTOR_COUNT, 0xA0 & (slave << 4));
	ata_iowait(port);

	outb(port + CDROM_REG_ERROR, 0x00);
	outb(port + CDROM_REG_LBA_MID, 2048 & 0xFF);
	outb(port + CDROM_REG_LBA_HIGH, 2048 >> 8);
	outb(port + CDROM_REG_COMMAND, 0xA0);

	ata_iowait();

	uint8_t status;
	int size;

	for (uint32_t i = 0; i < sectors && i < MAX_ITERATOR; i++) {
		while (1) {
			status = inb(port + CDROM_REG_COMMAND);

			if (status & 0x01)
				return 1;
			if (!(status & 0x80) && (status & 0x08))
				break;
		}

		size = inb(port + CDROM_REG_LBA_HIGH) << 8 | inb(port + CDROM_REG_LBA_MID);
		insw(port + CDROM_REG_DATA, (uint16_t*) ((uint8_t*) buffer + i * 0x800), size / 2);
	}
}