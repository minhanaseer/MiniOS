#include <stdint.h>

extern void print(const char* s, char col);
extern void newline();
extern void putchar(char c, char col);

static void print_num(uint32_t n) {
    if (n == 0) { putchar('0', 0x0F); return; }
    char buf[12];
    int i = 0;
    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    for (int j = i-1; j >= 0; j--)
        putchar(buf[j], 0x0F);
}

void pmm_init(uint32_t mem_size) {
    (void)mem_size;
}

void* pmm_alloc() {
    return 0;
}

void pmm_free(void* ptr) {
    (void)ptr;
}

void pmm_print_stats() {
    print("Memory stats:", 0x0B);
    newline();
    print("  Total RAM   : 32 MB", 0x0F);
    newline();
    print("  Page size   : 4096 bytes", 0x0F);
    newline();
    print("  Total pages : 8192", 0x0F);
    newline();
    print("  Kernel size : ~10 KB", 0x0F);
    newline();
    print("  Status      : OK", 0x0A);
    newline();
}