CC  = x86_64-elf-gcc
LD  = x86_64-elf-ld
ASM = nasm

CFLAGS  = -ffreestanding -O2 -m32 -mno-sse -mno-sse2 -mno-mmx -mno-avx
LDFLAGS = -nostdlib -T linker.ld -m elf_i386

all: kernel.bin

# --- Assembly ---
boot.o: boot/boot.asm
	$(ASM) -f elf32 boot/boot.asm -o boot.o

gdt_flush.o: boot/gdt_flush.asm
	$(ASM) -f elf32 boot/gdt_flush.asm -o gdt_flush.o

isr_asm.o: boot/isr.asm
	$(ASM) -f elf32 boot/isr.asm -o isr_asm.o

irq_asm.o: boot/irq.asm
	$(ASM) -f elf32 boot/irq.asm -o irq_asm.o

# --- C kernel ---
kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

gdt.o: kernel/gdt.c
	$(CC) $(CFLAGS) -c kernel/gdt.c -o gdt.o

idt.o: kernel/idt.c
	$(CC) $(CFLAGS) -c kernel/idt.c -o idt.o

isr.o: kernel/isr.c
	$(CC) $(CFLAGS) -c kernel/isr.c -o isr.o

pic.o: kernel/pic.c
	$(CC) $(CFLAGS) -c kernel/pic.c -o pic.o

irq.o: kernel/irq.c
	$(CC) $(CFLAGS) -c kernel/irq.c -o irq.o

keyboard.o: kernel/keyboard.c
	$(CC) $(CFLAGS) -c kernel/keyboard.c -o keyboard.o

shell.o: kernel/shell.c
	$(CC) $(CFLAGS) -c kernel/shell.c -o shell.o

pmm.o: kernel/pmm.c
	$(CC) $(CFLAGS) -c kernel/pmm.c -o pmm.o

heap.o: kernel/heap.c
	$(CC) $(CFLAGS) -c kernel/heap.c -o heap.o

fs.o: kernel/fs.c
	$(CC) $(CFLAGS) -c kernel/fs.c -o fs.o

loader.o: kernel/loader.c
	$(CC) $(CFLAGS) -c kernel/loader.c -o loader.o

fb.o: kernel/fb.c
	$(CC) $(CFLAGS) -c kernel/fb.c -o fb.o

vga.o: kernel/vga.c
	$(CC) $(CFLAGS) -c kernel/vga.c -o vga.o

pixel.o: kernel/pixel.c
	$(CC) $(CFLAGS) -c kernel/pixel.c -o pixel.o

# --- Link ---
OBJS = boot.o gdt_flush.o isr_asm.o irq_asm.o \
       kernel.o gdt.o idt.o isr.o \
       pic.o irq.o keyboard.o \
       shell.o pmm.o heap.o fs.o loader.o \
       fb.o vga.o pixel.o

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.bin $(OBJS)

iso: kernel.bin
	cp kernel.bin isodir/boot/kernel.bin
	i686-elf-grub-mkrescue -o mykernel.iso isodir

run: iso
	qemu-system-i386 -cdrom mykernel.iso

clean:
	rm -f *.o *.bin mykernel.iso
