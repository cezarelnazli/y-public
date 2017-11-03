/**
 * References
 * http://wiki.osdev.org/Serial_Ports
 */
#ifndef COM_H
#define COM_H

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

#define COM_DATA_REG    0
#define COM_INT         1
#define COM_DIV_LO      0
#define COM_DIV_HI      1
#define COM_FIFO        2
#define COM_CTRL_LINE   3
#define COM_CTRL_MDEM   4
#define COM_STAT_LINE   5
#define COM_STAT_MDEM   6
#define COM_SCRATCH     7

#define COM_INT_DISABLE 0x00
#define COM_DLAB_ENABLE 0x80
#define COM_LINE_PROTO  0x03
#define COM_FIFO_ENABLE 0xC7
#define COM_IRQ_ENABLE  0x0B

extern void com_init(void);
extern char com_read(void);
extern void com_write(char ch);

#endif
