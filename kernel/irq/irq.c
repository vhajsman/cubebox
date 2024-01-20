#include "irq.h"

#define IDT_MAX_DESCRIPTORS 256

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

static irq_handler_t irq_routines[16] = { NULL };

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];
static idtr_t idtr;

void exception_handler() {
    __asm__ volatile ("cli; hlt");
}

// ! Deprecated
void idt_setDescriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low =       (uint32_t) isr & 0xFFFF;
    descriptor->isr_high =      (uint32_t) isr >> 16;

    descriptor->kernel_cs =     0x08;
    descriptor->attributes =    flags;

    descriptor->reserved =      0;
}

void idt_setGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[num].isr_low = (base & 0xFFFF);
    idt[num].isr_high = (base >> 16) & 0xFFFF;
    idt[num].kernel_cs = sel;
    idt[num].reserved = 0;
    idt[num].attributes = flags | 0x60;
}

void idt_init(void) {
    idtr.base = (uintptr_t) &idt;
    idtr.limit = (sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS) - 1;

    memset(&idt, 0, sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS);
}

void irq_installHandler(int irq, irq_handler_t handler) {
    irq_routines[irq] = handler;
}

void irq_uninstallHandler(int irq) {
    irq_installHandler(irq, NULL);
}

#include "drivers/pic.h"

void irq_init() {
    pic_remap();

    idt_setGate(32, (unsigned)_irq0, 0x08, 0x8E);
    idt_setGate(33, (unsigned)_irq1, 0x08, 0x8E);
    idt_setGate(34, (unsigned)_irq2, 0x08, 0x8E);
    idt_setGate(35, (unsigned)_irq3, 0x08, 0x8E);
    idt_setGate(36, (unsigned)_irq4, 0x08, 0x8E);
    idt_setGate(37, (unsigned)_irq5, 0x08, 0x8E);
    idt_setGate(38, (unsigned)_irq6, 0x08, 0x8E);
    idt_setGate(39, (unsigned)_irq7, 0x08, 0x8E);
    idt_setGate(40, (unsigned)_irq8, 0x08, 0x8E);
    idt_setGate(41, (unsigned)_irq9, 0x08, 0x8E);
    idt_setGate(42, (unsigned)_irq10, 0x08, 0x8E);
    idt_setGate(43, (unsigned)_irq11, 0x08, 0x8E);
    idt_setGate(44, (unsigned)_irq12, 0x08, 0x8E);
    idt_setGate(45, (unsigned)_irq13, 0x08, 0x8E);
    idt_setGate(46, (unsigned)_irq14, 0x08, 0x8E);
    idt_setGate(47, (unsigned)_irq15, 0x08, 0x8E);

    // IRQ_RES;
}

void irq_ack(int irq) {
    debugln("IRQ: ACK.");

    if (irq >= 12)
        outb(0xA0, 0x20);

    outb(0x20, 0x20);
}

void irq_handler(struct registers* regs) {
    // IRQ_OFF;

    void (*handler) (struct registers* regs);
    handler = (regs->intNo > 47 || regs->intNo < 32) ? NULL : irq_routines[regs->intNo - 32];

    if (handler) {
        handler(regs);
    } else {
        irq_ack(regs->intNo - 32);
    }

    // IRQ_RES;
}
