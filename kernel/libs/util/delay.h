#ifndef _UTIL_DELAY_H
#define _UTIL_DELAY_H

#include "drivers/timer.h"
#include "cubebox.h"

#define SLEEP_TICKS(TICKS) {			\
	uint32_t d = timer_ticks;			\
	while(timer_ticks < d + TICKS) {}	\
										\
	delete d;							\
}

#define DELAY_US(TIME)		SLEEP_TICKS(TIME)
#define DELAY_SEC(TIME)		SLEEP_TICKS(TIME * 100)

#endif
