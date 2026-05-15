[BITS 32]

; Each IRQ stub pushes the raw IRQ number (0-15) and a dummy error code,
; then falls into irq_common_stub — same stack layout as isr_common_stub
; so we can reuse the registers_t struct.
%macro IRQ 1
global irq%1
irq%1:
    push dword 0    ; dummy err_code (IRQs never push one)
    push dword %1   ; raw IRQ number (0-15)
    jmp irq_common_stub
%endmacro

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

extern irq_handler

irq_common_stub:
    pusha

    mov ax, ds
    push eax

    mov ax, 0x10    ; kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp        ; pass registers_t* to irq_handler
    call irq_handler
    add esp, 4

    pop eax         ; restore original data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8      ; remove int_no and err_code
    iret
