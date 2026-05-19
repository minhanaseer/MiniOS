static char* video = (char*) 0xb8000;

static void vga_char(int row, int col, char c, char attr) {
    video[(row*80+col)*2]     = c;
    video[(row*80+col)*2 + 1] = attr;
}

static void vga_text(int row, int col, const char* s, char attr) {
    for (int i = 0; s[i]; i++)
        vga_char(row, col+i, s[i], attr);
}

static void vga_fill_row(int row, char c, char attr) {
    for (int col = 0; col < 80; col++)
        vga_char(row, col, c, attr);
}

static void vga_box(int row, int col, int w, char attr) {
    for (int i = 0; i < w; i++)
        vga_char(row, col+i, ' ', attr);
}

void vga_demo() {
    for (int row = 0; row < 25; row++)
        vga_fill_row(row, ' ', 0x17);

    vga_fill_row(0, ' ', 0x70);
    vga_text(0, 2,  "MyKernel v0.1", 0x70);
    vga_text(0, 35, "Graphics Demo", 0x70);
    vga_text(0, 72, "04/2026", 0x70);

    for (int row = 2; row < 20; row++)
        vga_box(row, 5, 45, 0x07);

    vga_box(2, 5, 45, 0x17);
    vga_text(2, 7,  "Terminal - MyKernel", 0x1F);
    vga_text(2, 47, "X", 0x4F);

    vga_text(4,  7, "MyKernel v0.1", 0x0A);
    vga_text(5,  7, "Built from scratch in C + Assembly", 0x07);
    vga_text(6,  7, "Developer: Minha Naseer", 0x0B);
    vga_text(7,  7, "Running on QEMU x86 virtual machine", 0x07);
    vga_text(12, 7, "> ", 0x0F);

    for (int row = 2; row < 20; row++)
        vga_box(row, 55, 24, 0x07);

    vga_box(2, 55, 24, 0x27);
    vga_text(2, 57, "System Info", 0x2F);

    vga_text(4,  57, "OS: MyKernel", 0x07);
    vga_text(5,  57, "Arch: x86 32bit", 0x07);
    vga_text(6,  57, "RAM: 32 MB", 0x07);
    vga_text(7,  57, "CPU: i386 QEMU", 0x07);
    vga_text(9,  57, "Status: OK", 0x0A);
    vga_text(11, 57, "Phases done:", 0x0F);
    vga_text(12, 57, " Boot     [OK]", 0x0A);
    vga_text(13, 57, " Shell    [OK]", 0x0A);
    vga_text(14, 57, " Memory   [OK]", 0x0A);
    vga_text(15, 57, " Files    [OK]", 0x0A);
    vga_text(16, 57, " Graphics [OK]", 0x0A);

    vga_fill_row(24, ' ', 0x70);
    vga_text(24, 2, "Press Enter to return to shell...", 0x70);

    while (1) {
        unsigned char status, sc;
        __asm__ volatile("inb $0x64, %0" : "=a"(status));
        if (!(status & 1)) continue;
        __asm__ volatile("inb $0x60, %0" : "=a"(sc));
        if (sc == 0x1C) break;
    }
}

void vga_set_graphics() {}
void vga_set_text()    {}
void vga_put_pixel(int x, int y, unsigned char c) { (void)x;(void)y;(void)c; }
void vga_clear(unsigned char c) { (void)c; }
void vga_draw_rect(int x, int y, int w, int h, unsigned char c) {
    (void)x;(void)y;(void)w;(void)h;(void)c;
}