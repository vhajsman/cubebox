#include "krnlout.h"
#include "drivers/vga.h"
#include "drivers/terminal.h"
#include "kernel/libs/common/converts.h"

void printColorized(string formattedMessage) {
	int messageLen = STRING_LENGTH(formattedMessage);

	unsigned char c;
	uint8_t bg, fg;

	for (int i = 0; i < messageLen; i++) {
		c = formattedMessage[i];

		if (c == '%') {
			if (formattedMessage[i + 1] != '%') {
				bg = ((uint8_t) formattedMessage[i + 1]) - 48;
				fg = ((uint8_t) formattedMessage[i + 2]) - 48;

				TERMINAL_SET_COLOR(VGA_MAKE_COLOR(fg, bg));

				i = i + 2;
			} else {
				i++;
				TERMINAL_WCHAR(formattedMessage[i]);
			}
		}
	}
}

void print(string str) {
	TERMINAL_WSTR(str);
}

void println(string str) {
	TERMINAL_WSTR(str);
	TERMINAL_WSTR("\n");
}

typedef struct {
	unsigned int _event;
	uint16_t _color;

	string _name;
	unsigned char _symbol;
} KRNL_STATUS_T;

void printStatusMsg_noSource(KRNL_STATUS_T status, string msg, int code) {
	string raw;
	string cds;

	int2arg(code, cds, DECIMAL);

	STRING_CONCAT(": ", msg,  raw);
	STRING_CONCAT(raw, " (",  raw);
	STRING_CONCAT(raw, cds,   raw);
	STRING_CONCAT(raw, ")\n", raw);

	TERMINAL_SET_COLOR(status._color);
	TERMINAL_WSTR(status._symbol);

	TERMINAL_SET_COLOR(VGA_MAKE_COLOR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY));
	TERMINAL_WSTR(raw);
}

void printStatusMsg(KRNL_STATUS_T status, string source, string msg, int code) {
	string noSource = "<";

	STRING_CONCAT(noSource, source, noSource);
	STRING_CONCAT(noSource, "> ", noSource);
	STRING_CONCAT(noSource, msg, noSource);

	printStatusMsg_noSource(status, noSource, code);
}