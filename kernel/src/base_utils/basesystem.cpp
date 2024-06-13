#include "basesystem.h"

basesystem::basesystem() {}

void basesystem::broadcast(const char *message, const char *sender) {
    GKRenderer->printf("[broadcast from ");
    GKRenderer->printf(sender);
    GKRenderer->printf("] :: ");
    GKRenderer->printf(message);
    return;
}