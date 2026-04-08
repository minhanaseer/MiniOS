typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

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

static void print_hex(uint32_t n) {
    char hex[] = "0123456789ABCDEF";
    print("0x", 0x0F);
    for (int i = 28; i >= 0; i -= 4)
        putchar(hex[(n >> i) & 0xF], 0x0F);
}

static void print_num(uint32_t n) {
    if (n == 0) { putchar('0', 0x0F); return; }
    char buf[12]; int i = 0;
    while (n > 0) { buf[i++] = '0'+(n%10); n/=10; }
    for (int j=i-1;j>=0;j--) putchar(buf[j], 0x0F);
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
    clear();

    print("=== MyKernel FB Debug ===", 0x0B); newline();

    print("flags : ", 0x0F); print_hex(mb->flags);   newline();
    print("fb_addr : ", 0x0F); print_hex(mb->fb_addr); newline();
    print("fb_w  : ", 0x0F); print_num(mb->fb_width);  newline();
    print("fb_h  : ", 0x0F); print_num(mb->fb_height); newline();
    print("fb_bpp: ", 0x0F); print_num(mb->fb_bpp);    newline();
    print("fb_pitch: ", 0x0F); print_num(mb->fb_pitch); newline();

    newline();
    print("If fb_addr is 0 = GRUB not passing framebuffer", 0x0E);
    newline();
    print("If fb_w is 0 = graphics mode not set", 0x0E);
    newline();

    pmm_init(0);
    fs_init();
    loader_init();
    shell_init();
    newline();
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
