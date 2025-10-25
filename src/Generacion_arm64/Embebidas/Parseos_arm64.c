#include "Parseos_arm64.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "EvaluadorExpr_arm64.h"
#include "../../semantico/tipos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern NodoAST* astGlobal;

// ======================== CREAR ARCHIVO PARSE_INT ========================
void crearArchivoParseInt(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("[ERROR] No se pudo crear %s\n", nombreArchivo);
        return;
    }

    fprintf(archivo,
        "// Implementación de parse_int en ARM64 puro\n"
        "// Convierte un string a entero\n"
        "// Entrada: x0 = puntero al string\n"
        "// Salida: w0 = valor entero\n"
        "\n"
        ".global parse_int\n"
        ".text\n"
        "\n"
        "parse_int:\n"
        "    // Guardar registros\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n"
        "    \n"
        "    // Inicializar variables\n"
        "    mov w1, #0              // resultado = 0\n"
        "    mov w2, #0              // signo = 0 (positivo)\n"
        "    mov x3, x0              // puntero al string\n"
        "    \n"
        "    // Saltar espacios en blanco\n"
        "skip_spaces:\n"
        "    ldrb w4, [x3]           // Cargar carácter\n"
        "    cmp w4, #32             // ' '\n"
        "    beq next_char_space\n"
        "    cmp w4, #9              // '\\t'\n"
        "    bne check_sign\n"
        "next_char_space:\n"
        "    add x3, x3, #1\n"
        "    b skip_spaces\n"
        "    \n"
        "check_sign:\n"
        "    // Verificar signo\n"
        "    ldrb w4, [x3]\n"
        "    cmp w4, #45             // '-'\n"
        "    bne check_plus\n"
        "    mov w2, #1              // signo = negativo\n"
        "    add x3, x3, #1\n"
        "    b parse_loop\n"
        "    \n"
        "check_plus:\n"
        "    cmp w4, #43             // '+'\n"
        "    bne parse_loop\n"
        "    add x3, x3, #1          // Saltar '+'\n"
        "    \n"
        "parse_loop:\n"
        "    ldrb w4, [x3]           // Cargar carácter\n"
        "    \n"
        "    // Verificar si es dígito (0-9)\n"
        "    cmp w4, #48             // '0'\n"
        "    blt end_parse\n"
        "    cmp w4, #57             // '9'\n"
        "    bgt end_parse\n"
        "    \n"
        "    // Convertir carácter a dígito\n"
        "    sub w4, w4, #48         // digito = char - '0'\n"
        "    \n"
        "    // resultado = resultado * 10 + digito\n"
        "    mov w5, #10\n"
        "    mul w1, w1, w5          // resultado *= 10\n"
        "    add w1, w1, w4          // resultado += digito\n"
        "    \n"
        "    // Siguiente carácter\n"
        "    add x3, x3, #1\n"
        "    b parse_loop\n"
        "    \n"
        "end_parse:\n"
        "    // Aplicar signo si es negativo\n"
        "    cmp w2, #1\n"
        "    bne return_result\n"
        "    neg w1, w1              // resultado = -resultado\n"
        "    \n"
        "return_result:\n"
        "    mov w0, w1              // Retornar en w0\n"
        "    \n"
        "    // Restaurar registros y retornar\n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n"
    );

    fclose(archivo);
    //printf("[INFO] Archivo %s creado exitosamente\n", nombreArchivo);
}

