#include "../include/keyboard.h"

static char* video = (char*) 0xb8000;
static int cursor = 160;

static unsigned char port_read(unsigned short port) {
    unsigned char result;
    __asm__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static void port_write(unsigned short port, unsigned char data) {
    __asm__("outb %0, %1" : : "a"(data), "Nd"(port));
}

static char sc_to_char(unsigned char sc) {
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

void keyboard_handler() {
    unsigned char sc = port_read(0x60);
    if (!(sc & 0x80)) {
        char c = sc_to_char(sc);
        if (c == '\n') {
            cursor = ((cursor / 80) + 1) * 80;
            if (cursor >= 80*25) cursor = 0;
            video[cursor*2]   = '>';
            video[cursor*2+1] = 0x0F;
            cursor++;
            video[cursor*2]   = ' ';
            video[cursor*2+1] = 0x0F;
            cursor++;
        } else if (c != 0) {
            if (cursor < 80*25) {
                video[cursor*2]   = c;
                video[cursor*2+1] = 0x0F;
                cursor++;
            }
        }
    }
    port_write(0x20, 0x20);
}

void keyboard_install() {
    port_write(0x20, 0x11);
    port_write(0xA0, 0x11);
    port_write(0x21, 0x20);
    port_write(0xA1, 0x28);
    port_write(0x21, 0x04);
    port_write(0xA1, 0x02);
    port_write(0x21, 0x01);
    port_write(0xA1, 0x01);
    port_write(0x21, 0xFD);
    port_write(0xA1, 0xFF);
    __asm__("sti");
}