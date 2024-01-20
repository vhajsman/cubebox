#include "pic.h"

void pic_sendCmd(uint8_t cmd, uint8_t irq) {
	if (irq >= 8)
		outb(PIC2_COMMAND, cmd);

	outb(PIC1_COMMAND, cmd);
}

void pic_remap(int offset1, int offset2) {
	uint8_t a1 = inb(PIC1_DATA);
	uint8_t a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);
	io_wait();
	outb(PIC2_DATA, offset2);
	io_wait();
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void pic_disable() {
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);
}

void irq_setMask(uint8_t irqline) {
	uint16_t port;
	uint8_t value;

	if (irqline < 8) {
		port = PIC1_DATA;
	} else {
		port = PIC2_DATA;
		irqline -= 8;
	}

	value = inb(port) | (1 << irqline);
	outb(port, value);
}


void irq_setMask(uint8_t irqline) {
	uint16_t port;
	uint8_t value;

	if (irqline < 8) {
		port = PIC1_DATA;
	}
	else {
		port = PIC2_DATA;
		irqline -= 8;
	}

	value = inb(port) & ~(1 << irqline);
	outb(port, value);
}

static uint16_t __pic_getIrqReg(int ocw3) {
	outb(PIC1_COMMAND, ocw3);
	outb(PIC2_COMMAND, ocw3);

	return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

uint16_t pic_getIrr(void) {
	return __pic_getIrqReg(PIC_READ_IRR);
}

uint16_t pic_getIsr(void) {
	return __pic_getIrqReg(PIC_READ_ISR);
}
