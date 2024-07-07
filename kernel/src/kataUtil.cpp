#include "kataUtil.h"

KAtaInfo kataInfo;

void PrepareMemory(BootInfo* bootInfo) {
	log->print("Preparing memory...");
	log->print("Setting mMapEntries...");
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

	log->print("Reading EFI Memory Map...");
    GlobalAllocator = PageFrameAllocator();
    GlobalAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

	log->print("Init KAta Size and Pages...");
    uint64_t KAtaSize = (uint64_t)&_KAtaEnd - (uint64_t)&_KAtaStart;
    uint64_t KAtaPages = (uint64_t)KAtaSize / 4096 + 1;

	log->print("Locking pages...");
    GlobalAllocator.LockPages(&_KAtaStart, KAtaPages);

	log->print("Setting PageTable PML4...");
    PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
    memset(PML4, 0, 0x1000);

	log->print("Setting PageTableManager...");
    GPageTableManager = PageTableManager(PML4);

	log->print("Mapping memory...");
    for (uint64_t t = 0; t < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize); t += 0x1000) {
		GPageTableManager.MapMemory((void*)t, (void*)t);
    }

	log->print("Locking pages and mapping memory...");
    uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;
    GlobalAllocator.LockPages((void*)fbBase, fbSize/ 0x1000 + 1);
    for (uint64_t t = 0; t < fbBase + fbSize; t += 4096) {
        GPageTableManager.MapMemory((void*)t, (void*)t);
    }

	log->print("Finishing up...");
    asm ("mov %0, %%cr3" : : "r" (PML4));

	log->print("Setting kataInfo pageTableManager to local one...");
	kataInfo.pageTableManager = &GPageTableManager;
	log->ok("Prepared memory.");
}

IDTR idtr;
uint8_t countInt = 0;
void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector) {
	countInt += 1;
	/* GKRenderer->printf("           "); GKRenderer->printf(to_hexstring(countInt)); GKRenderer->printf(" / "); GKRenderer->printf(to_hexstring((uint64_t)handler)); GKRenderer->printf(" / "); GKRenderer->printf(to_hexstring(entryOffset)); GKRenderer->printf(" / "); GKRenderer->printf(to_hexstring(type_attr)); GKRenderer->printf(" / "); GKRenderer->printf(to_hexstring(selector));
	GKRenderer->Next(); */
	IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
	interrupt->SetOffset((uint64_t)handler);
	interrupt->type_attr = type_attr;
	interrupt->selector = selector;
}

void PrepareInterrupts() {
	log->print("Preparing interrupts...");
	idtr.Limit = 0x0FFF;
	idtr.Offset = (uint64_t)GlobalAllocator.RequestPage();

	SetIDTGate((void*)syscall_Handler, 0x80, IDT_TA_InterruptGate, 0x08); // wanted to do 0x69 as interrupt vector
	SetIDTGate((void*)PageFault_Handler, 0xE, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)DoubleFault_Handler, 0x8, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)GPFault_Handler, 0xD, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)MouseInt_Handler, 0x2C, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)PITInt_Handler, 0x20, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)DEE_Handler, 0x0, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)D_Handler, 0x1, IDT_TA_TrapGate | IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)BPE_Handler, 0x3, IDT_TA_TrapGate, 0x08);
	SetIDTGate((void*)OFE_Handler, 0x4, IDT_TA_TrapGate, 0x08);
	SetIDTGate((void*)BREE_Handler, 0x5, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)IOCE_Handler, 0x6, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)DNAE_Handler, 0x7, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)ITSSE_Handler, 0xA, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)SNPE_Handler, 0xB, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)SSFE_Handler, 0xC, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)FPE_Handler, 0x10, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)ACE_Handler, 0x11, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)MCE_Handler, 0x12, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)SIMDFPE_Handler, 0x13, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)VEE_Handler, 0x14, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)CPE_Handler, 0x15, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)HIE_Handler, 0x1C, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)VMMCE_Handler, 0x1D, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)SE_Handler, 0x1E, IDT_TA_InterruptGate, 0x08);


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

