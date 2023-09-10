// ----------------------------------- framebf.h -------------------------------------
#include "mbox.h"
#include "uart.h"
// Use RGBA32 (32 bits for each pixel)
#define COLOR_DEPTH 32

// Pixel Order: BGR in memory order (little endian --> RGB in byte order)
#define PIXEL_ORDER 0

// Screen info
unsigned int screenWidth, screenHeight, pitch;

/* Frame buffer address
 * (declare as pointer of unsigned char to access each byte) */
unsigned char *fb;
void framebf_init();
void drawPixelARGB32(int x, int y, unsigned int attr);
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawLineARGB32(int x1, int y1, int x2, int y2, unsigned int attr);
void drawChar(char ch, int x, int y, unsigned attr);
void wait_msec(unsigned int n);