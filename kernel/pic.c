#include "../include/pic.h"

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI   0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" :: "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline void io_wait(void) {
    /* Writing to port 0x80 (POST diagnostic port) adds ~1us delay.
       Needed between PIC init commands on real hardware. */
    outb(0x80, 0);
}

void pic_init(void) {
    /* ICW1: start initialization, tell PIC that ICW4 will follow */
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4); io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4); io_wait();

    /* ICW2: vector offsets. Remap so IRQs don't collide with CPU exceptions (0-31).
       Master IRQ0-7 → INT 32-39, Slave IRQ8-15 → INT 40-47 */
    outb(PIC1_DATA, 0x20); io_wait();
    outb(PIC2_DATA, 0x28); io_wait();

    /* ICW3: cascade wiring */
    outb(PIC1_DATA, 0x04); io_wait(); /* master: slave is on IRQ2 line */
    outb(PIC2_DATA, 0x02); io_wait(); /* slave: connected to IRQ2 of master */

    /* ICW4: 8086 mode */
    outb(PIC1_DATA, ICW4_8086); io_wait();
    outb(PIC2_DATA, ICW4_8086); io_wait();

    /* Mask every IRQ. Drivers call pic_unmask_irq() to enable their line. */
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(PIC2_CMD, PIC_EOI); /* slave must be told first for IRQ 8-15 */
    outb(PIC1_CMD, PIC_EOI);
}

void pic_mask_irq(uint8_t irq) {
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    uint8_t  line = (irq < 8) ? irq : irq - 8;
    outb(port, inb(port) | (uint8_t)(1 << line));
}

void pic_unmask_irq(uint8_t irq) {
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    uint8_t  line = (irq < 8) ? irq : irq - 8;
    outb(port, inb(port) & (uint8_t)~(1 << line));
}
