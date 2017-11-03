#include "apic.h"
#include "stdint.h"
#include "kprintf.h"
#include "irq.h"
#include "syscall.h"
#include "tables.h"
#include "vga.h"
#include "vm.h"
#include "task.h"
#include "x86.h"

/**
 * The "coprocessor" is the x87 FPU.
 * The descriptions are copied from the manual.
 *
 * References
 * Intel Software Developer's Manual Volume 3A, Part 1
 */
static char *err_msg[] = {
    [0] = "Divide Error",
    [1] = "Debug Exception",
    [2] = "NMI Interrupt",
    [3] = "Breakpoint",
    [4] = "Overflow",
    [5] = "BOUND Range Exceeded",
    [6] = "Invalid Opcode (Undefined Opcode)",
    [7] = "Device Not Available (No Math Coprocessor)",
    [8] = "Double Fault",
    [9] = "Coprocesor Segment Overrun",
    [10] = "Invalid TSS",
    [11] = "Segment Not Present",
    [12] = "Stack-Segment Fault",
    [13] = "General Protection",
    [14] = "Page Fault",
    [15] = "Intel Reserved",
    [16] = "x87 FPU Floating-Point Error (Math Fault)",
    [17] = "Alignment Check",
    [18] = "Machine Check",
    [19] = "SIMD Floating-Point Exception",
    [20] = "Virtualization Exception",
};

/* generated from Python script */
%s

void irq_setup(void)
{
    /* generated from Python */
%s
}

static void irq_pgfault(const struct irq_regs *regs)
{
    kprintf(OUTDEV_COM, "pf_addr=0x%%x\n", rcr2());
    kprintf(OUTDEV_COM, "eip=0x%%x\n", regs->eip);
    kprintf(OUTDEV_COM, "task=%%d\n", fgtask->tid);
    kprintf(OUTDEV_COM, "uesp=0x%%x\n", regs->uesp);
    kprintf(OUTDEV_COM, "pdir=0x%%x\n\n", fgtask->pdir);
}


static void irq_default(const struct irq_regs *regs)
{
    if (regs->int_no < APIC_REMAP_START) {
        vga_set_color(VGA_RED, VGA_LGREY);
        kprintf(OUTDEV_COM, err_msg[regs->int_no]);
        vga_set_color(VGA_WHITE, VGA_BLACK);
    }
}


void handle_irq(struct irq_regs *regs)
{
    /* PIC interrupts */
    if (regs->int_no >= APIC_REMAP_START && regs->int_no < APIC_REMAP_END) {
        apic_handle_irq(regs->int_no);
    }

    /* hardware interrupts + sys calls */
    switch (regs->int_no) {
        case IRQ_PGFAULT:
            irq_pgfault(regs);
            break;
        default:
            irq_default(regs);
            break;
    }

    task_yield((uint8_t *) regs->uesp);
}

