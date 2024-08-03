#include "kheap.h"

#define MEMORY_POOL_SIZE 1024 * 1024
static uint8_t memory_pool[MEMORY_POOL_SIZE];
static size_t memory_pool_offset = 0;

void* kmalloc(size_t size) {
    if (memory_pool_offset + size > MEMORY_POOL_SIZE) {
        log->warn("Out of resources.");
        return nullptr;
    }
    void* ptr = &memory_pool[memory_pool_offset];
    memory_pool_offset += size;
    return ptr;
}

void kfree(void* ptr) {
    log->warn("TODO:");
}