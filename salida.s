// Código ARM64 generado por JavaLang Compiler
.global _start
.extern itoa
.extern valuesOf_int
.extern valueOf_bool
.extern valueOf_float
.extern valueOf_double
.extern valueOf_long
.extern printf


// declaraciones de variables
.data
str_const_46: .asciz "\n=== FIN DEL SISTEMA ==="
format_string_45: .asciz "%s\n"
format_concat_43: .asciz "Rectangulo -> base=%f cm, altura=%f cm, area=%f cm^2, perimetro=%f cm\n"
format_concat_42: .asciz "Circulo -> radio=%f cm, area=%f cm^2, perimetro=%f cm\n"
str_const_44: .asciz "\n--- FIGURAS GEOMETRICAS ---"
format_string_43: .asciz "%s\n"
perimetroRectCm: .word 0
areaRectCm2: .word 0
alturaRectCm: .double 0.0
baseRectCm: .double 0.0
perimetroCirculoCm: .word 0
areaCirculoCm2: .word 0
radioCirculoCm: .word 0
format_concat_41: .asciz "Resultado científico: %f\n"
str_const_42: .asciz "\n--- CALCULO FINAL (SIMAULACION CÁLCULO REAL ) ---"
format_string_41: .asciz "%s\n"
resultadoFinal: .word 0
variableComentada: .word 50
format_concat_40: .asciz "int %d -> char '%c'\n"
format_concat_39: .asciz "float %f -> int %d\n"
format_concat_38: .asciz "double %f -> float %f\n"
str_const_40: .asciz "Casting manual regresivo:"
format_string_39: .asciz "%s\n"
caracterFinal: .byte 0
valorEntero: .word 0
valorMedio: .float 0.0
valorGrande: .double 75.89
str_const_38: .asciz "\n--- NARROWING CASTING MANUAL ---"
format_string_37: .asciz "%s\n"
format_concat_37: .asciz "float %f -> double %f\n"
format_concat_36: .asciz "int %d -> float %f\n"
format_concat_35: .asciz "char %c -> int %d\n"
str_const_36: .asciz "Casting automático progresivo:"
format_string_35: .asciz "%s\n"
codigoDouble: .double 0.0
codigoFloat: .float 0.0
codigoAscii: .word 0
letraInicial: .byte 'M'
str_const_34: .asciz "\n--- WIDENING CASTING AUTOMATICO ---"
format_string_33: .asciz "%s\n"
format_concat_34: .asciz "Resultado condicional: %s\n"
str_lit_4: .asciz "Es null"
str_lit_3: .asciz "null"
resultado: .quad str_lit_resultado
str_lit_resultado: .asciz ""
otroTextoNulo: .quad str_lit_otroTextoNulo
str_lit_otroTextoNulo: .asciz "null"
format_concat_33: .asciz "textoNulo ahora: %s\n"
str_lit_2: .asciz "Valor asignado después de verificar null"
str_const_32: .asciz "La variable textoNulo es null"
format_string_31: .asciz "%s\n"
str_lit_1: .asciz "null"
str_const_30: .asciz "\n--- MANEJO DE NULL ---"
format_string_29: .asciz "%s\n"
textoNulo: .quad str_lit_textoNulo
str_lit_textoNulo: .asciz "null"
str_const_28: .asciz "Línea antes del salto compuesto\r\nLínea después del salto compuesto"
format_string_27: .asciz "%s\n"
str_const_26: .asciz "Tabulación:\t\tTexto con tab"
format_string_25: .asciz "%s\n"
str_const_24: .asciz "Retorno de carro:\rTexto con retorno"
format_string_23: .asciz "%s\n"
str_const_22: .asciz "Nueva línea:\nTexto en línea nueva"
format_string_21: .asciz "%s\n"
str_const_20: .asciz "Barra invertida: \\"
format_string_19: .asciz "%s\n"
str_const_18: .asciz "Comillas dobles: \"Hola mundo\""
format_string_17: .asciz "%s\n"
str_const_16: .asciz "\n--- SECUENCIAS DE ESCAPE ---"
format_string_15: .asciz "%s\n"
format_concat_32: .asciz "valor >> 2: %d\n"
format_concat_31: .asciz "valor << 2: %d\n"
format_concat_30: .asciz "valor = %d\n"
str_const_14: .asciz "\n--- DESPLAZAMIENTO DE BITS ---"
format_string_13: .asciz "%s\n"
valorDesplazamiento: .word 16
format_concat_29: .asciz "x ^ y (XOR): %d\n"
format_concat_28: .asciz "x | y (OR): %d\n"
format_concat_27: .asciz "x & y (AND): %d\n"
format_concat_26: .asciz "y = %d (binario: 1010)\n"
format_concat_25: .asciz "x = %d (binario: 1100)\n"
str_const_12: .asciz "\n--- OPERADORES DE BITS ---"
format_string_11: .asciz "%s\n"
y: .word 10
x: .word 12
format_concat_24: .asciz "!condicion2: %s\n"
format_concat_23: .asciz "!condicion1: %s\n"
format_concat_22: .asciz "condicion1 || condicion2: %s\n"
format_concat_21: .asciz "condicion1 && condicion2: %s\n"
format_concat_20: .asciz "condicion2 = %s\n"
format_concat_19: .asciz "condicion1 = %s\n"
str_const_10: .asciz "\n--- OPERACIONES LOGICAS ---"
format_string_9: .asciz "%s\n"
condicion2: .byte 0
condicion1: .byte 1
format_concat_18: .asciz "a != b: %d\n"
format_concat_17: .asciz "a == b: %d\n"
format_concat_16: .asciz "a <= b: %d\n"
format_concat_15: .asciz "a >= b: %d\n"
format_concat_14: .asciz "a < b: %d\n"
format_concat_13: .asciz "a > b: %d\n"
str_const_8: .asciz "\n--- OPERACIONES RELACIONALES ---"
format_string_7: .asciz "%s\n"
format_concat_12: .asciz "Módulo (a %% b): %d\n"
format_concat_11: .asciz "División (a / b): %d\n"
format_concat_10: .asciz "Multiplicación (a * b): %d\n"
format_concat_9: .asciz "Resta (a - b): %d\n"
format_concat_8: .asciz "Suma (a + b): %d\n"
format_concat_7: .asciz "a = %d, b = %d\n"
str_const_6: .asciz "\n--- OPERACIONES ARITMETICAS ---"
format_string_5: .asciz "%s\n"
b: .word 7
a: .word 25
format_concat_6: .asciz "Cadena: %s\n"
format_concat_5: .asciz "Booleano: %s\n"
format_concat_4: .asciz "Carácter: %c\n"
format_concat_3: .asciz "Flotante: %f\n"
format_concat_2: .asciz "Entero: %d\n"
str_const_4: .asciz "\n--- VALORES ASIGNADOS ---"
format_string_3: .asciz "%s\n"
str_lit_0: .asciz "Nuevo resultado"
float_const_1: .float 5.67
format_concat_1: .asciz "Constante máxima: %d\n"
format_concat_0: .asciz "Constante PI: %f\n"
format_string_2: .asciz "%s\n"
cadenaTexto: .quad str_lit_cadenaTexto
str_lit_cadenaTexto: .asciz "Resultado"
esVerdadero: .byte 1
caracter: .byte 'A'
numeroDoble: .double 2.718281828
numeroFlotante: .float 3.14
numeroEntero: .word 42
str_const_1: .asciz "=== SISTEMA DE CALCULO CIENTIFICO ==="
format_string_0: .asciz "%s\n"

