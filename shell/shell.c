#include "shell.h"
#include "drivers/terminal.h"
#include "drivers/hid/kbd.h"
#include "kernel/module.h"

MODULE(shell);

string welcomeMsg = "You are now using CubeBox shell. Type HELP for help, EXIT to exit.";
string prompt = "> ";

static char* inputBuffer;
int buffi = 0;

void shell_keyHandler(uint8_t scancode) {

}

void shell_prompt() {
	TERMINAL_WSTR(prompt);
}

void shell_enter() {
	print("\n");
	println(welcomeMsg);
}

MODULE_INITIALIZER(shell) {
	debugln("-------------------------");

	if (call.parentModule != "kmain")
		MODULE_STATUS(KRNL_STATUS_WARN, "Shell not started from kernel main", 401);

	kbd_addKeyEventHandler(1, shell_keyHandler);

	shell_enter();
}