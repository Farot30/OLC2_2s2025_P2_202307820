// Implementación de parse_double en ARM64 puro
// Convierte un string a double
// Entrada: x0 = puntero al string
// Salida: d0 = valor double

.global parse_double
.text

parse_double:
    // Guardar registros
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    
    // Inicializar variables
    fmov d0, xzr            // resultado = 0.0
    fmov d1, xzr            // parte_decimal = 0.0
    mov w1, #0              // signo = 0 (positivo)
    mov w2, #0              // encontro_punto = 0
    mov w3, #1              // divisor_decimal = 1
    mov x4, x0              // puntero al string
    
    // Saltar espacios en blanco
skip_spaces:
    ldrb w5, [x4]
    cmp w5, end_parse
    cmp w5, #32             // ' '
    beq next_space
    cmp w5, #9              // '\t'
    bne check_sign
next_space:
    add x4, x4, #1
    b skip_spaces
    
check_sign:
    ldrb w5, [x4]
    cmp w5, #0             // fin de string
    beq end_parse
    cmp w5, #45             // '-'
    bne check_plus
    mov w1, #1              // signo = negativo
    add x4, x4, #1
    b parse_loop
    
check_plus:
    cmp w5, #43             // '+'
    bne parse_loop
    add x4, x4, #1
    
parse_loop:
    ldrb w5, [x4]
    cbz w5, end_parse             // fin de string
    
    // Verificar si es punto decimal
    cmp w5, #46             // '.'
    bne check_digit
    mov w2, #1              // encontro_punto = true
    add x4, x4, #1
    b parse_loop
    
check_digit:
    // Verificar si es dígito
    cmp w5, #48             // '0'
    blt end_parse
    cmp w5, #57             // '9'
    bgt end_parse
    
    // Convertir carácter a dígito
    sub w5, w5, #48
    scvtf d2, w5            // Convertir a double
    
    // Si no hemos encontrado punto, procesar parte entera
    cmp w2, #0
    bne process_decimal
    
    // Parte entera: resultado = resultado * 10 + digito
    fmov d3, #10.0
    fmul d0, d0, d3
    fadd d0, d0, d2
    b next_char
    
process_decimal:
    // Parte decimal
    fmov d3, #10.0
    fmul d0, d0, d3         // digito / divisor
    fadd d0, d0, d2         // parte_decimal += digito/divisor
    
    // Incrementar divisor para el próximo dígito
    mov w7, #10
    mul w3, w3, w7          // divisor_decimal *= 10
    
next_char:
    add x4, x4, #1
    b parse_loop
    
end_parse:
    // Aplicar signo si es negativo
    cmp w2, #1
    bne return_result
    scvtf d3, w3
    fdiv d0, d0, d3
    
return_result:
    cmp w1, #1
    bne done
    fneg d0, d0
    // Resultado ya está en d0
    
done:
    ldp x29, x30, [sp], #16
    ret
