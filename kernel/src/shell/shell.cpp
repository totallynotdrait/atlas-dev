/* 
    Atlas Development Project

    Atlas Shell (ATA Shell), a basic shell for executing basic commands.


*/

#include "shell.h"
#include <liba/string.h>
#include <sound/pc_speaker.h>
#include <hardw/pci.h>

void init_ata_shell() {
    
    printf("\\c");
    GKRenderer->Color = 0x76b845;
    printf("___ooo_____oo____ooo__________________\n"); GKRenderer->Color = 0xf2be2e;
    printf("_oo___oo___oo_____oo____ooooo___oooo__\n"); GKRenderer->Color = 0xe19433;
    printf("oo_____oo_oooo____oo___oo___oo_oo___o_\n"); GKRenderer->Color = 0xc55152;
    printf("ooooooooo__oo_____oo___oo___oo___oo___\n"); GKRenderer->Color = 0xea4c89;
    printf("oo_____oo__oo__o__oo___oo___oo_o___oo_\n"); GKRenderer->Color = 0x009edc;
    printf("oo_____oo___ooo__ooooo__oooo_o__oooo__\n"); GKRenderer->Color = 0x009edc;
    printf("______________________________________\n"); GKRenderer->Color = hbc.Gray;
    printf("\n");

    GKRenderer->Color = hbc.Cyan;
    printf("[Atlas shell]\n");
    GKRenderer->Color = hbc.Gray;

    printf("\tKeyboard and mouse not working?\n");
    printf("\t\tYou probably using a usb keyboard or mouse which Atlas doesn't currently support.\n");
    printf("\tNo sounds from PC Speaker?\n");
    printf("\t\tYour computer probably doesn't have a PC Speaker.\n\n");

    while (true) {
        char cmdbuffer[256];
        scanf("shell> %s", cmdbuffer);
        printf("\n");
        if (strcmp(cmdbuffer, "help") == 0) {
            printf("You are in Atlas Shell\nThere isn't much that you can do...\n");
        } else if (strcmp(cmdbuffer, "exit") == 0) {
            GKRenderer->Color = hbc.Red;
            printf("no\n");
            GKRenderer->Color = hbc.Gray;
        } else if (strcmp(cmdbuffer, "clear") == 0) {
            printf("\\c");
        } else {
            printf("%s: command not found\n", cmdbuffer);
        }
    }
}