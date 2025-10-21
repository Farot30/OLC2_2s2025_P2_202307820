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
format_concat_32: .asciz "textoNulo ahora: %s\n"
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
format_concat_31: .asciz "valor >> 2: %d\n"
format_concat_30: .asciz "valor << 2: %d\n"
format_concat_29: .asciz "valor = %d\n"
str_const_14: .asciz "\n--- DESPLAZAMIENTO DE BITS ---"
format_string_13: .asciz "%s\n"
valorDesplazamiento: .word 16
format_concat_28: .asciz "x ^ y (XOR): %d\n"
format_concat_27: .asciz "x | y (OR): %d\n"
format_concat_26: .asciz "x & y (AND): %d\n"
format_concat_25: .asciz "y = %d (binario: 1010)\n"
format_concat_24: .asciz "x = %d (binario: 1100)\n"
str_const_12: .asciz "\n--- OPERADORES DE BITS ---"
format_string_11: .asciz "%s\n"
y: .word 10
x: .word 12
format_concat_23: .asciz "!condicion2: %s\n"
format_concat_22: .asciz "!condicion1: %s\n"
format_concat_21: .asciz "condicion1 || condicion2: %s\n"
format_concat_20: .asciz "condicion1 && condicion2: %s\n"
format_concat_19: .asciz "condicion2 = %s\n"
format_concat_18: .asciz "condicion1 = %s\n"
str_const_10: .asciz "\n--- OPERACIONES LOGICAS ---"
format_string_9: .asciz "%s\n"
condicion2: .byte 0
condicion1: .byte 1
format_concat_17: .asciz "a != b: %d\n"
format_concat_16: .asciz "a == b: %d\n"
format_concat_15: .asciz "a <= b: %d\n"
format_concat_14: .asciz "a >= b: %d\n"
format_concat_13: .asciz "a < b: %d\n"
format_concat_12: .asciz "a > b: %d\n"
str_const_8: .asciz "\n--- OPERACIONES RELACIONALES ---"
format_string_7: .asciz "%s\n"
format_concat_11: .asciz "Módulo (a %% b): %d\n"
format_concat_10: .asciz "División (a / b): %d\n"
format_concat_9: .asciz "Multiplicación (a * b): %d\n"
format_concat_8: .asciz "Resta (a - b): %d\n"
format_concat_7: .asciz "Suma (a + b): %d\n"
format_concat_6: .asciz "a = %d, b = %d\n"
str_const_6: .asciz "\n--- OPERACIONES ARITMETICAS ---"
format_string_5: .asciz "%s\n"
b: .word 7
a: .word 25
format_concat_5: .asciz "Cadena: %s\n"
format_concat_4: .asciz "Booleano: %s\n"
format_concat_3: .asciz "Carácter: %c\n"
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
    mov w1, w1
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
    mov w1, w1
    ldr x0, =format_concat_2
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =caracter
    ldrb w1, [x1]
    mov w1, w1
    ldr x0, =format_concat_3
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =esVerdadero
    ldrb w1, [x1]
    mov w0, w1
    ldr x1, =temp_str_valueof
    bl valueOf_bool
    ldr x1, =temp_str_valueof
    ldr x0, =format_concat_4
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =cadenaTexto
    ldr x1, [x1]
    ldr x0, =format_concat_5
    bl printf
    ldr x0, =format_string_5
    ldr x1, =str_const_6
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    mov w1, w1
    ldr x2, =b
    ldr w2, [x2]
    mov w1, w2
    ldr x0, =format_concat_6
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    add w1, w1, w2
    mov w1, w1
    ldr x0, =format_concat_7
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    sub w1, w1, w2
    mov w1, w1
    ldr x0, =format_concat_8
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    mul w1, w1, w2
    mov w1, w1
    ldr x0, =format_concat_9
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    sdiv w1, w1, w2
    mov w1, w1
    ldr x0, =format_concat_10
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    udiv w3, w1, w2
    msub w1, w3, w2, w1
    mov w1, w1
    ldr x0, =format_concat_11
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
    mov w1, w1
    ldr x0, =format_concat_12
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, lt
    mov w1, w1
    ldr x0, =format_concat_13
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, ge
    mov w1, w1
    ldr x0, =format_concat_14
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, le
    mov w1, w1
    ldr x0, =format_concat_15
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, eq
    mov w1, w1
    ldr x0, =format_concat_16
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =a
    ldr w1, [x1]
    ldr x2, =b
    ldr w2, [x2]
    cmp w1, w2
    cset w1, ne
    mov w1, w1
    ldr x0, =format_concat_17
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
    ldr x0, =format_concat_18
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =condicion2
    ldrb w1, [x1]
    mov w0, w1
    ldr x1, =temp_str_valueof
    bl valueOf_bool
    ldr x1, =temp_str_valueof
    ldr x0, =format_concat_19
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
    ldr x0, =format_concat_20
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
    ldr x0, =format_concat_21
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
    ldr x0, =format_concat_22
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
    ldr x0, =format_concat_23
    bl printf
    ldr x0, =format_string_11
    ldr x1, =str_const_12
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =x
    ldr w1, [x1]
    mov w1, w1
    ldr x0, =format_concat_24
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =y
    ldr w1, [x1]
    mov w1, w1
    ldr x0, =format_concat_25
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =x
    ldr w1, [x1]
    ldr x2, =y
    ldr w2, [x2]
    and w1, w1, w2
    mov w1, w1
    ldr x0, =format_concat_26
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =x
    ldr w1, [x1]
    ldr x2, =y
    ldr w2, [x2]
    orr w1, w1, w2
    mov w1, w1
    ldr x0, =format_concat_27
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =x
    ldr w1, [x1]
    ldr x2, =y
    ldr w2, [x2]
    eor w1, w1, w2
    mov w1, w1
    ldr x0, =format_concat_28
    bl printf
    ldr x0, =format_string_13
    ldr x1, =str_const_14
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =valorDesplazamiento
    ldr w1, [x1]
    mov w1, w1
    ldr x0, =format_concat_29
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =valorDesplazamiento
    ldr w1, [x1]
    mov w2, #2
    lsl w1, w1, w2
    mov w1, w1
    ldr x0, =format_concat_30
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =valorDesplazamiento
    ldr w1, [x1]
    mov w2, #2
    lsr w1, w1, w2
    mov w1, w1
    ldr x0, =format_concat_31
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
if_5:
    // Evaluación de condición IF
    // Saltar si condición es falsa
    cmp w0, #0
    beq if_end_5

    ldr x0, =format_string_31
    ldr x1, =str_const_32
    bl printf
    ldr x2, =str_lit_2
    ldr x1, =textoNulo
    str x2, [x1]
if_end_5:
    // Printf optimizado para concatenación
    ldr x1, =textoNulo
    ldr x1, [x1]
    ldr x0, =format_concat_32
    bl printf
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