// constantes
.section .rodata
valor10: .double 10.000000
valor2: .double 2.000000
MENSAJE_SISTEMA: .asciz "Calculadora Científica v1.0"
CONSTANTE_MAXIMA: .word 100
PI: .double 3.14159265359

// seccion bss
.bss
temp_str_valueof: .skip 32

// código ejecutable
.text

_start:
    // Punto de entrada del programa
    ldr x0, =format_string_0
    ldr x1, =str_const_1
    bl printf
    ldr x1, =MENSAJE_SISTEMA
    ldr x0, =format_string_2
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =PI
    ldr d0, [x1]
    fmov s1, w1
    ldr x0, =format_concat_0
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =CONSTANTE_MAXIMA
    ldr w1, [x1]
    mov w2, w1
    ldr x0, =format_concat_1
    bl printf
    mov w1, #100
    mov w1, w1
    ldr x2, =numeroEntero
    str w1, [x2]
    ldr x1, =float_const_1
    ldr s0, [x1]
    ldr x2, =numeroFlotante
    str s0, [x2]
    mov w1, #90
    mov w0, w1
    ldr x2, =caracter
    strb w0, [x2]
    mov w1, #0
    mov w1, w1
    ldr x2, =esVerdadero
    strb w1, [x2]
    ldr x2, =str_lit_0
    ldr x1, =cadenaTexto
    str x2, [x1]
    ldr x0, =format_string_3
    ldr x1, =str_const_4
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =numeroEntero
    ldr w1, [x1]
    mov w2, w1
    ldr x0, =format_concat_2
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =numeroFlotante
    ldr s0, [x1]
    fcvt d0, s0
    ldr x0, =format_concat_3
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =caracter
    ldrb w1, [x1]
    mov w1, w1
    ldr x0, =format_concat_4
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =esVerdadero
    ldrb w1, [x1]
    mov w0, w1
    ldr x1, =temp_str_valueof
    bl valueOf_bool
    ldr x1, =temp_str_valueof
    ldr x0, =format_concat_5
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =cadenaTexto
    ldr x1, [x1]
    ldr x0, =format_concat_6
    bl printf
    ldr x0, =format_string_5
    ldr x1, =str_const_6
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    mov w2, w1
    ldr x2, =b
    ldr w2, [x2]
    mov w3, w2
    ldr x0, =format_concat_7
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    add w1, w1, w2
    mov w4, w1
    ldr x0, =format_concat_8
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    sub w1, w1, w2
    mov w4, w1
    ldr x0, =format_concat_9
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    mul w1, w1, w2
    mov w4, w1
    ldr x0, =format_concat_10
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    sdiv w1, w1, w2
    mov w4, w1
    ldr x0, =format_concat_11
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    udiv w3, w1, w2
    msub w1, w3, w2, w1
    mov w4, w1
    ldr x0, =format_concat_12
    bl printf
    ldr x0, =format_string_7
    ldr x1, =str_const_8
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, gt
    mov w0, w1
    mov w4, w0
    ldr x0, =format_concat_13
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, lt
    mov w0, w1
    mov w4, w0
    ldr x0, =format_concat_14
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    mov w4, w0
    ldr x0, =format_concat_15
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, le
    mov w0, w1
    mov w4, w0
    ldr x0, =format_concat_16
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    mov w4, w0
    ldr x0, =format_concat_17
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, ne
    mov w0, w1
    mov w4, w0
    ldr x0, =format_concat_18
    bl printf
    ldr x0, =format_string_9
    ldr x1, =str_const_10
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =condicion1
    ldrb w1, [x1]
    mov w0, w1
    ldr x1, =temp_str_valueof
    bl valueOf_bool
    ldr x1, =temp_str_valueof
    ldr x0, =format_concat_19
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =condicion2
    ldrb w1, [x1]
    mov w0, w1
    ldr x1, =temp_str_valueof
    bl valueOf_bool
    ldr x1, =temp_str_valueof
    ldr x0, =format_concat_20
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =condicion1
    ldrb w1, [x1]
    ldr x2, =condicion2
    ldrb w2, [x2]
    and w1, w1, w2
    cmp w1, #0
    cset w1, ne
    mov w0, w1
    ldr x1, =temp_str_valueof
    bl valueOf_bool
    ldr x1, =temp_str_valueof
    ldr x0, =format_concat_21
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =condicion1
    ldrb w1, [x1]
    ldr x2, =condicion2
    ldrb w2, [x2]
    orr w1, w1, w2
    cmp w1, #0
    cset w1, ne
    mov w0, w1
    ldr x1, =temp_str_valueof
    bl valueOf_bool
    ldr x1, =temp_str_valueof
    ldr x0, =format_concat_22
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =condicion1
    ldrb w1, [x1]
    ldr x2, =condicion1
    ldrb w2, [x2]
    cmp w2, #0
    cset w3, eq
    mov w0, w3
    ldr x1, =temp_str_valueof
    bl valueOf_bool
    ldr x1, =temp_str_valueof
    ldr x0, =format_concat_23
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =condicion2
    ldrb w1, [x1]
    ldr x2, =condicion2
    ldrb w2, [x2]
    cmp w2, #0
    cset w3, eq
    mov w0, w3
    ldr x1, =temp_str_valueof
    bl valueOf_bool
    ldr x1, =temp_str_valueof
    ldr x0, =format_concat_24
    bl printf
    ldr x0, =format_string_11
    ldr x1, =str_const_12
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =x
    ldr w1, [x1]
    mov w2, w1
    ldr x0, =format_concat_25
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =y
    ldr w1, [x1]
    mov w2, w1
    ldr x0, =format_concat_26
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =x
    ldr w1, [x1]
    ldr x2, =y
    ldr w2, [x2]
    and w1, w1, w2
    mov w4, w1
    ldr x0, =format_concat_27
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =x
    ldr w1, [x1]
    ldr x2, =y
    ldr w2, [x2]
    orr w1, w1, w2
    mov w4, w1
    ldr x0, =format_concat_28
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =x
    ldr w1, [x1]
    ldr x2, =y
    ldr w2, [x2]
    eor w1, w1, w2
    mov w4, w1
    ldr x0, =format_concat_29
    bl printf
    ldr x0, =format_string_13
    ldr x1, =str_const_14
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =valorDesplazamiento
    ldr w1, [x1]
    mov w2, w1
    ldr x0, =format_concat_30
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =valorDesplazamiento
    ldr w1, [x1]
    mov w2, #2
    lsl w1, w1, w2
    mov w4, w1
    ldr x0, =format_concat_31
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =valorDesplazamiento
    ldr w1, [x1]
    mov w2, #2
    lsr w1, w1, w2
    mov w4, w1
    ldr x0, =format_concat_32
    bl printf
    ldr x0, =format_string_15
    ldr x1, =str_const_16
    bl printf
    ldr x0, =format_string_17
    ldr x1, =str_const_18
    bl printf
    ldr x0, =format_string_19
    ldr x1, =str_const_20
    bl printf
    ldr x0, =format_string_21
    ldr x1, =str_const_22
    bl printf
    ldr x0, =format_string_23
    ldr x1, =str_const_24
    bl printf
    ldr x0, =format_string_25
    ldr x1, =str_const_26
    bl printf
    ldr x0, =format_string_27
    ldr x1, =str_const_28
    bl printf
    ldr x0, =format_string_29
    ldr x1, =str_const_30
    bl printf
