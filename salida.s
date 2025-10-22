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
format_concat_0: .asciz "%d x %d = %d\n"
resultado: .word 0
j: .word 1
i: .word 1
str_const_1: .asciz "\nTabla de multiplicar (1-3 x 1-4):"
format_string_0: .asciz "%s\n"

// constantes
.section .rodata

// seccion bss
.bss

// código ejecutable
.text

_start:
    // Punto de entrada del programa
    ldr x0, =format_string_0
    ldr x1, =str_const_1
    bl printf
for_loop_0:
    // Reinicializar variable del bucle interno: j = 1
    mov w0, #1
    ldr x1, =j
    str w0, [x1]

    ldr x1, =i
    ldr w1, [x1]
    mov w2, #3
    cmp w1, w2
    cset w1, le
    mov w0, w1
    // Saltar al final si la condición es falsa
    cbz w0, for_end_0

for_loop_1:
    ldr x1, =j
    ldr w1, [x1]
    mov w2, #4
    cmp w1, w2
    cset w1, le
    mov w0, w1
    // Saltar al final si la condición es falsa
    cbz w0, for_end_1

    // Operación: resultado = <expresión>
    ldr x2, =i
    ldr w0, [x2]
    ldr x2, =j
    ldr w1, [x2]
    mul w0, w0, w1
    ldr x1, =resultado
    str w0, [x1]
    // Printf optimizado para concatenación
    ldr x1, =i
    ldr w1, [x1]
    mov w2, w1
    ldr x2, =j
    ldr w2, [x2]
    mov w3, w2
    ldr x3, =resultado
    ldr w3, [x3]
    mov w4, w3
    ldr x0, =format_concat_0
    bl printf
for_inc_1:
    // Incremento: j++
    ldr x1, =j
    ldr w0, [x1]
    add w0, w0, #1
    str w0, [x1]
    // Volver al inicio del bucle
    b for_loop_1
for_end_1:
for_inc_0:
    // Incremento: i++
    ldr x1, =i
    ldr w0, [x1]
    add w0, w0, #1
    str w0, [x1]
    // Volver al inicio del bucle
    b for_loop_0
for_end_0:
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
