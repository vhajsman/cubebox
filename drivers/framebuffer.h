#ifndef _FB_H
#define _FB_H

#ifndef _SQ_INDEX
#define _SQ_INDEX
	#define SQ_INDEX(X, Y)	((Y > 0) ? X : X + (X * Y))
#endif

#define FRAMEBUFFER_MAKE_PX(X, Y, PITCH, VRAM, PIXEL_WIDTH)	((unsigned char*) VRAM + Y * PITCH + X * PIXEL_WIDTH)
#define FRAMEBUFFER_MAKE_RGB(RED, GREEN, BLUE)				((RED << 16) + (GREEN << 8) + BLUE)

void framebuffer_putPixel(		unsigned int* vram, 
								int x, 
								int y, 
								unsigned char color);
void framebuffer_drawLine_90(	unsigned char* vram, 
								unsigned char red, 
								unsigned char green, 
								unsigned char blue, 
								unsigned char xa, 
								unsigned char ya, 
								unsigned char xb, 
								unsigned char yb);
void framebuffer_drawRect(		unsigned char* vram, 
								unsigned char red, 
								unsigned char green, 
								unsigned char blue, 
								unsigned char x, 
								unsigned char y, 
								unsigned char width, 
								unsigned char height);
void framebuffer_fillRect(		unsigned char* vram, 
								unsigned char red, 
								unsigned char green, 
								unsigned char blue, 
								unsigned char x, 
								unsigned char y, 
								unsigned char width, 
								unsigned char height, 

								/*	Does not "fill" the edges.
									PRO-TIP: Use, if rectangle has a border, to improve performance.	*/
								bool ignoreEdge);

#endif