#ifndef X86_H
#define X86_H

#include <stdint.h>

#define EFLAGS_IF (1 << 9)

extern void rdmsr(uint32_t msr, uint32_t *edx, uint32_t *eax);
extern void wrmsr(uint32_t msr, uint32_t edx, uint32_t eax);
extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);
extern void outsb(uint16_t port, uint8_t *addr, uint32_t cnt);
extern void insb(uint16_t port, uint8_t *addr, uint32_t cnt);
extern void lcr3(const pde_t *addr);
extern void invlpg(uint32_t pg);
extern void ltr(uint16_t seg);
extern void lgdt(void);
extern void lidt(void);
extern uint32_t rcr2(void);
extern void sysenter_eip(void);
extern void fork_child_ret(void);

#endif

