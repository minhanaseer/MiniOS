#ifndef ISR_H
#define ISR_H

#include <stdint.h>

/* Snapshot of CPU registers at the moment an exception fires.
   Layout must exactly match what isr_common_stub pushes onto the stack. */
typedef struct {
    uint32_t ds;                                        /* pushed by stub */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pusha          */
    uint32_t int_no, err_code;                          /* stub + cpu     */
    uint32_t eip, cs, eflags;                          /* pushed by cpu  */
} registers_t;

void isr_handler(registers_t* regs);

#endif
