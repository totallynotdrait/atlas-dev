#pragma once
#include <stdint.h>

#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111

#define IDT_FLAG_PRESENT        0b10000000
#define IDT_FLAG_RING0          0b00000000
#define IDT_FLAG_RING3          0b01100000


struct xmm_reg
{
    uint64_t low;
    uint64_t high;
};

struct Registers
{
    uint64_t ds;
    xmm_reg xmm[16];
    uint64_t rdi, rsi, rbp, useless, rbx, rdx, rcx, rax;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t int_no, err_code;
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed));

typedef void (*isr_t)(Registers*);

struct IDTDescEntry {
    uint16_t offset0;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset1;
    uint32_t offset2;
    uint32_t ignore;
    void SetOffset(uint64_t offset);
    uint64_t GetOffset();
};

struct IDTR {
    uint16_t Limit;
    uint64_t Offset;
} __attribute__((packed));

void RegisterInterruptHandler(uint8_t index, isr_t handler);