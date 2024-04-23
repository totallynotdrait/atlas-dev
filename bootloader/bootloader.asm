; data stack
[org 0x7c00]

mov [BOOT_DISK], dl

mov bp, 0x7c00
mov sp, bp

mov bx, TestString
call PrintString

call ReadDisk

jmp PROGRAM_SPACE


%include "console-l/print.asm"
%include "bootloader/DiskRead.asm"

times 510-($-$$) db 0

dw 0xaa55 ; sign of bootloader
