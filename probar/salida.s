// Código ARM64 generado por JavaLang Compiler
.global _start
.extern parse_int
.extern parse_double
.extern parse_float
.extern valuesOf_int
.extern valueOf_bool
.extern valueOf_float
.extern valueOf_double
.extern valueOf_long
.extern printf


// declaraciones de variables
.data
format_int_0: .asciz "%d\n"
i: .word 0

// constantes
.section .rodata

// seccion bss
.bss

// código ejecutable
.text

_start:
    // Punto de entrada del programa
    // Do-While loop 1
do_while_start_1:
    ldr x1, =i
    ldr w1, [x1]
    ldr x0, =format_int_0
    bl printf
    // Incremento: i++
    ldr x1, =i
    ldr w0, [x1]
    add w0, w0, #1
    str w0, [x1]
do_while_cond_1:
    ldr x1, =i
    ldr w1, [x1]
    mov w2, #5
    cmp w1, w2
    cset w1, lt
    mov w0, w1
    // Saltar al inicio si condición es verdadera
    cmp w0, #1
    beq do_while_start_1
do_while_end_1:
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
