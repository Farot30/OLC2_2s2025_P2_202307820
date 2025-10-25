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
    mov w3, #0              // divisor_decimal = 1
    mov x4, x0              // puntero al string
    
    // Saltar espacios en blanco
skip_spaces_d:
    ldrb w5, [x4]
    cmp w5, #32             // ' '
    beq next_char_space_d
    cmp w5, #9              // '\t'
    bne check_sign_d
next_char_space_d:
    add x4, x4, #1
    b skip_spaces_d
    
check_sign_d:
    ldrb w5, [x4]
    cmp w5, #45             // '-'
    bne check_plus_d
    mov w1, #1              // signo = negativo
    add x4, x4, #1
    b parse_loop_d
    
check_plus_d:
    cmp w5, #43             // '+'
    bne parse_loop_d
    add x4, x4, #1
    
parse_loop_d:
    ldrb w5, [x4]
    
    // Verificar si es punto decimal
    cmp w5, #46             // '.'
    bne check_digit_d
    mov w2, #1              // encontro_punto = true
    add x4, x4, #1
    b parse_loop_d
    
check_digit_d:
    // Verificar si es dígito
    cmp w5, #48             // '0'
    blt end_parse_d
    cmp w5, #57             // '9'
    bgt end_parse_d
    
    // Convertir carácter a dígito
    sub w5, w5, #48
    scvtf d2, w5            // Convertir a double
    
    // Si no hemos encontrado punto, procesar parte entera
    cmp w2, #0
    bne process_decimal_d
    
    // Parte entera: resultado = resultado * 10 + digito
    fmov d3, #10.0
    fmul d0, d0, d3
    fadd d0, d0, d2
    b next_char_d
    
process_decimal_d:
    // Parte decimal
    mov w6, #10
    mul w3, w3, w6          // divisor_decimal *= 10
    scvtf d3, w3            // Convertir divisor a double
    fdiv d2, d2, d3         // digito / divisor
    fadd d1, d1, d2         // parte_decimal += digito/divisor
    
next_char_d:
    add x4, x4, #1
    b parse_loop_d
    
end_parse_d:
    // Combinar parte entera y decimal
    fadd d0, d0, d1
    
    // Aplicar signo si es negativo
    cmp w1, #1
    bne return_result_d
    fneg d0, d0
    
return_result_d:
    // Resultado ya está en d0
    
    // Restaurar registros y retornar
    ldp x29, x30, [sp], #16
    ret
