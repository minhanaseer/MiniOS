[BITS 32]

global gdt_flush
gdt_flush:
    mov eax, [esp+4]    ; get address of gdt_ptr struct
    lgdt [eax]          ; load the GDT

    mov ax, 0x10        ; data segment selector (entry 2 in GDT)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.done      ; far jump reloads CS with code segment selector (entry 1)
.done:
    ret
