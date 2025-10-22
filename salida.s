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
format_concat_7: .asciz "%d. %s - Nota: %d\n"
notaS: .word 0
nombre: .quad str_lit_nombre
str_lit_nombre: .asciz ""
i: .word 1
str_const_47: .asciz "Listado numerado:"
format_string_46: .asciz "%s\n"
str_const_45: .asciz "\n--- PROCESAMIENTO CON FOR CLASICO ---"
format_string_44: .asciz "%s\n"
format_concat_6: .asciz "Quedan: %d estudiantes por procesar\n"
contador: .word 5
str_const_43: .asciz "\nContador regresivo:"
format_string_42: .asciz "%s\n"
format_concat_5: .asciz "Total de estudiantes destacados: %d\n"
format_concat_4: .asciz "Destacado encontrado: %s con %d\n"
format_concat_3: .asciz "Revisando estudiante %d: %s\n"
notaActual: .word 0
nombreActual: .quad str_lit_nombreActual
str_lit_nombreActual: .asciz ""
str_const_41: .asciz "Buscando estudiantes con nota >= 90:"
format_string_40: .asciz "%s\n"
estudiantesDestacados: .word 0
estudianteActual: .word 1
str_const_39: .asciz "\n--- PROCESAMIENTO CON WHILE ---"
format_string_38: .asciz "%s\n"
str_const_37: .asciz "Clasificacion: REGULAR"
format_string_36: .asciz "%s\n"
str_const_35: .asciz "Clasificacion: BIEN"
format_string_34: .asciz "%s\n"
str_const_33: .asciz "Clasificacion: NOTABLE"
format_string_32: .asciz "%s\n"
format_concat_2: .asciz "
Evaluando: %s\n"
str_const_31: .asciz "Resultado: No apto para beca"
format_string_30: .asciz "%s\n"
str_const_29: .asciz "Resultado: Apto para beca"
format_string_28: .asciz "%s\n"
str_const_27: .asciz "Clasificacion: REGULAR"
format_string_26: .asciz "%s\n"
str_const_25: .asciz "Clasificacion: NOTABLE"
format_string_24: .asciz "%s\n"
str_const_23: .asciz "Clasificacion: SOBRESALIENTE"
format_string_22: .asciz "%s\n"
format_concat_1: .asciz "
Evaluando: %s\n"
str_const_21: .asciz "Resultado: No apto para beca"
format_string_20: .asciz "%s\n"
str_const_19: .asciz "Resultado: Apto para beca"
format_string_18: .asciz "%s\n"
str_const_17: .asciz "Estado: Mayor de edad completa"
format_string_16: .asciz "%s\n"
str_const_15: .asciz "Clasificacion: INSUFICIENTE"
format_string_14: .asciz "%s\n"
str_const_13: .asciz "Clasificacion: SUFICIENTE"
format_string_12: .asciz "%s\n"
str_const_11: .asciz "Clasificacion: BIEN"
format_string_10: .asciz "%s\n"
str_const_9: .asciz "Clasificacion: NOTABLE"
format_string_8: .asciz "%s\n"
str_const_7: .asciz "Clasificacion: SOBRESALIENTE"
format_string_6: .asciz "%s\n"
format_concat_0: .asciz "Evaluando: %s\n"
str_const_5: .asciz "\n--- EVALUACION CON IF/ELSE ---"
format_string_4: .asciz "%s\n"
str_const_3: .asciz "Procesando 5 estudiantes individuales"
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
    ldr x0, =format_string_4
    ldr x1, =str_const_5
    bl printf
    // Printf optimizado para concatenación
    ldr x1, =estudiante1
    ldr x1, [x1]
    ldr x0, =format_concat_0
    bl printf
    // Evaluación de condición IF
    ldr x1, =nota1
    ldr w1, [x1]
    mov w2, #95
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_84

    ldr x0, =format_string_6
    ldr x1, =str_const_7
    bl printf
    b if_end_84

if_else_84:
    // Evaluación de condición IF
    ldr x1, =nota1
    ldr w1, [x1]
    mov w2, #85
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_85

    ldr x0, =format_string_8
    ldr x1, =str_const_9
    bl printf
    b if_end_85

