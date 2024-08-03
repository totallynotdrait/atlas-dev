#include "operators.h"

void* operator new(size_t size) {
    // Implement your own memory allocation logic here.
    // For simplicity, you might just use a fixed-size pool or an existing allocator.
    return malloc(size); // Replace with your memory allocation function
}

void operator delete(void* ptr) noexcept {
    // Implement your own memory deallocation logic here.
    // For simplicity, you might just use a fixed-size pool or an existing deallocator.
    free(ptr); // Replace with your memory deallocation function
}

void operator delete(void* ptr, size_t) noexcept {
    // This overload handles the case where the size is known.
    operator delete(ptr);
}