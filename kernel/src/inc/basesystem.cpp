#include "basesystem.h"

basesystem::basesystem() {}

void basesystem::broadcast(const char *message, const char *sender) {
    GKRenderer->printf("[broadcast from ");
    GKRenderer->printf(sender);
    GKRenderer->printf("] :: ");
    GKRenderer->printf(message);
    return;
}

void basesystem::reboot(BootInfo* bootInfo) {
    log->print("Rebooting system...");
    log->warn("Reboot and other ACPI functions are not yet implemented in this version.");
    log->failed("Failed to reboot system.");
}

void basesystem::shutdown(BootInfo* bootInfo) {
    log->print("Shuttingdown system...");
    log->warn("Shutdown and other ACPI functions are not yet implemented in this version.");
    log->failed("Failed to shutdown system.");
}