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
format_concat_0: .asciz "Constante PI: %f\n"

// constantes
.section .rodata
PI: .double 3.14159265359

// seccion bss
.bss

// código ejecutable
.text

_start:
    // Punto de entrada del programa
    // Printf optimizado para concatenación
    ldr x1, =PI
    ldr d0, [x1]
    ldr x0, =format_concat_0
    bl printf
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
