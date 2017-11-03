CC=i686-elf-gcc
AS=nasm

CCFLAGS=-O2 -nostdlib -ffreestanding
ASFLAGS=-w+orphan-labels -f elf32

.S.o:
	$(AS) $(ASFLAGS) -o $@ $<

sched: sched.c usyscall.o
	$(CC) $(CCFLAGS) -e main1 -o sched1 sched.c usyscall.o
	$(CC) $(CCFLAGS) -e main2 -o sched2 sched.c usyscall.o

test_fork: fork.c usyscall.o
	$(CC) $(CCFLAGS) -e main -o test_fork fork.c usyscall.o

test_exec: exec.c usyscall.o
	$(CC) $(CCFLAGS) -e main -o test_exec exec.c usyscall.o

euclid: euclid.c usyscall.o
	$(CC) $(CCFLAGS) -e main -o $@ euclid.c usyscall.o

shell: shell.c usyscall.o
	$(CC) $(CCFLAGS) -I.. -e main -o shell shell.c usyscall.o