// ======================== CREAR ARCHIVO PARSE_DOUBLE ========================
void crearArchivoParseDouble(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("[ERROR] No se pudo crear %s\n", nombreArchivo);
        return;
    }

    fprintf(archivo,
        "// Implementación de parse_double en ARM64 puro\n"
        "// Convierte un string a double\n"
        "// Entrada: x0 = puntero al string\n"
        "// Salida: d0 = valor double\n"
        "\n"
        ".global parse_double\n"
        ".text\n"
        "\n"
        "parse_double:\n"
        "    // Guardar registros\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n"
        "    \n"
        "    // Inicializar variables\n"
        "    fmov d0, xzr            // resultado = 0.0\n"
        "    fmov d1, xzr            // parte_decimal = 0.0\n"
        "    mov w1, #0              // signo = 0 (positivo)\n"
        "    mov w2, #0              // encontro_punto = 0\n"
        "    mov w3, #0              // divisor_decimal = 1\n"
        "    mov x4, x0              // puntero al string\n"
        "    \n"
        "    // Saltar espacios en blanco\n"
        "skip_spaces_d:\n"
        "    ldrb w5, [x4]\n"
        "    cmp w5, #32             // ' '\n"
        "    beq next_char_space_d\n"
        "    cmp w5, #9              // '\\t'\n"
        "    bne check_sign_d\n"
        "next_char_space_d:\n"
        "    add x4, x4, #1\n"
        "    b skip_spaces_d\n"
        "    \n"
        "check_sign_d:\n"
        "    ldrb w5, [x4]\n"
        "    cmp w5, #45             // '-'\n"
        "    bne check_plus_d\n"
        "    mov w1, #1              // signo = negativo\n"
        "    add x4, x4, #1\n"
        "    b parse_loop_d\n"
        "    \n"
        "check_plus_d:\n"
        "    cmp w5, #43             // '+'\n"
        "    bne parse_loop_d\n"
        "    add x4, x4, #1\n"
        "    \n"
        "parse_loop_d:\n"
        "    ldrb w5, [x4]\n"
        "    \n"
        "    // Verificar si es punto decimal\n"
        "    cmp w5, #46             // '.'\n"
        "    bne check_digit_d\n"
        "    mov w2, #1              // encontro_punto = true\n"
        "    add x4, x4, #1\n"
        "    b parse_loop_d\n"
        "    \n"
        "check_digit_d:\n"
        "    // Verificar si es dígito\n"
        "    cmp w5, #48             // '0'\n"
        "    blt end_parse_d\n"
        "    cmp w5, #57             // '9'\n"
        "    bgt end_parse_d\n"
        "    \n"
        "    // Convertir carácter a dígito\n"
        "    sub w5, w5, #48\n"
        "    scvtf d2, w5            // Convertir a double\n"
        "    \n"
        "    // Si no hemos encontrado punto, procesar parte entera\n"
        "    cmp w2, #0\n"
        "    bne process_decimal_d\n"
        "    \n"
        "    // Parte entera: resultado = resultado * 10 + digito\n"
        "    fmov d3, #10.0\n"
        "    fmul d0, d0, d3\n"
        "    fadd d0, d0, d2\n"
        "    b next_char_d\n"
        "    \n"
        "process_decimal_d:\n"
        "    // Parte decimal\n"
        "    mov w6, #10\n"
        "    mul w3, w3, w6          // divisor_decimal *= 10\n"
        "    scvtf d3, w3            // Convertir divisor a double\n"
        "    fdiv d2, d2, d3         // digito / divisor\n"
        "    fadd d1, d1, d2         // parte_decimal += digito/divisor\n"
        "    \n"
        "next_char_d:\n"
        "    add x4, x4, #1\n"
        "    b parse_loop_d\n"
        "    \n"
        "end_parse_d:\n"
        "    // Combinar parte entera y decimal\n"
        "    fadd d0, d0, d1\n"
        "    \n"
        "    // Aplicar signo si es negativo\n"
        "    cmp w1, #1\n"
        "    bne return_result_d\n"
        "    fneg d0, d0\n"
        "    \n"
        "return_result_d:\n"
        "    // Resultado ya está en d0\n"
        "    \n"
        "    // Restaurar registros y retornar\n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n"
    );

    fclose(archivo);
    //printf("[INFO] Archivo %s creado exitosamente\n", nombreArchivo);
}

// ======================== CREAR ARCHIVO PARSE_FLOAT ========================
void crearArchivoParseFloat(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("[ERROR] No se pudo crear %s\n", nombreArchivo);
        return;
    }

    fprintf(archivo,
        "// Implementación de parse_float en ARM64 puro\n"
        "// Convierte un string a float\n"
        "// Entrada: x0 = puntero al string\n"
        "// Salida: s0 = valor float\n"
        "\n"
        ".global parse_float\n"
        ".extern parse_double\n"
        ".text\n"
        "\n"
        "parse_float:\n"
        "    // Guardar registros\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n"
        "    \n"
        "    // Llamar a parse_double (reutilizar la lógica)\n"
        "    bl parse_double         // Resultado en d0\n"
        "    \n"
        "    // Convertir double a float\n"
        "    fcvt s0, d0\n"
        "    \n"
        "    // Restaurar registros y retornar\n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n"
    );

    fclose(archivo);
    //printf("[INFO] Archivo %s creado exitosamente\n", nombreArchivo);
}

