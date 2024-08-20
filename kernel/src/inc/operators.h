#include <stdint.h>
#include <mem/heap.h>

void* operator new(unsigned long int size);
void* operator new[](unsigned long int size);

void operator delete(void* ptr);
void operator delete[](void* ptr);

void operator delete(void* ptr, unsigned long int size);
void operator delete[](void* ptr, unsigned long int size);