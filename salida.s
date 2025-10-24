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
format_concat_0: .asciz "Nota %d: %d\n"
nota: .word 0
contadorNotas: .word 0
suma: .word 0
str_const_3: .asciz "Procesando notas con forEach:"
format_string_2: .asciz "%s\n"
notasParaForeach: .word 85, 92, 78, 95, 88
str_const_1: .asciz "\n--- FOREACH CON ARRAY SIMPLE ---"
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
    ldr x0, =format_string_2
    ldr x1, =str_const_3
    bl printf
    // For-each: int nota : notasParaForeach
foreach_init_0:
    mov w19, #0                // Índice inicial = 0
    mov w20, #5               // Tamaño del vector
    ldr x21, =notasParaForeach               // Dirección base del vector

foreach_loop_0:
    // Verificar si hemos terminado
    cmp w19, w20
    bge foreach_end_0                     // Si índice >= tamaño, salir

    // Cargar elemento actual: nota = notasParaForeach[índice]
    ldr w22, [x21, w19, SXTW #2]  // Cargar elemento
    ldr x23, =nota
    str w22, [x23]              // Almacenar en variable iteradora

    // Incremento: contadorNotas++
    ldr x1, =contadorNotas
    ldr w0, [x1]
    add w0, w0, #1
    str w0, [x1]
    // Asignación compuesta: suma += <valor>
    ldr x1, =suma
    ldr w0, [x1]
    ldr x2, =nota
    ldr w1, [x2]
    add w0, w0, w1
    ldr x1, =suma
    str w0, [x1]
    // Printf optimizado para concatenación
    ldr x1, =contadorNotas
    ldr w1, [x1]
    mov w2, w1
    ldr x2, =nota
    ldr w2, [x2]
    mov w3, w2
    ldr x0, =format_concat_0
    bl printf
    // Incrementar índice y continuar
    add w19, w19, #1
    b foreach_loop_0

foreach_end_0:
    // Fin del for-each

    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
