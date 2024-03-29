#include "cubebox.h"
#include "irq.h"

extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();
extern void _isr127();

static irq_handler_t isrs_routines[32] = { NULL };

void isr_installHandler(int isr, irq_handler_t handler) {
	isrs_routines[isr] = handler;
}

void isr_uninstallHandler(int isr) {
	isr_installHandler(isr, NULL);
}

void isr_init() {
	/* Exception Handlers */
	idt_setGate(0,  (unsigned)_isr0,  0x08, 0x8E);
	idt_setGate(1,  (unsigned)_isr1,  0x08, 0x8E);
	idt_setGate(2,  (unsigned)_isr2,  0x08, 0x8E);
	idt_setGate(3,  (unsigned)_isr3,  0x08, 0x8E);
	idt_setGate(4,  (unsigned)_isr4,  0x08, 0x8E);
	idt_setGate(5,  (unsigned)_isr5,  0x08, 0x8E);
	idt_setGate(6,  (unsigned)_isr6,  0x08, 0x8E);
	idt_setGate(7,  (unsigned)_isr7,  0x08, 0x8E);
	idt_setGate(8,  (unsigned)_isr8,  0x08, 0x8E);
	idt_setGate(9,  (unsigned)_isr9,  0x08, 0x8E);
	idt_setGate(10, (unsigned)_isr10, 0x08, 0x8E);
	idt_setGate(11, (unsigned)_isr11, 0x08, 0x8E);
	idt_setGate(12, (unsigned)_isr12, 0x08, 0x8E);
	idt_setGate(13, (unsigned)_isr13, 0x08, 0x8E);
	idt_setGate(14, (unsigned)_isr14, 0x08, 0x8E);
	idt_setGate(15, (unsigned)_isr15, 0x08, 0x8E);
	idt_setGate(16, (unsigned)_isr16, 0x08, 0x8E);
	idt_setGate(17, (unsigned)_isr17, 0x08, 0x8E);
	idt_setGate(18, (unsigned)_isr18, 0x08, 0x8E);
	idt_setGate(19, (unsigned)_isr19, 0x08, 0x8E);
	idt_setGate(20, (unsigned)_isr20, 0x08, 0x8E);
	idt_setGate(21, (unsigned)_isr21, 0x08, 0x8E);
	idt_setGate(22, (unsigned)_isr22, 0x08, 0x8E);
	idt_setGate(23, (unsigned)_isr23, 0x08, 0x8E);
	idt_setGate(24, (unsigned)_isr24, 0x08, 0x8E);
	idt_setGate(25, (unsigned)_isr25, 0x08, 0x8E);
	idt_setGate(26, (unsigned)_isr26, 0x08, 0x8E);
	idt_setGate(27, (unsigned)_isr27, 0x08, 0x8E);
	idt_setGate(28, (unsigned)_isr28, 0x08, 0x8E);
	idt_setGate(29, (unsigned)_isr29, 0x08, 0x8E);
	idt_setGate(30, (unsigned)_isr30, 0x08, 0x8E);
	idt_setGate(31, (unsigned)_isr31, 0x08, 0x8E);

	// idt_setGate(SYSCALL_VECTOR, (unsigned)_isr127, 0x08, 0x8E);
}

char* exceptionMessages[] = {
	"Division by zero",				/* 0 */
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	"Out-of-bounds",				/* 5 */
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",						/* 10 */
	"Segment not present",
	"Stack fault",
	"General protection fault",
	"Page fault",
	"Unknown interrupt",			/* 15 */
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};


