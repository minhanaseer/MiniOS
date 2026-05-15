#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include "isr.h"

typedef void (*irq_handler_t)(registers_t* regs);

void irq_init(void);
void irq_register(uint8_t irq, irq_handler_t handler);

/* Called from irq_common_stub in irq.asm — not for direct use */
void irq_handler(registers_t* regs);

#endif