if_else_85:
    // Evaluación de condición IF
    ldr x1, =nota1
    ldr w1, [x1]
    mov w2, #75
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_86

    ldr x0, =format_string_10
    ldr x1, =str_const_11
    bl printf
    b if_end_86

if_else_86:
    // Evaluación de condición IF
    ldr x1, =nota1
    ldr w1, [x1]
    mov w2, #65
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_87

    ldr x0, =format_string_12
    ldr x1, =str_const_13
    bl printf
    b if_end_87

if_else_87:
    ldr x0, =format_string_14
    ldr x1, =str_const_15
    bl printf
if_end_87:
if_end_86:
if_end_85:
if_end_84:
if_88:
    // Evaluación de condición IF
    ldr x1, =edad1
    ldr w1, [x1]
    mov w2, #21
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar si condición es falsa
    cbz w0, if_end_88

    ldr x0, =format_string_16
    ldr x1, =str_const_17
    bl printf
if_end_88:
    // Evaluación de condición IF
    ldr x1, =nota1
    ldr w1, [x1]
    mov w2, #90
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_89

    ldr x0, =format_string_18
    ldr x1, =str_const_19
    bl printf
    b if_end_89

if_else_89:
    ldr x0, =format_string_20
    ldr x1, =str_const_21
    bl printf
if_end_89:
    // Printf optimizado para concatenación
    ldr x1, =estudiante2
    ldr x1, [x1]
    ldr x0, =format_concat_1
    bl printf
    // Evaluación de condición IF
    ldr x1, =nota2
    ldr w1, [x1]
    mov w2, #95
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_90

    ldr x0, =format_string_22
    ldr x1, =str_const_23
    bl printf
    b if_end_90

if_else_90:
    // Evaluación de condición IF
    ldr x1, =nota2
    ldr w1, [x1]
    mov w2, #85
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_91

    ldr x0, =format_string_24
    ldr x1, =str_const_25
    bl printf
    b if_end_91

if_else_91:
    ldr x0, =format_string_26
    ldr x1, =str_const_27
    bl printf
if_end_91:
if_end_90:
    // Evaluación de condición IF
    ldr x1, =nota2
    ldr w1, [x1]
    mov w2, #90
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_92

    ldr x0, =format_string_28
    ldr x1, =str_const_29
    bl printf
    b if_end_92

if_else_92:
    ldr x0, =format_string_30
    ldr x1, =str_const_31
    bl printf
if_end_92:
    // Printf optimizado para concatenación
    ldr x1, =estudiante3
    ldr x1, [x1]
    ldr x0, =format_concat_2
    bl printf
    // Evaluación de condición IF
    ldr x1, =nota3
    ldr w1, [x1]
    mov w2, #85
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_93

    ldr x0, =format_string_32
    ldr x1, =str_const_33
    bl printf
    b if_end_93

if_else_93:
    // Evaluación de condición IF
    ldr x1, =nota3
    ldr w1, [x1]
    mov w2, #75
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_94

    ldr x0, =format_string_34
    ldr x1, =str_const_35
    bl printf
    b if_end_94

if_else_94:
    ldr x0, =format_string_36
    ldr x1, =str_const_37
    bl printf
if_end_94:
if_end_93:
    ldr x0, =format_string_38
    ldr x1, =str_const_39
    bl printf
    ldr x0, =format_string_40
    ldr x1, =str_const_41
    bl printf
while_inicio_9:
    ldr x1, =estudianteActual
    ldr w1, [x1]
    mov w2, #5
    cmp w1, w2
    cset w1, le
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, while_fin_9

    // Evaluación de condición IF
    ldr x1, =estudianteActual
    ldr w1, [x1]
    mov w2, #1
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_95

    ldr x1, =estudiante1
    ldr x1, [x1]
    ldr x2, =estudiante1
    ldr x2, [x2]
    ldr x1, =nombreActual
    str x2, [x1]
    ldr x1, =nota1
    ldr w1, [x1]
    ldr x1, =nota1
    ldr w1, [x1]
    ldr x2, =notaActual
    str w1, [x2]
    b if_end_95

