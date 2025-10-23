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
format_concat_6: .asciz "%s - Categoria: Adulto\n"
format_concat_5: .asciz "%s - Categoria: Adulto joven\n"
str_const_41: .asciz ""
format_string_40: .asciz "%s\n"
format_concat_4: .asciz "%s - Categoria: Joven adulto\n"
format_concat_3: .asciz "%s - Categoria: Joven adulto\n"
format_concat_2: .asciz "%s - Categoria: Muy joven\n"
str_const_39: .asciz ""
format_string_38: .asciz "%s\n"
str_const_37: .asciz "\nAnalisis por edad:"
format_string_36: .asciz "%s\n"
str_const_35: .asciz "Categoria: REGULAR"
format_string_34: .asciz "%s\n"
str_const_33: .asciz "Categoria: BUENO"
format_string_32: .asciz "%s\n"
str_const_31: .asciz "Categoria: MUY BUENO"
format_string_30: .asciz "%s\n"
str_const_29: .asciz "Categoria: EXCELENTE"
format_string_28: .asciz "%s\n"
str_const_27: .asciz ""
format_string_26: .asciz "%s\n"
categoria2: .word 0
format_concat_1: .asciz "
Analizando: %s (Nota: %d)\n"
str_const_25: .asciz "Accion: Apoyo urgente"
format_string_24: .asciz "%s\n"
str_const_23: .asciz "Categoria: INSUFICIENTE"
format_string_22: .asciz "%s\n"
str_const_21: .asciz "Accion: Reforzar conocimientos"
format_string_20: .asciz "%s\n"
str_const_19: .asciz "Categoria: SUFICIENTE"
format_string_18: .asciz "%s\n"
str_const_17: .asciz "Accion: Mejorar tecnicas"
format_string_16: .asciz "%s\n"
str_const_15: .asciz "Categoria: BUENO"
format_string_14: .asciz "%s\n"
str_const_13: .asciz "Accion: Continuar esfuerzo"
format_string_12: .asciz "%s\n"
str_const_11: .asciz "Categoria: MUY BUENO"
format_string_10: .asciz "%s\n"
str_const_9: .asciz "Accion: Mantener nivel"
format_string_8: .asciz "%s\n"
str_const_7: .asciz "Categoria: EXCELENTE"
format_string_6: .asciz "%s\n"
str_const_5: .asciz ""
format_string_4: .asciz "%s\n"
categoria1: .word 0
format_concat_0: .asciz "Analizando: %s (Nota: %d)\n"
str_const_3: .asciz "\n--- CLASIFICACION CON SWITCH/CASE ---"
format_string_2: .asciz "%s\n"
edad5: .word 22
edad4: .word 20
edad3: .word 21
edad2: .word 19
edad1: .word 20
nota5: .word 88
nota4: .word 95
nota3: .word 78
nota2: .word 92
nota1: .word 85
estudiante5: .quad str_lit_estudiante5
str_lit_estudiante5: .asciz "Sofia"
estudiante4: .quad str_lit_estudiante4
str_lit_estudiante4: .asciz "Carlos"
estudiante3: .quad str_lit_estudiante3
str_lit_estudiante3: .asciz "Maria"
estudiante2: .quad str_lit_estudiante2
str_lit_estudiante2: .asciz "Luis"
estudiante1: .quad str_lit_estudiante1
str_lit_estudiante1: .asciz "Ana"
str_const_1: .asciz "=== SISTEMA DE GESTION DE NOTAS INDIVIDUAL ==="
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
    // Printf optimizado para concatenación
    ldr x1, =estudiante1
    ldr x1, [x1]
    ldr x2, =nota1
    ldr w2, [x2]
    mov w3, w2
    ldr x0, =format_concat_0
    bl printf
    // Operación: categoria1 = <expresión>
    ldr x2, =nota1
    ldr w0, [x2]
    mov w1, #10
    sdiv w0, w0, w1
    ldr x1, =categoria1
    str w0, [x1]
    // Evaluación de expresión switch
    ldr x1, =categoria1
    ldr w0, [x1]
    mov w1, #10
    cmp w0, w1
    beq switch_case_0_0
    mov w1, #9
    cmp w0, w1
    beq switch_case_0_1
    mov w1, #8
    cmp w0, w1
    beq switch_case_0_2
    mov w1, #7
    cmp w0, w1
    beq switch_case_0_3
    mov w1, #6
    cmp w0, w1
    beq switch_case_0_4
    b switch_default_0
switch_case_0_0:
    ldr x0, =format_string_4
    ldr x1, =str_const_5
    bl printf
    b switch_end_0
