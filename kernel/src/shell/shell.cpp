/* 
    Atlas Development Project

    Atlas Shell (ATA Shell), a basic shell for executing basic commands.


*/

#include "shell.h"
#include <liba/string.h>
#include <sound/pc_speaker.h>
#include <hardw/pci.h>
#include <syscall/syscall.h>
#include <pageframe/PageFrameAllocator.h>
#include <mem/heap.h>

#define MAX_TOKENS 128

int check_command(char* cmd, const char* text)
{
    return strcmp(cmd, (char*)text) == 0;
}

int check_short_command(char* cmd, const char* text, int length)
{
    int i;
    for (i = 0; cmd[i] == text[i] && i < length; i++);

    if(i == length)
        return 1;
    else 
        return 0;
}

bool is_only_spaces(const char* str) {
    while (*str != '\0') {
        if (*str != ' ') {
            return false; // Found a non-space character
        }
        str++;
    }
    return true; // All characters were spaces or the string is empty
}

uint8_t hexCharToInt(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    // Handle error case or return default value
    return 0;
}

// Function to convert a two-character string representing hex (like "DB") to a uint8_t value
uint8_t hexStringToUInt8(const char* hexString) {
    uint8_t result = 0;
    result += (hexCharToInt(hexString[0]) << 4); // Most significant nibble
    result += hexCharToInt(hexString[1]);        // Least significant nibble
    return result;
}

void calculator_mode(char* input)
{
    if(check_command(input, "quit")) {
        state = COMMAND_MODE;
        return;
    }
    state = CALCULATOR_MODE;
}

void command_mode(char* input) {
    if(check_command(input, "help")) {
        printf("[Atlas shell] commands:\n");
        printf("usage: [command] [option] ...\n");
        printf("Commands (built-in atlas shell):\n");
        printf("help\t\t: Displays this menu\n");
        printf("exit\t\t: no\n");
        printf("echo <arg>\t\t: Prints to the screen a defined message\n");
        printf("shutdown\t\t: Shutdowns the system\n");
        printf("clear\t\t: Clears the screen\n");
        printf("putchar <hex>\t\t: Prints a character\n");
        printf("--- debug commands ---\n");
        printf("kata.init.0\t\t: Does a division by 0, causing Atlas to crash\n");
        printf("kata.init.1\t\t: Does a page fault, causing Atlas to crash\n");
        printf("kata.init.3\t\t: Causes a Non-Maskable Interrupt\n");

        printf("kata.test.psf\t\t: Prints all of the characters of the current PSF font (0x00 to 0xFF)");
    } else if (check_short_command(input, "echo", 4)) {
        char* msg = &input[5];
        printf(msg);
    } else if (check_short_command(input, "putchar", 7)) {
        const char* chr_str = &input[8];
        uint8_t chr = hexStringToUInt8(chr_str);
        GKRenderer->putChar((char)chr);
    } else if (check_short_command(input, "setcc", 5)) {
        const char* hex_str = &input[6];
        uint64_t hex = hexStringToUInt8(hex_str);
        GKRenderer->ClearColor = hex;
    } else if (check_command(input, "clear")) {
        printf("\\c");
    } else if (check_command(input, "whoami")) {
        printf("idk, lol");
    } else if (check_command(input, "kata.init.0")) {
        int a = 1;
        int b = 0;
        a/b;
    } else if (check_command(input, "kata.init.1")) {
        int* t = (int*)0x80000000000;
        *t = 2;
    } else if (check_command(input, "kata.init.3")) {
        asm volatile ("outb %al, $0x92");
    } else if (check_command(input, "kata.test.psf")) {
        for(int i = 0x00; i <= 0xFF; ++i) {
            GKRenderer->putChar((char)i);
            GKRenderer->putChar(' ');
        }
    } else if (check_command(input, "forse")) {
        printf("non far");
        GKRenderer->putChar((char)0xF2);
    } else {
        printf("%s: command not found", input);
    }
}


void shell_command(char* input) {
    switch(state)
    {
    case COMMAND_MODE:
        command_mode(input);
        break;
    case CALCULATOR_MODE:
        calculator_mode(input);
        break;
    }
}



void init_ata_shell() {
    char* tokens[MAX_TOKENS] = {0};

    uint64_t freeRam = GlobalAllocator.GetFreeRAM();
    uint64_t usedRam = GlobalAllocator.GetUsedRAM();
    uint64_t reservedRam = GlobalAllocator.GetReservedRAM();
    uint64_t totalRam = (freeRam + usedRam);

    printf("\\c");
    GKRenderer->Color = 0x76b845;
    printf("___ooo_____oo____ooo__________________\t\t[atlas-dev]\n"); GKRenderer->Color = 0xf2be2e;
    printf("_oo___oo___oo_____oo____ooooo___oooo__\t\tTotal memory: %d GB\n", totalRam/1073741824); GKRenderer->Color = 0xe19433;
    printf("oo_____oo_oooo____oo___oo___oo_oo___o_\t\tFree memory: %d GB\n", freeRam/1073741824); GKRenderer->Color = 0xc55152;
    printf("ooooooooo__oo_____oo___oo___oo___oo___\t\tUsed memory: %d MB\n", usedRam/1048576); GKRenderer->Color = 0xea4c89;
    printf("oo_____oo__oo__o__oo___oo___oo_o___oo_\t\tReserved memory: %d MB\n", reservedRam/1048576); GKRenderer->Color = 0x009edc;
    printf("oo_____oo___ooo__ooooo__oooo_o__oooo__\t\t\n"); 
    printf("______________________________________\t\t");
    
    GKRenderer->Color = hbc.White;
    
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Gray;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Red;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Maroon;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Yellow;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Lime;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Green;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Cyan;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Teal;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Blue;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Navy;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Magenta;
    GKRenderer->putChar((char)0x05);
    GKRenderer->Color = hbc.Purple;
    GKRenderer->putChar((char)0x05);
    printf("\n");
    GKRenderer->Color = 0x009edc;
    
    GKRenderer->Color = hbc.Gray;
    printf("\n");

    GKRenderer->Color = hbc.Cyan;
    printf("[Atlas shell]\n");
    GKRenderer->Color = hbc.Gray;

    printf("\tKeyboard and mouse not working?\n");
    printf("\t\tYou probably using a usb keyboard or mouse which Atlas doesn't currently support.\n");
    printf("\tNo sounds from PC Speaker?\n");
    printf("\t\tYour computer probably doesn't have a PC Speaker.\n");
    printf("\tHave a certain amount of RAM like 16 GB but it shows 15 GB?\n");
    printf("\t\tIt's normal, probably a few 100 or 200 MB of memory are reserved.\n\n");
    while (true) {
        char cmdbuffer[256] = "";
        GKRenderer->Color = 0x009edc; printf("atlas@shell "); GKRenderer->Color = 0xea4c89; printf("~ "); GKRenderer->Color = hbc.Gray; printf("> "); GKRenderer->Color = hbc.Gray;

        scanf("%s", cmdbuffer);
        printf("\n");
        size_t len = strlen(cmdbuffer);
        if (len > 0 && cmdbuffer[len - 1] == '\n') {
            cmdbuffer[len - 1] = '\0';
        }

        if (is_only_spaces(cmdbuffer)) {
            continue;
        }

        if (check_command(cmdbuffer, "exit")) {
            printf("Exiting atlas shell...");
            break;
        }

        shell_command(cmdbuffer);
        printf("\n");
    }
}