#include "cubebox.h"
#include "kernel/irq/irq.h"

#define PIT_A				0x40
#define PIT_B				0x41
#define PIT_C				0x42
#define PIT_CONTROL			0x43

#define PIT_MASK			0xFF
#define PIT_SCALE			1193180
#define PIT_SET				0x36

#define TIMER_IRQ			0

#define SUBTICKS_PER_TICK	100

unsigned long timer_ticks = 0;
unsigned char timer_subticks = 0;

void timer_setPhase(int hertz) {
	int divisor = PIT_SCALE / hertz;

	outb(PIT_CONTROL, PIT_SET);
	outb(PIT_A, divisor & PIT_MASK);
	outb(PIT_A, (divisor >> 8) & PIT_MASK);
}

void _timer_handler(struct registers* regs) {
	if (++timer_subticks == SUBTICKS_PER_TICK) {
		timer_ticks++;
		timer_subticks = 0;
	}

	irq_ack(TIMER_IRQ);
}

void relativeTime(unsigned long seconds, unsigned long subseconds, unsigned long* out_seconds, unsigned long* out_subseconds) {
	if (subseconds + timer_subticks > SUBTICKS_PER_TICK) {
		*out_seconds = timer_ticks + seconds + 1;
		*out_subseconds = subseconds + timer_subticks - SUBTICKS_PER_TICK;

		return;
	}

	*out_seconds = timer_ticks + seconds;
	*out_subseconds = timer_subticks + subseconds;
}

void timer_init() {
	debugln("PIT: Init.");

	irq_installHandler(TIMER_IRQ, _timer_handler);
	timer_setPhase(100);
}