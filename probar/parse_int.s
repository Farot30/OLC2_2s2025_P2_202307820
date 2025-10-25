.global parse_int
.text

parse_int:
    // Guardar registros
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    
    // Inicializar variables
    mov w1, #0              // resultado = 0
    mov w2, #0              // signo = 0 (positivo)
    mov x3, x0              // puntero al string
    
    // Saltar espacios en blanco
skip_spaces:
    ldrb w4, [x3]           // Cargar carácter
    cmp w4, #32             // ' '
    beq next_char_space
    cmp w4, #9              // '\t'
    bne check_sign
next_char_space:
    add x3, x3, #1
    b skip_spaces
    
check_sign:
    // Verificar signo
    ldrb w4, [x3]
    cmp w4, #45             // '-'
    bne check_plus
    mov w2, #1              // signo = negativo
    add x3, x3, #1
    b parse_loop
    
check_plus:
    cmp w4, #43             // '+'
    bne parse_loop
    add x3, x3, #1          // Saltar '+'
    
parse_loop:
    ldrb w4, [x3]           // Cargar carácter
    
    // Verificar si es dígito (0-9)
    cmp w4, #48             // '0'
    blt end_parse
    cmp w4, #57             // '9'
    bgt end_parse
    
    // Convertir carácter a dígito
    sub w4, w4, #48         // digito = char - '0'
    
    // resultado = resultado * 10 + digito
    mov w5, #10
    mul w1, w1, w5          // resultado *= 10
    add w1, w1, w4          // resultado += digito
    
    // Siguiente carácter
    add x3, x3, #1
    b parse_loop
    
end_parse:
    // Aplicar signo si es negativo
    cmp w2, #1
    bne return_result
    neg w1, w1              // resultado = -resultado
    
return_result:
    mov w0, w1              // Retornar en w0
    
    // Restaurar registros y retornar
    ldp x29, x30, [sp], #16
    ret
