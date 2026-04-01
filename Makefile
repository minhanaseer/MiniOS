CC = x86_64-elf-gcc
LD = x86_64-elf-ld
ASM = nasm

CFLAGS = -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -nostdlib -T linker.ld

all: kernel.bin

boot.o: boot/boot.asm
	$(ASM) -f elf64 boot/boot.asm -o boot.o

kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

kernel.bin: boot.o kernel.o
	$(LD) $(LDFLAGS) -o kernel.bin boot.o kernel.o

run: kernel.bin
	qemu-system-x86_64 -kernel kernel.bin

clean:
	rm -f *.o *.bin