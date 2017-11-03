/**
 * Implementation of LAPIC- and I/O APIC-related functionality.
 *
 * References (Local APIC)
 * Intel Software Developer's Manual, Vol. 3A, Ch. 10
 * https://github.com/mit-pdos/xv6-public/blob/master/lapic.c
 * http://wiki.osdev.org/APIC
 *
 * References (I/O APIC)
 * http://www.intel.com/design/chipsets/datashts/29056601.pdf
 * http://wiki.osdev.org/IOAPIC
 * https://github.com/mit-pdos/xv6-public/blob/master/ioapic.c
 */
#include "apic.h"
#include "vm.h"
#include "x86.h"
#include "vga.h"
#include "kbd.h"
#include "ide.h"
#include "string.h"
#include "kprintf.h"


char kbd_buf[0x200];
uint16_t kbd_buf_sz;
volatile uint8_t kbd_done;

char kbd_US[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',  '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};


static void lapic_timer_enable(void)
{
    uint32_t volatile *reg;

    /* Divide Configuration Register */
    /* Divide by 1 */
    reg = (uint32_t *) APIC_TMR_DIV_REG;
    *reg |= APIC_TMR_DIV_ONE;

    /* Initial Count Register */
    reg = (uint32_t *) APIC_TMR_INIT_REG;
    *reg = APIC_TMR_INIT_CNT;

    /* Timer Register */
    /* Periodic, mapped to IRQ 32 */
    reg = (uint32_t *) APIC_LVT_TMR_REG;
    *reg = APIC_TMR_PERIODIC | APIC_TMR_INTERRUPT;
}

static void ioapic_write(uint8_t offset, uint32_t val)
{
    uint32_t *ioregsel = (uint32_t *) 0xFEC00000,
             *iowin = (uint32_t *) 0xFEC00010;

    *ioregsel = offset;
    *iowin = val;
}

static void lapic_init(void)
{
    uint32_t edx, eax;
    uint32_t volatile *svr;

    /* Hardware enable the APIC */
    rdmsr(IA32_APIC_BASE, &edx, &eax);
    eax |= APIC_HW_ENABLE;
    wrmsr(IA32_APIC_BASE, edx, eax);

    /* Software enable the APIC */
    svr = (uint32_t *) APIC_SPUR_REG;
    *svr |= APIC_SW_ENABLE;

    /* Enable the APIC timer */
    lapic_timer_enable();
}

void apic_eoi(void)
{
    uint32_t volatile *eoi_reg = (uint32_t *) APIC_EOI_REG;

    *eoi_reg = APIC_EOI;
}

static uint8_t lapic_id(void)
{
    uint32_t volatile *apicid;

    apicid = (uint32_t *) APIC_ID_REG;
    return (uint8_t) (*apicid >> 24);
}

static void ioapic_redirect(uint8_t from, uint8_t to, uint32_t flags)
{
    uint8_t IOREDTBL = 0x10;

    ioapic_write(IOREDTBL + 2 * from, to | flags);
    ioapic_write(IOREDTBL + 2 * from + 1, lapic_id() << 24);
}

static void ioapic_init(void)
{
    int i;
    uint32_t IOAPIC_DISABLE = 1 << 16;

    for (i = 0; i < 24; i++) {
        ioapic_redirect(i, APIC_REMAP_START + i, IOAPIC_DISABLE);
    }
}

void apic_init(void)
{
    /* Remap PIC */
    outb(PIC_M_CMD, PIC_INIT_CONF);
    outb(PIC_S_CMD, PIC_INIT_CONF);
    outb(PIC_M_DATA, PIC_M_REMAP);
    outb(PIC_S_DATA, PIC_S_REMAP);
    outb(PIC_M_DATA, PIC_M_ACK_S);
    outb(PIC_S_DATA, PIC_S_ACK_S);
    outb(PIC_M_DATA, PIC_8086);
    outb(PIC_S_DATA, PIC_8086);

    /* Disable PIC */
    outb(PIC_S_DATA, PIC_MASK_ALL);
    outb(PIC_M_DATA, PIC_MASK_ALL);

    lapic_init();
    ioapic_init();

    /* Remap and enable the keyboard (IRQ 1) */
    ioapic_redirect(APIC_IRQ_KBD, APIC_REMAP_START + APIC_IRQ_KBD, 0);
    ioapic_redirect(APIC_IRQ_IDE, APIC_REMAP_START + APIC_IRQ_IDE, 0);
}

static void apic_handle_timer(void)
{
    static int ticks, odd = 0;

    ticks++;

    odd = !odd;
}


static void apic_handle_kbd(void)
{
    static uint8_t kbd_flags = 0;
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {                              /* key was released */
    } else {
        if (scancode == 29) {   /* Ctrl */
            kbd_flags |= (1 << 0);
        } else if (scancode == 42 || scancode == 54) {  /* Shift */
            kbd_flags |= (1 << 1);
        } else if (scancode == 56) {                    /* Alt */
            kbd_flags |= (1 << 2);
        } else if (scancode == 58) {                    /* Caps */
            kbd_flags |= (1 << 3);
        } else {
            if (kbd_US[scancode] != '\n') {
                kbd_buf[kbd_buf_sz++] = kbd_US[scancode];
                vga_putchar(kbd_US[scancode]);
            } else {
                kbd_buf[kbd_buf_sz++] = '\0';
                kbd_done = 1;
            }
        }
    }
}

void apic_handle_irq(uint32_t irq_no)
{
    switch (irq_no - APIC_REMAP_START) {
        case APIC_IRQ_TMR:
            apic_handle_timer();
            break;
        case APIC_IRQ_KBD:
            apic_handle_kbd();
            break;
        case APIC_IRQ_IDE:
            ide_handle_irq();
            break;
    }

    apic_eoi();
}

int kbd_get_str(char *str)
{
    int n;

    while (!kbd_done)
        ;

    n = kbd_buf_sz;
    kbd_done = kbd_buf_sz = 0;

    memcpy(str, kbd_buf, n);

    return n;
}

