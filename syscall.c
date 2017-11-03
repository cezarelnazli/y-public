#include "stdint.h"
#include "kprintf.h"
#include "pm.h"
#include "irq.h"
#include "syscall.h"
#include "tables.h"
#include "vm.h"
#include "elf.h"
#include "task.h"
#include "x86.h"
#include "exec.h"
#include "kbd.h"

#define IA32_SYSENTER_CS    0x174
#define IA32_SYSENTER_ESP   0x175
#define IA32_SYSENTER_EIP   0x176

enum {
    SYS_PRINT,
    SYS_YIELD,
    SYS_EXIT,
    SYS_FORK,
    SYS_EXEC,
    SYS_WAIT,
    SYS_PRINTS,
    SYS_READ
};


static int sys_print(const uint8_t *args)
{
    int num = *(int *) args;

    kprintf(OUTDEV_VGA, "%d", num);

    return 0;
}


int sys_prints(const uint8_t *args)
{
    char *str = *(char **) args;

    kprintf(OUTDEV_VGA, str);

    return 0;
}


int syscall_handle(uint8_t *uesp)
{
    uint32_t sys_num = *(uint32_t *) uesp;
    const uint8_t *args = uesp + 8;

    switch (sys_num) {
        case SYS_PRINT:
            return sys_print(args);
        case SYS_PRINTS:
            return sys_prints(args);
        case SYS_YIELD:
            task_yield(uesp);
            return 0;
        case SYS_EXIT:
            task_exit(fgtask);
            return 0;
        case SYS_FORK:
            return task_fork(uesp);
        case SYS_EXEC:
            return sys_exec(uesp, *(uint8_t **) args);
        case SYS_WAIT:
            task_wait(*(int *) args);
            return 0;
        case SYS_READ:
            kbd_get_str(*(char **) args);
            return 0;
    }

    return 1;
}

void syscall_init(void)
{
    wrmsr(IA32_SYSENTER_CS, 0, SEG_KCODE << 3);
    wrmsr(IA32_SYSENTER_ESP, 0, P2KH(pm_alloc_frame()));
    wrmsr(IA32_SYSENTER_EIP, 0, (uint32_t) sysenter_eip);
}

