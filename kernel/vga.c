static char* video = (char*) 0xb8000;

void vga_demo() {
    int colours[] = {1,2,3,4,5,6,9,10,11,12,13,14};

    for (int row = 0; row < 25; row++) {
        for (int col = 0; col < 80; col++) {
            int idx = (row + col) % 12;
            video[(row*80+col)*2]     = ' ';
            video[(row*80+col)*2 + 1] = (char)(colours[idx] << 4);
        }
    }

    const char* title = "MyKernel Graphics Demo!";
    for (int i = 0; title[i]; i++) {
        video[i*2]     = title[i];
        video[i*2 + 1] = 0x0F;
    }
}

void vga_set_graphics() {}
void vga_set_text()    {}
void vga_put_pixel(int x, int y, unsigned char c) { (void)x;(void)y;(void)c; }
void vga_clear(unsigned char c) { (void)c; }
void vga_draw_rect(int x, int y, int w, int h, unsigned char c) {
    (void)x;(void)y;(void)w;(void)h;(void)c;
}