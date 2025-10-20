.global valueOf_double

.text
valueOf_double:
    STP X29, X30, [SP, #-16]!
    STP X19, X20, [SP, #-16]!
    STP X21, X22, [SP, #-16]!
    STP X23, X24, [SP, #-16]!
    MOV X19, X1
    MOV X20, X1
    FMOV D21, D0
    MOV X0, #0x7FF0000000000000
    FMOV D1, X0
    FCMP D21, D1
    B.EQ vd_infinity
    MOV X0, #0xFFF0000000000000
    FMOV D1, X0
    FCMP D21, D1
    B.EQ vd_negative_infinity
    FCMP D21, D21
    B.VC vd_regular_number
vd_nan:
    MOV W0, #'N'
    STRB W0, [X19], #1
    MOV W0, #'a'
    STRB W0, [X19], #1
    MOV W0, #'N'
    STRB W0, [X19], #1
    B vd_null_terminate
vd_infinity:
    MOV W0, #'I'
    STRB W0, [X19], #1
    MOV W0, #'n'
    STRB W0, [X19], #1
    MOV W0, #'f'
    STRB W0, [X19], #1
    MOV W0, #'i'
    STRB W0, [X19], #1
    MOV W0, #'n'
    STRB W0, [X19], #1
    MOV W0, #'i'
    STRB W0, [X19], #1
    MOV W0, #'t'
    STRB W0, [X19], #1
    MOV W0, #'y'
    STRB W0, [X19], #1
    B vd_null_terminate
vd_negative_infinity:
    MOV W0, #'-'
    STRB W0, [X19], #1
    B vd_infinity
vd_regular_number:
    FCMP D21, #0.0
    B.GE vd_positive
    MOV W0, #'-'
    STRB W0, [X19], #1
    FNEG D21, D21
vd_positive:
    FRINTZ D0, D21
    FCVTZS X21, D0
    MOV X0, X21
    MOV X1, X19
    BL valueOf_long
    ADD X19, X19, X1
    SCVTF D0, X21
    FSUB D1, D21, D0
    FCMP D1, #0.0
    B.EQ vd_null_terminate
    MOV W0, #'.'
    STRB W0, [X19], #1
    MOV W22, #0
    MOV W0, #10
    SCVTF D2, W0
    MOV X23, X19
vd_decimal_loop:
    CMP W22, #15
    B.GE vd_trim_zeros
    FMUL D1, D1, D2
    FRINTZ D0, D1
    FCVTZS W0, D0
    FSUB D1, D1, D0
    ADD W0, W0, #'0'
    STRB W0, [X19], #1
    ADD W22, W22, #1
    FCMP D1, #0.0
    B.NE vd_decimal_loop
vd_trim_zeros:
    SUB X19, X19, #1
vd_trim_loop:
    CMP X19, X23
    B.LS vd_null_terminate
    LDRB W0, [X19]
    CMP W0, #'0'
    B.NE vd_null_terminate
    SUB X19, X19, #1
    B vd_trim_loop
vd_null_terminate:
    ADD X19, X19, #1
    MOV W0, #0
    STRB W0, [X19]
    SUB X1, X19, X20
    MOV X0, X20
    LDP X23, X24, [SP], #16
    LDP X21, X22, [SP], #16
    LDP X19, X20, [SP], #16
    LDP X29, X30, [SP], #16
    RET
