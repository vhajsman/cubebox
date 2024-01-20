#include "cubebox.h"

#define TERMINAL_SET_COLOR(COLOR)	{ terminal_color = COLOR; }

#define TERMINAL_WCHAR(CHAR)		{ terminal_write(CHAR, 1); }
#define TERMINAL_WSTR(STRING)		{ terminal_write(STRING, STRING_LENGTH(STRING)); }

void terminal_init(void);
void terminal_putEntry(char c, uint8_t color, size_t x, size_t y);
void terminal_putChar(char c);
void terminal_write(const char* data, size_t size);

#ifdef __TERMINAL_REQ_PRIVATE
#undef __TERMINAL_REQ_PRIVATE
	extern size_t		terminal_row;
	extern size_t		terminal_column;
	extern uint8_t		terminal_color;
	extern uint16_t*	terminal_buffer;
#endif
