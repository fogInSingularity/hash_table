global HashAsm

HashAsm:
        mov     rax, [rsi * 8 + .L4]
        jmp     rax
.exit:  
        mov rax, 0
        ret

.L4:
        dq   .L24
        dq   .L22
        dq   .L21
        dq   .L20
        dq   .L19
        dq   .L18
        dq   .L17
        dq   .L16
        dq   .L15
        dq   .L14
        dq   .L13
        dq   .L12
        dq   .L11
        dq   .L10
        dq   .L9
        dq   .L8
        dq   .L7
        dq   .L6
        dq   .L5
        dq   .L3
.L24:
        mov     eax, -99402089
        ret
.L3:
        imul    edx, DWORD [rdi], -862048943
        movzx   eax, BYTE  [rdi+16]
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+8], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+12], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        movzx   edx, WORD  [rdi+17]
        sal     edx, 8
        or      eax, edx
        imul    eax, eax, -862048943
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     eax, ecx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 19
.L26:
        xor     eax, edx
        imul    eax, eax, -2048144789
        mov     edx, eax
        shr     edx, 13
        xor     edx, eax
        imul    edx, edx, -1028477387
        mov     eax, edx
        shr     eax, 16
        xor     eax, edx
        ret
.L5:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, WORD  [rdi+16]
        imul    eax, eax, -862048943
        rorx    edx, edx, 17
        rorx    eax, eax, 17
        imul    edx, edx, 461845907
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+8], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+12], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 18
        jmp     .L26
.L6:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, BYTE  [rdi+16]
        imul    eax, eax, -862048943
        rorx    edx, edx, 17
        rorx    eax, eax, 17
        imul    edx, edx, 461845907
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+8], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+12], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 17
        jmp     .L26
.L7:
        imul    edx, DWORD  [rdi], -862048943
        imul    eax, DWORD  [rdi+12], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+8], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        rorx    eax, eax, 19
        lea     eax, [rax-430675100+rax*4]
        mov     edx, eax
        shr     eax, 16
        xor     edx, 16
        jmp     .L26
.L8:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, BYTE  [rdi+12]
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+8], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        movzx   edx, WORD  [rdi+13]
        sal     edx, 8
        or      eax, edx
        imul    eax, eax, -862048943
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     eax, ecx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 15
        jmp     .L26
.L9:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, WORD  [rdi+12]
        imul    eax, eax, -862048943
        rorx    edx, edx, 17
        rorx    eax, eax, 17
        imul    edx, edx, 461845907
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+8], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 14
        jmp     .L26
.L10:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, BYTE  [rdi+12]
        imul    eax, eax, -862048943
        rorx    edx, edx, 17
        rorx    eax, eax, 17
        imul    edx, edx, 461845907
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+8], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 13
        jmp     .L26
.L11:
        imul    edx, DWORD  [rdi], -862048943
        imul    eax, DWORD  [rdi+8], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        rorx    eax, eax, 19
        lea     eax, [rax-430675100+rax*4]
        mov     edx, eax
        shr     eax, 16
        xor     edx, 12
        jmp     .L26
.L12:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, BYTE  [rdi+8]
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        movzx   edx, WORD  [rdi+9]
        sal     edx, 8
        or      eax, edx
        imul    eax, eax, -862048943
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     eax, ecx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 11
        jmp     .L26
.L13:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, WORD  [rdi+8]
        imul    eax, eax, -862048943
        rorx    edx, edx, 17
        rorx    eax, eax, 17
        imul    edx, edx, 461845907
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 10
        jmp     .L26
.L14:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, BYTE  [rdi+8]
        imul    eax, eax, -862048943
        rorx    edx, edx, 17
        rorx    eax, eax, 17
        imul    edx, edx, 461845907
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        imul    edx, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, ecx
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 9
        jmp     .L26
.L15:
        imul    edx, DWORD  [rdi], -862048943
        imul    eax, DWORD  [rdi+4], -862048943
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        rorx    eax, eax, 19
        lea     eax, [rax-430675100+rax*4]
        mov     edx, eax
        shr     eax, 16
        xor     edx, 8
        jmp     .L26
.L16:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, BYTE  [rdi+4]
        rorx    edx, edx, 17
        imul    edx, edx, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     ecx, [rdx-430675100+rdx*4]
        movzx   edx, WORD  [rdi+5]
        sal     edx, 8
        or      eax, edx
        imul    eax, eax, -862048943
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     eax, ecx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 7
        jmp     .L26
.L17:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, WORD  [rdi+4]
        imul    eax, eax, -862048943
        rorx    edx, edx, 17
        rorx    eax, eax, 17
        imul    edx, edx, 461845907
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 6
        jmp     .L26
.L18:
        imul    edx, DWORD  [rdi], -862048943
        movzx   eax, BYTE  [rdi+4]
        imul    eax, eax, -862048943
        rorx    edx, edx, 17
        rorx    eax, eax, 17
        imul    edx, edx, 461845907
        imul    eax, eax, 461845907
        xor     edx, -1094799174
        rorx    edx, edx, 19
        lea     edx, [rdx-430675100+rdx*4]
        xor     eax, edx
        mov     edx, eax
        shr     eax, 16
        xor     edx, 5
        jmp     .L26
.L19:
        imul    eax, DWORD  [rdi], -862048943
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     eax, -1094799174
        rorx    eax, eax, 19
        lea     eax, [rax-430675100+rax*4]
        mov     edx, eax
        shr     eax, 16
        xor     edx, 4
        jmp     .L26
.L20:
        movzx   edx, WORD  [rdi+1]
        movzx   eax, BYTE  [rdi]
        sal     edx, 8
        or      eax, edx
        imul    eax, eax, -862048943
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     eax, -1094799175
.L25:
        mov     edx, eax
        shr     edx, 16
        jmp     .L26
.L21:
        movzx   eax, WORD  [rdi]
        imul    eax, eax, -862048943
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     eax, -1094799176
        jmp     .L25
.L22:
        movzx   eax, BYTE  [rdi]
        imul    eax, eax, -862048943
        rorx    eax, eax, 17
        imul    eax, eax, 461845907
        xor     eax, -1094799173
        jmp     .L25
.L2:
        xor     eax, eax
        ret