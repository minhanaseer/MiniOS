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

shell.o: kernel/shell.c
	$(CC) $(CFLAGS) -c kernel/shell.c -o shell.o

pmm.o: kernel/pmm.c
	$(CC) $(CFLAGS) -c kernel/pmm.c -o pmm.o

fs.o: kernel/fs.c
	$(CC) $(CFLAGS) -c kernel/fs.c -o fs.o

loader.o: kernel/loader.c
	$(CC) $(CFLAGS) -c kernel/loader.c -o loader.o

fb.o: kernel/fb.c
	$(CC) $(CFLAGS) -c kernel/fb.c -o fb.o

kernel.bin: boot.o kernel.o shell.o pmm.o fs.o loader.o fb.o vga.o
	$(LD) $(LDFLAGS) -o kernel.bin boot.o kernel.o shell.o pmm.o fs.o loader.o fb.o vga.o

clean:
	rm -f *.o *.bin
vga.o: kernel/vga.c
	$(CC) $(CFLAGS) -c kernel/vga.c -o vga.o