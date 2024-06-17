#include "kataUtil.h"
#include <gdt/gdt.h>
#include <interrupts/IDT.h>
#include <interrupts/interrupts.h>
#include <IO/IO.h>
#include <panic/panic.h>



KAtaInfo kataInfo;

void PrepareMemory(BootInfo* bootInfo) {
	log->print("Preparing memory.");
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

    GlobalAllocator = PageFrameAllocator();
    GlobalAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    uint64_t KAtaSize = (uint64_t)&_KAtaEnd - (uint64_t)&_KAtaStart;
    uint64_t KAtaPages = (uint64_t)KAtaSize / 4096 + 1;

    GlobalAllocator.LockPages(&_KAtaStart, KAtaPages);

    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

    GPageTableManager = PageTableManager(PML4);

    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t += 0x1000) {
        GPageTableManager.MapMemory((void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
    GlobalAllocator.LockPages((void*)fbBase, fbSize/ 0x1000 + 1);
    for (uint64_t t = 0; t < fbBase + fbSize; t += 4096) {
        GPageTableManager.MapMemory((void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));

	kataInfo.pageTableManager = &GPageTableManager;
	log->ok("Prepared memory.");
}

IDTR idtr;
void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector) {
	GKRenderer->printf("           "); GKRenderer->printf(to_hexstring((uint64_t)handler)); GKRenderer->printf(" / "); GKRenderer->printf(to_hexstring(entryOffset)); GKRenderer->printf(" / "); GKRenderer->printf(to_hexstring(type_attr)); GKRenderer->printf(" / "); GKRenderer->printf(to_hexstring(selector));
	GKRenderer->Next();
	IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
	interrupt->SetOffset((uint64_t)handler);
	interrupt->type_attr = type_attr;
	interrupt->selector = selector;
}

void PrepareInterrupts() {
	log->print("Preparing interrupts...");
	idtr.Limit = 0x0FFF;
	idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();

	SetIDTGate((void*)PageFault_Handler, 0xE, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)DoubleFault_Handler, 0x8, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)GPFault_Handler, 0xD, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)MouseInt_Handler, 0x2C, IDT_TA_InterruptGate, 0x08);


	asm ("lidt %0" : : "m" (idtr));

	RemapPIC();
	log->ok("Interrupts are ready.");
}

void PrepareACPI(BootInfo* BootInfo) {
	log->print("Preparing ACPI...");
	ACPI::SDTHeader* xsdt = (ACPI::SDTHeader*)(BootInfo->rsdp->XSDTAddress);
	ACPI::MCFGHeader* mcfg = (ACPI::MCFGHeader*)ACPI::FindTable(xsdt, (char*)"MCFG");

	PCI::EnumeratePCI(mcfg);
	log->ok("Prepared ACPI.");
}

KAtaRenderer r = KAtaRenderer(NULL, NULL);
KAtaInfo InitializeKAta(BootInfo* BootInfo) {
	memset(BootInfo->framebuffer->BaseAddress, 0, BootInfo->framebuffer->BufferSize);
	
	r = KAtaRenderer(BootInfo->framebuffer, BootInfo->psf1_Font);
	GKRenderer = &r;
	log->ok("KAtaRenderer is ready.");

	/* Panic("MA QUANTO BELLO ANDARE IN GIRO PER I COLLI BOLOGNESI");
	while (true); */
	
	log->print("Preparing and loading GDTDescriptor...");
	GDTDescriptor gdtDescriptor;
	gdtDescriptor.Size = sizeof(GDT) - 1;
	gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
	LoadGDT(&gdtDescriptor);
	log->ok("Loaded GDTDescriptor.");
	
	PrepareMemory(BootInfo);

	PrepareInterrupts();
	
	InitPS2Mouse();

	PrepareACPI(BootInfo);

	log->print("Finishing with interrupts...");
	outb(PIC1_DATA, 0b11111001);
	outb(PIC2_DATA, 0b11101111);

	asm ("sti");
	
	log->ok("KAta ready.");
	return kataInfo;
}