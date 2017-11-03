#include <stdint.h>
#include "string.h"
#include "kprintf.h"
#include "irq.h"
#include "pm.h"
#include "tables.h"
#include "vm.h"
#include "elf.h"
#include "task.h"
#include "x86.h"
#include "ramfs.h"

extern tss_t tss;
extern void sysenter_eip_epilogue(void);
extern void fork_child_ret(void);

static uint8_t tid;
struct context *kctx;   /* Kernel "task", where `swtch()` jumps. */
struct task *fgtask;    /* Foreground (currently running) task.  */
struct task *htask;     /* Used for maintaining the linked list. */
tss_t tss;

static volatile uint8_t is_running[0x200];

static void scheduler(void)
{
    for (fgtask = fgtask->next; ; fgtask = fgtask->next) {
        if (fgtask->state != RUNNABLE)
            continue;

        fgtask->state = RUNNING;
        lcr3((pde_t *) KH2P(fgtask->pdir));

        tss.esp0 = (uint32_t) fgtask->kstack;
        tss.ss0 = SEG_KDATA << 3;
        gdt_set_entry(SEG_TSS, (size_t) &tss, sizeof(tss) - 1, GDT_ACCESS_TSS);
        ltr(SEG_TSS << 3);

        swtch(0, &kctx, fgtask->ctx);
    }
}

void task_wait(int td)
{
    while (is_running[td])
        ;
}

void task_yield(const uint8_t *uesp)
{
    if (fgtask == 0)
        return;

    fgtask->state = RUNNABLE;
    swtch(uesp, &fgtask->ctx, kctx);
}

void task_exit(struct task *task)
{
    fgtask = task->next;
    task->prev->next = task->next;
    task->next->prev = task->prev;
    task->state = STOPPED;
    /*pm_free_frame(KH2P(task->kstack - PGSIZE + 4));*/
    /*pm_free_frame(task->regs->uesp - PGSIZE + 8);*/
    /*pm_free_frame(KH2P(task->pdir));*/
    /*pm_free_frame(KH2P(task));*/

    is_running[task->tid] = 0;

    swtch((uint8_t *) task->ctx->uesp, &task->ctx, kctx);
}

struct task *task_alloc(void)
{
    struct task *ntask;
    uint8_t *kstack;

    ntask = (struct task *) P2KH(pm_alloc_frame());
    if ((uint32_t) ntask == P2KH(0)) {
        kprintf(OUTDEV_COM, "\ttask_alloc (1)\n");
        return 0;
    }

    kstack = (uint8_t *) P2KH(pm_alloc_frame());
    if ((uint32_t) kstack == P2KH(0)) {
        kprintf(OUTDEV_COM, "\ttask_alloc (2)\n");
        return 0;
    }

    ntask->tid = tid++;

    if ((ntask->pdir = vm_new_pdir()) == 0) {
        kprintf(OUTDEV_COM, "\t\t\ttask_alloc\n");
        return 0;
    }
    lcr3((pde_t *) KH2P(ntask->pdir));

    kstack += PGSIZE - 4;
    ntask->kstack = kstack;

    kstack -= sizeof(struct irq_regs);
    ntask->regs = (struct irq_regs *) kstack;
    if ((ntask->regs->uesp = (uint32_t) pm_alloc_frame()) == 0) {
        kprintf(OUTDEV_COM, "\ttask_alloc (3)\n");
        return 0;
    }

    if (vm_map_page(ntask->pdir,
                    ntask->regs->uesp, ntask->regs->uesp,
                    PTE_W | PTE_U,
                    0) != 0) {
        kprintf(OUTDEV_COM, "\t\t\ttask_alloc\n");
        return 0;
    }

    ntask->regs->uesp += PGSIZE - 20;
    *((uint8_t *) ntask->regs->uesp) = 0;

    ntask->ctx = (struct context *) P2KH(pm_alloc_frame());
    if ((uint32_t) ntask->ctx == P2KH(0)) {
        kprintf(OUTDEV_COM, "\ttask_alloc (4)\n");
        return 0;
    }
    ntask->ctx->eip = (uint32_t) sysenter_eip_epilogue;
    ntask->ctx->uesp = ntask->regs->uesp;

