#include "memory.h"

extern "C" void __memset(void* dest, int val, size_t n);
extern "C" void __memcpy(void* dest, const void* src, size_t n);
extern "C" int __memcmp(void* dest, const void* src, size_t n);

int memcmp(void* a, void* b, size_t n)
{
    if(n <= 0)
        return -1;

    return (__memcmp(a, b, n) == 0);
}

void* memcpy(void* dest, const void* src, size_t n)
{
    if(n <= 0)
        return dest;

    size_t sz = n;
    size_t i = 0;

    for (i; i < n;)
    {
        uint64_t dest_curr = (uint64_t)dest + i;
        uint64_t src_curr = (uint64_t)src + i;

        if((sz >= 8) && !(dest_curr & 0x7) && !(src_curr & 0x7))
        {
            *(uint64_t*)dest_curr = *(uint64_t*)src_curr;

            sz -= 8;
            i += 8;
        }
        else if((sz >= 4) && !(dest_curr & 0x3) && !(src_curr & 0x3))
        {
            *(uint32_t*)dest_curr = *(uint32_t*)src_curr;

            sz -= 4;
            i += 4;
        }
        else if((sz >= 2) && !(dest_curr & 0x1) && !(src_curr & 0x1))
        {
            *(uint16_t*)dest_curr = *(uint16_t*)src_curr;

            sz -= 2;
            i += 2;
        }
        else
        {
            *(uint8_t*)dest_curr = *(uint8_t*)src_curr;

            sz -= 1;
            i += 1;
        }
    }

    //__memcpy(dest, src, n);
    return dest;
}

void* memmove(void* dest, const void* src, size_t n) {
    // Cast pointers to uint8_t* for byte-wise manipulation
    uint8_t* d = static_cast<uint8_t*>(dest);
    const uint8_t* s = static_cast<const uint8_t*>(src);

    // If the source and destination overlap, copy from the end to avoid overwriting
    if (d > s && d < s + n) {
        d += n;
        s += n;
        while (n--) {
            *(--d) = *(--s);
        }
    } else {
        // No overlap, or destination is before source, copy normally
        while (n--) {
            *d++ = *s++;
        }
    }

    return dest;
}

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize) {
    static uint64_t memorySizeBytes = 0;
    if (memorySizeBytes > 0) return memorySizeBytes;

    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        memorySizeBytes += desc->numPages * 4096;
    }

    return memorySizeBytes;
}
void memset(void* start, uint8_t value, uint64_t num) {
    for (uint64_t i = 0; i < num; i++) {
        *(uint8_t*)((uint64_t)start + i) = value;
    }
}

void _memset_128(void* dest, uint8_t value, int64_t size)
{
	// __uint128_t is 16 bytes
	auto _dest = (__uint128_t*)dest;

	// Create a 128 bit value with the byte value in each byte
	__uint128_t val = value;
	val |= val << 8;
	val |= val << 16;
	val |= val << 32;
	val |= val << 64;

	size >>= 4; // size /= 16
	while (size--)
		*(_dest++) = val;
}

void _memset(void* dest, uint8_t value, uint64_t size)
{
	if (size & 0xFFE0)//(size >= 32)
	{
		uint64_t s2 = size & 0xFFFFFFF0;
		_memset_128(dest, value, s2);
		if (!s2)
			return;
		dest = (void*)((uint64_t)dest + s2);
		size = size & 0xF;
	}

    char* d = (char*)dest;
    for (uint64_t i = 0; i < size; i++)
        *(d++) = value;
}