if_else_95:
    // Evaluación de condición IF
    ldr x1, =estudianteActual
    ldr w1, [x1]
    mov w2, #2
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_96

    ldr x1, =estudiante2
    ldr x1, [x1]
    ldr x2, =estudiante2
    ldr x2, [x2]
    ldr x1, =nombreActual
    str x2, [x1]
    ldr x1, =nota2
    ldr w1, [x1]
    ldr x1, =nota2
    ldr w1, [x1]
    ldr x2, =notaActual
    str w1, [x2]
    b if_end_96

if_else_96:
    // Evaluación de condición IF
    ldr x1, =estudianteActual
    ldr w1, [x1]
    mov w2, #3
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_97

    ldr x1, =estudiante3
    ldr x1, [x1]
    ldr x2, =estudiante3
    ldr x2, [x2]
    ldr x1, =nombreActual
    str x2, [x1]
    ldr x1, =nota3
    ldr w1, [x1]
    ldr x1, =nota3
    ldr w1, [x1]
    ldr x2, =notaActual
    str w1, [x2]
    b if_end_97

if_else_97:
    // Evaluación de condición IF
    ldr x1, =estudianteActual
    ldr w1, [x1]
    mov w2, #4
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_98

    ldr x1, =estudiante4
    ldr x1, [x1]
    ldr x2, =estudiante4
    ldr x2, [x2]
    ldr x1, =nombreActual
    str x2, [x1]
    ldr x1, =nota4
    ldr w1, [x1]
    ldr x1, =nota4
    ldr w1, [x1]
    ldr x2, =notaActual
    str w1, [x2]
    b if_end_98

if_else_98:
if_99:
    // Evaluación de condición IF
    ldr x1, =estudianteActual
    ldr w1, [x1]
    mov w2, #5
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    // Saltar si condición es falsa
    cbz w0, if_end_99

    ldr x1, =estudiante5
    ldr x1, [x1]
    ldr x2, =estudiante5
    ldr x2, [x2]
    ldr x1, =nombreActual
    str x2, [x1]
    ldr x1, =nota5
    ldr w1, [x1]
    ldr x1, =nota5
    ldr w1, [x1]
    ldr x2, =notaActual
    str w1, [x2]
if_end_99:
if_end_98:
if_end_97:
if_end_96:
if_end_95:
    // Printf optimizado para concatenación
    ldr x1, =estudianteActual
    ldr w1, [x1]
    mov w2, w1
    ldr x2, =nombreActual
    ldr x2, [x2]
    ldr x0, =format_concat_3
    bl printf
