#ifndef PM_H
#define PM_H

#include "multiboot.h"
#include "stdint.h"

extern void *pm_alloc_frame(void);
extern void pm_free_frame(uint32_t where);
extern void pm_init(const multiboot_info_t *mbi);

#endif
