#ifndef PIXEL_H
#define PIXEL_H

typedef unsigned int  uint32_t;
typedef unsigned char uint8_t;

void     pixel_init(uint32_t* addr, uint32_t w, uint32_t h, uint32_t pitch);
void     pixel_put(uint32_t x, uint32_t y, uint32_t colour);
uint32_t pixel_rgb(uint8_t r, uint8_t g, uint8_t b);
void     pixel_clear(uint32_t colour);
void     pixel_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t colour);
uint32_t pixel_get_w();
uint32_t pixel_get_h();
int      pixel_ready();

#endif
