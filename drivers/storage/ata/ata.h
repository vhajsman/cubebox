#ifndef _ATA_H
#define _ATA_H

#include "cubebox.h"
#include "drivers/part.h"

#define IDE_ATA        0x00
#define IDE_ATAPI      0x01

#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01

#define ATA_PRIMARY    0x00
#define ATA_SECONDARY  0x01

#define ATA_ERROR_BAD_READ	601
#define ATA_ERROR_BAD_WRITE 602

/*
	--- REGISTERS ---
	The values in these tables are relative to the so-called port base;
	a port offset of 1 from the I/O port base actually refers to port 0x1F1 (0x1F0 + 1 = 0x1F1). 
	This is done because the base port may vary depending on the hardware in any given system. 
	Also, some of these I/O ports map to different registers based on whether they are being 
	read from or written to.

		--- FROM IO BASE ---

		Offset	Direction		Function					Description														Param. size LBA28/LBA48
		----------------------------------------------------------------------------------------------------------------------------------------------------
		0		R/W			Data Register				Read/Write PIO data bytes											16-bit / 16-bit
		1		R			Error Register			Used to retrieve any error generated by the last ATA command executed.	8-bit / 16-bit
		1		W			Features Register			Used to control command specific interface features.					8-bit / 16-bit
		2		R/W			Sector Count Register		Number of sectors to read/write (0 is a special value).				8-bit / 16-bit
		3		R/W			Sector Number Register		This is CHS / LBA28 / LBA48 specific.								8-bit / 16-bit
		4		R/W			Cylinder Low Register		Partial Disk Sector address.										8-bit / 16-bit
		5		R/W			Cylinder High Register		Partial Disk Sector address.										8-bit / 16-bit
		6		R/W			Drive / Head Register		Used to select a drive and/or head. Supports extra address/flag bits.	8-bit / 8-bit
		7		R			Status Register			Used to read the current status.									8-bit / 8-bit
		7		W			Command Register			Used to send ATA commands to the device.								8-bit / 8-bit

		--- FROM CONTROL BASE --- 

		Offset	Direction		Function					Description														Param. size LBA28/LBA48
		----------------------------------------------------------------------------------------------------------------------------------------------------
		0		R			Alternate Status Register	A duplicate of the Status Register which does not affect interrupts.	8-bit / 8-bit
		0		W			Device Control Register	Used to reset the bus or enable/disable interrupts.					8-bit / 8-bit
		1		R			Drive Address Register		Provides drive select and head select information.					8-bit / 8-bit
*/

#define ATA_REG_DATA			0
#define ATA_REG_ERROR			1
#define ATA_REG_FEATURES		1
#define ATA_REG_SECT_COUNT		2
#define ATA_REG_SECT_INDEX		3
#define ATA_REG_CYLINDER_LOW	4
#define ATA_REG_CYLINDER_HIGH	5
#define ATA_REG_DRIVE			6
#define ATA_REG_HEAD			6
#define ATA_REG_STATUS			7
#define ATA_REG_COMMAND			7
#define ATA_REG_CONTROL			0x0C

	// --- ERROR REGISTER ---
	#define ATA_ADDR_MARK_NOT_F		0
	#define ATA_TRACK0_NOT_F		1
	#define ATA_ABORT				2
	#define ATA_MEDIA_CHG_REQ		3
	#define ATA_ID_NOT_F			4
	#define ATA_MEDIA_CHANGED		5
	#define ATA_UNCORRECTABLE		6
	#define ATA_BAD_BLOCK			7

	// --- DRIVE/HEAD REGISTER ---
	#define ATA_CHS0				0
	#define ATA_CHS1				1
	#define ATA_CHS2				2
	#define ATA_CHS3				3
	#define ATA_DRIVE_NO			4
	#define ATA_LBA					6

	// --- STATUS REGISTER ---
	#define	ATA_ERR					0
	#define	ATA_IDX					1
	#define	ATA_CORR				2
	#define	ATA_DRQ					3
	#define	ATA_SRV					4
	#define	ATA_DF					5
	#define	ATA_RDY					6
	#define	ATA_BSY					7

	// --- DEVICE CONTROL REGISTER ---
	#define ATA_STOP_IRQ			1
	#define ATA_SW_RESET			2
	#define ATA_HOB					7

	// --- DRIVE ADDRESS REGISTER ---
	#define ATA_DS0					0
	#define ATA_DS1					1
	#define ATA_HS0					2
	#define ATA_HS1					3
	#define ATA_HS2					4
	#define ATA_HS3					5
	#define ATA_WTG					6

	#define ATA_REG_LBA0			3
	#define ATA_REG_LBA1			4
	#define ATA_REG_LBA2			5

