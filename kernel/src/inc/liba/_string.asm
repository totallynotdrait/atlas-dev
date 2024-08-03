[bits 64]

global __strlen
global __strcpy

extern __memcpy

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



__strcpy:
    push    rdi

    mov     rdi, rsi
    call    __strlen
    mov     rdx, rax
    inc     rdx ; Copy the \0 as well

    pop     rdi

    call    __memcpy
    mov     rax, rdi
    ret  