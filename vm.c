#include "kprintf.h"
#include "pm.h"
#include "string.h"
#include "vm.h"
#include "x86.h"

pde_t *kpdir;
static struct vm_cache *kcache, *iocache;


void *vm_alloc_pte(void)
{
    void *new_ptable;

    if ((new_ptable = pm_alloc_frame()) == 0) {
        kprintf(OUTDEV_COM, "\tvm_alloc_pte\n");
        return (void *) 0;
    }

    memset((void *) P2KH(new_ptable), 0x00, PGSIZE);

    return new_ptable;
}


int vm_map_page(pde_t *pdir,
                uint32_t pa,
                uint32_t va,
                uint32_t flags,
                struct vm_cache *cache)
{
    pte_t *ptbl;
    pde_t *entry;

    if ((*(entry = &pdir[PDIRE(va)]) & PTE_P) == 0) {
        if (USE_CACHE(cache, va)) {
            entry = &cache->pages[PDIRE(va - cache->base)];
            if ((*entry & PTE_P) == 0) {
                if ((ptbl = (pte_t *) vm_alloc_pte()) == 0) {
                    kprintf(OUTDEV_COM, "\t\tvm_alloc_pte (1)\n");
                    return 1;
                }
            } else {
                ptbl = (pte_t *) *entry;
            }
        } else {
            if ((ptbl = (pte_t *) vm_alloc_pte()) == 0) {
                kprintf(OUTDEV_COM, "\t\tvm_alloc_pte (2)\n");
                return 2;
            }
        }

        pdir[PDIRE(va)] = (pde_t) ptbl | PTE_P | flags;
        if (USE_CACHE(cache, va)) {
            *entry = (pde_t) ptbl | PTE_P | flags;
        }
    } else {
        ptbl = (pte_t *) *entry;
    }

    ptbl = (pte_t *) ((uint32_t) P2KH(ptbl) & 0xFFFFF000);
    ptbl[PTBLE(va)] = (pa & 0xFFFFF000) | PTE_P | flags;

    return 0;
}


/* Maps the first 256 MiB of memory into the higher half. */
static int vm_map_kernel(pde_t *pdir)
{
    uint32_t va;

    for (va = KHALF; va < (uint32_t) P2KH(KHEAP); va += PGSIZE) {
        if (vm_map_page(pdir, (uint32_t) KH2P(va), va, PTE_W, kcache) != 0) {
            kprintf(OUTDEV_COM, "\t\t\tvm_map_kernel\n");
            return 1;
        }
    }

    return 0;
}


/**
 * Identity maps the memory needed for doing memory-mapped I/O.
 * MMIO is used when settings up the APIC.
 */
static int vm_map_io(pde_t *pdir)
{
    uint32_t va;

    for (va = IO_START; va != 0; va += PGSIZE) {
        if (vm_map_page(pdir, va, va, PTE_W, iocache) != 0) {
            kprintf(OUTDEV_COM, "\t\t\tvm_map_io\n");
            return 1;
        }
    }

    return 0;
}


pde_t *vm_new_pdir(void)
{
    pde_t *pdir;

    if ((pdir = (pde_t *) vm_alloc_pte()) == 0) {
        kprintf(OUTDEV_COM, "\t\tvm_new_pdir\n");
        return 0;
    }

    pdir = (pde_t *) P2KH(pdir);

    if (vm_map_kernel(pdir) != 0) {
        kprintf(OUTDEV_COM, "\t\t\t\tvm_new_pdir\n");
        return 0;
    }

    if (vm_map_io(pdir) != 0) {
        kprintf(OUTDEV_COM, "\t\t\t\tvm_new_pdir\n");
        return 0;
    }

    return pdir;
}

int vm_init(void)
{
    kcache = (struct vm_cache *) P2KH(pm_alloc_frame());
    if ((uint32_t) kcache == P2KH(0)) {
        kprintf(OUTDEV_COM, "\tvm_init\n");
        return 1;
    }
    kcache->base = KHALF;
    kcache->limit = KHALF + KHEAP;

    iocache = (struct vm_cache *) P2KH(pm_alloc_frame());
    if ((uint32_t) iocache == P2KH(0)) {
        kprintf(OUTDEV_COM, "\tvm_init\n");
        return 1;
    }
    iocache->base = IO_START;
    iocache->limit = IO_END;

    if ((kpdir = vm_new_pdir()) == 0) {
        kprintf(OUTDEV_COM, "\t\t\t\t\tvm_init\n");
        return 1;
    }

    lcr3((pde_t *) KH2P(kpdir));
    return 0;
}

