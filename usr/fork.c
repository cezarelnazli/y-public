#include "usyscall.h"

unsigned char sched1[] = {
  0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x60, 0x80, 0x04, 0x08, 0x34, 0x00, 0x00, 0x00, 0xf0, 0x02, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x28, 0x00,
  0x07, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x04, 0x08, 0x00, 0x80, 0x04, 0x08, 0x54, 0x01, 0x00, 0x00,
  0x54, 0x01, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x55, 0x89, 0xe5, 0x83, 0xec, 0x18, 0xc7, 0x45, 0xf4, 0x00, 0x00, 0x00,
  0x00, 0xeb, 0x17, 0x83, 0xec, 0x0c, 0xff, 0x75, 0xf4, 0xe8, 0x66, 0x00,
  0x00, 0x00, 0x83, 0xc4, 0x10, 0xe8, 0x64, 0x00, 0x00, 0x00, 0x83, 0x45,
  0xf4, 0x02, 0x83, 0x7d, 0xf4, 0x13, 0x7e, 0xe3, 0xe8, 0x5b, 0x00, 0x00,
  0x00, 0x90, 0xc9, 0xc3, 0x55, 0x89, 0xe5, 0x83, 0xec, 0x18, 0xc7, 0x45,
  0xf4, 0x01, 0x00, 0x00, 0x00, 0xeb, 0x17, 0x83, 0xec, 0x0c, 0xff, 0x75,
  0xf4, 0xe8, 0x32, 0x00, 0x00, 0x00, 0x83, 0xc4, 0x10, 0xe8, 0x30, 0x00,
  0x00, 0x00, 0x83, 0x45, 0xf4, 0x02, 0x83, 0x7d, 0xf4, 0x13, 0x7e, 0xe3,
  0x83, 0xec, 0x0c, 0x6a, 0x01, 0xe8, 0x16, 0x00, 0x00, 0x00, 0x83, 0xc4,
  0x10, 0xe8, 0x14, 0x00, 0x00, 0x00, 0xeb, 0xec, 0x66, 0x90, 0x66, 0x90,
  0x66, 0x90, 0x66, 0x90, 0x66, 0x90, 0x66, 0x90, 0x6a, 0x00, 0x89, 0xe0,
  0x0f, 0x34, 0x6a, 0x01, 0x89, 0xe0, 0x0f, 0x34, 0x6a, 0x02, 0x89, 0xe0,
  0x0f, 0x34, 0x6a, 0x03, 0x89, 0xe0, 0x0f, 0x34, 0x6a, 0x04, 0x89, 0xe0,
  0x0f, 0x34, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x7a, 0x52, 0x00, 0x01, 0x7c, 0x08, 0x01, 0x1b, 0x0c, 0x04, 0x04,
  0x88, 0x01, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
  0x40, 0xff, 0xff, 0xff, 0x34, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x08,
  0x85, 0x02, 0x42, 0x0d, 0x05, 0x70, 0xc5, 0x0c, 0x04, 0x04, 0x00, 0x00,
  0x18, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x54, 0xff, 0xff, 0xff,
  0x40, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0e, 0x08, 0x85, 0x02, 0x42, 0x0d,
  0x05, 0x00, 0x00, 0x00, 0x47, 0x43, 0x43, 0x3a, 0x20, 0x28, 0x47, 0x4e,
  0x55, 0x29, 0x20, 0x36, 0x2e, 0x33, 0x2e, 0x30, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x80, 0x04, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x81, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0xf1, 0xff, 0x09, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0xf1, 0xff,
  0x14, 0x00, 0x00, 0x00, 0xe6, 0x80, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x01, 0x00, 0x1a, 0x00, 0x00, 0x00, 0xe0, 0x80, 0x04, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00,
  0x60, 0x80, 0x04, 0x08, 0x34, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00,
  0x26, 0x00, 0x00, 0x00, 0x94, 0x80, 0x04, 0x08, 0x40, 0x00, 0x00, 0x00,
  0x12, 0x00, 0x01, 0x00, 0x2c, 0x00, 0x00, 0x00, 0xf8, 0x80, 0x04, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x31, 0x00, 0x00, 0x00,
  0xf2, 0x80, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00,
  0x36, 0x00, 0x00, 0x00, 0x54, 0x91, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x02, 0x00, 0x42, 0x00, 0x00, 0x00, 0x54, 0x91, 0x04, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x02, 0x00, 0x49, 0x00, 0x00, 0x00,
  0x54, 0x91, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x02, 0x00,
  0x4e, 0x00, 0x00, 0x00, 0xec, 0x80, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x01, 0x00, 0x00, 0x73, 0x63, 0x68, 0x65, 0x64, 0x2e, 0x63,
  0x00, 0x75, 0x73, 0x79, 0x73, 0x63, 0x61, 0x6c, 0x6c, 0x2e, 0x53, 0x00,
  0x79, 0x69, 0x65, 0x6c, 0x64, 0x00, 0x70, 0x72, 0x69, 0x6e, 0x74, 0x00,
  0x6d, 0x61, 0x69, 0x6e, 0x31, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x32, 0x00,
  0x65, 0x78, 0x65, 0x63, 0x00, 0x66, 0x6f, 0x72, 0x6b, 0x00, 0x5f, 0x5f,
  0x62, 0x73, 0x73, 0x5f, 0x73, 0x74, 0x61, 0x72, 0x74, 0x00, 0x5f, 0x65,
  0x64, 0x61, 0x74, 0x61, 0x00, 0x5f, 0x65, 0x6e, 0x64, 0x00, 0x65, 0x78,
  0x69, 0x74, 0x00, 0x00, 0x2e, 0x73, 0x79, 0x6d, 0x74, 0x61, 0x62, 0x00,
  0x2e, 0x73, 0x74, 0x72, 0x74, 0x61, 0x62, 0x00, 0x2e, 0x73, 0x68, 0x73,
  0x74, 0x72, 0x74, 0x61, 0x62, 0x00, 0x2e, 0x74, 0x65, 0x78, 0x74, 0x00,
  0x2e, 0x65, 0x68, 0x5f, 0x66, 0x72, 0x61, 0x6d, 0x65, 0x00, 0x2e, 0x63,
  0x6f, 0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
  0x60, 0x80, 0x04, 0x08, 0x60, 0x00, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x81, 0x04, 0x08, 0x00, 0x01, 0x00, 0x00,
  0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x54, 0x01, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x02, 0x00, 0x00,
  0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xbb, 0x02, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned int sched1_len = 1032;

int main(void)
{
    int pid;
    int i;

    print(10);

    pid = fork();
    if (pid == 0) {
        exec(sched1);
        while (1) {
            print(0);
        }
    } else {
        while (1) {
            print(1);
        }
    }

    exit();
}
