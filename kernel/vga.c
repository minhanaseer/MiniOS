static unsigned char* vga_mem = (unsigned char*) 0xA0000;
static int graphics_mode = 0;

void vga_set_graphics() {
    __asm__ volatile(
        "mov $0x13, %%ax\n"
        "int $0x10\n"
        : : : "ax"
    );
    graphics_mode = 1;
}

void vga_set_text() {
    __asm__ volatile(
        "mov $0x03, %%ax\n"
        "int $0x10\n"
        : : : "ax"
    );
    graphics_mode = 0;
}

void vga_put_pixel(int x, int y, unsigned char colour) {
    if (x < 0 || x >= 320 || y < 0 || y >= 200) return;
    vga_mem[y * 320 + x] = colour;
}

void vga_clear(unsigned char colour) {
    for (int i = 0; i < 320*200; i++)
        vga_mem[i] = colour;
}

void vga_draw_rect(int x, int y, int w, int h, unsigned char colour) {
    for (int row = y; row < y+h; row++)
        for (int col = x; col < x+w; col++)
            vga_put_pixel(col, row, colour);
}

void vga_draw_line(int x1, int y1, int x2, int y2, unsigned char colour) {
    int dx = x2-x1, dy = y2-y1;
    int steps = dx > dy ? dx : dy;
    if (steps < 0) steps = -steps;
    if (steps == 0) return;
    int xi = dx / steps;
    int yi = dy / steps;
    int x = x1, y = y1;
    for (int i = 0; i <= steps; i++) {
        vga_put_pixel(x, y, colour);
        x += xi;
        y += yi;
    }
}

void vga_demo() {
    vga_set_graphics();

    vga_clear(1);

    vga_draw_rect(0, 0, 320, 12, 9);
    vga_draw_rect(0, 188, 320, 12, 8);

    vga_draw_rect(10, 20, 140, 160, 0);
    vga_draw_rect(11, 28, 138, 151, 2);

    vga_draw_rect(170, 20, 140, 70, 0);
    vga_draw_rect(171, 28, 138, 61, 4);

    vga_draw_rect(170, 100, 140, 80, 0);
    vga_draw_rect(171, 108, 138, 71, 6);

    for (int i = 0; i < 300; i += 10)
        vga_draw_line(i, 30, i+10, 170, (i/10) % 255 + 1);

    for (volatile int i = 0; i < 50000000; i++);

    vga_set_text();
}