// --- COMMANDS ---
#define ATA_CMD_READ_PIO			0x20
#define ATA_CMD_READ_PIO_EXT		0x24
#define ATA_CMD_READ_DMA			0xC8
#define ATA_CMD_READ_DMA_EXT		0x25
#define ATA_CMD_WRITE_PIO			0x30
#define ATA_CMD_WRITE_PIO_EXT		0x34
#define ATA_CMD_WRITE_DMA			0xCA
#define ATA_CMD_WRITE_DMA_EXT		0x35
#define ATA_CMD_CACHE_FLUSH			0xE7
#define ATA_CMD_CACHE_FLUSH_EXT		0xEA
#define ATA_CMD_PACKET				0xA0
#define ATA_CMD_IDENTIFY_PACKET		0xA1
#define ATA_CMD_IDENTIFY			0xEC

/*
#define ATAPI_CMD_READ					0xA8
#define ATAPI_CMD_EJECT				0x1B
*/


#define ATA_IDENT_DEVICETYPE		0
#define ATA_IDENT_CYLINDERS			2
#define ATA_IDENT_HEADS				6
#define ATA_IDENT_SECTORS			12
#define ATA_IDENT_SERIAL			20
#define ATA_IDENT_MODEL				54
#define ATA_IDENT_CAPABILITIES		98
#define ATA_IDENT_FIELDVALID		106
#define ATA_IDENT_MAX_LBA			120
#define ATA_IDENT_COMMAND_SETS		164
#define ATA_IDENT_MAX_LBA_EXT		200

#define ATA_READ					0x00
#define ATA_WRITE					0x01


#define ATA_BUSY_WAIT		\
	while ((status = inb(bus + ATA_REG_STATUS)) & ATA_BSY)
#define ATA_WAIT_READY(BUS)	\
	while (inb(bus + ATA_REG_STATUS) & ATA_BSY)


typedef struct {
	uint16_t base;
	uint16_t ctrl;
	uint16_t bmide;
	uint16_t nien;
} ide_channel_regs_t;

typedef struct {
	uint8_t  reserved;
	uint8_t  channel;
	uint8_t  drive;
	uint16_t type;
	uint16_t signature;
	uint16_t capabilities;
	uint32_t command_sets;
	uint32_t size;
	uint8_t  model[41];
} ide_device_t;

/*
typedef struct {
	uint8_t  status;
	uint8_t  chs_first_sector[3];
	uint8_t  type;
	uint8_t  chs_last_sector[3];
	uint32_t lba_first_sector;
	uint32_t sector_count;
} partition_t;
*/


typedef struct {
	uint16_t flags;
	uint16_t unused1[9];
	char     serial[20];
	uint16_t unused2[3];
	char     firmware[8];
	char     model[40];
	uint16_t sectors_per_int;
	uint16_t unused3;
	uint16_t capabilities[2];
	uint16_t unused4[2];
	uint16_t valid_ext_data;
	uint16_t unused5[5];
	uint16_t size_of_rw_mult;
	uint32_t sectors_28;
	uint16_t unused6[38];
	uint64_t sectors_48;
	uint16_t unused7[152];
} ata_identify_t /*__attribute__((packed))*/;

mbr_t mbr;

void ata_iowait(uint16_t bus)

typedef uint8_t		atapi_packet[12] = {
	0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#endif
