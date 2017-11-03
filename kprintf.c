#include "kprintf.h"
#include "stdarg.h"
#include "stdint.h"
#include "vga.h"
#include "com.h"

static void (*outf)(char) = 0;

static void kputs(const char *s)
{
    while (*s) {
        outf(*s);
        s++;
    }
}

static void kputx(uint32_t n)
{
    uint32_t rn = 0, scale = 0, start = 1;

    while (n) {
        if (start && (n & 15) == 0) {
            scale++;
        } else {
            start = 0;
        }
        rn = (rn << 4) + (n & 15);
        n >>= 4;
    }

    if (!rn) {
        outf('0');
        outf('0');
    }

    while (rn) {
        outf("0123456789ABCDEF"[rn & 15]);
        rn >>= 4;
    }

    while (scale--) {
        outf('0');
    }
}

static void kputd(uint32_t n)
{
    uint32_t rn = 0, scale = 0, start = 1, m;
    int32_t nn;
    int s = 1;

    if (n >> 31 == 1) {
        nn = ~n + 1;
        s = -1;
    } else {
        nn = n;
    }

    while (nn) {
        if (start && nn % 10 == 0) {
            scale++;
        } else {
            start = 0;
        }
        rn = rn * 10 + nn % 10;
        nn /= 10;
    }

    if (!rn) {
        outf('0');
        return;
    }

    if (s == -1) {
        outf('-');
    }

    while (rn) {
        outf('0' + (rn % 10));
        rn /= 10;
    }

    while (scale--) {
        outf('0');
    }
}


static void kpute(char s)
{
    switch (s) {
        case 'b':
            outf('\b');
            break;
        case 'n':
            outf('\n');
            break;
        case 'r':
            outf('\r');
            break;
        case 't':
            outf('\t');
            break;
    }
}


static void kputc(char s)
{
    outf(s);
}


void kprintf(enum outdev dev, const char *restrict fmt, ...)
{
    va_list ap;
    char *s;

    outf = dev == OUTDEV_VGA ? vga_putchar : com_write;

    va_start(ap, fmt);

    for (; *fmt; fmt++) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case '%':
                    kputc('%');
                    break;
                case 'c':
                    kputc(va_arg(ap, int));
                    break;
                case 's':
                    s = va_arg(ap, char *);
                    kputs(s);
                    break;
                case 'd':
                    kputd(va_arg(ap, uint32_t));
                    break;
                case 'x':
                    kputx(va_arg(ap, uint32_t));
                    break;
                default:
                    kputs(":-??");
                    break;
            }
        } else {
            if (*fmt == '\\') {
                fmt++;
                kpute(*fmt);
            } else {
                kputc(*fmt);
            }
        }
    }

    va_end(ap);
}

