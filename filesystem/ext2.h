#ifndef _FS_EXT2_H
#define _FS_EXT2_H

#include "cubebox.h"

#define EXT2_BLOCK_SIZE		1024
#define EXT2_CACHE_ENTRIES	10240
#define EXT2_BLOCK_PTRS		256
#define EXT2_SECTOR_SIZE	512
#define EXT2_DISK_PORT		0x1F0

struct ext2_diskCacheEntry {
	uint32_t block;
	uint32_t lastUse;
	uint8_t  dirty;
	uint8_t* block;
};

typedef struct ext2_diskCacheEntry ext2_diskCacheEntry_t;

#endif
