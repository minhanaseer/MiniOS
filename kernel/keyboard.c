#include "../include/keyboard.h"
#include "../include/irq.h"
#include "../include/pic.h"

static int shift_held = 0;

/* Unshifted layout */
static const char sc_normal[58] = {
    0,   0,  '1','2','3','4','5','6','7','8','9','0','-','=',
    0,   0,  'q','w','e','r','t','y','u','i','o','p','[',']',
    '\n',0,  'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0,  ' '
};

/* Shifted layout (same indices, different characters) */
static const char sc_shift[58] = {
    0,   0,  '!','@','#','$','%','^','&','*','(',')','_','+',
    0,   0,  'Q','W','E','R','T','Y','U','I','O','P','{','}',
    '\n',0,  'A','S','D','F','G','H','J','K','L',':','"','~',
    0,  '|', 'Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0,  ' '
};

extern void putchar(char c, char col);
extern void backspace(void);
extern void shell_putchar(char c);

static unsigned char inb(unsigned short port) {
    unsigned char r;
    __asm__ volatile("inb %1, %0" : "=a"(r) : "Nd"(port));
    return r;
}

static void keyboard_handler(registers_t* regs) {
    (void)regs;
    unsigned char sc = inb(0x60);

    /* Key release: bit 7 set */
    if (sc & 0x80) {
        unsigned char rel = sc & 0x7F;
        if (rel == 0x2A || rel == 0x36)
            shift_held = 0;
        return;
    }

    /* Left shift = 0x2A, Right shift = 0x36 */
    if (sc == 0x2A || sc == 0x36) { shift_held = 1; return; }

    /* Backspace = 0x0E */
    if (sc == 0x0E) {
        backspace();
        shell_putchar('\b');
        return;
    }

    char c = (sc < 58) ? (shift_held ? sc_shift[sc] : sc_normal[sc]) : 0;
    if (c == '\n') {
        shell_putchar('\n');
    } else if (c != 0) {
        putchar(c, 0x0F);
        shell_putchar(c);
    }
}

void keyboard_init(void) {
    irq_register(1, keyboard_handler);
    pic_unmask_irq(1);
}
