#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kata_renderer.h>
#include <intnum/cstr.h>
#include <efim/efiMemory.h>
#include <mem/memory.h>
#include <bitmap/bitmap.h>
#include <pageframe/PageFrameAllocator.h>
#include <pageframe/PageMapIndexer.h>
#include <pageframe/paging.h>
#include <pageframe/PageTableManager.h>
#include <userinput/mouse.h>
#include <basesystem.h>
#include <acpi/acpi.h>
#include <hardw/pci.h>
#include <basesystem.h>
#include <efi.h>
#include <log.h>

struct BootInfo {
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	KEFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
	ACPI::RSDP2* rsdp;
	EFI_SYSTEM_TABLE *SystemTable;
};

extern uint64_t _KAtaStart;
extern uint64_t _KAtaEnd;


struct KAtaInfo {
    PageTableManager* pageTableManager;
};

KAtaInfo InitializeKAta(BootInfo* BootInfo);