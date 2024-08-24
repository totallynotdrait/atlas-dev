#pragma once

#include <stdint.h>
#include <stddef.h>
#include <mem/memory.h>

int strcmp(const char* str1, const char* str2);
char* strcpy(char* dst, const char* src);
size_t strlen(char* s);
int strncmp(char* s1, char* s2, int bytes);