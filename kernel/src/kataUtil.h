#pragma once

#include <stddef.h>
#include <stdint.h>
#include "kata_renderer/kata_renderer.h"
#include "base_utils/intnum/cstr.h"
#include "base_utils/efim/efiMemory.h"
#include "base_utils/mem/memory.h"
#include "base_utils/bitmap/bitmap.h"
#include "base_utils/pageframe/PageFrameAllocator.h"
#include "base_utils/pageframe/PageMapIndexer.h"
#include "base_utils/pageframe/paging.h"
#include "base_utils/pageframe/PageTableManager.h"
#include "base_utils/userinput/mouse.h"
#include "base_utils/basesystem.h"
#include "base_utils/acpi/acpi.h"
#include "base_utils/hardw/pci.h"
#include "base_utils/basesystem.h"
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