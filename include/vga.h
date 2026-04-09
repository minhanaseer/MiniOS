#ifndef VGA_H
#define VGA_H

void vga_set_graphics();
void vga_set_text();
void vga_put_pixel(int x, int y, unsigned char colour);
void vga_clear(unsigned char colour);
void vga_draw_rect(int x, int y, int w, int h, unsigned char colour);
void vga_draw_line(int x1, int y1, int x2, int y2, unsigned char colour);
void vga_demo();

#endif