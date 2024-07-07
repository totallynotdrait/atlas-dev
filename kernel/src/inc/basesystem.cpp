#include "basesystem.h"
#include <IO/IO.h>
#include <scheduling/PIT/PIT.h>
#include <liba/string.h>

#define KBRD_INTRFC 0x64

/* keyboard interface bits */
#define KBRD_BIT_KDATA 0 /* keyboard data is in buffer (output buffer is empty) (bit 0) */
#define KBRD_BIT_UDATA 1 /* user data is in buffer (command buffer is empty) (bit 1) */

#define KBRD_IO 0x60 /* keyboard IO port */
#define KBRD_RESET 0xFE /* reset CPU command */

#define bit(n) (1<<(n)) /* Set bit n to 1 */

/* Check if bit n in flags is set */
#define check_flag(flags, n) ((flags) & bit(n))

basesystem::basesystem() {}

void basesystem::broadcast(const char *message, const char *sender) {
    GKRenderer->printf("[broadcast from ");
    GKRenderer->printf(sender);
    GKRenderer->printf("] :: ");
    GKRenderer->printf(message);
    return;
}

void basesystem::reboot() {
    log->print("Rebooting system...");
    PIT::Sleepd(3);
    //log->warn("Reboot and other ACPI functions are not yet implemented in this version.");
    uint8_t temp;

    log->print("Disabling all interrupts...");
    asm volatile ("cli"); 

    log->print("Clearing all keyboard buffers...");
    do
    {
        temp = inb(KBRD_INTRFC); 
        if (check_flag(temp, KBRD_BIT_KDATA) != 0)
            inb(KBRD_IO); 
    } while (check_flag(temp, KBRD_BIT_UDATA) != 0);

    outb(KBRD_INTRFC, KBRD_RESET);
    log->print("Loop check...");
loop:
    asm volatile ("hlt");
    goto loop; 
    log->failed("Failed to reboot system.");
}

void basesystem::shutdown() {
    log->print("Shutting down system...");
    //log->warn("Shutdown and other ACPI functions are not yet implemented in this version.");
    log->warn("Will only work on VMs.");
    PIT::Sleepd(3);
    log->print("QEMU and Bochs...");
    outw(0xB004, 0x2000);
    log->print("QEMU...");
    outw(0x604, 0x2000);
    log->print("VirtualBox...");
    outw(0x604, 0x2000);
    log->print("Cloud Hypervisor...");
    outw(0x604, 0x2000);
    log->failed("Failed to shutdown system.");
}

void basesystem::execute_elf(const char* elf_path) {
    GKRenderer->printf("running ");
    GKRenderer->printf(elf_path);
    GKRenderer->Next();
}

void basesystem::enableMCE() {
    log->print("Enabling Machine Check Exception (MCE)");
    unsigned long cr4;
    asm volatile (
        "mov %%cr4, %0\n\t"
        "or $0x40, %0\n\t"
        "mov %0, %%cr4"
        : "=r" (cr4)
        : "0" (cr4)
        : "memory"
    );
    log->ok("Enabled Machine Check Exception (MCE)");

}

void basesystem::set_fpu_cw(const uint16_t cw) 
{
	asm volatile("fldcw %0" :: "m"(cw));
}

void basesystem::enableFPU() {
    log->print("Enabling Floating Point Unit (FPU)");
    size_t cr4;
	asm volatile ("mov %%cr4, %0" : "=r"(cr4));
	cr4 |= 0x200;
	asm volatile ("mov %0, %%cr4" :: "r"(cr4));
	basesystem::set_fpu_cw(0x37F);
    log->ok("Enabled Floating Point Unit (FPU)");
}

void basesystem::playBootSound(const char* boot) {
    if (strcmp(boot, "cieli_neri") == 0) {
        enable_pc_speaker();

        set_bpm(158);
        play_note(NoteFrequency::Ab4, 1130); // 68
        play_note(NoteFrequency::B4, 1130); // 71
        play_note(NoteFrequency::E5, 750); // 76
        play_note(NoteFrequency::E4, 2270);
        play_note(NoteFrequency::A4, 750);
        play_note(NoteFrequency::G4, 190);
        play_note(NoteFrequency::Gb4, 190);
        play_note(NoteFrequency::E4, 1890);
        play_note(NoteFrequency::D4, 750);
        play_note(NoteFrequency::B3, 3030);

        disable_pc_speaker();
    } else if (strcmp(boot, "beep") == 0) {
        enable_pc_speaker();
        play_note(NoteFrequency::A4, 1050);
        disable_pc_speaker();
    } else {
        log->failed("No boot sound, unknown boot sound.");
    }
}