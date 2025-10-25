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
format_concat_2: .asciz "Parseando '%s' a entero: %d\n"
format_concat_1: .asciz "Parseando '%s' a entero: %d\n"
format_concat_0: .asciz "Parseando '%s' a entero: %d\n"
num3: .word 0
num2: .word 0
num1: .word 0
numeroTexto3: .quad str_lit_numeroTexto3
str_lit_numeroTexto3: .asciz "789"
numeroTexto2: .quad str_lit_numeroTexto2
str_lit_numeroTexto2: .asciz "456"
numeroTexto1: .quad str_lit_numeroTexto1
str_lit_numeroTexto1: .asciz "123"

// constantes
.section .rodata

// seccion bss
.bss

// código ejecutable
.text

_start:
    // Punto de entrada del programa
    ldr x1, =numeroTexto1
    ldr x1, [x1]
    // Integer.parseInt(string)
    ldr x2, =numeroTexto1
    ldr x0, [x2]              // Cargar puntero al string
    bl parse_int               // Llamar parse_int custom (retorna en w0)
    ldr x1, =num1
    str w0, [x1]
    ldr x1, =numeroTexto2
    ldr x1, [x1]
    // Integer.parseInt(string)
    ldr x2, =numeroTexto2
    ldr x0, [x2]              // Cargar puntero al string
    bl parse_int               // Llamar parse_int custom (retorna en w0)
    ldr x1, =num2
    str w0, [x1]
    ldr x1, =numeroTexto3
    ldr x1, [x1]
    // Integer.parseInt(string)
    ldr x2, =numeroTexto3
    ldr x0, [x2]              // Cargar puntero al string
    bl parse_int               // Llamar parse_int custom (retorna en w0)
    ldr x1, =num3
    str w0, [x1]
    // Printf optimizado para concatenación
    ldr x1, =numeroTexto1
    ldr x1, [x1]
    ldr x2, =num1
    ldr w2, [x2]
    mov w3, w2
    ldr x0, =format_concat_0
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =numeroTexto2
    ldr x1, [x1]
    ldr x2, =num2
    ldr w2, [x2]
    mov w3, w2
    ldr x0, =format_concat_1
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =numeroTexto3
    ldr x1, [x1]
    ldr x2, =num3
    ldr w2, [x2]
    mov w3, w2
    ldr x0, =format_concat_2
    bl printf
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
