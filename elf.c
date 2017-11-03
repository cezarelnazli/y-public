#include <stdint.h>
#include "pm.h"
#include "string.h"
#include "vm.h"
#include "elf.h"
#include "kprintf.h"


/**
 * An ELF is valid and we can load it if it starts with {'\x7F', 'E', 'L', 'F'},
 * it is 32-bit, and the version is EV_CURRENT.
 */
int elf_is_valid(const uint8_t *content)
{
    Elf32_Ehdr *header = (Elf32_Ehdr *) content;

    return IS_ELF(*header) &&
           header->e_ident[EI_CLASS] == ELFCLASS32 &&
           header->e_ident[EI_VERSION] == EV_CURRENT;
}


/**
 * Goes through each segment in the program header and, if the segment must be
 * loaded, physical memory is allocated to hold it, and its contents are copied
 * to the new location.
 */
int elf_map_sections(pde_t *pdir, const uint8_t *content)
{
    Elf32_Ehdr *header = (Elf32_Ehdr *) content;
    Elf32_Addr phoff;
    Elf32_Phdr *eph;
    Elf32_Half pnum;
    Elf32_Word i, to_copy;
    uint8_t *pg;

    for (pnum = 0, phoff = header->e_phoff;
            pnum < header->e_phnum;
            pnum++, phoff = phoff + header->e_phentsize) {

        eph = (Elf32_Phdr *) ((uint32_t) header + phoff);

        if (eph->p_type == PT_LOAD) {

            for (i = 0; i < eph->p_filesz; i += PGSIZE) {

                if (eph->p_vaddr + i >= KHALF) {
                    kprintf(OUTDEV_COM,
                            "elf_map_sections: address too high (0x%x)\n",
                            eph->p_vaddr + i);
                    return 1;
                }

                if ((pg = pm_alloc_frame()) == 0) {
                    kprintf(OUTDEV_COM, "\telf_map_sections\n");
                    return 1;
                }

                if ((to_copy = eph->p_filesz - i) > PGSIZE) {
                    to_copy = PGSIZE;
                }

                memcpy((void *) (P2KH(pg) + eph->p_offset + i),
                       (void *) (content + eph->p_offset + i),
                       to_copy);
                if (vm_map_page(pdir,
                                (uint32_t) pg,
                                eph->p_vaddr + i,
                                PTE_W | PTE_U,
                                0) != 0) {
                    kprintf(OUTDEV_COM, "\t\t\telf_map_sections\n");
                    return 1;
                }

            }
        }
    }

    return 0;
}

/**
 * Returns the address of the entry point (e.g. address of main).
 */
entry_t elf_get_entry(const uint8_t *content)
{
    Elf32_Ehdr *header = (Elf32_Ehdr *) content;

    return (entry_t) header->e_entry;
}

