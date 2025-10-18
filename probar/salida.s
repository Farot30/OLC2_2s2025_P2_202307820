.global _start
.extern itoa
.extern printf


// declaraciones de variables
.data
format_string_1: .asciz "%s\n"
format_string_0: .asciz "%s\n"
str2: .asciz "panqueues "
peque: .skip 8
pan: .asciz "nose "
str1: .asciz "macaco "
str0: .asciz "chetos "
hola: .skip 8

// constantes
.section .rodata

// seccion bss
.bss
    .align 3
    sb_buffer:      .skip 8     // Puntero al buffer
    sb_capacity:    .skip 8     // Capacidad total
    sb_length:      .skip 8     // Longitud actual


// código ejecutable
.text

_start:
    // Punto de entrada del programa
    // Operación: hola = <expresión>
    // Concatenación de strings usando StringBuilder
    mov x0, #64
    bl sb_init
    ldr x0, =str0
    bl sb_append
    ldr x0, =str1
    bl sb_append

    // Obtener resultado final
    bl sb_get_string
    mov x19, x0                 // x19 tiene el string concatenado

    // Calcular longitud del string
    mov x0, x19
    bl strlen
    add x0, x0, #1              // +1 para null terminator
    mov x20, x0                 // x20 = longitud

    // Reservar memoria permanente
    bl sb_malloc
    mov x21, x0                 // x21 = nueva memoria

    // Copiar string a memoria permanente
    mov x0, x21                 // destino
    mov x1, x19                 // origen
    bl strcpy

    // Limpiar StringBuilder
    bl sb_free

    // Guardar puntero permanente
    ldr x1, =hola
    str x21, [x1]               // Guardar puntero permanente
    // Operación: peque = <expresión>
    // Concatenación de strings usando StringBuilder
    mov x0, #64
    bl sb_init
    ldr x0, =hola
    ldr x0, [x0]
    bl sb_append
    ldr x0, =str2
    bl sb_append
    ldr x0, =pan
    bl sb_append

    // Obtener resultado final
    bl sb_get_string
    mov x19, x0                 // x19 tiene el string concatenado

    // Calcular longitud del string
    mov x0, x19
    bl strlen
    add x0, x0, #1              // +1 para null terminator
    mov x20, x0                 // x20 = longitud

    // Reservar memoria permanente
    bl sb_malloc
    mov x21, x0                 // x21 = nueva memoria

    // Copiar string a memoria permanente
    mov x0, x21                 // destino
    mov x1, x19                 // origen
    bl strcpy

    // Limpiar StringBuilder
    bl sb_free

    // Guardar puntero permanente
    ldr x1, =peque
    str x21, [x1]               // Guardar puntero permanente
    ldr x1, =peque
    ldr x1, [x1]
    ldr x0, =format_string_0
    bl printf
    ldr x1, =pan
    ldr x0, =format_string_1
    bl printf
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0

// ============================================
// FUNCIONES STRINGBUILDER
// ============================================

// Inicializar StringBuilder
sb_init:
    stp x29, x30, [sp, #-16]!
    mov x19, x0                 // guardar capacidad
    
    // Reservar memoria
    bl sb_malloc
    ldr x1, =sb_buffer
    str x0, [x1]
    
    // Guardar capacidad
    ldr x1, =sb_capacity
    str x19, [x1]
    
    // Longitud inicial = 0
    ldr x1, =sb_length
    str xzr, [x1]
    
    // Poner null terminator
    ldr x1, =sb_buffer
    ldr x1, [x1]
    strb wzr, [x1]
    
    ldp x29, x30, [sp], #16
    ret

// Agregar string al StringBuilder
sb_append:
    stp x29, x30, [sp, #-32]!
    str x19, [sp, #16]
    str x20, [sp, #24]
    
    mov x19, x0                 // guardar string a agregar
    
    // Calcular longitud del string
    bl strlen
    mov x20, x0                 // x20 = longitud a agregar
    
    // Verificar si necesitamos expandir
    ldr x0, =sb_length
    ldr x0, [x0]
    add x0, x0, x20             // longitud_total = actual + nueva
    add x0, x0, #1              // +1 para null
    
    ldr x1, =sb_capacity
    ldr x1, [x1]
    cmp x0, x1
    ble sb_append_copy          // si cabe, copiar
    
    // Necesitamos expandir
    lsl x0, x0, #1              // duplicar tamaño
    bl sb_resize
    
sb_append_copy:
    // Copiar string al final
    ldr x0, =sb_buffer
    ldr x0, [x0]
    ldr x1, =sb_length
    ldr x1, [x1]
    add x0, x0, x1              // posición final
    
    mov x1, x19                 // string a copiar
    bl strcpy
    
    // Actualizar longitud
    ldr x0, =sb_length
    ldr x1, [x0]
    add x1, x1, x20
    str x1, [x0]
    
    ldr x20, [sp, #24]
    ldr x19, [sp, #16]
    ldp x29, x30, [sp], #32
    ret

// Redimensionar buffer
sb_resize:
    stp x29, x30, [sp, #-32]!
    str x19, [sp, #16]
    str x20, [sp, #24]
    
    mov x19, x0                 // nueva capacidad
    
    // Reservar nuevo buffer
    mov x0, x19
    bl sb_malloc
    mov x20, x0                 // nuevo buffer
    
    // Copiar contenido antiguo
    mov x0, x20
    ldr x1, =sb_buffer
    ldr x1, [x1]
    bl strcpy
    
    // Liberar buffer antiguo
    ldr x0, =sb_buffer
    ldr x0, [x0]
    ldr x1, =sb_capacity
    ldr x1, [x1]
    bl sb_free_buffer
    
    // Actualizar puntero y capacidad
    ldr x0, =sb_buffer
    str x20, [x0]
    ldr x0, =sb_capacity
    str x19, [x0]
    
    ldr x20, [sp, #24]
    ldr x19, [sp, #16]
    ldp x29, x30, [sp], #32
    ret

// Obtener string final
sb_get_string:
    ldr x0, =sb_buffer
    ldr x0, [x0]
    ret

// Liberar StringBuilder
sb_free:
    stp x29, x30, [sp, #-16]!
    
    ldr x0, =sb_buffer
    ldr x0, [x0]
    ldr x1, =sb_capacity
    ldr x1, [x1]
    bl sb_free_buffer
    
    ldp x29, x30, [sp], #16
    ret

// malloc usando mmap
sb_malloc:
    mov x8, #222
    mov x1, x0
    mov x0, #0
    mov x2, #3
    mov x3, #34
    mov x4, #-1
    mov x5, #0
    svc #0
    ret

// free usando munmap
sb_free_buffer:
    mov x8, #215
    svc #0
    ret

// strlen - loop iterativo
strlen:
    mov x2, #0
strlen_loop:
    ldrb w3, [x0, x2]
    cbz w3, strlen_end
    add x2, x2, #1
    b strlen_loop
strlen_end:
    mov x0, x2
    ret

// strcpy - loop iterativo
strcpy:
    mov x2, #0
strcpy_loop:
    ldrb w3, [x1, x2]
    strb w3, [x0, x2]
    cbz w3, strcpy_end
    add x2, x2, #1
    b strcpy_loop
strcpy_end:
    ret

