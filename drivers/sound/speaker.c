#include "cubebox.h"
#include "kernel/libs/util/delay.h"

static void makeSound(uint32_t frequency) {
	uint32_t d = 1193180 / frequency;
	uint8_t t;

	outb(0x43, 0xB6);
	outb(0x42, (uint8_t)(d));
	outb(0x42, (uint8_t)(d >> 8));

	t = inb(0x61);
	if (t != (t | 3))
		outb(0x61, t | 3);
}

static void stopSound() {
	outb(0x61, inb(0x61) & 0xFC);
}

void beep() {
	makeSound(1000);
	DELAY_US(25);

	stopSound();
}