// ======================== CREAR TODOS LOS ARCHIVOS ========================
void crearArchivosParseo() {
    crearArchivoParseInt("parse_int.s");
    crearArchivoParseDouble("parse_double.s");
    crearArchivoParseFloat("parse_float.s");
}

// ======================== PARSEINT ========================
ResultadoExprEmbebida generarParseIntARM64(GeneradorDeclaraciones* gen, NodoAST* nodo, int* regIndex) {
    ResultadoExprEmbebida resultado = {0};
    memset(resultado.codigo, 0, sizeof(resultado.codigo));
    memset(resultado.resultado, 0, sizeof(resultado.resultado));
    
    if (!gen || !nodo || nodo->tipo != AST_PARSEINT) {
        printf("[ERROR] Nodo inválido para parseInt\n");
        return resultado;
    }
    
    if (nodo->num_hijos < 1) {
        printf("[ERROR] parseInt sin argumento\n");
        return resultado;
    }
    
    NodoAST* argumentoNodo = nodo->hijos[0];
    
    printf("[DEBUG PARSEINT] Generando parseInt\n");
    
    char buffer[512];
    
    // **EVALUAR EL ARGUMENTO (string)**
    ResultadoExprEmbebida argResultado = generarExprEmbebidaARM64(gen, argumentoNodo, regIndex);
    strcat(resultado.codigo, argResultado.codigo);
    
    // Generar comentario
    snprintf(buffer, sizeof(buffer),
        "    // Integer.parseInt(string)\n");
    strcat(resultado.codigo, buffer);
    
    // **CASO 1: STRING LITERAL - Optimización en compile-time**
    if (argumentoNodo->tipo == AST_CADENA) {
        const char* valorStr = argumentoNodo->valor;
        
        // Remover comillas si existen
        char valorSinComillas[64];
        if (valorStr[0] == '"' && valorStr[strlen(valorStr)-1] == '"') {
            strncpy(valorSinComillas, valorStr + 1, sizeof(valorSinComillas) - 1);
            valorSinComillas[strlen(valorStr) - 2] = '\0';
        } else {
            strncpy(valorSinComillas, valorStr, sizeof(valorSinComillas) - 1);
        }
        
        // Convertir a int en compile-time
        int valorInt = atoi(valorSinComillas);
        
        // ✅ SIEMPRE USAR w0 PARA EL RESULTADO
        snprintf(buffer, sizeof(buffer),
            "    mov w0, #%d               // parseInt(\"%s\") optimizado\n",
            valorInt, valorSinComillas);
        strcat(resultado.codigo, buffer);
        
        snprintf(resultado.resultado, sizeof(resultado.resultado), "w0");
        resultado.tipo = TIPO_INT;
        
        printf("[DEBUG PARSEINT] Optimizado en compile-time: %d\n", valorInt);
        return resultado;
    }
    
    // **CASO 2: VARIABLE O EXPRESIÓN - Usar parse_int custom**
    
    // Cargar el string si es una variable
    if (argumentoNodo->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s\n"
            "    ldr x0, [x%d]              // Cargar puntero al string\n",
            *regIndex, argumentoNodo->valor, *regIndex);
        strcat(resultado.codigo, buffer);
        (*regIndex)++;
    } else if (argResultado.resultado[0] == 'x') {
        printf("[DEBUG PARSEINT] Argumento es puntero: %s\n", argResultado.resultado);
        // Ya es un puntero, moverlo a x0
        snprintf(buffer, sizeof(buffer),
            "    mov x0, %s                 // Puntero al string\n",
            argResultado.resultado);
        strcat(resultado.codigo, buffer);
    }
    
    printf("[DEBUG PARSEINT] Llamando a parse_int en runtime\n");
    // Llamar a parse_int (nuestra implementación)
    snprintf(buffer, sizeof(buffer),
        "    bl parse_int               // Llamar parse_int custom (retorna en w0)\n");
    strcat(resultado.codigo, buffer);
    
    // El resultado está en w0
    snprintf(resultado.resultado, sizeof(resultado.resultado), "w0");
    resultado.tipo = TIPO_INT;
    
    printf("[DEBUG PARSEINT] Código generado para parseInt runtime\n");
    return resultado;
}

