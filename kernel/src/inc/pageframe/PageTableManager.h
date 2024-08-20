#pragma once
#include "paging.h"


typedef uint64_t paddr_t;
typedef uint64_t vaddr_t;

class PageTableManager {
    public:
    PageTableManager(PageTable* PML4Address);
    PageTable* PML4;
    void MapMemory(void* virtualMemory, void* physicalMemory);
    paddr_t PhysicalAddress(vaddr_t virtualAddress);
    PageTable* pml4;
};

extern PageTableManager GPageTableManager;