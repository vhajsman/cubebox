#include "cubebox.h"

void kmain(void);
void kmain(void) {
	terminal_init();

	TERMINAL_WSTR("Cubebox kernel.\nVersion:");
	TERMINAL_WSTR(KERNEL_VERSION_STRING);
}
