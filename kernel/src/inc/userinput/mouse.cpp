/* 

    Atlas | KAta :: (Kernel ATA (ATA brevetation of Atlas))

    Originally written by Dr. AIT (AKAs: totallynotdrait, facilitycore, xourcx, 0x69)


    A basic PS/2 mouse input, since is PS/2 and not USB, your system should support PS/2 emulation, if not it might get some problems.

*/


#include "mouse.h"

uint8_t MousePointer[] = {
    0b11000000, 0b00000001, 
    0b10100000, 0b00000001, 
    0b10010000, 0b00000001, 
    0b10001000, 0b00000001, 
    0b10000100, 0b00000001, 
    0b10000010, 0b00000001, 
    0b10000001, 0b00000001, 
    0b10000000, 0b10000001, 
    0b10000000, 0b01000001, 
    0b10000000, 0b00100001, 
    0b10000000, 0b00010001, 
    0b10000000, 0b00001001, 
    0b10000000, 0b00000101, 
    0b10000000, 0b00000011, 
    0b10000000, 0b00000001, 
    0b10000000, 0b00000001, 
};

void MouseWait() {
    uint64_t timeout = 100000;
    while (timeout--) {
        if ((inb(0x64) & 0b10) == 0) {
            return;
        } 
    }
}

void MouseWaitInput() {
    uint64_t timeout = 100000;
    while (timeout--) {
        if (inb(0x64) & 0b1) {
            return;
        } 
    }
}

void MouseWrite(uint8_t value) {
    MouseWait();
    outb(0x64, 0xD4);
    MouseWait();
    outb(0x60, value);
}

uint8_t MouseRead() {
    MouseWaitInput();
    return inb(0x60);
}

uint8_t mouseCycle = 0;
uint8_t MousePacket[4];
bool MousePacketReady = false;
Point MousePosition;
Point MousePositionOld;

void HandlePS2Mouse(uint8_t data) {

    ProcessMousePacket();

    static bool skip = true;
    if (skip) { skip = false; return; }

    switch (mouseCycle) {
        case 0:
            
            if (data & 0b00001000 == 0) break;
            MousePacket[0] = data;
            mouseCycle++;
            break;
        case 1:
            
            MousePacket[1] = data;
            mouseCycle++;
            break;
        case 2:
            // if (MousePacketReady) break;
            MousePacket[2] = data;
            MousePacketReady = true;
            mouseCycle = 0;
            break;
    }
}

void ProcessMousePacket() {
    if (!MousePacketReady) return;

        bool xNegative, yNegative, xOverflow, yOverflow;

        if (MousePacket[0] & PS2XSign){
            xNegative = true;
        }else xNegative = false;

        if (MousePacket[0] & PS2YSign){
            yNegative = true;
        }else yNegative = false;

        if (MousePacket[0] & PS2XOverflow){
            xOverflow = true;
        }else xOverflow = false;

        if (MousePacket[0] & PS2YOverflow){
            yOverflow = true;
        }else yOverflow = false;

        if (!xNegative){
            MousePosition.X += MousePacket[1];
            if (xOverflow){
                MousePosition.X += 255;
            }
        } else
        {
            MousePacket[1] = 256 - MousePacket[1];
            MousePosition.X -= MousePacket[1];
            if (xOverflow){
                MousePosition.X -= 255;
            }
        }

        if (!yNegative){
            MousePosition.Y -= MousePacket[2];
            if (yOverflow){
                MousePosition.Y -= 255;
            }
        } else
        {
            MousePacket[2] = 256 - MousePacket[2];
            MousePosition.Y += MousePacket[2];
            if (yOverflow){
                MousePosition.Y += 255;
            }
        }

        if (MousePosition.X < 0) MousePosition.X = 0;
        if (MousePosition.X > GKRenderer->TargetFramebuffer->Width-1) MousePosition.X = GKRenderer->TargetFramebuffer->Width-1;
        
        if (MousePosition.Y < 0) MousePosition.Y = 0;
        if (MousePosition.Y > GKRenderer->TargetFramebuffer->Height-1) MousePosition.Y = GKRenderer->TargetFramebuffer->Height-1;
        
        GKRenderer->ClearMouseCursor(MousePointer, MousePositionOld);
        GKRenderer->DrawOverlayMouseCurosr(MousePointer, MousePosition, 0xffffffff);

        if (MousePacket[0] & PS2Leftbutton) {
            
        }

        if (MousePacket[0] & PS2Middlebutton) {
            
            
        }

        if (MousePacket[0] & PS2Rightbutton) {
            GKRenderer->printf("LLLLLLLLLLLLLLLLLEEEEEEEEEEEEEEEEEEEEEFFFFFFFFFFFFFFFFFFFTTTTTTTTTTTTTTTTTTT");
        }

        MousePacketReady = false;
        MousePositionOld = MousePosition;
}

void InitPS2Mouse() {
    log->print("Initializing PS2 Mouse...");
    outb(0x64, 0xA8);
    MouseWait();
    outb(0x64, 0x20);
    MouseWaitInput();
    uint8_t status = inb(0x60);
    status |= 0b10;
    MouseWait();
    outb(0x64, 0x60);
    MouseWait();
    outb(0x60, status);

    MouseWrite(0xF6);
    MouseRead();

    MouseWrite(0xF4);
    MouseRead();
    log->ok("Initialized PS2 Mouse.");
}