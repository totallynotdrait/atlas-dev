#include <stdint.h>
#include <mem/heap.h>

void* operator new(size_t size);
void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, size_t) noexcept;