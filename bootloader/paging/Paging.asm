PageTableEntry equ 0x1000

SetUpIdentityPaging:
    mov edi, PageTableEntry
    mov cr3, edi
    mov dword [edi], 0x2003
    ret