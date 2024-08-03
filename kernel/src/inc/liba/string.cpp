#include "string.h"

extern "C" size_t __strlen(char* s);
extern "C" int __strcmp(char* s1, char* s2);
extern "C" int __strncmp(char* s1, char* s2, size_t bytes);
extern "C" char* __strcpy(char* dst, const char* src);

int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}


size_t strlen(char* s) 
{
    return __strlen(s);
}

char* strcpy(char* dst, const char* src)
{
    return __strcpy(dst, src);
}