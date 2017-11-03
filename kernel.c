#include "apic.h"
#include "com.h"
#include "kprintf.h"
#include "multiboot.h"
#include "pm.h"
#include "tables.h"
#include "vm.h"
#include "elf.h"
#include "task.h"
#include "vga.h"
#include "x86.h"
#include "syscall.h"
#include "ide.h"

void kmain(const multiboot_info_t *mbi)
{
    com_init();
    pm_init(mbi);

    if (vm_init() != 0) {
        kprintf(OUTDEV_COM, "\t\tkmain\n");
        return;
    }

    gdt_init();
    idt_init();
    vga_init();
    apic_init();
    kprintf(OUTDEV_COM, "ekernel=0x%x\n", ekernel);
    kprintf(OUTDEV_VGA, "ekernel=0x%x\n", ekernel);
    syscall_init();

    if (task_init() != 0) {
        kprintf(OUTDEV_COM, "\t\tkmain\n");
    }

    for (;;)
        ;
}

