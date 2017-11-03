#include "stdint.h"
#include "vm.h"
#include "elf.h"
#include "exec.h"
#include "task.h"
#include "kprintf.h"
#include "x86.h"

int sys_exec(uint8_t *uesp, const uint8_t *elf)
{
    if (!elf_is_valid(elf)) {
        kprintf(OUTDEV_VGA, "sys_exec: invalid ELF (0x%x)\n", elf);
        return 1;
    }

    if (elf_map_sections(fgtask->pdir, elf) != 0) {
        kprintf(OUTDEV_COM, "\t\t\tsys_exec\n");
        return 2;
    }

    *(uesp + 4) = (uint32_t) elf_get_entry(elf);
    invlpg((uint32_t) elf_get_entry(elf));

    return 0;
}

