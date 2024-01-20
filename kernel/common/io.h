#ifndef _IO_H
#define _IO_H

unsigned char inportb(unsigned int port);
void outportb(unsigned int port, unsigned char value);

#define outb(port, value)	outportb(port, value)
#define inb(port)			( inportb(port) )

void io_wait();
void insl(uint16_t __port, unsigned int __address, uint32_t __count);

#endif
