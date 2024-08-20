#include "panic.h"
#include <liba/stdio.h>
#include <scheduling/PIT/PIT.h>
#include <aagui/aagui.h>
#include <xxd.h>
#include <liba/string.h>

extern int panic_screen = 1;

void Panic(const char* panicMessage, interrupt_frame* regs) {
    if (panic_screen == 0) {
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
    } else if (panic_screen == 1) {
        uint64_t oldCC = GKRenderer->ClearColor;
        uint64_t oldC = GKRenderer->Color;
        uint64_t oldPosX = GKRenderer->CursorPosition.X;
        uint64_t oldPosY = GKRenderer->CursorPosition.Y;

        GKRenderer->ClearColor = hbc.Red;
        GKRenderer->Color = hbc.White;

        GKRenderer->CursorPosition.X = 0;
        GKRenderer->CursorPosition.Y = 0;
        printf("KERNEL PANIC [%s]\n\nStack:\n", panicMessage);
        printf("Interrupt Number (%d)\n", regs->interrupt_number);
        printf(to_hexstring(regs->interrupt_number));
        printf("\nError Code (%d)\n", regs->error_code);
        printf(to_hexstring(regs->error_code));
        printf("\nRFLAGS (%d)\n", regs->rflags);
        printf(to_hexstring(regs->rflags));
        printf("\n\nEnd Of Line for Stack:\n");
        printf("See Atlas Documentation for guidance: https://github.com/totallynotdrait/atlas-dev");

        GKRenderer->ClearColor = oldCC;
        GKRenderer->Color = oldC;

        GKRenderer->CursorPosition.X = oldPosX;
        GKRenderer->CursorPosition.Y = oldPosY;
    }
}