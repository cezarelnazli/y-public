#include "usyscall.h"

struct gcd {
    int d;
    int x;
    int y;
};

static void euclid(int a, int b, struct gcd *aux)
{
    long xp;

    if (b == 0) {
        aux->d = a;
        aux->x = 1;
        aux->y = 0;
    } else {
        euclid(b, a % b, aux);
        xp = aux->x;
        aux->x = aux->y;
        aux->y = xp - (a / b) * aux->y;
    }
}

static int toint(char *s)
{
    int n = 0;

    while (*s) {
        n = n * 10 + *s - '0';
        s++;
    }

    return n;
}

int main(void)
{
    char s[0x10];
    int a, b;
    struct gcd aux;

    prints("a = ");
    read(s);
    a = toint(s);
    prints("\n");

    prints("b = ");
    read(s);
    b = toint(s);
    prints("\n");

    euclid(a, b, &aux);

    prints("d = ");
    print(aux.d);
    prints("\n");

    prints("x = ");
    print(aux.x);
    prints("\n");

    prints("y = ");
    print(aux.y);
    prints("\n");

    exit();
}

