#ifndef FB_H
#define FB_H

typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

void fb_init(uint32_t* addr, uint32_t w, uint32_t h, uint32_t pitch);
void fb_put_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void fb_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b);
void fb_clear(uint8_t r, uint8_t g, uint8_t b);
void fb_draw_char(char c, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void fb_draw_text(const char* s, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

#endif
