#ifndef _IRQ_H
#define _IRQ_H

#include "cubebox.h"
#include "kernel/common/regs.h"

extern void* isr_stubTable[];

typedef struct {
	uint16_t    isr_low;
	uint16_t    isr_high;
	uint16_t    kernel_cs;
	uint8_t     reserved;
	uint8_t     attributes;
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10)))
extern static idt_entry_t idt[256];

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;

extern static idtr_t idtr;

typedef void (*irq_handler_t) (struct regs*);

// void exception_handler(void);

// ! Deprecated
void idt_setDescriptor(uint8_t vector, void* isr, uint8_t flags);

void idt_setGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_init(void);

#define IRQ_PIT				0
#define IRQ_KBD				1
#define IRQ_CASCADE			2
#define IRQ_COM2			3
#define IRQ_COM1			4
#define IRQ_LPT2			5
#define IRQ_FD				6
#define IRQ_LPT1			7
#define IRQ_SPURIOUS		7
#define IRQ_CMOS			8
#define IRQ_SCSIL			9
#define IRQ_NIC0			9
#define IRQ_F0				9
#define IRQ_SCSI1			10
#define IRQ_NIC1			10
#define IRQ_F1				10
#define IRQ_SCSI2			11
#define IRQ_NIC2			11
#define IRQ_F2				11
#define IRQ_MOUSE			12
#define IRQ_FPU				13
#define IRQ_COPROCESSOR		13
#define IRQ_INTERPROCESSOR	13
#define IRQ_ATA_PRIMARY		14
#define IRQ_ATA_SECONDARY	15

#define IRQ_PORT_PIC_M_CTRL	0x20
#define IRQ_PORT_PIC_M_MASK 0x21
#define IRQ_PORT_PIC_S_CTRL 0xA0
#define IRQ_PORT_PIC_S_MASK 0xA1
#define IRQ_PORT_KBD_DATA	0x60
#define IRQ_PORT_KBD_CMD	0x64

void irq_installHandler(int irq, irq_handler_t handler);
void irq_uninstallHandler(int irq);
void irq_init();

void irq_ack(int irq);
void irq_handler(struct registers* regs);

#endif
