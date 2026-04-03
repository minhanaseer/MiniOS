static char* video = (char*) 0xb8000;

void kernel_main() {
    // Clear screen
    for (int i = 0; i < 80*25*2; i++) video[i] = 0;

    // Write directly to video memory
    video[0] = 'O';  video[1] = 0x0A;
    video[2] = 'K';  video[3] = 0x0A;
    video[4] = '!';  video[5] = 0x0A;

    // Poll keyboard - no GDT, no IDT, nothing
    while (1) {
        unsigned char status;
        __asm__ volatile("inb $0x64, %0" : "=a"(status));
        if (status & 1) {
            unsigned char sc;
            __asm__ volatile("inb $0x60, %0" : "=a"(sc));
            // just show raw scancode as a number
            video[10] = '0' + (sc & 0xF);
            video[11] = 0x0F;
        }
    }
}