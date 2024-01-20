#include "io.h"

#include "cubebox.h"

unsigned char inportb(unsigned int port) {
	unsigned char ret;
	asm volatile ("inb %%dx,%%al":"=a" (ret) : "d" (port));

	return ret;
}

void outportb(unsigned int port, unsigned char value) {
	asm volatile ("outb %%al,%%dx": : "d" (port), "a" (value));
}

void io_wait() {
    asm volatile("jmp 1f\n\t"
        "1:jmp 2f\n\t"
        "2:");
}

void insl(uint16_t __port, unsigned int __address, uint32_t __count) {
    asm volatile("cld; rep insl"
        : "=D" (__address), "=c" (__count)
        : "d" (__port), "0" (__address), "1" (__count)
        : "memory");
}

