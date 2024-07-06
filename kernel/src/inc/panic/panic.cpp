#include "panic.h"
#include <kata_renderer.h>
#include <scheduling/PIT/PIT.h>

void Panic(const char* panicMessage) {
    GKRenderer->ClearColor = 0x000000FF;
    GKRenderer->CursorPosition = {0,0};
    GKRenderer->Color = 0xFFFFFFFF;
    GKRenderer->Clear();
    GKRenderer->printf("HALT KERNEL PANIC");
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->printf("A problem occurred and Atlas has been halted to prevent damage to your system.");
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->printf("An instruction probably caused this crash or there's been a problem with KAta.");
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->printf("If it's the first time, please reboot and undo any changes that you've done. If it happened multiple times, please report this kernel panic screen to the Issues page of the Atlas repository.");
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->printf("Repositories:");
    GKRenderer->Next();
    GKRenderer->printf("https://github.com/totallynotdrait/Atlas (if you're using Atlas)");
    GKRenderer->Next();
    GKRenderer->printf("https://github.com/totallynotdrait/atlas-dev (if you're using atlas-dev)");
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->printf("Or it may be a Caparezzino inside KAta :3");
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->printf("TECHNICAL INFORMATION:");
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->printf("Message from handler: ");
    GKRenderer->printf(panicMessage);
    GKRenderer->Next();
    GKRenderer->printf("Clock time since startup (PIT, not accurated): ");
    GKRenderer->printf(to_string(PIT::TimeSinceBoot));
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->printf("Atlas and KAta in general is halted.");
    GKRenderer->Next();
    GKRenderer->printf("It's now safe to shutdown your system.");
}