#pragma once

#include <stdint.h>
#include <efim/efiMemory.h>

typedef uint64_t size_t;

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);
void memset(void* start, uint8_t value, uint64_t num);
void _memset_128(void* dest, uint8_t value, int64_t size);
void _memset(void* dest, uint8_t value, uint64_t size);
void* memcpy(void* dest, const void* src, size_t n);
int memcmp(void* a, void* b, size_t n);