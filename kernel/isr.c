#include "../include/isr.h"

extern void print(const char* s, char col);
extern void newline();
extern void putchar(char c, char col);

static const char* exception_names[] = {
    "Division By Zero",         /* 0  */
    "Debug",                    /* 1  */
    "Non Maskable Interrupt",   /* 2  */
    "Breakpoint",               /* 3  */
    "Overflow",                 /* 4  */
    "Out of Bounds",            /* 5  */
    "Invalid Opcode",           /* 6  */
    "No FPU",                   /* 7  */
    "Double Fault",             /* 8  */
    "FPU Segment Overrun",      /* 9  */
    "Invalid TSS",              /* 10 */
    "Segment Not Present",      /* 11 */
    "Stack Fault",              /* 12 */
    "General Protection Fault", /* 13 */
    "Page Fault",               /* 14 */
    "Reserved",                 /* 15 */
    "FPU Exception",            /* 16 */
    "Alignment Check",          /* 17 */
    "Machine Check",            /* 18 */
    "SIMD Exception",           /* 19 */
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Security Exception", "Reserved"
};

static void print_hex(uint32_t n) {
    char buf[8];
    int i = 0;
    print("0x", 0x0C);
    if (n == 0) { putchar('0', 0x0C); return; }
    while (n > 0) {
        int d = n & 0xF;
        buf[i++] = d < 10 ? '0' + d : 'A' + d - 10;
        n >>= 4;
    }
    for (int j = i - 1; j >= 0; j--)
        putchar(buf[j], 0x0C);
}

void isr_handler(registers_t* regs) {
    newline();
    print("*** KERNEL EXCEPTION ***", 0x04); newline();
    print("  Exception : ", 0x0C);
    print(exception_names[regs->int_no], 0x0C); newline();
    print("  Number    : ", 0x07); print_hex(regs->int_no);   newline();
    print("  Err code  : ", 0x07); print_hex(regs->err_code); newline();
    print("  EIP       : ", 0x07); print_hex(regs->eip);      newline();
    print("  CS        : ", 0x07); print_hex(regs->cs);       newline();

    /* For page faults (exception 14), CR2 holds the faulting virtual address */
    if (regs->int_no == 14) {
        uint32_t cr2;
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        print("  CR2 (fault addr): ", 0x07); print_hex(cr2); newline();
    }

    print("System halted.", 0x04); newline();
    __asm__ volatile("cli; hlt");
}
