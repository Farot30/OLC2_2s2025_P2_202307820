.global _start
.extern itoa
.extern printf


// declaraciones de variables
.data
str_false: .asciz "false"
str_true: .asciz "true"
format_concat_4: .asciz "Estado: %s\n"
activo: .byte 1
format_concat_3: .asciz "Letra: %c\n"
letra: .byte 'A'
format_concat_2: .asciz "Precio: $%f\n"
precio: .double 15.99
format_concat_1: .asciz "hola%dmano\n"
format_concat_0: .asciz "Resultado: %d\n"
total: .word 8

// constantes
.section .rodata

// seccion bss
.bss

// código ejecutable
.text

_start:
    // Punto de entrada del programa
    // Printf optimizado para concatenación
    ldr x1, =total
    ldr w1, [x1]
    ldr x0, =format_concat_0
    bl printf
    mov w0, #45
    ldr x1, =total
    str w0, [x1]
    // Printf optimizado para concatenación
    ldr x1, =total
    ldr w1, [x1]
    ldr x0, =format_concat_1
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =precio
    ldr d0, [x1]
    ldr x0, =format_concat_2
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =letra
    ldrb w1, [x1]
    ldr x0, =format_concat_3
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =activo
    ldrb w1, [x1]
    cmp w1, #0
    ldr x1, =str_false
    ldr x19, =str_true
    csel x1, x19, x1, ne
    ldr x0, =format_concat_4
    bl printf
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
