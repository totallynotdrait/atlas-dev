[bits 64]

global __strlen

__strlen:
    push    rbx
    push    rcx

    mov     rbx, rdi
    xor     al, al
    mov     rcx, 0xffffffff

    repne   scasb
    sub     rdi, rbx
    mov     rax, rdi
    dec     rax

    pop     rcx
    pop     rbx

    ret