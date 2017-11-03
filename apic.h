/**
 * References
 * Intel Software Developer's Manual, Vol. 3A, Ch. 10
 * http://wiki.osdev.org/APIC_timer
 * http://wiki.osdev.org/APIC
 * http://wiki.osdev.org/8259_PIC
 */
#ifndef APIC_H
#define APIC_H

#include <stdint.h>

#define APIC_REMAP_START    0x20
#define APIC_REMAP_END      (APIC_REMAP_START + 0x10)
#define APIC_IRQ_TMR        0
#define APIC_IRQ_KBD        1
#define APIC_IRQ_IDE        14

/**
 * These are actually 32-bit values, but uint8_t is used in order for the
 * arithmetic to make more sense (e.g. `APIC_REGISTER_BASE + 0x3E0` instead
 * of something like `APIC_REGISTER_BASE + (0x3E0 >> 2)`).
 *
 * xv6 defines macros to the offsets divided by sizeof (uint32_t), which does
 * make sense, but this is the version I prefer, for now.
 */
#define APIC_REGISTER_BASE  0xFEE00000

#define APIC_TMR_DIV_REG    (APIC_REGISTER_BASE + 0x3E0)
#define APIC_TMR_INIT_REG   (APIC_REGISTER_BASE + 0x380)
#define APIC_LVT_TMR_REG    (APIC_REGISTER_BASE + 0x320)

#define APIC_TMR_DIV_ONE    0xB
#define APIC_TMR_INIT_CNT   1000000000
#define APIC_TMR_PERIODIC   (1 << 17)
#define APIC_TMR_INTERRUPT  (APIC_REMAP_START + APIC_IRQ_TMR)

#define IA32_APIC_BASE      0x1B
#define APIC_HW_ENABLE      (1 << 11)

#define APIC_SPUR_REG       (APIC_REGISTER_BASE + 0xF0)
#define APIC_SW_ENABLE      (1 << 8)

#define APIC_EOI_REG        (APIC_REGISTER_BASE + 0xB0)
#define APIC_EOI            0

#define APIC_ID_REG         (APIC_REGISTER_BASE + 0x20)

/**
 * Required for disabling and remapping the 8259 PIC.
 */
#define PIC_M_CMD       0x20
#define PIC_S_CMD       0xA0
#define PIC_M_DATA      0x21
#define PIC_S_DATA      0xA1
#define PIC_INIT_CONF   0x11
#define PIC_M_REMAP     0x20
#define PIC_S_REMAP     0x28
#define PIC_M_ACK_S     0x04
#define PIC_S_ACK_S     0x02
#define PIC_8086        0x01
#define PIC_MASK_ALL    0xFF

#define CPUID_APIC_AVAILABLE (1 << 9)

extern void apic_init(void);
extern void apic_eoi(void);
extern void apic_handle_irq(uint32_t irq_no);

#endif
