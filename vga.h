#ifndef VGA_H
#define VGA_H

#include <stdint.h>

enum vga_color {
    VGA_BLACK,
    VGA_BLUE,
    VGA_GREEN,
    VGA_CYAN,
    VGA_RED,
    VGA_MAGENTA,
    VGA_BROWN,
    VGA_LGREY,
    VGA_DGREY,
    VGA_LBLUE,
    VGA_LGREEN,
    VGA_LCYAN,
    VGA_LRED,
    VGA_LMAGENTA,
    VGA_LBROWN,
    VGA_WHITE
};

extern void vga_init(void);
extern void vga_set_color(enum vga_color fg, enum vga_color bg);
extern uint16_t vga_encode_char(char ch);
extern void vga_putchar(char ch);

#endif
