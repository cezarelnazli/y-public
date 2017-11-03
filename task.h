#ifndef TASK_H
#define TASK_H

#include "stdint.h"

typedef struct tss {
    /* used for hw multitasking; we implement sw multitasking (faster) */
    uint32_t prev_tss;

    uint32_t esp0;      /* kernel stack when handling interrupt */
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;

    uint32_t cr3;       /* page directory for process */

    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;

    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;

    uint32_t ldt_seg;

    uint16_t trap;      /* ??? */
    uint16_t io_base;   /* ??? */
} tss_t;

/* Rest of registers can be clobbered */
struct context {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t eip;
    uint32_t uesp;
};

enum task_state {
    RUNNING,
    RUNNABLE,
    STOPPED
};

/**
 * kstack should be first so that we don't have to worry about
 * updating offsets in `x86.S` when changing the task structure.
 */
struct task {
    uint8_t *kstack;
    uint8_t tid;
    struct irq_regs *regs;
    struct context *ctx;
    pde_t *pdir;
    enum task_state state;
    struct task *next;
    struct task *prev;
};

extern struct task *fgtask;

void swtch(const uint8_t *uesp, struct context **old, const struct context *new);
void swtch_save(const uint8_t *where);
struct task *task_alloc(void);
int task_init(void);
void task_yield(const uint8_t *uesp);
void task_exit(struct task *task);
int task_fork(const uint8_t *uesp);
void task_wait(int td);

#endif
