#include "kernel/libs/krnlout.h"
#include "kernel/libs/krnlstatus.h"
#include "kernel/macros/bytes.h"
#include "kernel/irq/irq.h"
#include "kernel/module.h"
#include "kernel/libs/common/strings.h"
#include "kbd.h"

MODULE(kbd);

bool _kbd_nl = 0;
bool _kbd_cl = 0;
bool _kbd_sl = 0;

bool _kbd_enabled = 0;

bool _kbd_ctrl = 0;
bool _kbd_shift = 0;
bool _kbd_alt = 0;

uint8_t kbd_ctrl_readStatus() {
	return inb(KBD_CTRL_STATS_REG);
}

void kbd_ctrl_sendCmd(uint8_t cmd) {
	KBD_CTRL_IN_BUFF_WAIT();
	outb(KBD_CTRL_CMD_REG, cmd);
}

uint8_t kbd_enc_readBuffer() {
	return inb(KBD_ENC_IN_BUFF);
}

void kbd_enc_sendCmd(uint8_t cmd) {
	KBD_CTRL_IN_BUFF_WAIT();
	outb(KBD_ENC_CMD_REG, cmd);
}


bool kbd_selfTest() {
	kbd_ctrl_sendCmd(KBD_CMD_RESET);

	while (1) {
		if (kbd_ctrl_readStatus() & KBD_CTRL_STATS_MASK_OUT_BUFF)
			break;
	}

	return kbd_enc_readBuffer() == 0x55;
}

uint8_t kbd_makeLeds(int n, int c, int s) {
	uint8_t data;

	s ? BIT_SET(data, 0) : BIT_CLEAR(data, 0);
	n ? BIT_SET(data, 1) : BIT_CLEAR(data, 1);
	c ? BIT_SET(data, 2) : BIT_CLEAR(data, 2);

	return data;
}

void kbd_setLeds(uint8_t leds) {
	kbd_enc_sendCmd(KBD_CMD_SET_LEDS);
	kbd_enc_sendCmd(leds);
}

void kbd_setLocks(int n, int c, int s) {
	_kbd_nl = (n == UNCHANGED) _kbd_nl : n;
	_kbd_cl = (c == UNCHANGED) _kbd_cl : c;
	_kbd_sl = (s == UNCHANGED) _kbd_sl : s;

	KBD_UPDATE_LEDS(_kbd_nl, _kbd_cl, _kbd_sl);
}

void kbd_enable() {
	kbd_ctrl_sendCmd(0xF4);
	_kbd_enabled = true;
}

void kbd_disable() {
	kbd_ctrl_sendCmd(0xF5);
	_kbd_enabled = false;
}

uint8_t _kbd_lastScancode = 0;

#define KBD_SPECIAL_CTRL_L		0x1D
#define KBD_SPECIAL_SHIFT_L		0x2A
#define KBD_SPECIAL_ALT_L		0x38
#define KBD_SPECIAL_CL			0x3A
#define KBD_SPECIAL_NL			0x45
#define KBD_SPECIAL_SL			0x46
#define KBD_SPECIAL_CTRL_L_r	0x9D
#define KBD_SPECIAL_SHIFT_L_r	0xAA
#define KBD_SPECIAL_ALT_L_r		0xB8

typedef void (*_kbd_keyHandler_t)(uint8_t scancode);
_kbd_keyHandler_t _kbdKeyHandlers[16] = {nullptr};

void kbd_addKeyEventHandler(int id, void (*handler)(uint8_t scancode)) {
	if (id >= 16) {
		MODULE_STATUS(KRNL_STATUS_FAIL, "Handler index out of range", 504);
		return;
	}

	if (_kbdKeyHandlers[id] != nullptr)
		MODULE_STATUS(KRNL_STATUS_WARN, "Handler already installed. Overwrite...", 402);

	_kbdKeyHandlers[id] = handler;
}

void _kbd_irq(struct registers* regs) {
	_kbd_lastScancode = kbd_enc_readBuffer();

	switch (_kbd_lastScancode) {
		case KBD_SPECIAL_CTRL_L: 
			_kbd_alt = true; 
			break;
		case KBD_SPECIAL_CTRL_L_r: 
			_kbd_alt = false; 
			break;

		case KBD_SPECIAL_SHIFT_L: 
			_kbd_shift = true; 
			break;
		case KBD_SPECIAL_SHIFT_L_r: 
			_kbd_shift = false; 
			break;

		case KBD_SPECIAL_ALT_L:
			_kbd_alt = true;
			break;
		case KBD_SPECIAL_ALT_L_r:
			_kbd_alt = false;
			break;

		case KBD_SPECIAL_NL:
			_kbd_nl = !_kbd_nl;
			goto updateLeds;
			break;

		case KBD_SPECIAL_CL:
			_kbd_cl = !_kbd_cl;
			goto updateLeds;
			break;

		case KBD_SPECIAL_SL:
			_kbd_sl = !_kbd_sl;
			goto updateLeds;
			break;
	}

	goto callHandler;

updateLeds:
	kbd_setLocks(_kbd_nl, _kbd_cl, _kbd_sl);
	goto callHandler;

callHander:
	for (int i; i < 16; i++) {
		if (_kbdKeyHandlers[i] != nullptr)
			_kbdKeyHandlers[i](_kbd_lastScancode);
	}

	return;
}

void kbd_init() {
	// Install keyboard IRQ handler
	irq_installHandler(1, _kbd_irq);

	kbd_enable();
	kbd_setLocks(false, false, false);

	printStatusMsg(KRNL_STATUS_OKAY, "KBD", "Keyboard init.", 0);
}
