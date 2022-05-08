#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>

#define PSF_FONT_MAGIC 0x864ab572

extern char _binary_font_psf_start;

typedef struct {
    uint32_t magic;         /* magic bytes to identify PSF */
    uint32_t version;       /* zero */
    uint32_t headersize;    /* offset of bitmaps in file, 32 */
    uint32_t flags;         /* 0 if there's no unicode table */
    uint32_t numglyph;      /* number of glyphs */
    uint32_t bytesperglyph; /* size of each glyph */
    uint32_t height;        /* height in pixels */
    uint32_t width;         /* width in pixels */
} PSF_font;

void PlotPixel_32bpp(int x, int y, uint32_t pixel);
void putchar(unsigned short int c, int cx, int cy, uint32_t fg, uint32_t bg);
void print(char string[], int x, int y);
void println(char string[]);
void terminit(void * fb, long int ppsl);
void clearscreen(uint32_t colour);

#endif