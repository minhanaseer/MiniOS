typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

void fb_init(uint32_t* addr, uint32_t w, uint32_t h, uint32_t pitch);
void fb_clear(uint8_t r, uint8_t g, uint8_t b);
void fb_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b);
void fb_draw_text(const char* s, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void shell_init();
void shell_putchar(char c);
void pmm_init(uint32_t mem_size);
void fs_init();
void loader_init();

static char* vga = (char*) 0xb8000;
static int cursor = 0;

void clear() {
    for (int i = 0; i < 80*25*2; i++) vga[i] = 0;
    cursor = 0;
}

void putchar(char c, char col) {
    if (cursor >= 80*25) { 
        for (int i = 0; i < 24*80*2; i++) vga[i] = vga[i+80*2];
        for (int i = 24*80*2; i < 25*80*2; i+=2) { vga[i]=' '; vga[i+1]=0x07; }
        cursor = 24*80;
    }
    vga[cursor*2]   = c;
    vga[cursor*2+1] = col;
    cursor++;
}

void print(const char* s, char col) {
    for (int i = 0; s[i]; i++) putchar(s[i], col);
}

void newline() {
    cursor = ((cursor/80)+1)*80;
    if (cursor >= 80*25) cursor = 24*80;
}

void backspace() {
    if (cursor > 0) {
        cursor--;
        vga[cursor*2]   = ' ';
        vga[cursor*2+1] = 0x07;
    }
}

static unsigned char inb(unsigned short port) {
    unsigned char r;
    __asm__ volatile("inb %1,%0":"=a"(r):"Nd"(port));
    return r;
}

static void wait() {
    for (volatile int i = 0; i < 10000; i++);
}

static char sc_map[58] = {
    0,  0,  '1','2','3','4','5','6','7','8','9','0','-','=',
    0,  0,  'q','w','e','r','t','y','u','i','o','p','[',']',
    '\n',0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
    0, '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0, ' '
};

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

    uint32_t fb_addr  = mb->fb_addr;
    uint32_t fb_w     = mb->fb_width;
    uint32_t fb_h     = mb->fb_height;
    uint32_t fb_pitch = mb->fb_pitch;
    uint8_t  fb_bpp   = mb->fb_bpp;

    if (fb_addr != 0 && fb_w > 0 && fb_h > 0 && fb_bpp == 32) {
        fb_init((uint32_t*)fb_addr, fb_w, fb_h, fb_pitch);
        fb_clear(20, 30, 60);
        fb_draw_rect(0, 0, fb_w, 30, 30, 50, 120);
        fb_draw_rect(0, fb_h-40, fb_w, 40, 30, 40, 80);
        fb_draw_text("MyKernel v0.1", 10, 10, 255, 255, 255);
        fb_draw_text("Phase 6 - Graphics Mode!", 200, 10, 100, 220, 255);
        fb_draw_rect(40, 45, fb_w-80, fb_h-110, 10, 10, 20);
        fb_draw_rect(40, 45, fb_w-80, 22, 50, 50, 100);
        fb_draw_text("Terminal - MyKernel", 50, 51, 200, 200, 255);
        fb_draw_rect(fb_w-86, 47, 18, 18, 180, 40, 40);
        fb_draw_text("X", fb_w-83, 51, 255, 255, 255);
        fb_draw_text("Welcome to MyKernel Graphics Mode!", 50, 80, 80, 255, 80);
        fb_draw_text("Built from scratch in C + Assembly", 50, 96, 160, 160, 160);
        fb_draw_text("Developer: Minha Naseer", 50, 112, 80, 180, 255);
        fb_draw_text("Phase 6 complete - pixels on screen!", 50, 128, 255, 200, 80);
        fb_draw_text(">", 50, 155, 255, 255, 0);
        while(1) {}
    }

    // Fallback to text mode
    clear();
    pmm_init(0);
    fs_init();
    loader_init();
    shell_init();
    print("MyKernel v0.1", 0x0A); newline();
    print("Graphics mode not available - text mode", 0x0E); newline();
    print("FB addr: ", 0x0F);
    print(fb_addr ? "exists" : "zero!", 0x04); newline();
    print("> ", 0x0F);

    unsigned char last_sc = 0;
    int hold_counter = 0;
    int key_held = 0;

    while (1) {
        unsigned char status = inb(0x64);
        if (!(status & 1)) continue;
        unsigned char sc = inb(0x60);
        if (sc & 0x80) {
            if ((sc & 0x7F) == last_sc) { last_sc=0; key_held=0; hold_counter=0; }
            continue;
        }
        if (sc != last_sc) {
            last_sc=sc; key_held=0; hold_counter=0;
            if (sc == 0x0E) { backspace(); shell_putchar('\b'); }
            else {
                char c = (sc < 58) ? sc_map[sc] : 0;
                if (c == '\n') shell_putchar('\n');
                else if (c) { putchar(c, 0x0F); shell_putchar(c); }
            }
        } else {
            hold_counter++;
            if (hold_counter > 3000) key_held = 1;
            if (key_held) {
                char c = (sc < 58) ? sc_map[sc] : 0;
                if (c && c != '\n') { putchar(c, 0x0F); shell_putchar(c); wait(); }
            }
        }
    }
}
