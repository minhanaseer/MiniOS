#include "../include/shell.h"
#include "../include/pmm.h"
#include "../include/fs.h"
#include "../include/loader.h"
#include "../include/pixel.h"

typedef unsigned int   uint32_t;
typedef unsigned char  uint8_t;

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

static void parse_mb2(uint32_t addr) {
    uint8_t* ptr = (uint8_t*)(addr + 8);
    uint8_t* end = (uint8_t*)addr + *(uint32_t*)addr;
    while (ptr < end) {
        uint32_t type = *(uint32_t*)ptr;
        uint32_t size = *(uint32_t*)(ptr + 4);
        if (type == 0) break;
        if (type == 8) {
            uint32_t lo  = *(uint32_t*)(ptr + 8);
            uint32_t hi  = *(uint32_t*)(ptr + 12);
            uint32_t pitch  = *(uint32_t*)(ptr + 16);
            uint32_t width  = *(uint32_t*)(ptr + 20);
            uint32_t height = *(uint32_t*)(ptr + 24);
            uint8_t  bpp    = *(uint8_t* )(ptr + 28);
            if (hi == 0 && bpp == 32)
                pixel_init((uint32_t*)lo, width, height, pitch);
            break;
        }
        ptr += (size + 7) & ~7;
    }
}

void kernel_main(uint32_t magic, uint32_t mb_addr) {
    if (magic == 0x36D76289 && mb_addr != 0)
        parse_mb2(mb_addr);

    clear();
    print("MyKernel v0.1", 0x0A);
    newline();
    print("Keyboard: OK", 0x0A);
    newline();

    if (pixel_ready()) {
        print("Pixel graphics: OK (", 0x0A);
        print("800x600", 0x0A);
        print(")", 0x0A);
        newline();
    } else {
        print("Pixel graphics: text mode only", 0x0E);
        newline();
    }

    print("Type 'help' for commands", 0x08);
    newline();
    print("----------------------------", 0x08);
    newline();

    shell_init();
    fs_init();
    loader_init();
    pmm_init(0);
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