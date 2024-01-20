#ifndef _KERNEL_MEMORY_H
#define _KERNEL_MEMORY_H

#include "cubebox.h"

#define MEMORY_BLOCKS_PER_BYTE	8
#define MEMORY_BLOCK_SIZE		4096
#define MEMORY_BLOCK_ALIGN		MEMORY_BLOCK_SIZE

void memory_init(size_t memorySize, uint32_t bitmap);

void memory_initRegion(uint32_t base, size_t size);
void memory_deinitRegion(uint32_t base, size_t size);

extern static uint32_t  _memory_size;
extern static uint32_t  _memory_used;
extern static uint32_t  _memory_max;
extern static uint32_t* _memory_map;

void mmap_set(int bit);
void mmap_unset(int bit);
bool mmap_test(int bit);

int mmap_firstFree();		// Find first free block

void* memory_allocBlock();
void* memory_freeBlock(void* _Ptr);

/*
typedef struct {
	uint8_t base;
	uint8_t length;
	uint8_t type;

	bool reserved;
} mmap_entry;
*/

#endif