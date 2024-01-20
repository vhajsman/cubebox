#ifndef _STORAGE_H
#define _STORAGE_H

#include "part.h"

typedef enum {
	powerOff = 1,
	powerOn,
	reset,

} storage_op;

struct storageDevice {
	mbr_t partitionTable;
	int type;

	uint16_t base;
};

typedef storageDevice_t;

uint32_t storage_do(storageDevice_t& device, storage_op operation, uint32_t params[4]);

#endif
