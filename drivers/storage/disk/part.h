#ifndef _PART_H
#define _PART_H

#include "cubebox.h"

typedef struct {
	uint8_t  status;
	uint8_t  chs_first_sector[3];
	uint8_t  type;
	uint8_t  chs_last_sector[3];
	uint32_t lba_first_sector;
	uint32_t sector_count;
} partition_t;

typedef struct {
	uint8_t     boostrap[446];
	partition_t partitions[4];
	uint8_t     signature[2];
} _/*_attribute__((packed))*/ mbr_t;

#endif
