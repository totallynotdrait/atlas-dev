#include "log.h"

Log* log;

Log::Log() {}

void Log::print(const char *str) {
    GKRenderer->Color = hbc.Gray;
    GKRenderer->printf("           ");
    GKRenderer->printf(str);
    GKRenderer->Next();
}

void Log::info(const char *str) {
    GKRenderer->Color = hbc.White;
    GKRenderer->printf("[ "); GKRenderer->Color = hbc.Cyan;
    GKRenderer->printf("INFO"); GKRenderer->Color = hbc.White;
    GKRenderer->printf(" ]"); GKRenderer->Color = hbc.Gray;
    GKRenderer->printf("   ");
    GKRenderer->printf(str);
    GKRenderer->Next();
}

void Log::ok(const char *str) {
    GKRenderer->Color = hbc.White; // for [
    GKRenderer->printf("[  "); GKRenderer->Color = hbc.Lime; // before type
    GKRenderer->printf("OK"); GKRenderer->Color = hbc.White; // after type ]
    GKRenderer->printf("  ]"); GKRenderer->Color = hbc.Gray; // standalone
    GKRenderer->printf("   ");
    GKRenderer->printf(str);
    GKRenderer->Next();
}

void Log::warn(const char *str) {
    GKRenderer->Color = hbc.White;
    GKRenderer->printf("[ "); GKRenderer->Color = hbc.Red; // 0xFF9900
    GKRenderer->printf("WARN"); GKRenderer->Color = hbc.White;
    GKRenderer->printf(" ]"); GKRenderer->Color = hbc.Gray;
    GKRenderer->printf("   ");
    GKRenderer->printf(str);
    GKRenderer->Next();
}

void Log::failed(const char *str) {
    GKRenderer->Color = hbc.White;
    GKRenderer->printf("["); GKRenderer->Color = hbc.Red;
    GKRenderer->printf("FAILED"); GKRenderer->Color = hbc.White;
    GKRenderer->printf("]"); GKRenderer->Color = hbc.Gray;
    GKRenderer->printf("   ");
    GKRenderer->printf(str);
    GKRenderer->Next();
}