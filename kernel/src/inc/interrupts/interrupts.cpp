#include "interrupts.h"
#include <panic/panic.h>
#include <IO/IO.h>
#include <userinput/keyboard.h>
#include <scheduling/PIT/PIT.h>
#include <liba/stdio.h>

__attribute__((interrupt)) void syscall_Handler(interrupt_frame* frame) {
    int syscall = frame->rax;
    frame->rax = 0;
    
    /* GKRenderer->Clear();
    log->info(to_hexstring(frame->cr0));
    log->info(to_hexstring(frame->cr2));
    log->info(to_hexstring(frame->cr3));
    log->info(to_hexstring(frame->cr4));
    log->info(to_hexstring(frame->r15));
    log->info(to_hexstring(frame->r14));
    log->info(to_hexstring(frame->r13));
    log->info(to_hexstring(frame->r12));
    log->info(to_hexstring(frame->r11));
    log->info(to_hexstring(frame->r10));
    log->info(to_hexstring(frame->r9));
    log->info(to_hexstring(frame->r8));
    log->info(to_hexstring(frame->rdi));
    log->info(to_hexstring(frame->rsi));
    log->info(to_hexstring(frame->rbp));
    log->info(to_hexstring(frame->rbx));
    log->info(to_hexstring(frame->rdx));
    log->info(to_hexstring(frame->rcx));
    log->info(to_hexstring(frame->rax)); // 0 = 3659234830974979
    log->info(to_hexstring(frame->interrupt_number));
    log->info(to_hexstring(frame->error_code));
    log->info(to_hexstring(frame->rip));
    log->info(to_hexstring(frame->cs));
    log->info(to_hexstring(frame->rflags));
    log->info(to_hexstring(frame->rsp));
    log->info(to_hexstring(frame->ss)); */

    uint64_t rbx;
    asm ("movq %%rbx, %0" : "=r" (rbx));

    char* str = (char*)rbx;
    GKRenderer->printf(to_string((uint64_t)str));
    
}

// fault handlers
__attribute__((interrupt)) void PageFault_Handler(interrupt_frame* frame) {
    Panic("PAGE_FAULT");
    while (true);
}
__attribute__((interrupt)) void DoubleFault_Handler(interrupt_frame* frame) {
    Panic("DOUBLE_FAULT");
    while (true);
}
__attribute__((interrupt)) void GPFault_Handler(interrupt_frame* frame) {
    Panic("GENERAL_PROTECTION_FAULT");
    while (true);
}
__attribute__((interrupt)) void DEE_Handler(interrupt_frame* frame) {
    log->failed("[INTERNAL_SYSTEM] [DIVISION_ERROR] Please stop trying to crash atlas or create a black hole inside the computer."); // sincerely...
}
__attribute__((interrupt)) void D_Handler(interrupt_frame* frame) {
    Panic("hello debug");
    while (true);
}
__attribute__((interrupt)) void BPE_Handler(interrupt_frame* frame) {
    Panic("BREAKPOINT (hello debug)");
    while (true);
}
__attribute__((interrupt)) void OFE_Handler(interrupt_frame* frame) {
    Panic("OVERFLOW");
    while (true);
}
__attribute__((interrupt)) void BREE_Handler(interrupt_frame* frame) {
    Panic("BOUND_RANGE_EXCEEDED");
    while (true);
}
__attribute__((interrupt)) void IOCE_Handler(interrupt_frame* frame) {
    Panic("INVALID_OPCODE");
    while (true);
}
__attribute__((interrupt)) void DNAE_Handler(interrupt_frame* frame) {
    Panic("DEVICE_NOT_AVAILABLE");
    while (true);
}
__attribute__((interrupt)) void ITSSE_Handler(interrupt_frame* frame) {
    Panic("INVALID_TSS");
    while (true);
}
__attribute__((interrupt)) void SNPE_Handler(interrupt_frame* frame) {
    Panic("SEGMENT_NOT_PRESENT");
    while (true);
}
__attribute__((interrupt)) void SSFE_Handler(interrupt_frame* frame) {
    Panic("STACK-SEGMENT_FAULT");
    while (true);
}
__attribute__((interrupt)) void FPE_Handler(interrupt_frame* frame) {
    Panic("x87_FLOATING-POINT_EXCEPTION");
    while (true);
}
__attribute__((interrupt)) void ACE_Handler(interrupt_frame* frame) {
    Panic("ALIGNMENT_CHECK");
    while (true);
}
__attribute__((interrupt)) void MCE_Handler(interrupt_frame* frame) {
    Panic("MACHINE_CHECK");
    while (true);
}
__attribute__((interrupt)) void SIMDFPE_Handler(interrupt_frame* frame) {
    Panic("SIMD_FLOATING-POINT_EXCEPTION");
    while (true);
}
__attribute__((interrupt)) void VEE_Handler(interrupt_frame* frame) {
    Panic("VIRTUALIZATION_EXCEPTION");
    while (true);
}
__attribute__((interrupt)) void CPE_Handler(interrupt_frame* frame) {
    Panic("CONTROL_PROTECTION_EXCEPTION");
    while (true);
}
__attribute__((interrupt)) void HIE_Handler(interrupt_frame* frame) {
    Panic("HYPERVISOR_INJECTION_EXCEPTION");
    while (true);
}
__attribute__((interrupt)) void VMMCE_Handler(interrupt_frame* frame) {
    Panic("VMM_COMMUNICATION_EXCEPTION");
    while (true);
}
__attribute__((interrupt)) void SE_Handler(interrupt_frame* frame) {
    Panic("SECURITY_EXCEPTION");
    while (true);
}


__attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame) {
    uint8_t scancode = inb(0x60);
    HandleKeyboard(scancode);

    KeyboardInfo *key_info = (KeyboardInfo *)KEY_INFO_ADDRESS;
    
    
    PIC_EndMaster();
}
__attribute__((interrupt)) void MouseInt_Handler(interrupt_frame* frame) {
    uint8_t mouseData = inb(0x60);

    HandlePS2Mouse(mouseData);

    PIC_EndSlave();
}
__attribute__((interrupt)) void PITInt_Handler(interrupt_frame* frame) {
    PIT::Tick();
    PIC_EndMaster();
}

void PIC_EndMaster() {
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave() {
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void RemapPIC() {
    uint8_t a1, a2; 

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
}