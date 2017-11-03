#!/usr/bin/env python
"""
This script prints to stdout NASM-compatible Assembly for the 32 ISRs
corresponding to x86 processor interrupts, as well as the common part
which makes the jump to C. The ISRs need to be written in ASM because
of stack alignment issues. For more details, see [1].

The idea of generating these functions in a scripting language was taken
from MIT's xv6 (see [2]). The ISRs are written as described in [3].

The script attempts to be compatible with Python 2 and 3, so as to
minimise dependencies and requirements.

[1]: http://wiki.osdev.org/Interrupt_Service_Routines
[2]: https://github.com/mit-pdos/xv6-public/blob/master/vectors.pl
[3]: http://www.osdever.net/bkerndev/Docs/isrs.htm
"""
from __future__ import print_function
import sys

# `INTEL_ERR_CODE` is a list of exception numbers for which values are
# pushed on the stack, and for which we should not push a dummy value.
#
# References
# Intel Software Developer's Manual Volume 3A, Part 1
# http://wiki.osdev.org/James_Molloy%27s_Tutorial_Known_Bugs
INTEL_ERR_CODE = [8, 10, 11, 12, 13, 14, 17, 30]

FUNC_PROTO = """
GLOBAL irq{i}
irq{i}:
    cli
    {dummy}
    push byte {i}
    jmp irq_common
"""

IRQ_COMMON_CODE = """
EXTERN handle_irq
irq_common:
    pushad              ; in order EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
    push ds             ; DS is overwritten when switching to ring 3
    mov ax, 16
    mov ds, ax
    cld                 ; Sys V ABI requires DF to be cleared on function entry
                        ;
    push esp            ; `handle_irq` takes a pointer (an address) to the
                        ; structure. We created the structure on the stack, so
                        ; push the current stack pointer.
                        ;
    call handle_irq     ;
    add esp, 4          ; account for `push esp` two lines above
                        ;
    pop ds              ;
    popad               ;
    add esp, 8          ; account for the two bytes pushed in `irqX`
    iretd               ; pops EIP, CS, EFLAGS
                        ; and SS and ESP if privilege level change
"""

PIC = 16
X86 = 32
IRQS = [i for i in range(PIC + X86)]

def gen_asm_irq():
    """
    Generate NASM-compatible ASM for the 32 irqs.
    """

    for i in IRQS:
        if i in INTEL_ERR_CODE:
            dummy = '; pushed by processor'
        else:
            dummy = 'push byte 0'
        print(FUNC_PROTO.format(i=i, dummy=dummy))

    print(IRQ_COMMON_CODE)


def gen_irq_c():
    """
    Generate list of functions in `irq.c`
    """
    one_idt = 'idt_encode({i}, (size_t) irq{i}, IDT_SEL_KCODE, DPL_KERN);'

    irq_list = '\n'.join('extern void irq{}(void);'.format(i) for i in IRQS)
    idt_list = '\n'.join(one_idt.format(i=i) for i in IRQS)

    with open('irq_tmpl.c') as th, open('irq.c', 'w') as fh:
        tmpl = th.read()
        fh.write(tmpl.decode() % (irq_list, idt_list, ))


if __name__ == '__main__':
    if len(sys.argv) > 1:
        if sys.argv[1] == 'asm':
            gen_asm_irq()
        elif sys.argv[1] == 'irq_c':
            gen_irq_c()
