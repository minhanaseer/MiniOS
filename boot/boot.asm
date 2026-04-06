MBALIGN  equ 1 << 0
MEMINFO  equ 1 << 1
VIDINFO  equ 1 << 2
FLAGS    equ MBALIGN | MEMINFO | VIDINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
    dd 0, 0, 0, 0, 0
    dd 0
    dd 800, 600, 32

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global start
extern kernel_main

start:
    mov esp, stack_top
    push ebx
    push eax
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang
