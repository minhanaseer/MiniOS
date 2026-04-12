section .multiboot2
align 8

mb2_header_start:
    dd 0xE85250D6
    dd 0
    dd mb2_header_end - mb2_header_start
    dd -(0xE85250D6 + 0 + (mb2_header_end - mb2_header_start))

    align 8
    dw 5
    dw 0
    dd 20
    dd 800
    dd 600
    dd 32

    align 8
    dw 0
    dw 0
    dd 8
mb2_header_end:

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