if_0:
    // Evaluación de condición IF
    ldr x1, =textoNulo
    ldr x1, [x1]
    ldr x0, =textoNulo
    ldr x0, [x0]
    ldr x0, =textoNulo
    ldr x0, [x0]
    bl strcmp
    cmp w0, #0
    cset w0, eq
    // Saltar si condición es falsa
    cbz w0, if_end_0

    ldr x0, =format_string_31
    ldr x1, =str_const_32
    bl printf
    ldr x2, =str_lit_2
    ldr x1, =textoNulo
    str x2, [x1]
if_end_0:
    // Printf optimizado para concatenación
    ldr x1, =textoNulo
    ldr x1, [x1]
    ldr x0, =format_concat_33
    bl printf
    // Evaluación de condición IF
    ldr x1, =otroTextoNulo
    ldr x1, [x1]
    ldr x0, =otroTextoNulo
    ldr x0, [x0]
    ldr x0, =otroTextoNulo
    ldr x0, [x0]
    bl strcmp
    cmp w0, #0
    cset w0, eq
    // Saltar a else si condición es falsa
    cbz w0, if_else_1

    ldr x2, =str_lit_4
    ldr x1, =resultado
    str x2, [x1]
    b if_end_1

if_else_1:
    ldr x1, =otroTextoNulo
    ldr x1, [x1]
    ldr x2, =otroTextoNulo
    ldr x2, [x2]
    ldr x1, =resultado
    str x2, [x1]
