void vga_demo() {}
void vga_set_graphics() {}
void vga_set_text() {}
void vga_put_pixel(int x, int y, unsigned char c) { (void)x;(void)y;(void)c; }
void vga_clear(unsigned char c) { (void)c; }
void vga_draw_rect(int x, int y, int w, int h, unsigned char c) {
    (void)x;(void)y;(void)w;(void)h;(void)c;
}
