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
void free(void* address);

void ExpandHeap(size_t length);

inline void* operator new(size_t size) {return malloc(size);}
inline void* operator new[](size_t size) {return malloc(size);}

inline void operator delete(void* p) {free(p);}