// Código ARM64 generado por JavaLang Compiler
.global _start


// declaraciones de variables
.data
result: .byte 0

// constantes
.section .rodata

// seccion bss
.bss

// código ejecutable
.text

_start:
    // Punto de entrada del programa
    // Operación: result = <expresión>
    mov w5, #5
    mov w6, #3
    cmp w5, w6
    cset w0, gt
    mov w3, w0
    mov w7, #2
    mov w8, #4
    cmp w7, w8
    cset w0, lt
    mov w4, w0
    and w0, w3, w4
    cmp w0, #0
    cset w0, ne
    mov w1, w0
    mov w9, #1
    mov w10, #1
    cmp w9, w10
    cset w0, eq
    mov w2, w0
    orr w0, w1, w2
    cmp w0, #0
    cset w0, ne
    ldr x1, =result
    strb w0, [x1]
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
