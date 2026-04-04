typedef unsigned char  uint8_t;
typedef unsigned int   uint32_t;

#define PAGE_SIZE    4096
#define MAX_PAGES    1024

static uint8_t  pages[MAX_PAGES];
static uint32_t total_pages = 0;
static uint32_t used_pages  = 0;

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
    total_pages = mem_size / PAGE_SIZE;
    if (total_pages > MAX_PAGES)
        total_pages = MAX_PAGES;
    for (uint32_t i = 0; i < total_pages; i++)
        pages[i] = 0;
    used_pages = 0;
}

void* pmm_alloc() {
    for (uint32_t i = 0; i < total_pages; i++) {
        if (pages[i] == 0) {
            pages[i] = 1;
            used_pages++;
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0;
}

void pmm_free(void* ptr) {
    uint32_t addr = (uint32_t) ptr;
    uint32_t page = addr / PAGE_SIZE;
    if (page < total_pages && pages[page] == 1) {
        pages[page] = 0;
        used_pages--;
    }
}

void pmm_print_stats() {
    print("Memory stats:", 0x0B);
    newline();
    print("  Total pages : ", 0x0F);
    print_num(total_pages);
    newline();
    print("  Used pages  : ", 0x0F);
    print_num(used_pages);
    newline();
    print("  Free pages  : ", 0x0F);
    print_num(total_pages - used_pages);
    newline();
    print("  Page size   : 4096 bytes", 0x0F);
    newline();
}