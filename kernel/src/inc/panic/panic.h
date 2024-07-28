#pragma once
#include "../interrupts/interrupts.h"

void Panic(const char* panicMessage, interrupt_frame* regs);
