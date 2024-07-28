#include "kataInit.h"
#include <acpi/rsdp.h>

KAtaRenderer r = KAtaRenderer(NULL, NULL);
KAtaRenderer ar = KAtaRenderer(NULL, NULL);
KAtaInfo InitializeKAta(BootInfo* BootInfo) {
	memset(BootInfo->framebuffer->BaseAddress, 0, BootInfo->framebuffer->BufferSize);
	ACPI::RSDP* rsdp = (ACPI::RSDP*)BootInfo->rsdp;

	r = KAtaRenderer(BootInfo->framebuffer, BootInfo->psf1_Font);
	ar = KAtaRenderer(BootInfo->framebuffer, BootInfo->psf1_Font);
	GKRenderer = &r;
	
	log->ok("KAtaRenderer is ready.");
	log->print("Preparing Atlas Advanced Graphical User Interface Renderer...");

	aaguir = &ar;
	log->ok("Prepared Atlas Advanced Graphical User Interface Renderer.");

	/* Panic("MA QUANTO BELLO ANDARE IN GIRO PER I COLLI BOLOGNESI");
	while (true); */
	log->info("I must have died and gone to Heaven.");
	log->print("Preparing and loading GDTDescriptor...");
	GDTDescriptor gdtDescriptor;
	basesystem bs;
	gdtDescriptor.Size = sizeof(GDT) - 1;
	gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
	LoadGDT(&gdtDescriptor);
	log->ok("Loaded GDTDescriptor.");
	
	PrepareMemory(BootInfo);

	bs.enableMCE();
	bs.enableFPU();

	GKRenderer->printf("           ");
	GKRenderer->printf("Free memory: ");
	GKRenderer->printf(to_string(GlobalAllocator.GetFreeRAM()/1073741824)); // GB
	GKRenderer->printf(" GB");
	GKRenderer->Next();

	GKRenderer->printf("           ");
	GKRenderer->printf("Used memory: ");
	GKRenderer->printf(to_string(GlobalAllocator.GetUsedRAM()/1048576)); // MB
	GKRenderer->printf(" MB");
	GKRenderer->Next();

	GKRenderer->printf("           ");
	GKRenderer->printf("Reserved memory: ");
	GKRenderer->printf(to_string(GlobalAllocator.GetReservedRAM()/1048576)); // MB
	GKRenderer->printf(" MB");
	GKRenderer->Next();


	InitializeHeap((void*)0x0000100000000000, 0x10);

	PrepareInterrupts();

	InitPS2Mouse();

	PrepareACPI(BootInfo);

	log->print("Finishing with interrupts...");
	outb(PIC1_DATA, 0b11111000);
	outb(PIC2_DATA, 0b11101111);

	asm ("sti");
	InitialiseSyscalls();

	bs.enableNMI();

	if(!rsdp->is_valid())
    {
        Panic("RSDP_NOT_VALID", nullptr);
        while(true) asm("hlt");
    }

	bs.playBootSound("beep");
	
	log->ok("KAta ready.");
	


	return kataInfo;
}