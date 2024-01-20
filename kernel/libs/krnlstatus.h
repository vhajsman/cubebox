#ifndef _LIBK_KRNLSTATUS_H
#define _LIBK_KRNLSTATUS_H

#include "cubebox.h"
#include "drivers/vga.h"

typedef struct {
	unsigned int _event;
	uint16_t _color;

	string _name;
	unsigned char _symbol;
} KRNL_STATUS_T;

#define KRNL_MAKE_STATUS(STRUCT, EVENT, COLOR, NAME, SYMBOL) {			\
	KRNL_STATUS_T STRUCT;												\
																		\
	STRUCT->_event =	EVENT;											\
	STRUCT->_name =		NAME;											\
	STRUCT->_symbol =	SYMBOL;											\
																		\
	STRUCT->_color =	VGA_MAKE_COLOR(COLOR, VGA_COLOR_BLACK);			\
}

extern KRNL_STATUS_UNKNOWN;
extern KRNL_STATUS_OKAY;
extern KRNL_STATUS_NOTE;
extern KRNL_STATUS_WARNING;
extern KRNL_STATUS_FAIL;
extern KRNL_STATUS_FATAL;

#endif