// ======================== PARSEDOUBLE ========================
ResultadoExprEmbebida generarParseDoubleARM64(GeneradorDeclaraciones* gen, NodoAST* nodo, int* regIndex) {
    ResultadoExprEmbebida resultado = {0};
    memset(resultado.codigo, 0, sizeof(resultado.codigo));
    memset(resultado.resultado, 0, sizeof(resultado.resultado));
    
    if (!gen || !nodo || nodo->tipo != AST_PARSEDOUBLE) {
        printf("[ERROR] Nodo inválido para parseDouble\n");
        return resultado;
    }
    
    if (nodo->num_hijos < 1) {
        printf("[ERROR] parseDouble sin argumento\n");
        return resultado;
    }
    
    NodoAST* argumentoNodo = nodo->hijos[0];
    
    printf("[DEBUG PARSEDOUBLE] Generando parseDouble\n");
    
    char buffer[512];
    
    // **EVALUAR EL ARGUMENTO (string)**
    ResultadoExprEmbebida argResultado = generarExprEmbebidaARM64(gen, argumentoNodo, regIndex);
    strcat(resultado.codigo, argResultado.codigo);
    
    // Generar comentario
    snprintf(buffer, sizeof(buffer),
        "    // Double.parseDouble(string)\n");
    strcat(resultado.codigo, buffer);
    
    // **CASO 1: STRING LITERAL - Optimización**
    if (argumentoNodo->tipo == AST_CADENA) {
        const char* valorStr = argumentoNodo->valor;
        
        // Remover comillas
        char valorSinComillas[64];
        if (valorStr[0] == '"' && valorStr[strlen(valorStr)-1] == '"') {
            strncpy(valorSinComillas, valorStr + 1, sizeof(valorSinComillas) - 1);
            valorSinComillas[strlen(valorStr) - 2] = '\0';
        } else {
            strncpy(valorSinComillas, valorStr, sizeof(valorSinComillas) - 1);
        }
        
        // Crear constante en .data
        char nombreConst[64];
        snprintf(nombreConst, sizeof(nombreConst), "double_parse_%d", gen->contador_literales_string++);
        
        snprintf(buffer, sizeof(buffer), "%s: .double %s\n", nombreConst, valorSinComillas);
        insertarEnSeccionData(gen, buffer);
        
        // Cargar la constante
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s\n"
            "    ldr d%d, [x%d]             // parseDouble(\"%s\") optimizado\n",
            *regIndex, nombreConst, *regIndex, *regIndex, valorSinComillas);
        strcat(resultado.codigo, buffer);
        
        snprintf(resultado.resultado, sizeof(resultado.resultado), "d%d", *regIndex);
        (*regIndex)++;
        resultado.tipo = TIPO_DOUBLE;
        
        printf("[DEBUG PARSEDOUBLE] Optimizado en compile-time: %s\n", valorSinComillas);
        return resultado;
    }
    
    // **CASO 2: VARIABLE - Usar parse_double custom**
    
    if (argumentoNodo->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s\n"
            "    ldr x0, [x%d]              // Cargar puntero al string\n",
            *regIndex, argumentoNodo->valor, *regIndex);
        strcat(resultado.codigo, buffer);
        (*regIndex)++;
    } else if (argResultado.resultado[0] == 'x') {
        snprintf(buffer, sizeof(buffer),
            "    mov x0, %s                 // Puntero al string\n",
            argResultado.resultado);
        strcat(resultado.codigo, buffer);
    }
    
    // Llamar a parse_double
    snprintf(buffer, sizeof(buffer),
        "    bl parse_double            // Llamar parse_double custom (retorna en d0)\n");
    strcat(resultado.codigo, buffer);
    
    snprintf(resultado.resultado, sizeof(resultado.resultado), "d0");
    resultado.tipo = TIPO_DOUBLE;
    
    printf("[DEBUG PARSEDOUBLE] Código generado para parseDouble runtime\n");
    return resultado;
}

