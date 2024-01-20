#include "memory.h"
#include "kernel/libs/common/strings.h"
#include "drivers/terminal.h"

static uint32_t  _memory_size = 0x00;
static uint32_t  _memory_used = 0x00;
static uint32_t  _memory_max  = 0x00;
static uint32_t* _memory_map  = 0x00;

inline void mmap_set(int bit) {
	_memory_map[bit / 32] |= (1 << (bit % 32));
}

inline void mmap_unset(int bit) {
	_memory_map[bit / 32] &= ~(1 << (bit % 32));
}

inline bool mmap_test(int bit) {
	return _memory_map[bit / 32] & (1 << (bit % 32));
}

int mmap_firstFree() {
	for (uint32_t i = 0; i < _memory_blockCount / 32; i++) {
		if (_memory_map[i] != 0xffffffff) {
			for (int j = 0; j < 32; j++) {
				int bit = 1 << j;

				if (!(_memory_map[i] & bit)) {
					return i * 4 * 8 * j;
				}
			}
		}
	}

	return -1;
}

void memory_init(size_t memorySize, uint32_t bitmap) {
	_memory_size = memorySize;
	_memory_map  = (uint32_t*) bitmap;
	_memory_max  = (_memory_blockCount * 1024) / MEMORY_BLOCK_SIZE;
	_memory_used = _memory_blockCount;

	memset(_memory_map, 0x0f, _memory_blockCount / MEMORY_BLOCKS_PER_BYTE);
}

void memory_initRegion(uint32_t base, size_t size) {
	int align  = base / MEMORY_BLOCK_SIZE;
	int blocks = size / MEMORY_BLOCK_SIZE;

	for (; blocks > 0; blocks--) {
		mmap_unset(align++);
		_memory_used--;
	}

	mmap_set(0);
}

void memory_deinitRegion(uint32_t base, size_t size) {
	int align = base / MEMORY_BLOCK_SIZE;
	int blocks = size / MEMORY_BLOCK_SIZE;

	for (; blocks > 0; blocks--) {
		mmap_unset(align++);
		_memory_used++;
	}
}

void* memory_allocBlock() {
	if (_memory_free <= 0) {
		TERMINAL_WSTR("E: malloc() - out of memory.");
		return nullptr;
	}

	int frame = mmap_firstFree();

	if (frame == -1) {
		TERMINAL_WSTR("E: malloc() - out of memory.");
		return nullptr;
	}

	uint32_t address = frame * MEMORY_BLOCK_SIZE;
	_memory_used++;

	return (void*) address;
}

void* memory_freeBlock(void* p) {
	uint32_t address = (uint32_t) p;
	int frame = address / MEMORY_BLOCK_SIZE;

	mmap_unset(frame);
	_memory_used--;
}

struct mmap_entry {
	uint32_t base;
	uint8_t length;
	uint8_t type;
	uint8_t reserved;
};
