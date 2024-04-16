bits 64

extern strncmp
global FasterStrncmp

; rdi = source1
; rsi = source2
; rdx = len

FasterStrncmp:
        xor rax, rax
        cmp rdx, 3
        je .len3
        cmp rdx, 2
        ja .call_strcmp
        lea rax, [.len_table + 8 * rdx]
        mov rax, [rax]
        jmp rax

        .exit:
        ret
.len3:
        xor rax, rax
        xor r10, r10
        mov eax, [rdi]
        mov r10d, [rsi]
        xor al, al
        xor r10b, r10b
        sub rax, r10
        jmp .exit
.len_table:
dq      .len0
dq      .len1
dq      .len2
; dq      .len3
; dq      .len4

.call_strcmp:
        xor rax, rax
        call strncmp
        jmp .exit

.len0:
        xor rax, rax
        jmp .exit

.len1:
        xor rax, rax
        xor r10, r10
        mov al, [rdi]
        mov r10b, [rsi]
        sub rax, r10
        jmp .exit
.len2:
        xor rax, rax
        xor r10, r10
        mov ax, [rdi]
        mov r10w, [rsi]
        sub rax, r10
        jmp .exit
