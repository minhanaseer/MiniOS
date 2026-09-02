# MyKernel 

> A bare metal operating system kernel built from scratch in C and Assembly running on x86 hardware inside QEMU.

---

## What is MyKernel?

MyKernel is a fully custom operating system kernel written from absolute zero — no libraries, no operating system underneath, no shortcuts. Every single line of code was written by hand, from the bootloader that starts the CPU, to the interactive shell you type commands into.

This is a real, working, programmable operating system. It boots, reads keyboard input, manages memory, stores files and runs programs — all on bare metal x86 hardware emulated inside QEMU.

---

## Live Demo

```
MyKernel v0.1
Keyboard: OK
Type 'help' for commands
----------------------------
> help

Commands:
  help              - show this list
  hello             - say hello
  clear             - clear screen
  about             - about MyKernel
  maths             - calculator
  mem               - memory stats
  ls                - list files
  write [file.txt]  - create file
  read  [file.txt]  - read file
  del   [file.txt]  - delete file
  run   [program]   - run a program

> run hello
Hello from MyKernel program!
This is a program running on your OS!

> mem
Memory stats:
  Total RAM   : 32 MB
  Page size   : 4096 bytes
  Total pages : 8192
  Status      : OK

> write notes.txt
File created: notes.txt

> ls
  notes.txt  (10 bytes)
```

---

## What Was Built

### Phase 1 — Foundation ✅
- Custom bootloader written in x86 Assembly (NASM)
- Multiboot-compliant kernel entry point
- GRUB bootloader integration
- Bootable ISO image that runs in QEMU

### Phase 2 — Hardware Communication ✅
- GDT (Global Descriptor Table) — tells CPU how memory is structured
- Keyboard input via direct hardware port polling (port 0x60/0x64)
- VGA text mode output — writes directly to memory address 0xb8000
- Screen scrolling when terminal fills up
- Backspace key support

### Phase 3 — Interactive Shell ✅
- Command buffer that stores keystrokes in real time
- Command parser that reads full input on Enter press
- 10+ built-in commands
- Unknown command handling with helpful error messages
- Screen clear and reset functionality

### Phase 4 — Memory Manager ✅
- Physical Memory Manager (PMM)
- Page tracking system (4096 bytes per page)
- Memory statistics reporting via `mem` command

### Phase 5 — Programmable OS ✅
- In-memory file system — create, read, list and delete files
- Program loader that executes built-in programs
- Programs: hello, count, info, banner
- `run` command to execute any program from the shell

---

## Project Structure

```
MyKernel/
├── boot/
│   └── boot.asm          ← Bootloader (Assembly)
├── kernel/
│   ├── kernel.c          ← Main kernel + VGA + keyboard
│   ├── shell.c           ← Shell and command parser
│   ├── fs.c              ← File system
│   ├── loader.c          ← Program loader
│   ├── pmm.c             ← Memory manager
│   ├── gdt.c             ← Global Descriptor Table
│   └── idt.c             ← Interrupt Descriptor Table
├── include/
│   ├── shell.h
│   ├── fs.h
│   ├── loader.h
│   ├── pmm.h
│   ├── gdt.h
│   └── idt.h
├── isodir/
│   └── boot/grub/
│       └── grub.cfg      ← GRUB configuration
├── linker.ld             ← Linker script
└── Makefile              ← Build system
```

---

## Languages Used

| Language | Purpose |
|----------|---------|
| **C** | Kernel core, shell, file system, memory manager, program loader |
| **x86 Assembly (NASM)** | Bootloader, CPU setup, hardware port access |
| **Makefile** | Build automation |
| **Linker Script** | Memory layout and section placement |

---

## Tools Used

| Tool | Purpose |
|------|---------|
| **QEMU** | Virtual machine to safely run and test the kernel |
| **NASM** | Assembler for bootloader code |
| **x86_64-elf-gcc** | Cross-compiler for 32-bit kernel code |
| **x86_64-elf-ld** | Cross-linker for kernel binary |
| **GRUB + i686-elf-grub** | Bootloader that loads the kernel |
| **xorriso** | Creates bootable ISO image |
| **Git** | Version control |
| **VSCode** | Code editor |
| **Homebrew** | Package manager on macOS |

