section .multiboot
align 4
    dd 0x1BADB002
    dd 0x00000003
    dd -(0x1BADB002 + 0x00000003)

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
    push 0
    popf
    push ebx
    push eax
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang

global gdt_flush
gdt_flush:
    mov eax, [esp+4]
    lgdt [eax]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush
.flush:
    ret

global idt_flush
idt_flush:
    mov eax, [esp+4]
    lidt [eax]
    ret
global keyboard_handler_asm
extern keyboard_handler

keyboard_handler_asm:
    pusha
    call keyboard_handler
    popa
    iret