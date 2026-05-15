[BITS 32]

; Load IDT into the CPU
global idt_flush
idt_flush:
    mov eax, [esp+4]
    lidt [eax]
    ret

; ISR macro for exceptions that do NOT push an error code automatically.
; We push a dummy 0 so the stack layout is always the same.
%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    push dword 0
    push dword %1
    jmp isr_common_stub
%endmacro

; ISR macro for exceptions that DO push an error code automatically.
%macro ISR_ERRCODE 1
global isr%1
isr%1:
    push dword %1
    jmp isr_common_stub
%endmacro

; CPU exceptions 0-31
; Exceptions with error codes: 8, 10, 11, 12, 13, 14, 17, 30
ISR_NOERRCODE 0     ; Division By Zero
ISR_NOERRCODE 1     ; Debug
ISR_NOERRCODE 2     ; Non Maskable Interrupt
ISR_NOERRCODE 3     ; Breakpoint
ISR_NOERRCODE 4     ; Overflow
ISR_NOERRCODE 5     ; Out of Bounds
ISR_NOERRCODE 6     ; Invalid Opcode
ISR_NOERRCODE 7     ; No FPU
ISR_ERRCODE   8     ; Double Fault
ISR_NOERRCODE 9     ; FPU Segment Overrun
ISR_ERRCODE   10    ; Invalid TSS
ISR_ERRCODE   11    ; Segment Not Present
ISR_ERRCODE   12    ; Stack Fault
ISR_ERRCODE   13    ; General Protection Fault
ISR_ERRCODE   14    ; Page Fault
ISR_NOERRCODE 15    ; Reserved
ISR_NOERRCODE 16    ; FPU Exception
ISR_ERRCODE   17    ; Alignment Check
ISR_NOERRCODE 18    ; Machine Check
ISR_NOERRCODE 19    ; SIMD Exception
ISR_NOERRCODE 20    ; Reserved
ISR_NOERRCODE 21    ; Reserved
ISR_NOERRCODE 22    ; Reserved
ISR_NOERRCODE 23    ; Reserved
ISR_NOERRCODE 24    ; Reserved
ISR_NOERRCODE 25    ; Reserved
ISR_NOERRCODE 26    ; Reserved
ISR_NOERRCODE 27    ; Reserved
ISR_NOERRCODE 28    ; Reserved
ISR_NOERRCODE 29    ; Reserved
ISR_ERRCODE   30    ; Security Exception
ISR_NOERRCODE 31    ; Reserved

extern isr_handler

; All ISRs jump here. At this point the stack holds (top to bottom):
;   int_no, err_code, eip, cs, eflags  (cpu-pushed + our pushes above)
isr_common_stub:
    pusha               ; push eax,ecx,edx,ebx,esp,ebp,esi,edi

    mov ax, ds
    push eax            ; save data segment selector

    mov ax, 0x10        ; load kernel data segment into all segment regs
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp            ; pass pointer to the stack frame as registers_t*
    call isr_handler
    add esp, 4

    pop eax             ; restore original data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                ; restore general-purpose registers
    add esp, 8          ; remove int_no and err_code from stack
    iret                ; restores eip, cs, eflags automatically
