#include "video.h"

long int scanline;
void * framebuffer;
int font_x;
int font_y;
int cur_x = 0;
int cur_y = 0;

void terminit(void * fb, long int ppsl)
{
    framebuffer = fb;
    scanline = ppsl;
    clearscreen(0x00000000);
}

void PlotPixel_32bpp(int x, int y, uint32_t pixel)
{
   *((uint32_t*)(framebuffer + 4 * scanline * y + 4 * x)) = pixel;
}

void putchar(unsigned short int c, int cx, int cy, uint32_t fg, uint32_t bg)
{
    PSF_font *font = (PSF_font*)&_binary_font_psf_start;
    int bytesperline = (font->width+7)/8;

    font_x = font->width;
    font_y = font->height;

    unsigned char *glyph = (unsigned char*)&_binary_font_psf_start + font->headersize + (c > 0 && c < font->numglyph ? c : 0) * font->bytesperglyph;

    int offs = (cy * font->height * scanline) + (cx * (font->width + 1) * sizeof(uint32_t));

    int x, y, line, mask;
    for (y = 0; y < font->height; y++)
    {
        line = offs;
        mask = 1 << (font->width + 1);

        for (x = 0; x < font->width; x++)
        {
            PlotPixel_32bpp(x + cx, y + cy, *((unsigned int *)glyph) & mask ? fg : bg);
            mask >>= 1;
            line += sizeof(uint32_t);
        }

        glyph += bytesperline;
        offs += scanline;
    }
}

void print(char string[], int x, int y)
{
    for (int i = 0; string[i] != '\0'; i++)
        putchar(string[i], i * (font_x / 2 - 1) + x, y, 0xffffffff, 0x00000000);
}

void println(char string[])
{
    for (int i = 0; string[i] != '\0'; i++)
    {
        switch (string[i])
        {
            case '\n':
                cur_y += font_y;
                cur_x = 0;
                break;
            default:
                putchar(string[i], i * (font_x / 2 - 1) + cur_x, cur_y, 0xffffffff, 0x00000000);
                cur_x += font_x;
                break;
        }
    }
}

void clearscreen(uint32_t colour)
{
    for (int y = 0; y < 1280; y++) // Fix this
    {
        for (int x = 0; x < 1920; x++) // Also fix this
        {
            PlotPixel_32bpp(x, y, colour);
        }
    }
}