/* 

    Atlas | KAta :: (Kernel ATA (ATA brevetation of Atlas))

    Originally written by Dr. AIT (AKAs: totallynotdrait, facilitycore, xourcx, 0x69)


    A basic PS/2 mouse input, since is PS/2 and not USB, your system should support PS/2 emulation, if not it might get some problems.

*/


#include "mouse.h"
#include <aagui/rendering/BasicColors.h>

DBasicColors dbc;

bool LeftClicked;
bool MiddleClicked;
bool RightClicked;

uint32_t MousePointerMap[] = {
    dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.White, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.White, dbc.Blue, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.White, dbc.White, dbc.Blue, dbc.Blue, dbc.Blue, dbc.Blue, dbc.Blue, dbc.Blue, dbc.Blue, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.White, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    dbc.Blue, dbc.Blue, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


uint8_t MousePointerBitmap[] = {
    0b10000000, 0b00000000, 
    0b11000000, 0b00000000, 
    0b11100000, 0b00000000, 
    0b11110000, 0b00000000, 
    0b11111000, 0b00000000, 
    0b11111100, 0b00000000, 
    0b11111110, 0b00000000, 
    0b11111111, 0b00000000, 
    0b11111111, 0b10000000, 
    0b11111111, 0b11000000, 
    0b11111111, 0b11100000, 
    0b11111111, 0b11110000, 
    0b11111000, 0b00000000, 
    0b11110000, 0b00000000, 
    0b11100000, 0b00000000, 
    0b11000000, 0b00000000, 
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
            
            if ((data & 0b00001000) == 0) break;
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
        
        // render the cursor, the mouse position will still work
        if (isMouseCursorVisible) {
            GKRenderer->ClearMouseCursor(MousePointerBitmap, MousePositionOld);
            GKRenderer->DrawOverlayMouseCursor(MousePointerBitmap, MousePosition, MousePointerMap);
        }

        if (MousePacket[0] & PS2Leftbutton) {
            LeftClicked = true;
            showMouseCursor();
        } else {
            LeftClicked = false;
        }

        if (MousePacket[0] & PS2Middlebutton) {
            MiddleClicked = true;
            GKRenderer->printf(to_hexstring((uint8_t)MiddleClicked));
        } else {
            MiddleClicked = false;
        }
        
        if (MousePacket[0] & PS2Rightbutton) {
            RightClicked = true;
            hideMouseCursor();
        } else {
            RightClicked = false;
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

void showMouseCursor() {
    isMouseCursorVisible = true;
}

void hideMouseCursor() {
    isMouseCursorVisible = false;
    GKRenderer->ClearMouseCursor(MousePointerBitmap, MousePositionOld);
}