#pragma once
#include <kata_renderer.h>

class Log {
    public:
    Log();
    void print(const char *str);
    void info(const char *str);
    void ok(const char *str);
    void warn(const char *str);
    void failed(const char *str);
};

extern Log* log;