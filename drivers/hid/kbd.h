#ifndef _KBD_H
#define _KBD_H

#include "cubebox.h"

#define KBD_ID							0x83AB

#ifndef UNCHANGED
#define UNCHANGED						0xFF
#endif

#ifndef _KBD_TOTAL_SCANCODES
#define _KBD_TOTAL_SCANCODES 128
#endif

#ifndef _KBD_LANG
#define _KBD_LANG us
#endif

#ifndef _KBD_LAYOUT
#define _KBD_LAYOUT "kbd-us128"
#endif

#ifndef _KBD_LAYOUT_HEADER _KBD_LAYOUT ## #.h

#include _KBD_LAYOUT_HEADER

#define KBD_CMD_SET_LEDS				0xED
#define KBD_CMD_ECHO					0xEE
#define KBD_CMD_SC_SG					0xF0
#define KBD_CMD_IDENTIFY				0xF2
#define KBD_CMD_SET_RATE				0xF3
#define KBD_CMD_SET_DELAY				0xF3
#define KBD_CMD_SCAN					0xF4
#define KBD_CMD_STOP_SCAN				0xF5
#define KBD_CMD_DEFAULT					0xF6
#define KBD_CMD_BYTE_RESEND				0xFE
#define KBD_CMD_RESET					0xFF

#define KBD_RESP_ERROR					0x00
#define KBD_RESP_BUFF_OVERRUN			0x00
#define KBD_RESP_SELF_OK				0xAA
#define KBD_RESP_ECHO					0xEE
#define KBD_RESP_ACK					0xFA
#define KBD_RESP_SELF_FAIL				0xFC
#define KBD_RESP_RESEND					0xFE

#define KBD_ENC_IN_BUFF					0x60
#define KBD_ENC_CMD_REG					0x60
#define KBD_CTRL_STATS_REG				0x64
#define KBD_CTRL_CMD_REG				0x64

#define KBD_CTRL_STATS_MASK_OUT_BUFF	0x01
#define KBD_CTRL_STATS_MASK_IN_BUFF		0x02
#define KBD_CTRL_STATS_MASK_SYSTEM		0x04
#define KBD_CTRL_STATS_MASK_CMD_DATA	0x08
#define KBD_CTRL_STATS_MASK_LOCKED		0x10
#define KBD_CTRL_STATS_MASK_AUX_BUFF	0x20
#define KBD_CTRL_STATS_MASK_TIMEOUT		0x40
#define KBD_CTRL_STATS_MASK_PARITY		0x80

#define KBD_CTRL_IN_BUFF_WAIT()			while ((kbd_ctrl_readStatus() & KBD_CTRL_STATS_MASK_IN_BUFF) != 0x00) {}
#define KBD_UPDATE_LEDS(NL, CL, SL)		kbd_setLeds(kbd_makeLeds(NL, CL, SL))
#define KBD_IS_SCANCODE(RETURN_CODE)	(			\
	(RETURN_CORE > 0x00 && RETURN_CODE < 0x59) ||	\
	(RETURN_CODE > 0x80 && RETURN_CODE < 0xD9)		)

#ifdef _KBD_LOCKS_EXT
#undef _KBD_LOCKS_EXT
	extern bool _kbd_nl;
	extern bool _kbd_cl;
	extern bool _kbd_sl;
#endif

#ifdef _KBD_ENABLE_EXT
#undef _KBD_ENABLE_EXT
	extern bool _kbd_enabled;
#endif

uint8_t	kbd_ctrl_readStatus();
void	kbd_ctrl_sendCmd(uint8_t cmd);

uint8_t kbd_enc_readBuffer();
void	kbd_enc_sendCmd(uint8_t cmd);

bool	kbd_selfTest();

uint8_t kbd_makeLeds(int n, int c, int s);
void	kbd_setLocks(int n, int c, int s);
void	kbd_setLeds(uint8_t leds);

void	kbd_enable();
void	kbd_disable();

void	kbd_init();

void kbd_addKeyEventHandler(int id, void (*handler)(uint8_t scancode))

#endif