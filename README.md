# MyKernel

A custom x86 operating system built from scratch in C and Assembly, inspired by macOS and Windows.

---

## Phases Completed

| Phase | Feature | Files |
|-------|---------|-------|
| 1 | Foundation — boot, VGA text, keyboard | `boot/boot.asm`, `kernel/kernel.c`, `Makefile`, `grub.cfg` |
| 2 | Hardware — GDT setup | `kernel/gdt.c`, `include/gdt.h` |
| 3 | Shell — interactive command line | `kernel/shell.c`, `include/shell.h` |
| 4 | Memory — physical memory manager, heap | `kernel/pmm.c`, `include/pmm.h` |
| 5 | Programs — file system + program loader | `kernel/fs.c`, `kernel/loader.c`, `include/fs.h`, `include/loader.h` |
| 6 | Graphics — VGA demo, framebuffer driver, pixel engine | `kernel/vga.c`, `kernel/fb.c`, `kernel/pixel.c` |

---

## Shell Commands

| Command | Description |
|---------|-------------|
| `help` | Show all commands |
| `hello` | Print welcome message |
| `clear` | Clear the screen |
| `about` | About MyKernel |
| `maths` | Simple calculator output |
| `mem` | Memory stats |
| `ls` | List files |
| `write [file]` | Create a file |
| `read [file]` | Read a file |
| `del [file]` | Delete a file |
| `run [program]` | Run a built-in program (`hello`, `count`, `info`, `banner`) |
| `graphics` | Launch the graphics demo screen |

---

## Build & Run

**Requirements:**
- `x86_64-elf-gcc` — cross compiler
- `nasm` — assembler
- `i686-elf-grub` — GRUB bootloader tools
- `qemu-system-i386` — emulator

```bash
# Build kernel binary
make

# Build bootable ISO
make iso

# Build and launch in QEMU
make run

# Clean build files
make clean
```

---

## Project Structure

```
MyKernel/
├── boot/
│   └── boot.asm          # Multiboot header + entry point
├── kernel/
│   ├── kernel.c          # Kernel main, VGA text output, keyboard
│   ├── shell.c           # Interactive shell
│   ├── gdt.c             # Global Descriptor Table
│   ├── idt.c             # Interrupt Descriptor Table
│   ├── pmm.c             # Physical memory manager
│   ├── fs.c              # Simple RAM file system
│   ├── loader.c          # Built-in program loader
│   ├── fb.c              # Framebuffer / pixel drawing
│   ├── vga.c             # VGA graphics demo
│   └── pixel.c           # Pixel engine
├── include/              # Header files
├── isodir/               # ISO directory for GRUB
│   └── boot/grub/grub.cfg
├── linker.ld             # Linker script
└── Makefile
```

---

## Developer

**Minha Naseer** — Built from scratch using C + x86 Assembly  
Running on QEMU x86 virtual machine