---

## How to Run

### Prerequisites (macOS)

```bash
brew install nasm qemu x86_64-elf-gcc x86_64-elf-binutils i686-elf-grub xorriso
```

### Build and Run

```bash
git clone https://github.com/minhanaseer/MyKernel.git
cd MyKernel
make
cp kernel.bin isodir/boot/kernel.bin
i686-elf-grub-mkrescue -o mykernel.iso isodir
qemu-system-i386 -cdrom mykernel.iso
```

---

## How It Works

```
You run QEMU
      ↓
GRUB reads the ISO and finds kernel.bin
      ↓
boot.asm runs — sets up the CPU stack
      ↓
kernel_main() starts in C
      ↓
GDT installed — CPU memory layout defined
      ↓
VGA initialised — prints to screen at 0xb8000
      ↓
Shell, file system, loader and memory manager start
      ↓
Keyboard polling loop runs forever
      ↓
You type → shell reads → command runs → result printed
      ↓
All of this runs on emulated x86 hardware with nothing underneath
```

---

## What I Learned

- How a CPU actually boots from power-on to running code
- What an operating system kernel actually does underneath apps
- How VGA text mode works — writing characters directly to memory
- How keyboards communicate with the CPU through hardware ports
- What the GDT and IDT are and why they are essential
- How to write and use a cross-compiler
- How GRUB loads a kernel and what the Multiboot standard is
- How linker scripts control where code lives in memory
- How to build a shell from scratch — buffers, parsers, command handlers
- How a file system works at the most fundamental level
- How QEMU emulates real x86 hardware
- How to debug a kernel with no standard library and no error messages
- Why OS development is one of the hardest areas of programming
- The difference between user space and kernel space
- What bare metal programming actually means

---

## Difficulties Faced

### Cross Compilation on Apple Silicon M3
Building x86 kernel code on an ARM64 Mac required a cross-compiler. Setting up `x86_64-elf-gcc` correctly on Apple Silicon took significant time and research.

### No Standard Library
In kernel development there is no `printf`, no `malloc`, no `strlen` — every utility function had to be written from scratch including string comparison, number printing and memory operations.

### No Error Messages When Crashing
When the kernel crashes it shows orange stripes or a black screen with zero helpful information. Debugging required adding temporary print statements and rebuilding the full ISO each time to find the exact problem.

### Boot and GRUB Issues
Several times the kernel compiled correctly but QEMU would show boot failures or get stuck at GRUB. Root causes included mismatched multiboot headers, incorrect linker section names and ISO caching issues.

### Protected Mode Restrictions
BIOS interrupts do not work in 32-bit protected mode. This blocked standard approaches to switching video modes and required working directly with VGA hardware registers instead.

### Memory Corruption
Static arrays in the kernel caused silent memory corruption that crashed the entire system. The memory manager had to be carefully simplified to avoid overwriting kernel code during initialisation.

### Keyboard Input
Getting keyboard input working correctly without characters repeating or being missed required understanding scan codes, key press vs key release events and implementing a hold counter system.

---

## Platform

| Item | Detail |
|------|--------|
| Developer machine | MacBook Air 13-inch M3 2024 |
| Target architecture | x86 32-bit |
| Runs on | QEMU virtual machine |
| macOS version | Sonoma 14.6 |
| Started | April 2026 |

---

## Roadmap

This project is actively in development. Planned upcoming features:

- Pixel graphics mode
- Mouse driver
- Window manager
- Basic GUI applications
- Multitasking and process scheduler
- Network driver
- Boot on real Intel hardware via USB

---

## Inspiration

This project was inspired by how Linus Torvalds started Linux in 1991 with a simple bootloader and a basic shell on a 386 PC. Every major operating system started exactly like this.

> "Just a hobby, won't be big and professional." — Linus Torvalds, 1991

---
