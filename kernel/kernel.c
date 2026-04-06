typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

void fb_init(uint32_t* addr, uint32_t w, uint32_t h, uint32_t pitch);
void fb_clear(uint8_t r, uint8_t g, uint8_t b);
void fb_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b);
void fb_draw_text(const char* s, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

struct mb_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint32_t fb_addr;
    uint32_t fb_pitch;
    uint32_t fb_width;
    uint32_t fb_height;
    uint8_t  fb_bpp;
};

void kernel_main(uint32_t magic, struct mb_info* mb) {
    (void)magic;

    uint32_t* fb = (uint32_t*)(mb->fb_addr);
    uint32_t w   = mb->fb_width;
    uint32_t h   = mb->fb_height;
    uint32_t p   = mb->fb_pitch;

    fb_init(fb, w, h, p);
    fb_clear(20, 30, 60);
    fb_draw_rect(0, 0, w, 30, 30, 50, 100);
    fb_draw_rect(0, h-40, w, 40, 40, 40, 80);
    fb_draw_text("MyKernel v0.1 - Phase 6 Graphics!", 10, 10, 255, 255, 255);
    fb_draw_rect(50, 50, w-100, h-120, 15, 15, 25);
    fb_draw_rect(50, 50, w-100, 24, 60, 60, 120);
    fb_draw_text("Terminal", 60, 57, 255, 255, 255);
    fb_draw_rect(w-106, 52, 20, 20, 200, 50, 50);
    fb_draw_text("X", w-102, 57, 255, 255, 255);
    fb_draw_text("Welcome to MyKernel Graphics Mode!", 60, 90, 100, 255, 100);
    fb_draw_text("Built from scratch in C + Assembly", 60, 106, 180, 180, 180);
    fb_draw_text("Developer: Minha", 60, 122, 100, 200, 255);
    fb_draw_text(">", 60, 150, 255, 255, 0);

    while(1) {}
}
