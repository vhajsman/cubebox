#ifndef _ISR_H
#define _ISR_H

void isr_installHandler(int isr, irq_handler_t handler);
void isr_uninstallHandler(int isr);
void isr_init();

extern char* exceptionMessages[];

#endif