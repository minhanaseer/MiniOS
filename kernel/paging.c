#include <stdint.h>
#include "../include/paging.h"

#define PAGE_PRESENT  0x1
#define PAGE_WRITABLE 0x2

/* One page directory and one page table, each 4 KB-aligned.
   Together they identity-map the first 4 MB of physical memory,
   which covers: low memory (0–1 MB), VGA buffer (0xB8000),
   the kernel image (loads at 1 MB), stack, heap, and PMM pages. */
static uint32_t page_dir  [1024] __attribute__((aligned(4096)));
static uint32_t page_table[1024] __attribute__((aligned(4096)));

void paging_init(void) {
    /* Fill page table: entry i maps virtual page i → physical page i (identity) */
    for (uint32_t i = 0; i < 1024; i++)
        page_table[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITABLE;

    /* Wire directory entry 0 to our page table (covers virtual 0x00000000–0x003FFFFF) */
    page_dir[0] = (uint32_t)page_table | PAGE_PRESENT | PAGE_WRITABLE;

    /* All other directory entries are not-present */
    for (uint32_t i = 1; i < 1024; i++)
        page_dir[i] = 0;

    /* CR3 = physical address of page directory */
    __asm__ volatile("mov %0, %%cr3" :: "r"(page_dir) : "memory");

    /* Set CR0.PG (bit 31) to enable paging */
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000u;
    __asm__ volatile("mov %0, %%cr0" :: "r"(cr0) : "memory");
}