switch_case_0_1:
    ldr x0, =format_string_6
    ldr x1, =str_const_7
    bl printf
    ldr x0, =format_string_8
    ldr x1, =str_const_9
    bl printf
    b switch_end_0
switch_case_0_2:
    ldr x0, =format_string_10
    ldr x1, =str_const_11
    bl printf
    ldr x0, =format_string_12
    ldr x1, =str_const_13
    bl printf
    b switch_end_0
switch_case_0_3:
    ldr x0, =format_string_14
    ldr x1, =str_const_15
    bl printf
    ldr x0, =format_string_16
    ldr x1, =str_const_17
    bl printf
    b switch_end_0
switch_case_0_4:
    ldr x0, =format_string_18
    ldr x1, =str_const_19
    bl printf
    ldr x0, =format_string_20
    ldr x1, =str_const_21
    bl printf
    b switch_end_0
switch_default_0:
    ldr x0, =format_string_22
    ldr x1, =str_const_23
    bl printf
    ldr x0, =format_string_24
    ldr x1, =str_const_25
    bl printf
    // Break: salir del switch
    b switch_end_0
switch_end_0:
    // Printf optimizado para concatenación
    ldr x1, =estudiante2
    ldr x1, [x1]
    ldr x2, =nota2
    ldr w2, [x2]
    mov w3, w2
    ldr x0, =format_concat_1
    bl printf
    // Operación: categoria2 = <expresión>
    ldr x2, =nota2
    ldr w0, [x2]
    mov w1, #10
    sdiv w0, w0, w1
    ldr x1, =categoria2
    str w0, [x1]
    // Evaluación de expresión switch
    ldr x1, =categoria2
    ldr w0, [x1]
    mov w1, #10
    cmp w0, w1
    beq switch_case_1_0
    mov w1, #9
    cmp w0, w1
    beq switch_case_1_1
    mov w1, #8
    cmp w0, w1
    beq switch_case_1_2
    mov w1, #7
    cmp w0, w1
    beq switch_case_1_3
    b switch_default_1
switch_case_1_0:
    ldr x0, =format_string_26
    ldr x1, =str_const_27
    bl printf
    b switch_end_1
switch_case_1_1:
    ldr x0, =format_string_28
    ldr x1, =str_const_29
    bl printf
    b switch_end_1
switch_case_1_2:
    ldr x0, =format_string_30
    ldr x1, =str_const_31
    bl printf
    b switch_end_1
switch_case_1_3:
    ldr x0, =format_string_32
    ldr x1, =str_const_33
    bl printf
    b switch_end_1
switch_default_1:
    ldr x0, =format_string_34
    ldr x1, =str_const_35
    bl printf
    // Break: salir del switch
    b switch_end_1
switch_end_1:
    ldr x0, =format_string_36
    ldr x1, =str_const_37
    bl printf
    // Evaluación de expresión switch
    ldr x1, =edad1
    ldr w0, [x1]
    mov w1, #18
    cmp w0, w1
    beq switch_case_2_0
    mov w1, #19
    cmp w0, w1
    beq switch_case_2_1
    mov w1, #20
    cmp w0, w1
    beq switch_case_2_2
    mov w1, #21
    cmp w0, w1
    beq switch_case_2_3
    mov w1, #22
    cmp w0, w1
    beq switch_case_2_4
    mov w1, #23
    cmp w0, w1
    beq switch_case_2_5
    b switch_default_2
switch_case_2_0:
    ldr x0, =format_string_38
    ldr x1, =str_const_39
    bl printf
    b switch_end_2
switch_case_2_1:
    // Printf optimizado para concatenación
    ldr x1, =estudiante1
    ldr x1, [x1]
    ldr x0, =format_concat_2
    bl printf
    b switch_end_2
switch_case_2_2:
    // Printf optimizado para concatenación
    ldr x1, =estudiante1
    ldr x1, [x1]
    ldr x0, =format_concat_3
    bl printf
    b switch_end_2
switch_case_2_3:
    // Printf optimizado para concatenación
    ldr x1, =estudiante1
    ldr x1, [x1]
    ldr x0, =format_concat_4
    bl printf
    b switch_end_2
switch_case_2_4:
    ldr x0, =format_string_40
    ldr x1, =str_const_41
    bl printf
    b switch_end_2
switch_case_2_5:
    // Printf optimizado para concatenación
    ldr x1, =estudiante1
    ldr x1, [x1]
    ldr x0, =format_concat_5
    bl printf
    b switch_end_2
switch_default_2:
    // Printf optimizado para concatenación
    ldr x1, =estudiante1
    ldr x1, [x1]
    ldr x0, =format_concat_6
    bl printf
    // Break: salir del switch
    b switch_end_2
switch_end_2:
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
