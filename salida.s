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
format_string_0: .asciz "%s\n"
str_const_1: .asciz "=== SISTEMA DE ANALISIS DE DATOS ACADEMICOS ==="
format_string_2: .asciz "%s\n"
str_const_3: .asciz "\n--- DECLARACION DE MATRICES 2D ---"
calificacionesPorMateria: .word 85, 92, 78, 90, 90, 88, 85, 93, 78, 82, 90, 87, 95, 91, 88, 94
matrizA: .word 3, 2, 1, 1, 4, 2, 2, 1, 3
matrizB: .word 2, 1, 3, 3, 2, 1, 1, 3, 2
format_string_4: .asciz "%s\n"
str_const_5: .asciz "Matrices 2D declaradas exitosamente:"
format_concat_0: .asciz "- calificacionesPorMateria: %d x %d\n"
format_concat_1: .asciz "- matrizA: %d x %d\n"
format_concat_2: .asciz "- matrizB: %d x %d\n"
format_string_6: .asciz "%s\n"
str_const_7: .asciz "\n--- MODIFICACION DE ELEMENTOS 2D ---"
format_concat_3: .asciz "Calificación original del Estudiante 1 en Física: %d\n"
format_concat_4: .asciz "Nueva calificación del Estudiante 1 en Física: %d\n"

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
    ldr x0, =format_string_2
    ldr x1, =str_const_3
    bl printf
    ldr x0, =format_string_4
    ldr x1, =str_const_5
    bl printf
    // Printf optimizado para concatenación
    // Array.length: calificacionesPorMateria.length = 4 (filas)
    mov w1, #4
    mov w3, w1
    mov w1, w3
    // Array.length: calificacionesPorMateria[i].length = 4 (columnas)
    mov w2, #4
    mov w3, w2
    mov w2, w3
    ldr x0, =format_concat_0
    bl printf
    // Printf optimizado para concatenación
    // Array.length: matrizA.length = 3 (filas)
    mov w1, #3
    mov w3, w1
    mov w1, w3
    // Array.length: matrizA[i].length = 3 (columnas)
    mov w2, #3
    mov w3, w2
    mov w2, w3
    ldr x0, =format_concat_1
    bl printf
    // Printf optimizado para concatenación
    // Array.length: matrizB.length = 3 (filas)
    mov w1, #3
    mov w3, w1
    mov w1, w3
    // Array.length: matrizB[i].length = 3 (columnas)
    mov w2, #3
    mov w3, w2
    mov w2, w3
    ldr x0, =format_concat_2
    bl printf
    ldr x0, =format_string_6
    ldr x1, =str_const_7
    bl printf
    // Printf optimizado para concatenación
    // Acceso a matriz calificacionesPorMateria[fila][columna]
    mov w1, #0
    mov w2, #1
    // Calcular offset: (fila * 4 + columna) * 4
    mov w4, #4
    mul w3, w1, w4
    add w3, w3, w2
    mov w4, #4
    mul w3, w3, w4
    ldr x5, =calificacionesPorMateria
    add x5, x5, w3, sxtw
    ldr w5, [x5]
    mov w3, w5
    mov w1, w3
    ldr x0, =format_concat_3
    bl printf
    // Escritura en matriz: calificacionesPorMateria[fila][columna] = valor
    mov w10, #0
    mov w11, #1
    // Calcular offset: (fila * 4 + columna) * 4
    mov w13, #4
    mul w12, w10, w13
    add w12, w12, w11
    mov w13, #4
    mul w12, w12, w13
    mov w14, #95
    ldr x15, =calificacionesPorMateria
    add x15, x15, w12, sxtw
    str w14, [x15]
    // Escritura en matriz: calificacionesPorMateria[fila][columna] = valor
    mov w10, #2
    mov w11, #0
    // Calcular offset: (fila * 4 + columna) * 4
    mov w13, #4
    mul w12, w10, w13
    add w12, w12, w11
    mov w13, #4
    mul w12, w12, w13
    mov w14, #82
    ldr x15, =calificacionesPorMateria
    add x15, x15, w12, sxtw
    str w14, [x15]
    // Escritura en matriz: calificacionesPorMateria[fila][columna] = valor
    mov w10, #3
    mov w11, #2
    // Calcular offset: (fila * 4 + columna) * 4
    mov w13, #4
    mul w12, w10, w13
    add w12, w12, w11
    mov w13, #4
    mul w12, w12, w13
    mov w14, #90
    ldr x15, =calificacionesPorMateria
    add x15, x15, w12, sxtw
    str w14, [x15]
    // Printf optimizado para concatenación
    // Acceso a matriz calificacionesPorMateria[fila][columna]
    mov w1, #0
    mov w2, #1
    // Calcular offset: (fila * 4 + columna) * 4
    mov w4, #4
    mul w3, w1, w4
    add w3, w3, w2
    mov w4, #4
    mul w3, w3, w4
    ldr x5, =calificacionesPorMateria
    add x5, x5, w3, sxtw
    ldr w5, [x5]
    mov w3, w5
    mov w1, w3
    ldr x0, =format_concat_4
    bl printf
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
