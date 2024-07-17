#pragma once

#include <kata_renderer/kata_renderer.h>
#include <stdarg.h>
#include <intnum/cstr.h>

void printf_vargs(const char* format, va_list args);
void printf(const char* format, ...);
void scanf(const char* format, ...);