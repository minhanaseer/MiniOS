#include "../include/gdt.h"

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

void kernel_main() {
    // Clear screen
    for (int i = 0; i < 80*25*2; i++) {
        video[i] = 0;
    }

    // Install GDT
    gdt_install();

    // Show success
    print("GDT installed successfully!", 0x0A);
    cursor = 80;
    print("MyKernel v0.1 - Ready!", 0x0F);

    while(1);
}