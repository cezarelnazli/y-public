#include "com.h"
#include "vm.h"
#include "x86.h"

static int have_data(void)
{
    return inb(COM1 + COM_STAT_LINE) & 1;
}

static int is_clear(void)
{
    return inb(COM1 + COM_STAT_LINE) & 0x20;
}

void com_init(void)
{
    outb(COM1 + COM_INT, COM_INT_DISABLE);
    outb(COM1 + COM_CTRL_LINE, COM_DLAB_ENABLE);
    outb(COM1 + COM_DIV_LO, 3);
    outb(COM1 + COM_DIV_HI, 0);
    outb(COM1 + COM_CTRL_LINE, COM_LINE_PROTO);
    outb(COM1 + COM_FIFO, COM_FIFO_ENABLE);
    outb(COM1 + COM_CTRL_MDEM, COM_IRQ_ENABLE);
}

char com_read(void)
{
    while (!have_data())
        ;
    return inb(COM1);
}

void com_write(char ch)
{
    while (!is_clear())
        ;
    outb(COM1, ch);
}

