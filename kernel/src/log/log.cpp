#include "log.h"
#include "liba/stdio.h"

Log* log;

Log::Log() {}

void Log::print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    GKRenderer->Color = hbc.Gray;
    GKRenderer->printf("           ");
    printf_vargs(format, args);
    GKRenderer->Next();
    va_end(args);
}

void Log::info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    GKRenderer->Color = hbc.White;
    GKRenderer->printf("[ "); GKRenderer->Color = hbc.Cyan;
    GKRenderer->printf("INFO"); GKRenderer->Color = hbc.White;
    GKRenderer->printf(" ]"); GKRenderer->Color = hbc.Gray;
    GKRenderer->printf("   ");
    printf_vargs(format, args);
    GKRenderer->Next();
    va_end(args);
}

void Log::ok(const char* format, ...) {
    va_list args;
    va_start(args, format);
    GKRenderer->Color = hbc.White; // for [
    GKRenderer->printf("[  "); GKRenderer->Color = hbc.Lime; // before type
    GKRenderer->printf("OK"); GKRenderer->Color = hbc.White; // after type ]
    GKRenderer->printf("  ]"); GKRenderer->Color = hbc.Gray; // standalone
    GKRenderer->printf("   ");
    printf_vargs(format, args);
    GKRenderer->Next();
    va_end(args);
}

void Log::warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    GKRenderer->Color = hbc.White;
    GKRenderer->printf("[ "); GKRenderer->Color = hbc.Red; // 0xFF9900
    GKRenderer->printf("WARN"); GKRenderer->Color = hbc.White;
    GKRenderer->printf(" ]"); GKRenderer->Color = hbc.Gray;
    GKRenderer->printf("   ");
    printf_vargs(format, args);
    GKRenderer->Next();
    va_end(args);
}

void Log::failed(const char* format, ...) {
    va_list args;
    va_start(args, format);
    GKRenderer->Color = hbc.White;
    GKRenderer->printf("["); GKRenderer->Color = hbc.Red;
    GKRenderer->printf("FAILED"); GKRenderer->Color = hbc.White;
    GKRenderer->printf("]"); GKRenderer->Color = hbc.Gray;
    GKRenderer->printf("   ");
    printf_vargs(format, args);
    GKRenderer->Next();
    va_end(args);
}
