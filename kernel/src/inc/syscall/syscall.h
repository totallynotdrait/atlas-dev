#pragma once

#include <stdint.h>
#include <stdarg.h>
#include "syscall_defs.h"
#include <liba/stdio.h>

extern "C" uint64_t MakeSyscall(uint64_t syscall, uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4);
extern "C" uint64_t ExecuteSyscall(uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t addr);
extern "C" void SetupSysret();

void InitialiseSyscalls();

#define DECL_SYSCALL0(fn) uint64_t syscall_##fn();
#define DECL_SYSCALL1(fn,p1) uint64_t syscall_##fn(p1);
#define DECL_SYSCALL2(fn,p1,p2) uint64_t syscall_##fn(p1,p2);
#define DECL_SYSCALL3(fn,p1,p2,p3) uint64_t syscall_##fn(p1,p2,p3);
#define DECL_SYSCALL4(fn,p1,p2,p3,p4) uint64_t syscall_##fn(p1,p2,p3,p4);
#define DECL_SYSCALL5(fn,p1,p2,p3,p4,p5) uint64_t syscall_##fn(p1,p2,p3,p4,p5);

DECL_SYSCALL2(printf, const char*, va_list);