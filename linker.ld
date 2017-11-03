ENTRY(start)   /* defined in bootstrap.S */

KHALF = 0xC0000000;
KVA = KHALF + 1M;

/* align to 4K so that each section is on a separate page */
SECTIONS
{
    . = KVA;

    .text BLOCK(4K) : AT(ADDR(.text) - KHALF) ALIGN(4K)
    {
        *(.mboot)
        *(.text)
    }

    .rodata BLOCK(4K) : AT(ADDR(.rodata) - KHALF) ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : AT(ADDR(.data) - KHALF) ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : AT(ADDR(.bss) - KHALF) ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /**
     * `ekernel` (end of kernel) is referenced in the kernel (specifically, used
     * by the memory manager to know where to begin allocating new memory) and
     * defined by the linker here, because it's easiest for the linker to
     * provide this information.
     */
    . = ALIGN(4K);
    PROVIDE(ekernel = .);
}

