#include <stdint.h>

static uint32_t* fb      = 0;
static uint32_t  fb_w    = 0;
static uint32_t  fb_h    = 0;
static uint32_t  fb_p    = 0;

void pixel_init(uint32_t* addr, uint32_t w, uint32_t h, uint32_t pitch) {
    fb   = addr;
    fb_w = w;
    fb_h = h;
    fb_p = pitch;
}

void pixel_put(uint32_t x, uint32_t y, uint32_t colour) {
    if (x >= fb_w || y >= fb_h) return;
    fb[y * (fb_p/4) + x] = colour;
}

uint32_t pixel_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

void pixel_clear(uint32_t colour) {
    for (uint32_t y = 0; y < fb_h; y++)
        for (uint32_t x = 0; x < fb_w; x++)
            pixel_put(x, y, colour);
}

void pixel_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t colour) {
    for (uint32_t row = y; row < y+h && row < fb_h; row++)
        for (uint32_t col = x; col < x+w && col < fb_w; col++)
            pixel_put(col, row, colour);
}

uint32_t pixel_get_w() { return fb_w; }
uint32_t pixel_get_h() { return fb_h; }
int pixel_ready() { return fb != 0 && fb_w > 0; }
