/**
 * Keyboard layouts, represented as arrays of ASCII characters indexed by
 * scan code.
 *
 * References
 * http://www.osdever.net/bkerndev/Docs/keyboard.htm
 */
#ifndef KBD_H
#define KBD_H

extern char kbd_US[];
extern char kbd_buf[];

extern int kbd_get_str(char *str);

#endif

