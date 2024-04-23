; run code after bootloader, and before kata main run
; https://www.youtube.com/watch?v=sk_ngabpwXQ&list=PLxN4E629pPnKKqYsNVXpmCza8l0Jb6l8-&index=6&ab_channel=Poncho (6:13)

[org 0x7e00]

jmp EnterProtectedMode

%include "bootloader/gdt.asm"
%include "console-l/print.asm"
%include "bootloader/CPUID.asm"
%include "bootloader/paging/Paging.asm"

EnterProtectedMode:
    call EnableA20
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp codeseg:StartProtectedMode

EnableA20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret 

[bits 32]

StartProtectedMode:

    mov ax, dataseg
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov [0xb8000], byte 'h'


    ; in case of more space for stack
    ; mov ebp, 0x90000
    ; mov esp, ebp

    call DetectCPUID
    call DetectLongMode
    call SetUpIdentityPaging
    jmp $

times 2048-($-$$) db 0