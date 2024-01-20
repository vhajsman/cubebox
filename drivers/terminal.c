#include "cubebox.h"
#include "vga.h"

size_t		terminal_row;
size_t		terminal_column;
uint8_t		terminal_color;
uint16_t*	terminal_buffer;

void terminal_init(void) {
	terminal_row = 0;
	terminal_column = 0;

	terminal_color = VGA_MAKE_COLOR(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = VGA_MAKE_ENTRY(' ', terminal_color);
		}
	}
}

void terminal_putEntry(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = VGA_MAKE_ENTRY(c, color);
}

void terminal_putChar(char c) {
	terminal_putEntry(c, terminal_color, terminal_column, terminal_row);

	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;

		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_scroll() {
	
}

void terminal_newLine() {

}
