#ifndef _TIMER_H
#define _TIMER_H

void timer_setPhase(int hertz);
void relativeTime(unsigned long seconds, unsigned long subseconds, unsigned long* out_seconds, unsigned long* out_subseconds);
void timer_init();

extern unsigned long timer_ticks;
extern unsigned char timer_subticks;

#endif