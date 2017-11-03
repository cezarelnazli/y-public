QEMU=qemu-system-i386
CC=i686-elf-gcc
AS=nasm

CFLAGS=-O2 -Wall -Wextra -std=c11 -pedantic
CFLAGS:=$(CFLAGS) -ffreestanding -nostdlib -lgcc
ASFLAGS=-w+orphan-labels -f elf32
QEMUFLAGS=-m 1G -cdrom y.iso -serial stdio

.PHONY: clean listing

OBJS=bootstrap.o    \
     apic.o         \
     com.o          \
     elf.o          \
     exec.o         \
     irq.o          \
     irqs.o         \
     ide.o			\
     kernel.o       \
     kprintf.o      \
     pm.o           \
     string.o       \
     swtch.o        \
     syscall.o      \
     tables.o       \
     task.o         \
     vga.o          \
     vm.o           \
     x86.o          \

all: kernel.bin iso
	$(QEMU) $(QEMUFLAGS)

kernel.bin: irq.c $(OBJS) linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(OBJS)

irqs.S: irq.py
	python irq.py asm > $@

irq.c: irq.py irq_tmpl.c
	python irq.py irq_c

.S.o:
	$(AS) $(ASFLAGS) -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -MD -c -o $@ $<

clean:
	rm -f *.o *.d kernel.bin y.iso irqs.S irq.c sched1 sched2 y.pdf
	rm -f usr/test_fork usr/test_exec usr/sched1 usr/sched2
	rm -rf isodir

listing:
	cat *.c `ls -1 *.h | grep -v ramfs.h | tr '\n' ' '` *.S > code.cat
	enscript -B -C -p y.ps code.cat
	ps2pdf y.ps
	rm code.cat y.ps

# References
# http://wiki.osdev.org/Meaty_Skeleton#iso.sh
iso:
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/kernel.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o y.iso isodir

-include *.d
