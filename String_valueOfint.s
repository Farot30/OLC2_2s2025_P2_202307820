.global valueOf_int

.section .rodata
intmin_str: .asciz "2147483648"

valueOf_int:
    STP X29, X30, [SP, #-16]!
    STP X19, X20, [SP, #-16]!
    STP X21, X22, [SP, #-16]!
    MOV X19, X1
    MOV W20, W0
    MOV X21, X1
    MOV X22, #0
    CBNZ W0, vi_check_negative
    MOV W2, #'0'
    STRB W2, [X19], #1
    MOV W2, #0
    STRB W2, [X19]
    MOV X0, X21
    MOV X1, #1
    B vi_end
vi_check_negative:
    CMP W20, #0
    B.GE vi_positive
    MOV W2, #'-'
    STRB W2, [X19], #1
    ADD X22, X22, #1
    MOV W3, #0x8000
    MOVK W3, #0x0000, LSL #16
    CMP W20, W3
    B.NE vi_regular_negative
    ADR X2, intmin_str
    MOV X3, #10
vi_copy_intmin:
    LDRB W4, [X2], #1
    STRB W4, [X19], #1
    SUBS X3, X3, #1
    B.NE vi_copy_intmin
    MOV W4, #0
    STRB W4, [X19]
    MOV X0, X21
    MOV X1, #11
    B vi_end
vi_regular_negative:
    NEG W20, W20
    B vi_positive
vi_positive:
    MOV X2, X19
    MOV W3, #10
vi_extract_loop:
    UDIV W4, W20, W3
    MSUB W5, W4, W3, W20
    ADD W5, W5, #'0'
    STRB W5, [X2], #1
    ADD X22, X22, #1
    MOV W20, W4
    CBNZ W20, vi_extract_loop
    MOV W5, #0
    STRB W5, [X2]
    MOV X19, X21
    ADD X19, X19, X22
    SUB X19, X19, #1
    MOV X2, X21
vi_reverse_loop:
    CMP X2, X19
    B.GE vi_finish
    LDRB W5, [X2]
    LDRB W6, [X19]
    STRB W6, [X2], #1
    STRB W5, [X19], #-1
    B vi_reverse_loop
vi_finish:
    MOV X0, X21
    MOV X1, X22
vi_end:
    LDP X21, X22, [SP], #16
    LDP X19, X20, [SP], #16
    LDP X29, X30, [SP], #16
    RET
