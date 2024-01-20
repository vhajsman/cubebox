#ifndef _REGS_H
#define _REGS_H

#include "cubebox.h"

struct registers {
	unsigned int gs;
	unsigned int fs;
	unsigned int es;
	unsigned int ds;

	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int esp;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;

	unsigned int intNo;
	unsigned int errCode;

	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
	unsigned int useresp;
	unsigned int ss;
};

typedef struct registers registers_t;

typedef struct tss_entry {
	uint32_t	prev_tss;
	uint32_t	esp0;
	uint32_t	ss0;
	uint32_t	esp1;
	uint32_t	ss1;
	uint32_t	esp2;
	uint32_t	ss2;
	uint32_t	cr3;
	uint32_t	eip;
	uint32_t	eflags;
	uint32_t	eax;
	uint32_t	ecx;
	uint32_t	edx;
	uint32_t	ebx;
	uint32_t	esp;
	uint32_t	ebp;
	uint32_t	esi;
	uint32_t	edi;
	uint32_t	es;
	uint32_t	cs;
	uint32_t	ss;
	uint32_t	ds;
	uint32_t	fs;
	uint32_t	gs;
	uint32_t	ldt;
	uint16_t	trap;
	uint16_t	iomap_base;
} __attribute__((packed)) tss_entry_t;

#endif