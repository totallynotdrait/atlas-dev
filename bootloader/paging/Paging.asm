PageTableEntry equ 0x1000

SetUpIdentityPaging:
    mov edi, PageTableEntry
    mov cr3, edi

    mov dword [edi], 0x2003
    add edi, 0x1000

    mov dword [edi], 0x3003
    add edi, 0x1000

    mov dword [edi], 0x4003
    add edi, 0x1000
    
    mov ebx, 0x00000003
    mov ebx, 512

    .SetEntry:
        mov dwrod [edi], ebx
        add ebx, 0x1000
        add edi, 8
        loop .SetEntry

    mov eax, cr4
    or eax 1 << 8
    wrnsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret