/**
 * References
 * Intel Software Developer's Manual, Vol. 3A, Ch. 4
 * http://wiki.osdev.org/Paging
 * https://github.com/mit-pdos/xv6-public/blob/master/mmu.h
 */
#ifndef VM_H
#define VM_H

#include <stdint.h>

/**
 * +----------------------+------------------+--------+
 * | Page Directory Entry | Page Table Entry | Offset |
 * +----------------------+------------------+--------+
 * 31                   22 21              12 11      0
 */
#define PDIRE(x) (((x) >> 22) & 0x3FF)
#define PTBLE(x) (((x) >> 12) & 0x3FF)

/**
 * Packed bitfield structs would not work, because they need to be aligned on
 * word boundries, so we would end up having part of the address in one member,
 * and part of the address in a different member.
 */
typedef uint32_t pde_t;
typedef uint32_t pte_t;

/* Shamelessly copied from xv6's mmu.h */
#define PTE_P 0x001
#define PTE_W 0x002
#define PTE_U 0x004
#define PTE_PWT 0x008
#define PTE_PCD 0x010
#define PTE_A 0x020
#define PTE_D 0x040
#define PTE_PS 0x080
#define PTE_MBZ 0x180

#define PGSIZE      0x1000
#define KHALF       0xC0000000  /* Start of Higher Half (3 GiB) */
#define KVA         0xC0100000  /* Kernel Virtual Address (3 GiB + 1 MiB) */
#define KHEAP       0x10000000
#define IO_START    0xFE000000
#define IO_END      0xFFFFFFFF

extern uint8_t ekernel[];       /* end of kernel, defined in `linker.ld` */
extern pde_t *kpdir;

#define KH2P(x) (((uint32_t) x) - KHALF)
#define P2KH(x) (((uint32_t) x) + KHALF)
#define USE_CACHE(cache, va) ((cache) && \
        (cache->base <= va) && \
        (va <= cache->limit))

struct vm_cache {
    pde_t pages[64];
    uint32_t base;
    uint32_t limit;
};

extern pde_t *vm_new_pdir(void);
extern int vm_init(void);
extern int vm_map_page(pde_t *pdir,
                       uint32_t pa,
                       uint32_t va,
                       uint32_t flags,
                       struct vm_cache *cache);
extern int vm_map_hhalf(pde_t *pdir);

#endif
