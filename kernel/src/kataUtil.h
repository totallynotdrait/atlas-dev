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
#include <log.h>
#include <gdt/gdt.h>
#include <interrupts/IDT.h>
#include <interrupts/interrupts.h>
#include <IO/IO.h>
#include <panic/panic.h>
#include <aagui/rendering/BasicColors.h>
#include <mem/heap.h>
#include <aagui/aagui.h>
#include <sound/pc_speaker.h>

struct BootInfo {
	FrameBuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* mMap;
	uint64_t mMapSize;
	uint64_t mMapDescSize;
	ACPI::RSDP2* rsdp;
};

extern uint64_t _KAtaStart;
extern uint64_t _KAtaEnd;


struct KAtaInfo {
    PageTableManager* pageTableManager;
};

extern KAtaInfo kataInfo;
void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector);
void PrepareInterrupts();
void PrepareACPI(BootInfo* BootInfo);
void PrepareMemory(BootInfo* bootInfo);