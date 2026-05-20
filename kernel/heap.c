#include <stdint.h>
#include "../include/heap.h"
#include "../include/pmm.h"

#define ALIGN4(x)   (((uint32_t)(x) + 3u) & ~3u)
#define HEADER_SZ   ((uint32_t)sizeof(block_t))
#define MIN_SPLIT   16u

typedef struct block {
    uint32_t      size;  /* usable bytes, not counting this header */
    uint32_t      free;  /* 1 = free, 0 = allocated */
    struct block* next;
} block_t;

static block_t* head = 0;

void heap_init(void) {
    void* page = pmm_alloc();
    if (!page) return;
    head       = (block_t*)page;
    head->size = 4096u - HEADER_SZ;
    head->free = 1;
    head->next = 0;
}

/* Grab one 4 KB page from PMM and append it to the free list. */
static block_t* expand(void) {
    void* page = pmm_alloc();
    if (!page) return 0;
    block_t* blk = (block_t*)page;
    blk->size = 4096u - HEADER_SZ;
    blk->free = 1;
    blk->next = 0;
    if (!head) { head = blk; return blk; }
    block_t* curr = head;
    while (curr->next) curr = curr->next;
    curr->next = blk;
    return blk;
}

/* Carve `size` bytes out of `blk`, splitting the remainder if large enough. */
static void* carve(block_t* blk, uint32_t size) {
    if (blk->size >= size + HEADER_SZ + MIN_SPLIT) {
        block_t* split = (block_t*)((uint8_t*)blk + HEADER_SZ + size);
        split->size = blk->size - size - HEADER_SZ;
        split->free = 1;
        split->next = blk->next;
        blk->size   = size;
        blk->next   = split;
    }
    blk->free = 0;
    return (void*)((uint8_t*)blk + HEADER_SZ);
}

void* kmalloc(uint32_t size) {
    if (!size) return 0;
    size = ALIGN4(size);

    /* First-fit search */
    for (block_t* b = head; b; b = b->next)
        if (b->free && b->size >= size)
            return carve(b, size);

    /* No fit — get another page */
    block_t* blk = expand();
    if (!blk || blk->size < size) return 0;
    return carve(blk, size);
}

void kfree(void* ptr) {
    if (!ptr) return;
    block_t* blk = (block_t*)((uint8_t*)ptr - HEADER_SZ);
    blk->free = 1;

    /* Coalesce adjacent free blocks in one forward pass */
    for (block_t* c = head; c && c->next; ) {
        if (c->free && c->next->free) {
            c->size += HEADER_SZ + c->next->size;
            c->next  = c->next->next;
        } else {
            c = c->next;
        }
    }
}

/* ---- stats (used by shell 'mem' command) ---- */

extern void print(const char* s, char col);
extern void newline(void);
extern void putchar(char c, char col);

static void print_num(uint32_t n) {
    if (n == 0) { putchar('0', 0x0F); return; }
    char buf[12]; int i = 0;
    while (n > 0) { buf[i++] = '0' + (n % 10); n /= 10; }
    for (int j = i - 1; j >= 0; j--) putchar(buf[j], 0x0F);
}

void heap_stats(void) {
    uint32_t free_b = 0, used_b = 0, blocks = 0;
    for (block_t* c = head; c; c = c->next) {
        blocks++;
        if (c->free) free_b += c->size;
        else         used_b += c->size;
    }
    print("Heap stats:", 0x0B);                           newline();
    print("  Blocks      : ", 0x0F); print_num(blocks);   newline();
    print("  Used bytes  : ", 0x0F); print_num(used_b);   newline();
    print("  Free bytes  : ", 0x0F); print_num(free_b);   newline();
    print("  Status      : ", 0x0F);
    print(head ? "OK" : "NOT INIT", head ? 0x0A : 0x04);  newline();
}
