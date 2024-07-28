#include "panic.h"
#include <liba/stdio.h>
#include <scheduling/PIT/PIT.h>
#include <aagui/aagui.h>

void Panic(const char* panicMessage, interrupt_frame* regs) {
    GKRenderer->ClearColor = hbc.Blue;
    GKRenderer->CursorPosition = {0,0};
    GKRenderer->Color = hbc.White;
    printf("\\c");
    printf("HALT! KERNEL PANIC\n\n");
    printf("Ooops! You're seeing this screen because a bad instruction or a irrevocable error happend.\nIf it's first time, please reboot your system. If you've made recent changes to the Kernel or bootloader, please undo them or re-install Atlas.\n\nOr it may be always be a Caparezzino inside KAta ;)\n\n\n\n");
    printf("TECHNICAL INFORMATIONS:\n");
    printf("STOP CODE (handler message): %s\n", panicMessage);
    printf("ERROR CODE: %d\n", regs->error_code);
    printf("FPU STATE: %d\n", regs->fpu_state);
    printf("INT NUMBER: %d\n", regs->interrupt_number);
    printf("RFLAGS: %d\n", regs->rflags);
    printf("SYSTEM UPTIME: %d\n", PIT::TimeSinceBoot);
    printf("\n\n\n\nAtlas is halted.\nIt's not safe to reboot the system.");
    
}