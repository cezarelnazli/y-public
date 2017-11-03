/**
 * Implementation of basic VGA functionality.
 *
 * References
 * http://www.brackeen.com/vga/
 * http://www.osdever.net/bkerndev/Docs/printing.htm
 * http://wiki.osdev.org/Meaty_Skeleton
 */
#include <stddef.h>
#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *VGA_PTR = (uint16_t *) 0xC00B8000;

static uint16_t color = 0x0F00;     /* fg = white, bg = black */
static size_t row, col;

uint16_t vga_encode_char(char ch)
{
    uint16_t ch16 = (uint16_t) (ch & 0xFF);

    return color | ch16;
}

static void scroll(void)
{
    size_t i, j, idx;

    if (row == VGA_HEIGHT) {
        for (i = 1; i < VGA_HEIGHT; i++) {
            for (j = 0; j < VGA_WIDTH; j++) {
                idx = i * VGA_WIDTH + j;
                VGA_PTR[idx - VGA_WIDTH] = VGA_PTR[idx];
            }
        }

        for (i = 0; i < VGA_WIDTH; i++) {
            VGA_PTR[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = vga_encode_char(' ');
        }
    }
}

void vga_set_color(enum vga_color fg, enum vga_color bg)
{
    color = (fg << 8) | (bg << 12);
}

void vga_init(void)
{
    size_t w, h, idx;

    vga_set_color(VGA_WHITE, VGA_BLACK);

    for (h = 0; h < 25; h++) {
        for (w = 0; w < 80; w++) {
            idx = h * VGA_WIDTH + w;
            VGA_PTR[idx] = vga_encode_char(' ');
        }
    }
}

void vga_putchar(char ch)
{
    size_t pos = row * VGA_WIDTH + col;

    if (ch == '\b') {
        if (col > 0) {
            col--;
        }
    } else if (ch == '\n') {
        col = 0;
        row++;
    } else if (ch == '\r') {
        col = 0;
    } else if (ch == '\t') {
        col += 4 - (col & 3);   /* does not use a loop because of speed. */
    } else if (ch >= ' ') {
        VGA_PTR[pos] = vga_encode_char(ch);
        col++;
    }

    if (col >= VGA_WIDTH) {
        col = 0;
        row++;
    }

    if (row == VGA_HEIGHT) {
        scroll();
        row = VGA_HEIGHT - 1;
        col = 0;
    }
}

