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

void kernel_main(uint32_t magic, uint32_t mb_addr) {
    clear();
    print("=== MyKernel Debug ===", 0x0B); newline();
    print("magic: ", 0x0F); print_hex(magic); newline();
    print("mb_addr: ", 0x0F); print_hex(mb_addr); newline();

    if (magic != 0x2BADB002) {
        print("ERROR: Bad multiboot magic!", 0x04); newline();
    } else {
        print("Multiboot OK!", 0x0A); newline();
    }

    newline();
    pmm_init(0);
    fs_init();
    loader_init();
    shell_init();
    print("MyKernel v0.1 - Ready!", 0x0A); newline();
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
