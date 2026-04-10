static char* video = (char*) 0xb8000;

void vga_demo() {
    // Fill screen with colourful pattern using VGA text colours
    int colours[] = {1,2,3,4,5,6,9,10,11,12,13,14};
    int num_colours = 12;

    // Fill entire screen with coloured blocks
    for (int row = 0; row < 25; row++) {
        for (int col = 0; col < 80; col++) {
            int idx = (row + col) % num_colours;
            char bg = colours[idx] << 4;
            char fg = 15;
            video[(row*80 + col)*2]     = ' ';
            video[(row*80 + col)*2 + 1] = bg | fg;
        }
    }

    // Draw title bar
    for (int col = 0; col < 80; col++) {
        video[col*2]     = ' ';
        video[col*2 + 1] = 0x17;
    }

    // Write title
    const char* title = "  MyKernel v0.1 - Graphics Demo!  ";
    for (int i = 0; title[i]; i++) {
        video[i*2]     = title[i];
        video[i*2 + 1] = 0x17;
    }

    // Draw a white box in the middle
    for (int row = 8; row < 18; row++) {
        for (int col = 20; col < 60; col++) {
            video[(row*80+col)*2]     = ' ';
            video[(row*80+col)*2 + 1] = 0x70;
        }
    }

    // Write text in the box
    const char* msg1 = "  Welcome to MyKernel Graphics!  ";
    const char* msg2 = "  Built from scratch in C + ASM  ";
    const char* msg3 = "       Developer: Minha           ";
    const char* msg4 = "  Phase 6 - Colour Mode Active!  ";

    for (int i = 0; msg1[i]; i++) {
        video[(10*80+21+i)*2]     = msg1[i];
        video[(10*80+21+i)*2 + 1] = 0x70;
    }
    for (int i = 0; msg2[i]; i++) {
        video[(11*80+21+i)*2]     = msg2[i];
        video[(11*80+21+i)*2 + 1] = 0x70;
    }
    for (int i = 0; msg3[i]; i++) {
        video[(12*80+21+i)*2]     = msg3[i];
        video[(12*80+21+i)*2 + 1] = 0x74;
    }
    for (int i = 0; msg4[i]; i++) {
        video[(14*80+21+i)*2]     = msg4[i];
        video[(14*80+21+i)*2 + 1] = 0x72;
    }

    // Draw bottom bar
    for (int col = 0; col < 80; col++) {
        video[(24*80+col)*2]     = ' ';
        video[(24*80+col)*2 + 1] = 0x27;
    }
    const char* bottom = "  Press any key to return to shell...";
    for (int i = 0; bottom[i]; i++) {
        video[(24*80+i)*2]     = bottom[i];
        video[(24*80+i)*2 + 1] = 0x27;
    }

    // Wait for keypress
    unsigned char status, sc;
    do {
        do {
            __asm__ volatile("inb $0x64, %0" : "=a"(status));
        } while (!(status & 1));
        __asm__ volatile("inb $0x60, %0" : "=a"(sc));
    } while (sc & 0x80);
}

void vga_set_graphics() {}
void vga_set_text() {}
void vga_put_pixel(int x, int y, unsigned char colour) {
    (void)x; (void)y; (void)colour;
}
void vga_clear(unsigned char colour) { (void)colour; }
void vga_draw_rect(int x, int y, int w, int h, unsigned char colour) {
    (void)x; (void)y; (void)w; (void)h; (void)colour;
}