#include "cubebox.h"
#include "kernel/libs/krnlout.h"
#include "kernel/libs/krnlstatus.h"

void framebuffer_putPixel(unsigned int* vram, int x, int y, unsigned char color) {
	unsigned location = x * PIXEL_WIDTH + y * PITCH;

	vram[where]		= color			& 255;
	vram[where + 1]	= (color >> 8)	& 255;
	vram[where + 2]	= (color >> 16) & 255;
}

void framebuffer_drawLine_90(unsigned char* vram, unsigned char red, unsigned char green, unsigned char blue, unsigned char xa, unsigned char ya, unsigned char xb, unsigned char yb) {
	unsigned char color = FRAMEBUFFER_MAKE_RGB(red, green, blue);

	// Horizontal line
	if (xa == xb && ya != yb) {
		if (ya > yb) {
			unsigned char t = ya;
			ya = yb;
			yb = t;
		}

		for (unsigned char i = 0; i < yb - ya; i++)
			framebuffer_putPixel(vram, xa, ya + i; color);

		return;
	}

	// Vertical line
	if (xa != xb && ya == yb) {
		if (xa > xb) {
			unsigned char t = xa;
			xa = xb;
			xb = t;
		}

		for (unsigned char i = 0; i < xb - xa; i++)
			framebuffer_putPixel(vram, xa + i, ya, color);

		return;
	}

	if (xa == xb && ya == yb) {
		framebuffer_putPixel(vram, xa, ya, color);
		return;
	}

	printStatusMsg(KRNL_STATUS_FAIL, "FRAMEBUFFER", "Line angle to framebuffer can not be divided by 90.", 501);
}

void framebuffer_drawRect(unsigned char* vram, unsigned char red, unsigned char green, unsigned char blue, unsigned char x, unsigned char y, unsigned char width, unsigned char height) {
	/*
		D			C
		+-------------+
		|             |
		|             |
		+-------------+
		A			B
	*/

	framebuffer_drawLine_90(vram, red, green, blue, x, y + height, x + width, y + height);	// AB
	framebuffer_drawLine_90(vram, red, green, blue, x, y, x + width, y);					// DC
	framebuffer_drawLine_90(vram, red, green, blue, x, y, x, y + height);					// AD
	framebuffer_drawLine_90(vram, red, green, blue, x + width, y, x + width, y + height);	// BC
}

void framebuffer_fillRect(unsigned char* vram, unsigned char red, unsigned char green, unsigned char blue, unsigned char x, unsigned char y, unsigned char width, unsigned char height, bool ignoreEdge) {
	unsigned char startx	= ignoreEdge ? x : x + 1;
	unsigned char starty	= ignoreEdge ? y : y + 1;
	unsigned char endx		= ignoreEdge ? x : x - 1;
	unsigned char endy		= ignoreEdge ? y : y - 1;

	for (unsigned char li = startx; li < endx; li++)
		framebuffer_drawLine_90(vram, red, green, blue, li, starty, li, endy);
}