// ======================== PARSEFLOAT ========================
ResultadoExprEmbebida generarParseFloatARM64(GeneradorDeclaraciones* gen, NodoAST* nodo, int* regIndex) {
    ResultadoExprEmbebida resultado = {0};
    memset(resultado.codigo, 0, sizeof(resultado.codigo));
    memset(resultado.resultado, 0, sizeof(resultado.resultado));
    
    if (!gen || !nodo || nodo->tipo != AST_PARSEFLOAT) {
        printf("[ERROR] Nodo inválido para parseFloat\n");
        return resultado;
    }
    
    if (nodo->num_hijos < 1) {
        printf("[ERROR] parseFloat sin argumento\n");
        return resultado;
    }
    
    NodoAST* argumentoNodo = nodo->hijos[0];
    
    printf("[DEBUG PARSEFLOAT] Generando parseFloat\n");
    
    char buffer[512];
    
    // **EVALUAR EL ARGUMENTO**
    ResultadoExprEmbebida argResultado = generarExprEmbebidaARM64(gen, argumentoNodo, regIndex);
    strcat(resultado.codigo, argResultado.codigo);
    
    snprintf(buffer, sizeof(buffer),
        "    // Float.parseFloat(string)\n");
    strcat(resultado.codigo, buffer);
    
    // **CASO 1: STRING LITERAL**
    if (argumentoNodo->tipo == AST_CADENA) {
        const char* valorStr = argumentoNodo->valor;
        
        char valorSinComillas[64];
        if (valorStr[0] == '"' && valorStr[strlen(valorStr)-1] == '"') {
            strncpy(valorSinComillas, valorStr + 1, sizeof(valorSinComillas) - 1);
            valorSinComillas[strlen(valorStr) - 2] = '\0';
        } else {
            strncpy(valorSinComillas, valorStr, sizeof(valorSinComillas) - 1);
        }
        
        char nombreConst[64];
        snprintf(nombreConst, sizeof(nombreConst), "float_parse_%d", gen->contador_literales_string++);
        
        snprintf(buffer, sizeof(buffer), "%s: .float %s\n", nombreConst, valorSinComillas);
        insertarEnSeccionData(gen, buffer);
        
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s\n"
            "    ldr s%d, [x%d]             // parseFloat(\"%s\") optimizado\n",
            *regIndex, nombreConst, *regIndex, *regIndex, valorSinComillas);
        strcat(resultado.codigo, buffer);
        
        snprintf(resultado.resultado, sizeof(resultado.resultado), "s%d", *regIndex);
        (*regIndex)++;
        resultado.tipo = TIPO_FLOAT;
        
        printf("[DEBUG PARSEFLOAT] Optimizado en compile-time: %s\n", valorSinComillas);
        return resultado;
    }
    
    // **CASO 2: VARIABLE - Usar parse_float custom**
    
    if (argumentoNodo->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s\n"
            "    ldr x0, [x%d]              // Cargar puntero al string\n",
            *regIndex, argumentoNodo->valor, *regIndex);
        strcat(resultado.codigo, buffer);
        (*regIndex)++;
    } else if (argResultado.resultado[0] == 'x') {
        snprintf(buffer, sizeof(buffer),
            "    mov x0, %s                 // Puntero al string\n",
            argResultado.resultado);
        strcat(resultado.codigo, buffer);
    }
    
    // Llamar a parse_float
    snprintf(buffer, sizeof(buffer),
        "    bl parse_float             // Llamar parse_float custom (retorna en s0)\n");
    strcat(resultado.codigo, buffer);
    
    snprintf(resultado.resultado, sizeof(resultado.resultado), "s0");
    resultado.tipo = TIPO_FLOAT;
    
    printf("[DEBUG PARSEFLOAT] Código generado para parseFloat runtime\n");
    return resultado;
}