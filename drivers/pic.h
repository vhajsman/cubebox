#ifndef _PIC_H
#define _PIC_H

#include "cubebox.h"
#include "kernel/irq/irq.h"

#define PIC1			IRQ_PORT_PIC_M_CTRL
#define PIC2			IRQ_PORT_PIC_S_CtRL

#define PIC1_COMMAND	PIC1
#define PIC1_DATA		( PIC1+1 )
#define PIC2_COMMAND	PIC2
#define PIC2_DATA		( PIC2+1 )

#define PIC_EOI			0x20


#define ICW1_ICW4		0x01
#define ICW1_SINGLE		0x02
#define ICW1_INTERVAL4	0x04
#define ICW1_LEVEL		0x08
#define ICW1_INIT		0x10

#define ICW4_8086		0x01
#define ICW4_AUTO		0x02
#define ICW4_BUF_SLAVE	0x08
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM		0x10

#define PIC_READ_IRR    0x0A
#define PIC_READ_ISR    0x0B

void pic_sendCmd(uint8_t cmd, uint8_t irq);
#define PIC_SEND_EOI(IRQ)	{ pic_sendCmd(PIC_EOI, IRQ); }

void pic_remap(int offset1, int offset2);
void pic_disable(void);

void irq_setMask(uint8_t irqline);
void irq_removeMask(uint8_t irqline)

uint16_t pic_getIrr(void);
uint16_t pic_getIsr(void);

#endif
