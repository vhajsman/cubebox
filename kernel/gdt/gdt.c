#include "gdt.h"
#include "drivers/terminal.h"

void gdt_encodeEntry(uint8_t* target, gdt_t source) {
	if (source.limit > 0xFFFFF) {
		TERMINAL_WSTR("\nE: GDT limit too large.");
		return;
	}

    target[0] =  source.limit           & 0xFF;
    target[1] = (source.limit >> 8)     & 0xFF;
    target[6] = (source.limit >> 16)    & 0x0F;

    target[2] =  source.base            & 0xFF;
    target[3] = (source.base >> 8)      & 0xFF;
    target[4] = (source.base >> 16)     & 0xFF;
    target[7] = (source.base >> 24)     & 0xFF;

    target[5] = source.access_byte;

    target[6] |= (source.flags << 4);
}