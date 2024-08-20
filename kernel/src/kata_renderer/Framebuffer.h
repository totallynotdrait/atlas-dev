#pragma once
#include <stddef.h>
#include <stdint.h>

struct FrameBuffer {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanline;
	uint32_t bpp, pitch;
};