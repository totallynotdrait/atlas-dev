#include "log.h"
// "[FAILED]   A"
// "[  OK  ]   D"
// "           F"
// "[ INFO ]"
// "[ WARN ]"
// "           d"

Log* log;

Log::Log() {}

void Log::print(const char *str) {
    GKRenderer->printf("           ");
    GKRenderer->printf(str);
    GKRenderer->Next();
}

void Log::info(const char *str) {
    GKRenderer->printf("[ "); GKRenderer->Color = 0x00b2ff;
    GKRenderer->printf("INFO"); GKRenderer->Color = 0xffffff;
    GKRenderer->printf(" ]");
    GKRenderer->printf("   ");
    GKRenderer->printf(str);
    GKRenderer->Next();
}

void Log::ok(const char *str) {
    GKRenderer->printf("[  "); GKRenderer->Color = 0x00ff3a;
    GKRenderer->printf("OK"); GKRenderer->Color = 0xffffff;
    GKRenderer->printf("  ]");
    GKRenderer->printf("   ");
    GKRenderer->printf(str);
    GKRenderer->Next();
}

void Log::warn(const char *str) {
    GKRenderer->printf("[ "); GKRenderer->Color = 0xff8700;
    GKRenderer->printf("WARN"); GKRenderer->Color = 0xffffff;
    GKRenderer->printf(" ]");
    GKRenderer->printf("   ");
    GKRenderer->printf(str);
    GKRenderer->Next();
}

void Log::failed(const char *str) {
    GKRenderer->printf("["); GKRenderer->Color = 0xff3333;
    GKRenderer->printf("FAILED"); GKRenderer->Color = 0xffffff;
    GKRenderer->printf("]");
    GKRenderer->printf("   ");
    GKRenderer->printf(str);
    GKRenderer->Next();
}