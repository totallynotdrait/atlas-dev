#pragma once
#include <kata_renderer.h>
#include <userinput/mouse.h>
#include <log.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0XA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20 // End Of Interrupts

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct interrupt_frame{
    uint8_t fpu_state[256];

    uint64_t cr4;
    uint64_t cr3;
    uint64_t cr2;
    uint64_t cr0;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t rbx;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rax;
    uint64_t interrupt_number;
    uint64_t error_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

__attribute__((interrupt)) void syscall_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void DoubleFault_Handler( interrupt_frame* frame);
__attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void PITInt_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void DEE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void D_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void BPE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void OFE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void BREE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void IOCE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void DNAE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void ITSSE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void SNPE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void SSFE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void FPE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void ACE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void MCE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void SIMDFPE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void VEE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void CPE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void HIE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void VMMCE_Handler(interrupt_frame* frame);
__attribute__((interrupt)) void SE_Handler(interrupt_frame* frame);

void RemapPIC();
void PIC_EndMaster();
void PIC_EndSlave();