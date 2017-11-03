; This is the bootstrap file. It is meant to take care of the initialization
; steps specific to the x86 architecture, and to allow everything else to be
; done in C.
;
; http://www.osdever.net/bkerndev/Docs/basickernel.htm
; https://github.com/mit-pdos/xv6-public/blob/master/entry.S

%include "constants.S"

; The Multiboot Specification states that the header must be 32-bit aligned.
; https://gnu.org/software/grub/manual/multiboot/multiboot.html#OS-image-format
;
ALIGN 4
SECTION .mboot
dd MBOOT_MAGIC
dd MBOOT_FLAGS
dd MBOOT_CKSUM

SECTION .bss
ALIGN 0x1000
init_pgtbl:
    resd 1024

stack_bottom:
    resb 0x2000
stack_top:

SECTION .text
GLOBAL start
EXTERN kmain
start:

    ; enable pse for simpler initial paging
    mov ecx, cr4
    or ecx, CR4_PSE
    mov cr4, ecx

    xor ecx, ecx
    or ecx, PTE_PSE | PTE_PW    ; present, writeable, 4 MiB page

    ; identity map first 4 MiB for the rest of this section
    mov edx, V2P(init_pgtbl)
    mov [edx], ecx

    add edx, 768 * 4
    mov [edx], ecx

    ; load the page directory into cr3
    mov ecx, V2P(init_pgtbl)
    mov cr3, ecx

    ; enable paging
    mov ecx, cr0
    or ecx, CR0_PAGE | CR0_WP
    mov cr0, ecx

    ; setup stack
    mov esp, stack_top

    ; call kmain
    add ebx, KHALF          ; GRUB puts MBI's physical address in EBX
    push ebx
    push ebx
    mov ecx, kmain
    jmp ecx
    hlt
