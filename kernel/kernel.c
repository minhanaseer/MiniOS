#include "../include/gdt.h"
#include "../include/idt.h"

static char* video = (char*) 0xb8000;
static int cursor = 0;

void clear_screen() {
    for (int i = 0; i < 80*25*2; i++) video[i] = 0;
    cursor = 0;
}

void print(const char* str, char color) {
    int i = 0;
    while (str[i] != 0) {
        video[cursor*2]   = str[i];
        video[cursor*2+1] = color;
        cursor++;
        i++;
    }
}

void newline() {
    cursor = ((cursor / 80) + 1) * 80;
    if (cursor >= 80*25) cursor = 0;
}

unsigned char port_read(unsigned short port) {
    unsigned char result;
    __asm__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

char sc_to_char(unsigned char sc) {
    char keys[58] = {
        0, 0, '1','2','3','4','5','6','7','8','9','0','-','=',
        0, 0, 'q','w','e','r','t','y','u','i','o','p','[',']',
        '\n', 0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
        0,'\\','z','x','c','v','b','n','m',',','.','/', 0,
        '*', 0, ' '
    };
    if (sc < 58) return keys[sc];
    return 0;
}

void kernel_main() {
    clear_screen();

    gdt_install();
    idt_install();

    print("GDT installed!", 0x0A);
    newline();
    print("IDT installed!", 0x0A);
    newline();
    print("MyKernel v0.1 - Type something!", 0x0A);
    newline();
    print("> ", 0x0F);

    unsigned char last_sc = 0;

    // Simple polling loop - no interrupts needed
    while (1) {
        unsigned char status = port_read(0x64);
        if (status & 0x01) {
            unsigned char sc = port_read(0x60);

            // Only on key press and new key
            if (!(sc & 0x80) && sc != last_sc) {
                char c = sc_to_char(sc);
                if (c == '\n') {
                    newline();
                    if (cursor >= 80*25) cursor = 0;
                    print("> ", 0x0F);
                } else if (c != 0 && cursor < 80*25) {
                    video[cursor*2]   = c;
                    video[cursor*2+1] = 0x0F;
                    cursor++;
                }
            }
            last_sc = (sc & 0x80) ? 0 : sc;
        }
    }
}