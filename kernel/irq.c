#include "../include/irq.h"
#include "../include/pic.h"

static irq_handler_t irq_handlers[16];

void irq_init(void) {
    for (int i = 0; i < 16; i++)
        irq_handlers[i] = 0;
}

void irq_register(uint8_t irq, irq_handler_t handler) {
    if (irq < 16)
        irq_handlers[irq] = handler;
}

/* Called by irq_common_stub in irq.asm.
   regs->int_no holds the raw IRQ number (0-15). */
void irq_handler(registers_t* regs) {
    uint8_t irq = (uint8_t)regs->int_no;
    if (irq < 16 && irq_handlers[irq])
        irq_handlers[irq](regs);
    pic_send_eoi(irq);
}
