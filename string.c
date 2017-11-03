#include "string.h"
#include <stdint.h>

size_t strlen(const char *s)
{
    size_t n = 0;

    while (*s++) {
        n++;
    }

    return n;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const uint8_t *a = (const uint8_t *) s1;
    const uint8_t *b = (const uint8_t *) s2;

    for (; n--; a++, b++) {
        if (*a != *b) {
            return *a - *b;
        }
    }

    return 0;
}

void *memcpy(void *restrict s1, const void *restrict s2, size_t n)
{
    uint8_t *a = (uint8_t *) s1;
    const uint8_t *b = (const uint8_t *) s2;

    while (n--) {
        *a++ = *b++;
    }

    return s1;
}

void *memmove(void *s1, const void *s2, size_t n)
{
    uint8_t *a = (uint8_t *) s1;
    const uint8_t *b = (const uint8_t *) s2;
    size_t i;

    if (a < b) {
        return memcpy(s1, s2, n);
    }

    for (i = n; i > 0; i--) {
        a[i] = b[i];
    }

    return s1;
}

void *memset(void *s, int c, size_t n)
{
    uint8_t *t = (uint8_t *) s;

    while (n--) {
        *t = (uint8_t) c;
        t++;
    }

    return s;
}