    ntask->state = RUNNABLE;

    if (htask != 0) {
        ntask->next = htask;
        ntask->prev = htask->prev;

        htask->prev->next = ntask;
        htask->prev = ntask;
    } else {
        ntask->next = ntask;
        ntask->prev = ntask;
    }
    htask = ntask;

    return ntask;
}

static struct task *task_add(const uint8_t *elf)
{
    struct task *ntask;

    if (!elf_is_valid(elf)) {
        kprintf(OUTDEV_COM, "add_task: invalid ELF\n");
        return 0;
    }

    if ((ntask = task_alloc()) == 0) {
        kprintf(OUTDEV_COM, "\t\ttask_add\n");
        return 0;
    }

    if (elf_map_sections(ntask->pdir, elf) != 0) {
        kprintf(OUTDEV_COM, "\t\t\ttask_add\n");
        return 0;
    }

    *((uint32_t *) (ntask->regs->uesp + 4)) = (uint32_t) elf_get_entry(elf);

    return ntask;
}

void task_copy_vm(pde_t *npdir)
{
    uint32_t *entry_addr, *tentry_addr, *ptbl_cpy, *pdir_cpy;
    uint32_t addr = 0x08048080;
    pde_t entry;
    pte_t tentry;
    uint8_t first_time = 1;

    while (1) {
        entry = fgtask->pdir[PDIRE(addr)];
        entry_addr = P2KH(entry & 0xFFFFF000);

        if ((entry & PTE_P) == 0) {
            break;
        }

        tentry = entry_addr[PTBLE(addr)];
        tentry_addr = P2KH(tentry & 0xFFFFF000);

        if ((tentry & PTE_P) == 0) {
            break;
        }

        ptbl_cpy = P2KH(pm_alloc_frame());
        memcpy(ptbl_cpy, tentry_addr, PGSIZE);

        if (first_time) {
            pdir_cpy = P2KH(pm_alloc_frame());
            npdir[PDIRE(addr)] = KH2P(pdir_cpy) | (entry & 0xFFF);
        } else {
            pdir_cpy = P2KH(npdir[PDIRE(addr)] & 0xFFFFF000);
        }

        pdir_cpy[PTBLE(addr)] = KH2P(ptbl_cpy) | (tentry & 0xFFF);

        addr += PGSIZE;
        first_time = 0;
    }
}

int task_fork(const uint8_t *uesp)
{
    struct task *ntask;

    if ((ntask = task_alloc()) == 0) {
        kprintf(OUTDEV_COM, "\t\ttask_fork\n");
        return -1;
    }

    memcpy(ntask->pdir, fgtask->pdir, PGSIZE);
    task_copy_vm(ntask->pdir);
    if (vm_map_page(ntask->pdir,
                    ntask->regs->uesp, ntask->regs->uesp,
                    PTE_W | PTE_U,
                    0) != 0) {
        kprintf(OUTDEV_COM, "\t\t\ttask_fork\n");
        return 0;
    }

    memcpy(ntask->kstack - PGSIZE + 4, fgtask->kstack - PGSIZE + 4, PGSIZE);
    swtch_save((uint8_t *) ntask->ctx);
    ntask->ctx->eip = (uint32_t) fork_child_ret;
    *((uint32_t *) (ntask->ctx->uesp + 4)) = *(uint32_t *) (uesp + 4);
    *((uint32_t *) (ntask->regs->uesp + 4)) = *(uint32_t *) (uesp + 4);

    is_running[ntask->tid] = 1;

    return ntask->tid;
}

int task_init(void)
{
    struct task *ntask;

    if ((ntask = task_add(usr_shell)) == (struct task *) 0) {
        kprintf(OUTDEV_COM, "task_init: cannot init task 1\n");
    } else if (fgtask == 0) {
        fgtask = ntask;
    }

    kctx = (struct context *) P2KH(pm_alloc_frame());
    if ((uint32_t) kctx == P2KH(0)) {
        kprintf(OUTDEV_COM, "\ttask_add (4)\n");
        return 1;
    }
    kctx += 1;

    if (fgtask != 0) {
        scheduler();
        return 0;
    } else {
        return 1;
    }
}

