// Código ARM64 generado por JavaLang Compiler
.global _start


// declaraciones de constantes
.section .rodata
s: .word 8

// código ejecutable
.text

_start:
    // Punto de entrada del programa
    // Cargar s en registro x9
    adr x1, s
    ldr x9, [x1]
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
