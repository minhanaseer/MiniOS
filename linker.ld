ENTRY(start)

SECTIONS {
    . = 1M;

    .multiboot2 ALIGN(8) : {
        *(.multiboot2)
    }

    .text ALIGN(4) : {
        *(.text)
    }

    .data ALIGN(4) : {
        *(.data)
    }

    .bss ALIGN(4) : {
        *(COMMON)
        *(.bss)
    }
}
