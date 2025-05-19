section .text

global mystrcmp

mystrcmp:
    push rbp

    vmovdqu ymm0, [rsi]
    vmovdqu ymm1, [rdi]

    vpcmpeqb ymm2, ymm0, ymm1
    vpmovmskb eax, ymm2

    cmp eax, 0xffffffff
    jne .not_equal

.equal:
   xor rax, rax

.not_equal:
.done:
    pop rbp
    ret
