#include "heap.h"
#include <pageframe/PageTableManager.h>
#include <pageframe/PageFrameAllocator.h>

void* heapStart;
void* heapEnd;
HeapSegHeader* LastHeader;

void InitializeHeap(void* heapAddress, size_t pageCount) {
    void* pos = heapAddress;


    for (size_t i = 0; i < pageCount; i++) {
        GPageTableManager.MapMemory(pos, GlobalAllocator.RequestPage());
        pos = (void*)((size_t)pos + 0x1000);
    }
    size_t heapLength = pageCount * 0x1000;

    heapStart = heapAddress;
    heapEnd = (void*)((size_t)heapStart + heapLength);
    HeapSegHeader* startSeg = (HeapSegHeader*)heapAddress;
    startSeg->length = heapLength - sizeof(HeapSegHeader);
    startSeg->next = NULL;
    startSeg->last = NULL;
    startSeg->free = true;
    LastHeader = startSeg;
}

void free(void* address) {
    HeapSegHeader* segment = (HeapSegHeader*)address - 1;
    segment->free = true;
    segment->CombineForward();
    segment->CombineBackwward();
}

void* malloc(size_t size) {
    if (size % 0x10 > 0) {
        size -= (size % 0x10);
        size += 0x10;
    }

    if (size == 0) return NULL;

    HeapSegHeader* currentSeg = (HeapSegHeader*) heapStart;
    while (true) {
        if (currentSeg->free) {
            if (currentSeg->length > size) {
                currentSeg->Split(size);
                currentSeg->free = false;
                return (void*)((uint64_t)currentSeg + sizeof(HeapSegHeader));
            }
            if (currentSeg->length == size) {
                currentSeg->free = false;
                return (void*)((uint64_t)currentSeg + sizeof(HeapSegHeader));
            }
        }
        if (currentSeg->next == NULL) break;
        currentSeg = currentSeg->next;
    }
    ExpandHeap(size);
    return malloc(size);
}

void* alloc(uint64_t size, uint64_t alignment)
{
    // it is not a multiple of 0x10
    if ((size % 0x10) > 0)
    { 
        size -= (size % 0x10);
        size += 0x10;
    }

    if (size == 0) 
    {
        return nullptr;
    }

    HeapSegHeader* currentSeg = (HeapSegHeader*) heapStart;
    while(true)
    {
        if(currentSeg->free)
        {
            if (currentSeg->length > size)
            {
                currentSeg->Split(size);
            }

            if (!(currentSeg->length < size))
            {
                currentSeg->free = false;
                return (void*)((uint64_t)currentSeg + sizeof(HeapSegHeader));
            }
        }

        if (currentSeg->next == nullptr) 
        {
            break;
        }

        currentSeg = currentSeg->next;
    }

    ExpandHeap(size);
    return alloc(size, alignment);
}

HeapSegHeader* HeapSegHeader::Split(size_t splitLength) {
    if (splitLength < 0x10) return NULL;
    int64_t splitSegLength = length - splitLength - (sizeof(HeapSegHeader));
    if (splitSegLength < 0x10) return NULL;

    HeapSegHeader* newSplitHeader = (HeapSegHeader*) ((size_t)this + splitLength + sizeof(HeapSegHeader));
    next->last = newSplitHeader;
    newSplitHeader->next = next;
    next = newSplitHeader;
    newSplitHeader->last = this;
    newSplitHeader->length = splitSegLength;
    newSplitHeader->free = free;
    length = splitLength;

    if (LastHeader == this) LastHeader = newSplitHeader;
    return newSplitHeader;
}

void ExpandHeap(size_t length) {
    if (length % 0x1000) {
        length -= length % 0x1000;
        length += 0x1000;
    }

    size_t pageCount = length / 0x1000;
    HeapSegHeader* newSegment = (HeapSegHeader*)heapEnd;

    for (size_t i = 0; i < pageCount; i++) {
        GPageTableManager.MapMemory(heapEnd, GlobalAllocator.RequestPage());
        heapEnd = (void*)((size_t)heapEnd + 0x1000);
    }
    newSegment->free = true;
    newSegment->last = LastHeader;
    LastHeader->next = newSegment;
    LastHeader = newSegment;
    newSegment->next = NULL;
    newSegment->length = length - sizeof(HeapSegHeader);
    newSegment->CombineBackwward();
}

void HeapSegHeader::CombineForward() {
    if (next == NULL) return;
    if (!next->free) return;

    if (next == LastHeader) LastHeader = this;

    if (next->next != NULL) {
        next->next->last = this;
    }

    length = length + next->length + sizeof(HeapSegHeader);

    next = next->next;
}

void HeapSegHeader::CombineBackwward() {
    if (last != NULL && last->free) last->CombineForward();
}


extern uint64_t _KAtaEnd;
uint64_t free_mem_addr = (uint64_t)&_KAtaEnd;

uint8_t heap_init = 0;

extern PageTableManager KernelDirectory;
uint64_t kmalloc_int(uint64_t size, int align, uint64_t* phys_addr)
{
    if(heap_init == 0)
    {
        if (align == 1 && (free_mem_addr & 0x00000FFF)) 
        {
            free_mem_addr &= 0xFFFFF000;
            free_mem_addr += 0x1000;
        }

        if (phys_addr) 
        {
            *phys_addr = free_mem_addr;
        }

        uint64_t ret = free_mem_addr;
        free_mem_addr += size;

        return ret;
    }
    else
    {
        void* addr = alloc(size);

        if (phys_addr != 0)
        {
            uint64_t phys = KernelDirectory.PhysicalAddress((uint64_t)addr);
            *phys_addr = (phys + ((uint64_t)addr & 0xFFF));
        }

        return (uint64_t)addr;
    }
}

void kfree(void* ptr)
{
    if(heap_init == 1)
    {
        free(ptr);
    }
}

uint64_t kmalloc_a(uint64_t size)
{
    return kmalloc_int(size, 1, 0);
}

uint64_t kmalloc_p(uint64_t size, uint64_t* phys)
{
    return kmalloc_int(size, 0, phys);
}

uint64_t kmalloc_ap(uint64_t size, uint64_t* phys)
{
    return kmalloc_int(size, 1, phys);
}

uint64_t kmalloc(uint64_t size)
{
    return kmalloc_int(size, 0, 0);
}