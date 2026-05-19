#include <stdint.h>
#include "../include/pmm.h"

#define PAGE_SIZE  4096
#define MAX_PAGES  65536    /* covers up to 256 MB */

typedef struct {
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
} __attribute__((packed)) mb_info_t;

typedef struct {
    uint32_t size;
    uint32_t addr_low;
    uint32_t addr_high;
    uint32_t len_low;
    uint32_t len_high;
    uint32_t type;          /* 1 = usable RAM */
} __attribute__((packed)) mb_mmap_t;

extern uint32_t kernel_end;

extern void print(const char* s, char col);
extern void newline();
extern void putchar(char c, char col);

static uint32_t bitmap[MAX_PAGES / 32];
static uint32_t total_pages = 0;
static uint32_t free_pages  = 0;
static uint32_t total_mem_kb = 0;

static void page_set_used(uint32_t page) { bitmap[page/32] |=  (1u << (page%32)); }
static void page_set_free(uint32_t page) { bitmap[page/32] &= ~(1u << (page%32)); }
static int  page_is_used (uint32_t page) { return (bitmap[page/32] >> (page%32)) & 1; }

void pmm_init(uint32_t mb_info_addr) {
    mb_info_t* mb = (mb_info_t*)mb_info_addr;

    total_mem_kb = 1024 + mb->mem_upper;
    total_pages  = (total_mem_kb * 1024) / PAGE_SIZE;
    if (total_pages > MAX_PAGES) total_pages = MAX_PAGES;

    /* Mark everything used to start */
    for (uint32_t i = 0; i < MAX_PAGES / 32; i++)
        bitmap[i] = 0xFFFFFFFF;

    /* Walk Multiboot memory map and free usable regions */
    if (mb->flags & (1 << 6)) {
        mb_mmap_t* entry = (mb_mmap_t*)mb->mmap_addr;
        uint32_t   end   = mb->mmap_addr + mb->mmap_length;

        while ((uint32_t)entry < end) {
            if (entry->type == 1 && entry->addr_high == 0) {
                uint32_t page  = entry->addr_low / PAGE_SIZE;
                uint32_t count = entry->len_low  / PAGE_SIZE;
                for (uint32_t i = 0; i < count && (page+i) < total_pages; i++)
                    page_set_free(page + i);
            }
            entry = (mb_mmap_t*)((uint32_t)entry + entry->size + 4);
        }
    } else {
        /* Fallback: mark everything above 1MB as free */
        for (uint32_t i = 256; i < total_pages; i++)
            page_set_free(i);
    }

    /* Re-mark low memory (0–1 MB) and the kernel image as used */
    uint32_t kend_page = ((uint32_t)&kernel_end + PAGE_SIZE - 1) / PAGE_SIZE;
    for (uint32_t i = 0; i < kend_page && i < total_pages; i++)
        page_set_used(i);

    /* Count free pages */
    free_pages = 0;
    for (uint32_t i = 0; i < total_pages; i++)
        if (!page_is_used(i)) free_pages++;
}

void* pmm_alloc() {
    for (uint32_t i = 1; i < total_pages; i++) {
        if (!page_is_used(i)) {
            page_set_used(i);
            free_pages--;
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0;
}

void pmm_free(void* ptr) {
    uint32_t page = (uint32_t)ptr / PAGE_SIZE;
    if (page > 0 && page < total_pages && page_is_used(page)) {
        page_set_free(page);
        free_pages++;
    }
}

static void print_num(uint32_t n) {
    if (n == 0) { putchar('0', 0x0F); return; }
    char buf[12]; int i = 0;
    while (n > 0) { buf[i++] = '0' + (n % 10); n /= 10; }
    for (int j = i-1; j >= 0; j--) putchar(buf[j], 0x0F);
}

void pmm_print_stats() {
    print("Memory stats:", 0x0B);                                              newline();
    print("  Total RAM   : ", 0x0F); print_num(total_mem_kb / 1024);
    print(" MB", 0x0F);                                                        newline();
    print("  Page size   : 4096 bytes", 0x0F);                                 newline();
    print("  Total pages : ", 0x0F); print_num(total_pages);                   newline();
    print("  Free pages  : ", 0x0F); print_num(free_pages);                    newline();
    print("  Free RAM    : ", 0x0F);
    print_num((free_pages * PAGE_SIZE) / (1024 * 1024));
    print(" MB", 0x0F);                                                        newline();
    print("  Status      : OK", 0x0A);                                         newline();
}
