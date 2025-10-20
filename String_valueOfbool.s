.global valueOf_bool

valueOf_bool:
    STP X29, X30, [SP, #-16]!
    MOV X2, X1
    CBZ W0, vb_false

vb_true:
    MOV W3, #'t'
    STRB W3, [X2], #1
    MOV W3, #'r'
    STRB W3, [X2], #1
    MOV W3, #'u'
    STRB W3, [X2], #1
    MOV W3, #'e'
    STRB W3, [X2], #1
    MOV W3, #0
    STRB W3, [X2]
    MOV X0, X1
    MOV X1, #4
    B vb_end

vb_false:
    MOV W3, #'f'
    STRB W3, [X2], #1
    MOV W3, #'a'
    STRB W3, [X2], #1
    MOV W3, #'l'
    STRB W3, [X2], #1
    MOV W3, #'s'
    STRB W3, [X2], #1
    MOV W3, #'e'
    STRB W3, [X2], #1
    MOV W3, #0
    STRB W3, [X2]
    MOV X0, X1
    MOV X1, #5

vb_end:
    LDP X29, X30, [SP], #16
    RET
