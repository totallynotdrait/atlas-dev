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
    glog->print("Rebooting system...");
    glog->warn("Reboot and other ACPI functions are not yet implemented in this version.");
    glog->failed("Failed to reboot system.");
}

void basesystem::shutdown(BootInfo* bootInfo) {
    glog->print("Shuttingdown system...");
    glog->warn("Shutdown and other ACPI functions are not yet implemented in this version.");
    glog->failed("Failed to shutdown system.");
}