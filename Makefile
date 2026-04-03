CC = x86_64-elf-gcc
LD = x86_64-elf-ld
ASM = nasm

CFLAGS = -ffreestanding -O2 -Wall -Wextra -m32
LDFLAGS = -nostdlib -T linker.ld -m elf_i386

all: kernel.bin

boot.o: boot/boot.asm
	$(ASM) -f elf32 boot/boot.asm -o boot.o

kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

gdt.o: kernel/gdt.c
	$(CC) $(CFLAGS) -c kernel/gdt.c -o gdt.o

idt.o: kernel/idt.c
	$(CC) $(CFLAGS) -c kernel/idt.c -o idt.o

keyboard.o: kernel/keyboard.c
	$(CC) $(CFLAGS) -c kernel/keyboard.c -o keyboard.o

kernel.bin: boot.o kernel.o gdt.o idt.o keyboard.o
	$(LD) $(LDFLAGS) -o kernel.bin boot.o kernel.o gdt.o idt.o keyboard.o

run: kernel.bin
	qemu-system-i386 \
		-kernel kernel.bin \
		-serial stdio

clean:
	rm -f *.o *.bin