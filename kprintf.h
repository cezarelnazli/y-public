#ifndef KPRINTF_H
#define KPRINTF_H

enum outdev {
    OUTDEV_VGA,
    OUTDEV_COM
};

void kprintf(enum outdev dev, const char *restrict fmt, ...);

#endif
