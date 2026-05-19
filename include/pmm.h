#ifndef PMM_H
#define PMM_H

#include <stdint.h>

void  pmm_init(uint32_t mb_info_addr);
void* pmm_alloc();
void  pmm_free(void* ptr);
void  pmm_print_stats();

#endif