#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

#define IRQ_PGFAULT 14

struct irq_regs
{
    /* DS is pushed manually, and used when switching to ring 3 */
    uint32_t ds;

    /* These are pushed by `pushad` in `irq.S` (generated from Python) */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

    /* Pushed by our wrapper in order to identify the IRQ */
    uint32_t int_no;

    /* These are pushed by the processor when the interrupt is received,
     * and control is transferred to the routines specified in the IDT.
     * The last two are pushed when the privilege level changes.
     *
     * More information can be found in
     * Intel's Software Programmer Manual, Volume 1, Chapter 6.
     */
    uint32_t err_code, eip, cs, eflags, uesp, ss;
};

extern void irq_setup(void);

#endif
