#include "syscall.h"
#include "interrupts/IDT.h"

#define DEFN_SYSCALL0(fn, num) uint64_t syscall_##fn() { return MakeSyscall(num, 0, 0, 0, 0, 0); }
#define DEFN_SYSCALL1(fn, num, P1) uint64_t syscall_##fn(P1 p1) { return MakeSyscall(num, (uint64_t)p1, 0, 0, 0, 0); }
#define DEFN_SYSCALL2(fn, num, P1, P2) uint64_t syscall_##fn(P1 p1, P2 p2) { return MakeSyscall(num, (uint64_t)p1, (uint64_t)p2, 0, 0, 0); }
#define DEFN_SYSCALL3(fn, num, P1, P2, P3) uint64_t syscall_##fn(P1 p1, P2 p2, P3 p3) { return MakeSyscall(num, (uint64_t)p1, (uint64_t)p2, (uint64_t)p3, 0, 0); }
#define DEFN_SYSCALL4(fn, num, P1, P2, P3, P4) uint64_t syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4) { return MakeSyscall(num, (uint64_t)p1, (uint64_t)p2, (uint64_t)p3, (uint64_t)p4, 0); }
#define DEFN_SYSCALL5(fn, num, P1, P2, P3, P4, P5) uint64_t syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) { return MakeSyscall(num, (uint64_t)p1, (uint64_t)p2, (uint64_t)p3, (uint64_t)p4, (uint64_t)p5); }

DEFN_SYSCALL2(printf, 0, const char*, va_list)

const uint64_t num_syscalls = 1;

static void* syscalls[num_syscalls] = {
    (void*)&printf_vargs,
};


static void syscall_handler(Registers* registers)
{
    log->info(to_string(registers->rax));
    if(registers->rax >= num_syscalls) //For now lets call this kprintf
    {
        return;
    }

    void* function = syscalls[registers->rax];

    uint64_t ret = ExecuteSyscall(registers->rdi, registers->rsi, registers->rdx, registers->rcx, registers->rbx, (uint64_t)function);    
    registers->rax = ret;
}

void InitialiseSyscalls()
{
    log->print("Initializing System Calls to 0x80...");
    RegisterInterruptHandler(0x80, syscall_handler);
    SetupSysret();
    log->ok("Initiliazed System Calls.");
}