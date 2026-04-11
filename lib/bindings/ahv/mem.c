/*
 * Simplified AHV memory management - no linker symbols needed
 */

#include "bindings.h"

static uint64_t heap_start = 0x2000000;  /* Start heap at 32MB */

void mem_init(void)
{
    uint64_t mem_size = platform_mem_size();
    heap_start = 0x2000000;  /* 32MB fixed */
    
    log(INFO, "Solo5 AHV: Memory map: %llu MB addressable\n",
            (unsigned long long)mem_size >> 20);
    log(INFO, "Solo5 AHV: heap @ 0x%llx - 0x%llx\n",
            (unsigned long long)heap_start, (unsigned long long)mem_size);
}

void mem_lock_heap(uintptr_t *start, size_t *size)
{
    *start = heap_start;
    *size = platform_mem_size() - heap_start;
}

void *mem_ialloc_pages(size_t num)
{
    uintptr_t prev = heap_start;
    heap_start += num << PAGE_SHIFT;
    return (void *)prev;
}