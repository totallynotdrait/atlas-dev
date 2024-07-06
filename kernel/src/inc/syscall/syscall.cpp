#include "syscall.h"
void syscall_globalPrint(const char* str)
{
    log->info("system call service was called (globalprint) (no way)");
    int syscall = SYSCALL_GLOBAL_PRINT;
    asm("int $0x80" : : "a"(syscall), "b"(str));
}
