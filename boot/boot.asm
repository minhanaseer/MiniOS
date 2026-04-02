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
