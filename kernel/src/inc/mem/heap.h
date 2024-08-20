#pragma once
#include <stdint.h>
#include <stddef.h>

struct HeapSegHeader{
    size_t length;
    HeapSegHeader* next;
    HeapSegHeader* last;
    bool free;
    void CombineForward();
    void CombineBackwward();
    HeapSegHeader* Split(size_t splitLength);
};


void InitializeHeap(void* heapAddress, size_t pageCount);

void* malloc(size_t size);
void* alloc(uint64_t size, uint64_t alignment = 0); // i could make a overdrive
void free(void* address);

uint64_t kmalloc_int(uint64_t size, int align, uint64_t* phys_addr);
void kfree(void* ptr);

uint64_t kmalloc_a(uint64_t size); // page aligned.
uint64_t kmalloc_p(uint64_t size, uint64_t* phys); // returns a physical address.
uint64_t kmalloc_ap(uint64_t size, uint64_t* phys); // page aligned.
uint64_t kmalloc(uint64_t size); // uint64_t kmalloc

void ExpandHeap(size_t length);