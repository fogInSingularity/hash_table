bits 64

extern strncmp
global FasterStrncmp

FasterStrncmp:
        mov rax, 0
        ret