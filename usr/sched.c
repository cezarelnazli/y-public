#include "usyscall.h"

int main1(void)
{
    for (int i = 0; i < 20; i += 2) {
        print(i);
    }

    exit();
}

int main2(void)
{
    for (int i = 1; i < 20; i += 2) {
        print(i);
        yield();
    }

    while (1) {
        print(1);
        yield();
    }

    exit();
}