if_100:
    // Evaluación de condición IF
    ldr x1, =notaActual
    ldr w1, [x1]
    mov w2, #90
    cmp w1, w2
    cset w1, ge
    mov w0, w1
    // Saltar si condición es falsa
    cbz w0, if_end_100

    // Printf optimizado para concatenación
    ldr x1, =nombreActual
    ldr x1, [x1]
    ldr x2, =notaActual
    ldr w2, [x2]
    mov w3, w2
    ldr x0, =format_concat_4
    bl printf
    // Incremento: estudiantesDestacados++
    ldr x1, =estudiantesDestacados
    ldr w0, [x1]
    add w0, w0, #1
    str w0, [
if_end_100:
    // Incremento: estudianteActual++
    ldr x1, =estudianteActual
    ldr w0, [x1]
    add w0, w0, #1
    str w0, [x1]
    b while_inicio_9
while_fin_9:
    // Printf optimizado para concatenación
    ldr x1, =estudiantesDestacados
    ldr w1, [x1]
    mov w2, w1
    ldr x0, =format_concat_5
    bl printf
    ldr x0, =format_string_42
    ldr x1, =str_const_43
    bl printf
while_inicio_10:
    ldr x1, =contador
    ldr w1, [x1]
    mov w2, #0
    cmp w1, w2
    cset w1, gt
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, while_fin_10

    // Printf optimizado para concatenación
    ldr x1, =contador
    ldr w1, [x1]
    mov w2, w1
    ldr x0, =format_concat_6
    bl printf
    // Decremento: contador--
    ldr x1, =contador
    ldr w0, [x1]
    sub w0, w0, #1
    str w0, [x1]
    b while_inicio_10
while_fin_10:
    ldr x0, =format_string_44
    ldr x1, =str_const_45
    bl printf
    ldr x0, =format_string_46
    ldr x1, =str_const_47
    bl printf
for_loop_8:
    ldr x1, =i
    ldr w1, [x1]
    mov w2, #5
    cmp w1, w2
    cset w1, le
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, for_end_8

    // Evaluación de condición IF
    ldr x1, =i
    ldr w1, [x1]
    mov w2, #1
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_101

    ldr x1, =estudiante1
    ldr x1, [x1]
    ldr x2, =estudiante1
    ldr x2, [x2]
    ldr x1, =nombre
    str x2, [x1]
    ldr x1, =nota1
    ldr w1, [x1]
    ldr x1, =nota1
    ldr w1, [x1]
    ldr x2, =notaS
    str w1, [x2]
    b if_end_101

if_else_101:
    // Evaluación de condición IF
    ldr x1, =i
    ldr w1, [x1]
    mov w2, #2
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_102

    ldr x1, =estudiante2
    ldr x1, [x1]
    ldr x2, =estudiante2
    ldr x2, [x2]
    ldr x1, =nombre
    str x2, [x1]
    ldr x1, =nota2
    ldr w1, [x1]
    ldr x1, =nota2
    ldr w1, [x1]
    ldr x2, =notaS
    str w1, [x2]
    b if_end_102

if_else_102:
    // Evaluación de condición IF
    ldr x1, =i
    ldr w1, [x1]
    mov w2, #3
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_103

    ldr x1, =estudiante3
    ldr x1, [x1]
    ldr x2, =estudiante3
    ldr x2, [x2]
    ldr x1, =nombre
    str x2, [x1]
    ldr x1, =nota3
    ldr w1, [x1]
    ldr x1, =nota3
    ldr w1, [x1]
    ldr x2, =notaS
    str w1, [x2]
    b if_end_103

if_else_103:
    // Evaluación de condición IF
    ldr x1, =i
    ldr w1, [x1]
    mov w2, #4
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    // Saltar a else si condición es falsa
    cbz w0, if_else_104

    ldr x1, =estudiante4
    ldr x1, [x1]
    ldr x2, =estudiante4
    ldr x2, [x2]
    ldr x1, =nombre
    str x2, [x1]
    ldr x1, =nota4
    ldr w1, [x1]
    ldr x1, =nota4
    ldr w1, [x1]
    ldr x2, =notaS
    str w1, [x2]
    b if_end_104

if_else_104:
if_105:
    // Evaluación de condición IF
    ldr x1, =i
    ldr w1, [x1]
    mov w2, #5
    cmp w1, w2
    cset w1, eq
    mov w0, w1
    // Saltar si condición es falsa
    cbz w0, if_end_105

    ldr x1, =estudiante5
    ldr x1, [x1]
    ldr x2, =estudiante5
    ldr x2, [x2]
    ldr x1, =nombre
    str x2, [x1]
    ldr x1, =nota5
    ldr w1, [x1]
    ldr x1, =nota5
    ldr w1, [x1]
    ldr x2, =notaS
    str w1, [x2]
if_end_105:
if_end_104:
if_end_103:
if_end_102:
if_end_101:
    // Printf optimizado para concatenación
    ldr x1, =i
    ldr w1, [x1]
    mov w2, w1
    ldr x2, =nombre
    ldr x2, [x2]
    ldr x3, =notaS
    ldr w3, [x3]
    mov w4, w3
    ldr x0, =format_concat_7
    bl printf
for_inc_8:
    // Incremento: i++
    ldr x1, =i
    ldr w0, [x1]
    add w0, w0, #1
    str w0, [x1]
    // Volver al inicio del bucle
    b for_loop_8
for_end_8:
    // Salida del programa por defecto
    mov x0, #0
    mov x8, #93
    svc #0
