#include <stdint.h>
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/pic.h"
#include "../include/irq.h"
#include "../include/keyboard.h"
#include "../include/shell.h"
#include "../include/pmm.h"
#include "../include/fs.h"
#include "../include/loader.h"

char* video = (char*) 0xb8000;
int   cursor = 0;

static void scroll() {
    for (int i = 0; i < 24*80*2; i++)
        video[i] = video[i + 80*2];
    for (int i = 24*80*2; i < 25*80*2; i += 2) {
        video[i]   = ' ';
        video[i+1] = 0x07;
    }
    cursor = 24*80;
}

void clear() {
    for (int i = 0; i < 80*25*2; i++) video[i] = 0;
    cursor = 0;
}

void putchar(char c, char col) {
    if (cursor >= 80*25) scroll();
    video[cursor*2]   = c;
    video[cursor*2+1] = col;
    cursor++;
}

void print(const char* s, char col) {
    for (int i = 0; s[i]; i++) putchar(s[i], col);
}

void newline() {
    cursor = ((cursor/80)+1)*80;
    if (cursor >= 80*25) scroll();
}

void backspace() {
    if (cursor > 0) {
        cursor--;
        video[cursor*2]   = ' ';
        video[cursor*2+1] = 0x07;
    }
}

void kernel_main(uint32_t mb_magic, uint32_t mb_info_addr) {
    (void)mb_magic;
    gdt_install();   /* load our own GDT                      */
    idt_install();   /* install exception + IRQ gates         */
    pic_init();      /* remap PIC, mask all IRQs              */
    irq_init();      /* zero the IRQ handler table            */
    keyboard_init(); /* register keyboard handler, unmask IRQ1 */

    clear();
    print("MyKernel v0.1", 0x0A);   newline();
    print("GDT/IDT : OK",  0x0A);   newline();
    print("PIC/IRQ : OK",  0x0A);   newline();
    print("Keyboard: IRQ", 0x0A);   newline();
    print("Type 'help' for commands", 0x08); newline();
    print("----------------------------",   0x08); newline();

    shell_init();
    fs_init();
    loader_init();
    pmm_init(mb_info_addr);
    print("> ", 0x0F);

    /* Enable hardware interrupts and sleep until the next one fires.
       The keyboard IRQ handler does all the work from here on. */
    __asm__ volatile("sti");
    while (1)
        __asm__ volatile("hlt");
}
