#pragma once

#include <stdint.h>
#include <efim/efiMemory.h>

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);
void memset(void* start, uint8_t value, uint64_t num);
void _memset_128(void* dest, uint8_t value, int64_t size);
void _memset(void* dest, uint8_t value, uint64_t size);