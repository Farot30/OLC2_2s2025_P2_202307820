.global _start
.extern itoa
.extern printf
.extern sprintf

.data
str2: .asciz "Mundo"
str1: .asciz "estan"
str0: .asciz "como"
c: .skip 8
b: .skip 8
a: .asciz "Hola"
format: .asciz "%s\n"

// constantes
.section .rodata

// seccion bss
.bss
    .align 3
    sb_buffer:      .skip 8     // Puntero al buffer
    sb_capacity:    .skip 8     // Capacidad total
    sb_length:      .skip 8     // Longitud actual


.text
_start:
    // Punto de entrada del programa
    // Operación: b = <expresión>
    // Concatenación de strings usando StringBuilder
    mov x0, #64
    bl sb_init
    adrp x0, a
    add x0, x0, :lo12:a
    bl sb_append
    adrp x0, str2
    add x0, x0, :lo12:str2
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
    ldr x1, =b
    str x21, [x1]               // Guardar puntero permanente

    ldr x1, =b                  // x0 = dirección de b
    ldr x1, [x1]                // x0 = puntero al string
    ldr x0, =format
    mov x2, x1
    bl printf                   // printf("%s\n", x0)

    // Operación: c = <expresión>
    // Concatenación de strings usando StringBuilder
    mov x0, #64
    bl sb_init
    adrp x0, b
    add x0, x0, :lo12:b
    ldr x0, [x0]                // Cargar contenido de la variable
    bl sb_append
    adrp x0, str0
    add x0, x0, :lo12:str0
    bl sb_append
    adrp x0, str1
    add x0, x0, :lo12:str1
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
    ldr x1, =c
    str x21, [x1]               // Guardar puntero permanente

    ldr x0, =format
    ldr x1, =c
    ldr x1, [x1]
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
    adr x1, sb_buffer
    str x0, [x1]
    
    // Guardar capacidad
    adr x1, sb_capacity
    str x19, [x1]
    
    // Longitud inicial = 0
    adr x1, sb_length
    str xzr, [x1]
    
    // Poner null terminator
    adr x1, sb_buffer
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
    adr x0, sb_length
    ldr x0, [x0]
    add x0, x0, x20             // longitud_total = actual + nueva
    add x0, x0, #1              // +1 para null
    
    adr x1, sb_capacity
    ldr x1, [x1]
    cmp x0, x1
    ble sb_append_copy          // si cabe, copiar
    
    // Necesitamos expandir
    lsl x0, x0, #1              // duplicar tamaño
    bl sb_resize
    
sb_append_copy:
    // Copiar string al final
    adr x0, sb_buffer
    ldr x0, [x0]
    adr x1, sb_length
    ldr x1, [x1]
    add x0, x0, x1              // posición final
    
    mov x1, x19                 // string a copiar
    bl strcpy
    
    // Actualizar longitud
    adr x0, sb_length
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
    adr x1, sb_buffer
    ldr x1, [x1]
    bl strcpy
    
    // Liberar buffer antiguo
    adr x0, sb_buffer
    ldr x0, [x0]
    adr x1, sb_capacity
    ldr x1, [x1]
    bl sb_free_buffer
    
    // Actualizar puntero y capacidad
    adr x0, sb_buffer
    str x20, [x0]
    adr x0, sb_capacity
    str x19, [x0]
    
    ldr x20, [sp, #24]
    ldr x19, [sp, #16]
    ldp x29, x30, [sp], #32
    ret

// Obtener string final
sb_get_string:
    adr x0, sb_buffer
    ldr x0, [x0]
    ret

// Liberar StringBuilder
sb_free:
    stp x29, x30, [sp, #-16]!
    
    adr x0, sb_buffer
    ldr x0, [x0]
    adr x1, sb_capacity
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

