#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/keyboard.h"

char* video = (char*) 0xb8000;
int cursor = 0;

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
}

void kernel_main() {
    // Clear screen first
    for (int i = 0; i < 80*25*2; i++) video[i] = 0;

    // Setup in correct order
    gdt_install();
    idt_install();

    // Only enable keyboard AFTER gdt and idt are ready
    keyboard_install();

    print("GDT installed!", 0x0A);
    newline();
    print("IDT installed!", 0x0A);
    newline();
    print("Keyboard ready! Type below:", 0x0A);
    newline();
    newline();
    print("> ", 0x0F);

    // Just wait — keyboard interrupts handle the rest
    while(1) {
        __asm__("hlt");
    }
}