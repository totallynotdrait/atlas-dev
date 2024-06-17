#pragma once

#include <stdint.h>
#include <efim/efiMemory.h>

uint64_t GetMemorySize(KEFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);
void memset(void* start, uint8_t value, uint64_t num);