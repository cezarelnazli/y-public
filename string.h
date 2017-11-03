#ifndef STRING_H
#define STRING_H

#include <stddef.h>

extern int memcmp(const void *s1, const void *s2, size_t n);
extern size_t strlen(const char *s);
extern void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
extern void *memmove(void *s1, const void *s2, size_t n);
extern void *memset(void *s, int c, size_t n);

#endif
