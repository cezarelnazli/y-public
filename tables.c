/**
 * This file contains the C code for setting up the GDT and the IDT. Although
 * they are not logically very similar, they were merged because they are
 * similar enough in structure.
 *
 * References
 * http://www.osdever.net/bkerndev/Docs/gdt.htm
 * http://wiki.osdev.org/GDT_Tutorial
 * http://wiki.osdev.org/GDT
 * http://www.osdever.net/bkerndev/Docs/idt.htm
 * http://wiki.osdev.org/Interrupt_Descriptor_Table
 */
#include "irq.h"
#include "tables.h"
#include "vm.h"
#include "x86.h"


struct gdt_entry {
    uint16_t limit0;
    uint16_t base0;
    uint8_t base16;
    uint8_t access;
    uint8_t limit16 : 4;
    uint8_t flags : 4;
    uint8_t base24;
} GDT[6];


struct idt_entry {
    uint16_t base0;
    uint16_t selector;      /* offset into the GDT */
    uint8_t reserved;       /* "reserved" fields must be 0 */
    uint8_t attr;           /* constant values and variable DPL */
    uint16_t base16;
} IDT[0x100];


struct tbl_ptr {
    uint16_t limit;
    size_t address;
} __attribute__((packed))
gdtp, idtp;


void gdt_set_entry(size_t idx, size_t base, size_t limit, uint8_t access)
{
    GDT[idx].limit0 = limit & 0xFFFF;
    GDT[idx].limit16 = (limit >> 16) & 0x0F;

    GDT[idx].base0 = base & 0xFFFF;
    GDT[idx].base16 = (base >> 16) & 0xFF;
    GDT[idx].base24 = (base >> 24) & 0xFF;

    if (access == GDT_ACCESS_TSS) {
        GDT[idx].access = 0x80 | access;
        GDT[idx].flags = 0;
    } else {
        GDT[idx].access = 0x90 | GDT_ACCESS_WRITE | access;
        GDT[idx].flags = 0xC;
    }
}


void gdt_add_entry(size_t base, size_t limit, uint8_t access)
{
    static uint8_t idx = 0;

    gdt_set_entry(idx, base, limit, access);

    idx++;
}


void gdt_init(void)
{
    gdtp.limit = sizeof(GDT) - 1;
    gdtp.address = (size_t) GDT;

    /* NULL segment */
    gdt_add_entry(0, 0, 0);

    /* kernel code segment */
    gdt_add_entry(MEM_START, MEM_END, GDT_ACCESS_KMODE | GDT_ACCESS_EXEC);

    /* kernel data segment */
    gdt_add_entry(MEM_START, MEM_END, GDT_ACCESS_KMODE);

    /* user code segment */
    gdt_add_entry(MEM_START, MEM_END, GDT_ACCESS_UMODE | GDT_ACCESS_EXEC);

    /* user data segment */
    gdt_add_entry(MEM_START, MEM_END, GDT_ACCESS_UMODE);

    /* Defined in `bootstrap.S` */
    lgdt();
}


void idt_encode(size_t idx, size_t addr, uint16_t sel, uint8_t dpl)
{
    IDT[idx].base0 = addr & 0xFFFF;
    IDT[idx].base16 = (addr >> 16) & 0xFFFF;

    IDT[idx].selector = sel;
    IDT[idx].attr = IDT_ATTR_PRESENT | IDT_ATTR_INTERRUPT | (dpl << 5);

    IDT[idx].reserved = 0x00;
}


void idt_init(void)
{
    idtp.limit = sizeof(IDT) - 1;
    idtp.address = (size_t) IDT;

    irq_setup();

    lidt();      /* Defined in `bootstrap.S` */
}

