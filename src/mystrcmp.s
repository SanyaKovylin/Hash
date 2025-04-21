section .text

global mystrcmp
; extern strlen

; mystrcmp:
;     mov [rel RetOffs], rbp
;     pop rbp
;
;     lea rax, [rel my_strcmp]
;     call rax
;
;     push rbp
;     mov rbp, [rel RetOffs]
;     ret

mystrcmp:
    push rbp

    xchg rsi, rdi
;     lea rax, [rel mystrlen]
;     call rax
;
;     mov rcx, rax
;
;     mov edx, 1
;     shl edx, cl
;     dec edx
;     not edx

    vmovdqu ymm0, [rsi]
    vmovdqu ymm1, [rdi]

    vpcmpeqb ymm2, ymm0, ymm1
    vpcmpeqb ymm3, ymm0, ymm0
    vpcmpeqb ymm4, ymm0, ymm3

    vpmovmskb edx, ymm2
    vpmovmskb eax, ymm2
    or eax, edx
    cmp eax, edx
    jne .not_equal

.equal:
   xor rax, rax
   jmp .done
.not_equal:
    ; mov rax, 1

.done:
    pop rbp
    ret



mystrlen:
    mov rcx, -1
    xor al, al
    repne scasb
    not rcx
    dec rcx
    mov rax, rcx
    ret

section .data
RetOffs         dq  0
