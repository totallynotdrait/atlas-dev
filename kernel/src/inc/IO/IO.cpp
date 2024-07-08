#include "IO.h"

#define DEF_IO_WAIT 0

void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

void outw(uint16_t port, uint16_t value)
{
    __asm__ volatile("outw %w0, %w1" : : "a" (value), "Nd" (port));
    io_wait(DEF_IO_WAIT);
}

uint8_t inb(uint16_t port) {
    uint8_t returnVal;
    asm volatile ("inb %1, %0"
    : "=a"(returnVal)
    : "Nd"(port));
    return returnVal;
}

void io_wait() {
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}

void io_wait(uint64_t us) 
{
    while(us--)
        io_wait();
}

uint16_t inw(uint16_t port) 
{
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}


uint32_t inl(uint32_t port) 
{
    uint32_t result;
    asm volatile("inl %%dx, %%eax" : "=a" (result) : "d"(port));
    return result;
}


void outl(uint32_t port, uint32_t value) 
{
	asm volatile("outl %%eax, %%dx" :: "d" (port), "a" (value));
}
