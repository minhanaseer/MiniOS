CC = x86_64-elf-gcc
LD = x86_64-elf-ld
ASM = nasm

CFLAGS = -ffreestanding -O2 -m32
LDFLAGS = -nostdlib -T linker.ld -m elf_i386

all: kernel.bin

boot.o: boot/boot.asm
	$(ASM) -f elf32 boot/boot.asm -o boot.o

kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

kernel.bin: boot.o kernel.o
	$(LD) $(LDFLAGS) -o kernel.bin boot.o kernel.o

clean:
	rm -f *.o *.bin