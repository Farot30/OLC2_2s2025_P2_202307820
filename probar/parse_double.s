// Versión SIMPLIFICADA de parse_double
.global parse_double
.text

parse_double:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    
    // Inicializar
    fmov d0, xzr            // resultado = 0.0
    mov w1, #0              // signo = positivo
    mov w2, #0              // decimales = false
    mov w3, #1              // divisor = 1
    mov x4, x0              // ptr
    
    // Saltar espacios
skip_spaces:
    ldrb w5, [x4]
    cbz w5, end_parse
    cmp w5, #32
    beq next_space
    cmp w5, #9
    bne check_sign
next_space:
    add x4, x4, #1
    b skip_spaces
    
check_sign:
    ldrb w5, [x4]
    cbz w5, end_parse
    cmp w5, #45
    bne check_plus
    mov w1, #1
    add x4, x4, #1
    b main_loop
check_plus:
    cmp w5, #43
    bne main_loop
    add x4, x4, #1
    
main_loop:
    ldrb w5, [x4]
    cbz w5, end_parse
    
    cmp w5, #46
    bne check_digit
    mov w2, #1
    add x4, x4, #1
    b main_loop
    
check_digit:
    cmp w5, #48
    blt end_parse
    cmp w5, #57
    bgt end_parse
    
    sub w6, w5, #48
    scvtf d2, w6
    
    cmp w2, #0
    bne decimal_part
    
    // Entera
    fmov d3, #10.0
    fmul d0, d0, d3
    fadd d0, d0, d2
    b next_digit
    
decimal_part:
    // Decimal: acumular y luego dividir al final
    fmov d3, #10.0
    fmul d0, d0, d3
    fadd d0, d0, d2
    mov w7, #10
    mul w3, w3, w7
    
next_digit:
    add x4, x4, #1
    b main_loop
    
end_parse:
    // Ajustar parte decimal
    cmp w2, #1
    bne apply_sign
    scvtf d3, w3
    fdiv d0, d0, d3
    
apply_sign:
    cmp w1, #1
    bne done
    fneg d0, d0
    
done:
    ldp x29, x30, [sp], #16
    ret