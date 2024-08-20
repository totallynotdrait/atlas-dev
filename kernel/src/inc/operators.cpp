#include "operators.h"



void* operator new(unsigned long int size) { return (void*)kmalloc((uint64_t)size); }
void* operator new[](unsigned long int size) { return (void*)kmalloc((uint64_t)size); }

void operator delete(void* ptr) { return kfree(ptr); }
void operator delete[](void* ptr) { return kfree(ptr); }

void operator delete(void* ptr, unsigned long int size) { return kfree(ptr); }
void operator delete[](void* ptr, unsigned long int size) { return kfree(ptr); }