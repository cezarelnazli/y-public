#include "string.h"
#include "kprintf.h"
#include "pm.h"
#include "vm.h"

#define MNGR_SIZE (1 << 15)
#define PGSIZE 0x1000

static uint32_t pm_mngr[MNGR_SIZE];


static inline void pm_lock_frame(uint32_t where)
{
    pm_mngr[where >> 17] |= 1 << ((where >> 12) & 31);
}


void pm_free_frame(uint32_t where)
{
    pm_mngr[where >> 17]  &= ~(1 << ((where >> 12) & 31));
}


void *pm_alloc_frame(void)
{
    uint32_t block, frame;

    for (block = 0; block < MNGR_SIZE; block++) {
        if (pm_mngr[block] != 0xFFFFFFFF) {
            for (frame = 0; frame < 32; frame++) {
                if (!(pm_mngr[block] & (1 << frame))) {
                    pm_lock_frame(((block << 5) + frame) << 12);
                    return (void *) (((block << 5) + frame) << 12);
                }
            }
        }
    }

    kprintf(OUTDEV_COM, "pm_alloc_frame: out of memory\n");
    return 0;
}


void pm_init(const multiboot_info_t *mbi)
{
    multiboot_memory_map_t *mmap;
    uint32_t start;

    memset(pm_mngr, 0xFF, sizeof pm_mngr);

    /**
     * If there is a memory map, use it.
     * Otherwise, mark as free everything from `ekernel` to memory size.
     */
    if (mbi->flags & MULTIBOOT_INFO_MEM_MAP) {
        kprintf(OUTDEV_COM, "pm_init: using multiboot memory map\n");

        for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
                (uint32_t) mmap < mbi->mmap_addr + mbi->mmap_length;
                mmap = (multiboot_memory_map_t *)
                ((unsigned) mmap + mmap->size + sizeof(mmap->size))) {
            if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
                for (start = mmap->addr; start < mmap->len; start += PGSIZE) {
                    if (start > (uint32_t) KH2P(ekernel)) {
                        pm_free_frame(start);
                    }
                }
            }
        }
    } else {
        kprintf(OUTDEV_COM, "pm_init: using multiboot mem_upper field\n");

        for (start = (uint32_t) KH2P(ekernel);
                start < mbi->mem_upper;
                start += PGSIZE) {
            pm_free_frame(start);
        }
    }
}

