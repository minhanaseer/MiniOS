# MyKernel

A custom x86 32-bit operating system built from scratch in C and Assembly, bootable via GRUB on QEMU.

---

## What's Built

| Phase | Feature | Files |
|-------|---------|-------|
| 1 | Boot — Multiboot header, GRUB, loads at 1MB | `boot/boot.asm`, `linker.ld`, `isodir/boot/grub/grub.cfg` |
| 2 | VGA text output + keyboard input | `kernel/kernel.c`, `kernel/vga.c` |
| 3 | GDT — Global Descriptor Table, loaded via `gdt_flush` | `kernel/gdt.c`, `boot/gdt_flush.asm` |
| 4 | IDT — Interrupt Descriptor Table | `kernel/idt.c` |
| 5 | PIC — 8259A remapped, IRQ dispatch system | `kernel/pic.c`, `kernel/irq.c`, `boot/irq.asm` |
| 6 | CPU exception handlers (ISRs 0–31) — panic + halt on any exception | `kernel/isr.c`, `boot/isr.asm` |
| 7 | Interactive shell | `kernel/shell.c` |
| 8 | RAM filesystem — flat store, 16 files, 256 bytes each | `kernel/fs.c` |
| 9 | Built-in program loader | `kernel/loader.c` |
| 10 | VGA graphics demo — color windowed UI in text mode | `kernel/vga.c` |

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
| `graphics` | Launch the graphics demo |

---

## Build & Run

**Requirements:**
- `x86_64-elf-gcc` — cross compiler
- `nasm` — assembler
- `i686-elf-grub` — GRUB bootloader tools
- `qemu-system-i386` — emulator

```bash
make        # Build kernel binary
make iso    # Build bootable ISO
make run    # Build and launch in QEMU
make clean  # Clean build files
```

---

## Project Structure

```
MyKernel/
├── boot/
│   ├── boot.asm          # Multiboot header + kernel entry point
│   ├── gdt_flush.asm     # Loads GDT register (lgdt + far jump)
│   ├── isr.asm           # ISR stubs for CPU exceptions 0–31
│   └── irq.asm           # IRQ stubs for hardware interrupts 0–15
├── kernel/
│   ├── kernel.c          # Kernel main — wires up GDT, IDT, PIC, shell
│   ├── gdt.c             # Global Descriptor Table
│   ├── idt.c             # Interrupt Descriptor Table
│   ├── pic.c             # 8259A PIC init + EOI
│   ├── isr.c             # CPU exception handler (panic + halt)
│   ├── irq.c             # Hardware IRQ dispatch
│   ├── shell.c           # Interactive command-line shell
│   ├── keyboard.c        # Keyboard driver
│   ├── fs.c              # RAM-based flat filesystem
│   ├── loader.c          # Built-in program loader
│   ├── vga.c             # VGA text mode + graphics demo
│   ├── fb.c              # Framebuffer driver (in progress)
│   ├── pixel.c           # Pixel engine (in progress)
│   └── pmm.c             # Physical memory manager (in progress)
├── include/              # Header files
├── isodir/               # ISO directory for GRUB
├── linker.ld             # Linker script (loads kernel at 1MB)
└── Makefile
```

---

## Developer

**Minha Naseer** — Built from scratch in C + x86 Assembly  
Running on QEMU x86 (i386) virtual machine
