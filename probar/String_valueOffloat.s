.global valueOf_float

.text
valueOf_float:
    STP X29, X30, [SP, #-16]!
    STP X19, X20, [SP, #-16]!
    STP X21, X22, [SP, #-16]!
    MOV X19, X1
    MOV X20, X1
    FMOV S21, S0
    MOV W0, #0x7F800000
    FMOV S1, W0
    FCMP S21, S1
    B.EQ vf_infinity
    MOV W0, #0xFF800000
    FMOV S1, W0
    FCMP S21, S1
    B.EQ vf_negative_infinity
    FCMP S21, S21
    B.VC vf_regular_number
vf_nan:
    MOV W0, #'N'
    STRB W0, [X19], #1
    MOV W0, #'a'
    STRB W0, [X19], #1
    MOV W0, #'N'
    STRB W0, [X19], #1
    B vf_null_terminate
vf_infinity:
    MOV W0, #'I'
    STRB W0, [X19], #1
    MOV W0, #'n'
    STRB W0, [X19], #1
    MOV W0, #'f'
    STRB W0, [X19], #1
    B vf_null_terminate
vf_negative_infinity:
    MOV W0, #'-'
    STRB W0, [X19], #1
    MOV W0, #'I'
    STRB W0, [X19], #1
    MOV W0, #'n'
    STRB W0, [X19], #1
    MOV W0, #'f'
    STRB W0, [X19], #1
    B vf_null_terminate
vf_regular_number:
    FCMP S21, #0.0
    B.GE vf_positive
    MOV W0, #'-'
    STRB W0, [X19], #1
    FNEG S21, S21
vf_positive:
    FRINTZ S0, S21
    FCVTZS W21, S0
    MOV W0, W21
    MOV X1, X19
    BL valueOf_int
    ADD X19, X19, X1
    SCVTF S0, W21
    FSUB S1, S21, S0
    FCMP S1, #0.0
    B.EQ vf_null_terminate
    MOV W0, #'.'
    STRB W0, [X19], #1
    MOV W22, #0
    MOV W0, #10
    SCVTF S2, W0
vf_decimal_loop:
    CMP W22, #6
    B.GE vf_null_terminate
    FMUL S1, S1, S2
    FRINTZ S0, S1
    FCVTZS W0, S0
    FSUB S1, S1, S0
    ADD W0, W0, #'0'
    STRB W0, [X19], #1
    ADD W22, W22, #1
    FCMP S1, #0.0
    B.NE vf_decimal_loop
vf_null_terminate:
    MOV W0, #0
    STRB W0, [X19]
    SUB X1, X19, X20
    MOV X0, X20
    LDP X21, X22, [SP], #16
    LDP X19, X20, [SP], #16
    LDP X29, X30, [SP], #16
    RET
