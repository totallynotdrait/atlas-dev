#pragma once
#include <kata_renderer.h>

class Log {
    public:
    Log();
    void print(const char* format, ...);
    void info(const char* format, ...);
    void ok(const char* format, ...);
    void warn(const char* format, ...);
    void failed(const char* format, ...);
};

extern Log* log;