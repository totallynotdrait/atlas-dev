#pragma once
#include "../kata_renderer/kata_renderer.h"

class log {
    public:
    log();
    void print(const char *str);
    void info(const char *str);
    void ok(const char *str);
    void warn(const char *str);
    void failed(const char *str);
};

extern log* glog;