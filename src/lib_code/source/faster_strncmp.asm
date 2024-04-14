bits 64

extern strncmp
global FasterStrncmp

; rdi = source1
; rsi = source2
; rdx = len

FasterStrncmp:
        xor rax, rax
        cmp rdx, 1
        ja .call_strcmp
        lea rax, [.len_table + 8 * rdx]
        jmp rax

        .exit:
        ret

.len_table:
dq      .len0
dq      .len1
; dq      .len2
; dq      .len3
; dq      .len4

.call_strcmp:
        xor rax, rax
        call FasterStrncmp
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