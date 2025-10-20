.global valueOf_long

.section .rodata
longmin_str: .asciz "9223372036854775808"

.text
valueOf_long:
    STP X29, X30, [SP, #-16]!
    STP X19, X20, [SP, #-16]!
    STP X21, X22, [SP, #-16]!
    STP X23, X24, [SP, #-16]!
    MOV X19, X1
    MOV X20, X0
    MOV X21, X1
    MOV X22, #0
    MOV X23, X1
    CBNZ X20, vl_check_negative
    MOV W2, #'0'
    STRB W2, [X19], #1
    MOV W2, #0
    STRB W2, [X19]
    MOV X0, X21
    MOV X1, #1
    B vl_end
vl_check_negative:
    CMP X20, #0
    B.GE vl_check_longmin
    MOV W2, #'-'
    STRB W2, [X19], #1
    ADD X22, X22, #1
    MOV X23, X19
    MOV X24, #0x8000
    MOVK X24, #0x0000, LSL #16
    MOVK X24, #0x0000, LSL #32
    MOVK X24, #0x0000, LSL #48
    CMP X20, X24
    B.NE vl_regular_negative
    ADR X2, longmin_str
    MOV X3, #19
vl_copy_longmin:
    LDRB W4, [X2], #1
    STRB W4, [X19], #1
    SUBS X3, X3, #1
    B.NE vl_copy_longmin
    MOV W4, #0
    STRB W4, [X19]
    MOV X0, X21
    MOV X1, #20
    B vl_end
vl_regular_negative:
    NEG X20, X20
    B vl_positive
vl_check_longmin:
    MOV X23, X19
    B vl_positive
vl_positive:
    MOV X2, X23
    MOV X3, #10
vl_extract_loop:
    UDIV X4, X20, X3
    MSUB X5, X4, X3, X20
    ADD W5, W5, #'0'
    STRB W5, [X2], #1
    ADD X22, X22, #1
    MOV X20, X4
    CBNZ X20, vl_extract_loop
    MOV W5, #0
    STRB W5, [X2]
    SUB X2, X2, #1
    MOV X19, X23
vl_reverse_loop:
    CMP X19, X2
    B.GE vl_finish
    LDRB W5, [X19]
    LDRB W6, [X2]
    STRB W6, [X19], #1
    STRB W5, [X2], #-1
    B vl_reverse_loop
vl_finish:
    MOV X0, X21
    MOV X1, X22
vl_end:
    LDP X23, X24, [SP], #16
    LDP X21, X22, [SP], #16
    LDP X19, X20, [SP], #16
    LDP X29, X30, [SP], #16
    RET
