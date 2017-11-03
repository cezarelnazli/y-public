#ifndef TABLES_H
#define TABLES_H

#include <stddef.h>
#include <stdint.h>

#define DPL_KERN 0
#define DPL_USER 3

#define GDT_ACCESS_WRITE (1 << 1)
#define GDT_ACCESS_EXEC (1 << 3)
#define GDT_ACCESS_KMODE 0
#define GDT_ACCESS_UMODE ((DPL_USER) << 5)
#define GDT_ACCESS_TSS (GDT_ACCESS_UMODE | 9)

#define IDT_ATTR_PRESENT (1 << 7)
#define IDT_ATTR_INTERRUPT 0xE

#define MEM_START   0x00000000
#define MEM_END     0xFFFFFFFF

typedef enum _seg_t {
    SEG_NULL,
    SEG_KCODE,
    SEG_KDATA,
    SEG_UCODE,
    SEG_UDATA,
    SEG_TSS,
} seg_t;

#define IDT_SEL_KCODE ((SEG_KCODE) << 3)

extern void gdt_init(void);
extern void gdt_add_entry(size_t base, size_t limit, uint8_t access);
extern void gdt_set_entry(size_t idx, size_t base, size_t limit, uint8_t access);

extern void idt_init(void);
extern void idt_encode(size_t idx, size_t addr, uint16_t sel, uint8_t dpl);

#endif