if_end_1:
    // Printf optimizado para concatenación
    ldr x1, =resultado
    ldr x1, [x1]
    ldr x0, =format_concat_34
    bl printf
    ldr x0, =format_string_33
    ldr x1, =str_const_34
    bl printf
    // Widening: char -> int
    ldr x1, =letraInicial
    ldrb w0, [x1]
    ldr x1, =codigoAscii
    str w0, [x1]
    // Widening: int -> float
    ldr x1, =codigoAscii
    ldr w0, [x1]
    scvtf s0, w0
    ldr x1, =codigoFloat
    str s0, [x1]
    // Widening: float -> double
    ldr x1, =codigoFloat
    ldr s0, [x1]
    fcvt d0, s0
    ldr x1, =codigoDouble
    str d0, [x1]
    ldr x0, =format_string_35
    ldr x1, =str_const_36
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =letraInicial
    ldrb w1, [x1]
    ldr x2, =letraInicial
    ldrb w2, [x2]
    mov w3, w2
    ldr x4, =codigoAscii
    ldr w4, [x4]
    mov w5, w4
    ldr x0, =format_concat_35
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =codigoAscii
    ldr w1, [x1]
    mov w2, w1
    ldr x2, =codigoFloat
    ldr s1, [x2]
    fcvt d1, s1
    ldr x0, =format_concat_36
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =codigoFloat
    ldr s0, [x1]
    fcvt d0, s0
    ldr x2, =codigoDouble
    ldr d1, [x2]
    fmov d2, d0
    ldr x0, =format_concat_37
    bl printf
    ldr x0, =format_string_37
    ldr x1, =str_const_38
    bl printf
    // Narrowing: double -> float
    ldr x1, =valorGrande
    ldr d0, [x1]
    fcvt s0, d0
    ldr x1, =valorMedio
    str s0, [x1]
    // Narrowing: float -> int
    ldr x1, =valorMedio
    ldr s0, [x1]
    fcvtzs w0, s0
    ldr x1, =valorEntero
    str w0, [x1]
    // Narrowing: int -> char
    ldr x1, =valorEntero
    ldr w0, [x1]
    ldr x1, =caracterFinal
    strb w0, [x1]
    ldr x0, =format_string_39
    ldr x1, =str_const_40
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =valorGrande
    ldr d0, [x1]
    fmov d1, d0
    ldr x2, =valorMedio
    ldr s1, [x2]
    fcvt d1, s1
    ldr x0, =format_concat_38
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =valorMedio
    ldr s0, [x1]
    fcvt d0, s0
    ldr x2, =valorEntero
    ldr w2, [x2]
    mov w3, w2
    ldr x0, =format_concat_39
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =valorEntero
    ldr w1, [x1]
    mov w2, w1
    ldr x2, =caracterFinal
    ldrb w2, [x2]
    ldr x3, =caracterFinal
    ldrb w3, [x3]
    mov w4, w3
    ldr x0, =format_concat_40
    bl printf
    // Operación: resultadoFinal = <expresión>
    ldr x1, =PI
    ldr d1, [x1]
    ldr x2, =valorEntero
    ldr w2, [x2]
    scvtf d2, w2
    ldr x3, =codigoAscii
    ldr w3, [x3]
    scvtf d3, w3
    fadd d0, d2, d3
    fmov d2, d0
    fmul d0, d1, d2
    fmov d1, d0
    ldr x6, =valor2
    ldr d6, [x6]
    fdiv d0, d1, d6
    ldr x1, =resultadoFinal
    str d0, [x1]
    ldr x0, =format_string_41
    ldr x1, =str_const_42
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =resultadoFinal
    ldr d0, [x1]
    fmov d1, d0
    ldr x0, =format_concat_41
    bl printf
    // Operación: radioCirculoCm = <expresión>
    ldr x1, =valorEntero
    ldr w1, [x1]
    scvtf d1, w1
    ldr x2, =codigoAscii
    ldr w2, [x2]
    scvtf d2, w2
    fadd d0, d1, d2
    fmov d1, d0
    ldr x4, =valor10
    ldr d4, [x4]
    fdiv d0, d1, d4
    ldr x1, =radioCirculoCm
    str d0, [x1]
    // Operación: areaCirculoCm2 = <expresión>
    ldr x1, =PI
    ldr d1, [x1]
    ldr x2, =radioCirculoCm
    ldr d2, [x2]
    fmul d0, d1, d2
    fmov d1, d0
    ldr x4, =radioCirculoCm
    ldr d4, [x4]
    fmul d0, d1, d4
    ldr x1, =areaCirculoCm2
    str d0, [x1]
    // Operación: perimetroCirculoCm = <expresión>
    mov w1, #2
    scvtf d1, w1
    ldr x2, =PI
    ldr d2, [x2]
    fmul d0, d1, d2
    fmov d1, d0
    ldr x4, =radioCirculoCm
    ldr d4, [x4]
    fmul d0, d1, d4
    ldr x1, =perimetroCirculoCm
    str d0, [x1]
    // Widening: int -> double
    ldr x1, =a
    ldr w0, [x1]
    scvtf d0, w0
    ldr x1, =baseRectCm
    str d0, [x1]
    // Widening: int -> double
    ldr x1, =b
    ldr w0, [x1]
    scvtf d0, w0
    ldr x1, =alturaRectCm
    str d0, [x1]
    // Operación: areaRectCm2 = <expresión>
    ldr x2, =baseRectCm
    ldr d0, [x2]
    ldr x2, =alturaRectCm
    ldr d1, [x2]
    fmul d0, d0, d1
    ldr x1, =areaRectCm2
    str d0, [x1]
    // Operación: perimetroRectCm = <expresión>
    mov w1, #2
    scvtf d1, w1
    ldr x2, =baseRectCm
    ldr d2, [x2]
    ldr x3, =alturaRectCm
    ldr d3, [x3]
    fadd d0, d2, d3
    fmov d2, d0
    fmul d0, d1, d2
    ldr x1, =perimetroRectCm
    str d0, [x1]
    ldr x0, =format_string_43
    ldr x1, =str_const_44
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =radioCirculoCm
    ldr d0, [x1]
    fmov s1, w1
    ldr x2, =areaCirculoCm2
    ldr d1, [x2]
    fmov s2, w2
    ldr x3, =perimetroCirculoCm
    ldr d2, [x3]
    fmov s3, w3
    ldr x0, =format_concat_42
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =baseRectCm
    ldr d0, [x1]
    fmov s1, w1
    ldr x2, =alturaRectCm
    ldr d1, [x2]
    fmov s2, w2
    ldr x3, =areaRectCm2
    ldr d2, [x3]
    fmov s3, w3
    ldr x4, =perimetroRectCm
    ldr d3, [x4]
    fmov s4, w4
    ldr x0, =format_concat_43
    bl printf
    ldr x0, =format_string_45
    ldr x1, =str_const_46
    bl printf
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
