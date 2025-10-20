#include "EvaluadorExpr_arm64.h"
#include "../InstruccionesNorm_arm64/Operadores_arm64.h"
#include "../Embebidas/String_valueOf.h"
#include "../InstruccionesNorm_arm64/StringBuilder.h"
#include "../Impresion_arm64.h"
#include <stdio.h>
#include <string.h>

extern NodoAST* astGlobal;

// Detecta si hay string en la operación (para distinguir concat de suma)
static bool contieneStringEnOperacion(NodoAST* nodo) {
    if (!nodo) return false;
    TipoDato tipo = deducirTipoExprARM64(nodo, astGlobal);
    if (tipo == TIPO_STRING) return true;
    for (int i = 0; i < nodo->num_hijos; i++) {
        if (contieneStringEnOperacion(nodo->hijos[i])) return true;
    }
    return false;
}

static void insertarSeccionValue(GeneradorDeclaraciones* gen) {
    if (!esVariableAscizImpresion(gen, "temp_str_valueof")) {
        insertarEnSeccionBss(gen, "temp_str_valueof: .skip 32\n");
    }
}

ResultadoExprEmbebida generarExprEmbebidaARM64(GeneradorDeclaraciones* gen, NodoAST* expr, int* regIndex) {
    ResultadoExprEmbebida res = {0};
    if (!expr) return res;

    // 1. Operaciones aritméticas o concatenación
    if (expr->tipo == AST_OPERACION) {
        if (contieneStringEnOperacion(expr)) {
            // --- CONCATENACIÓN ---
            // Aquí deberías llamar a tu función real de concatenación ARM64
            // Por ejemplo, usando StringBuilder:
            char nombreTemp[64];
            snprintf(nombreTemp, sizeof(nombreTemp), "__temp_concat_%d", *regIndex);
            generarConcatenacionStringBuilder(gen, nombreTemp, expr);
            snprintf(res.codigo, sizeof(res.codigo),
                "    ldr x%d, =%s\n"
                "    ldr x%d, [x%d]\n", *regIndex, nombreTemp, *regIndex, *regIndex);
            snprintf(res.resultado, sizeof(res.resultado), "x%d", *regIndex);
            res.tipo = TIPO_STRING;
            (*regIndex)++;
            return res;
        } else {
            // --- OPERACIÓN ARITMÉTICA ---
            char buffer[256];
            ResultadoExprEmbebida izq = generarExprEmbebidaARM64(gen, expr->hijos[0], regIndex);
            ResultadoExprEmbebida der = generarExprEmbebidaARM64(gen, expr->hijos[1], regIndex);
            strcat(res.codigo, izq.codigo);
            strcat(res.codigo, der.codigo);
            int regRes = *regIndex;
            // Puedes mejorar esto para soportar otros operadores
            if (strcmp(expr->valor, "+") == 0)
                snprintf(buffer, sizeof(buffer), "    add w%d, %s, %s\n", regRes, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "-") == 0)
                snprintf(buffer, sizeof(buffer), "    sub w%d, %s, %s\n", regRes, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "*") == 0)
                snprintf(buffer, sizeof(buffer), "    mul w%d, %s, %s\n", regRes, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "/") == 0)
                snprintf(buffer, sizeof(buffer), "    sdiv w%d, %s, %s\n", regRes, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "%") == 0)
                snprintf(buffer, sizeof(buffer), "    srem w%d, %s, %s\n", regRes, izq.resultado, der.resultado);
            else 
                snprintf(buffer, sizeof(buffer), "    // Operador no soportado: %s\n", expr->valor);
            strcat(res.codigo, buffer);
            snprintf(res.resultado, sizeof(res.resultado), "w%d", regRes);
            res.tipo = TIPO_INT; // Puedes deducir el tipo real si lo necesitas
            (*regIndex)++;
            return res;
        }
    }

    // 2. Embebidas: String.valueOf, parseInt, etc.
    if (expr->tipo == AST_VALUEOF && expr->num_hijos == 1) {
        insertarSeccionValue(gen);
        ResultadoExprEmbebida hijo = generarExprEmbebidaARM64(gen, expr->hijos[0], regIndex);
        // Primero, copia el código generado por el hijo
        strcat(res.codigo, hijo.codigo);
        // Luego, agrega las instrucciones ARM64 para valueOf_int
        char buffer[256];
        if(hijo.tipo == TIPO_INT) {
            snprintf(buffer, sizeof(buffer),
                "    mov w0, %s\n"
                "    ldr x1, =temp_str_valueof\n"
                "    bl valueOf_int\n",
                hijo.resultado);
            strcat(res.codigo, buffer);
            strcpy(res.resultado, "x0"); // x0 tendrá la dirección del string
            res.tipo = TIPO_STRING;
            return res;
        }
        else if(hijo.tipo == TIPO_BOOLEAN) {
            snprintf(buffer, sizeof(buffer),
                "    mov w0, %s\n"
                "    ldr x1, =temp_str_valueof\n"
                "    bl valueOf_bool\n",
                hijo.resultado);
        } else if(hijo.tipo == TIPO_FLOAT) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =temp_str_valueof\n"
                "    bl valueOf_float\n",
                hijo.resultado);
        } else if(hijo.tipo == TIPO_DOUBLE) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x1, =temp_str_valueof\n"
                "    bl valueOf_double\n",
                hijo.resultado);
        }
        strcat(res.codigo, buffer);
        strcpy(res.resultado, "x0"); // x0 tendrá la dirección del string
        res.tipo = TIPO_STRING;
        return res;
    }

    // 3. Literales y variables
    if (expr->tipo == AST_NUMERO) {
        char buffer[256];
        TipoDato tipo = deducirTipoExprARM64(expr, astGlobal);

        if (tipo == TIPO_INT) {
            snprintf(buffer, sizeof(buffer), "    mov w%d, #%s\n", *regIndex, expr->valor);
            strcat(res.codigo, buffer);
            snprintf(res.resultado, sizeof(res.resultado), "w%d", *regIndex);
            res.tipo = TIPO_INT;
            (*regIndex)++;
            return res;
        }
        else if (tipo == TIPO_FLOAT) {
            // Quitar la 'f' si viene en el literal
            char valorFloat[32];
            strncpy(valorFloat, expr->valor, sizeof(valorFloat));
            valorFloat[sizeof(valorFloat)-1] = '\0';
            size_t len = strlen(valorFloat);
            if (len > 0 && (valorFloat[len-1] == 'f' || valorFloat[len-1] == 'F')) {
                valorFloat[len-1] = '\0';
            }
            // Crear constante en .data
            char nombreConst[64];
            snprintf(nombreConst, sizeof(nombreConst), "float_const_%d", *regIndex);
            snprintf(buffer, sizeof(buffer), "%s: .float %s\n", nombreConst, valorFloat);
            insertarEnSeccionData(gen, buffer);

            // Cargar el valor en sN
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr s0, [x%d]\n", *regIndex, nombreConst, *regIndex);
            strcat(res.codigo, buffer);
            snprintf(res.resultado, sizeof(res.resultado), "s0");
            res.tipo = TIPO_FLOAT;
            (*regIndex)++;
            return res;
        }
        else if (tipo == TIPO_DOUBLE) {
            // Quitar la 'd' si viene en el literal (opcional)
            char valorDouble[32];
            strncpy(valorDouble, expr->valor, sizeof(valorDouble));
            valorDouble[sizeof(valorDouble)-1] = '\0';
            size_t len = strlen(valorDouble);
            if (valorDouble[len-1] == 'd' || valorDouble[len-1] == 'D') {
                valorDouble[len-1] = '\0';
            }
            // Crear constante en .data
            char nombreConst[64];
            snprintf(nombreConst, sizeof(nombreConst), "double_const_%d", *regIndex);
            snprintf(buffer, sizeof(buffer), "%s: .double %s\n", nombreConst, valorDouble);
            insertarEnSeccionData(gen, buffer);

            // Cargar el valor en dN
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr d0, [x%d]\n", *regIndex, nombreConst, *regIndex);
            strcat(res.codigo, buffer);
            snprintf(res.resultado, sizeof(res.resultado), "d0");
            res.tipo = TIPO_DOUBLE;
            (*regIndex)++;
            return res;
        }
    }
    if (expr->tipo == AST_CARACTER) {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", *regIndex, (int)expr->valor[0]);
        strcat(res.codigo, buffer);
        snprintf(res.resultado, sizeof(res.resultado), "w%d", *regIndex);
        res.tipo = TIPO_CHAR;
        (*regIndex)++;
        return res;
    }
    if (expr->tipo == AST_BOOLEAN) {
        char buffer[128];
        int valor = (strcmp(expr->valor, "true") == 0) ? 1 : 0;
        snprintf(buffer, sizeof(buffer), "    mov w%d, #%d\n", *regIndex, valor);
        strcat(res.codigo, buffer);
        snprintf(res.resultado, sizeof(res.resultado), "w%d", *regIndex);
        res.tipo = TIPO_BOOLEAN;
        (*regIndex)++;
        return res;
    }
    if (expr->tipo == AST_IDENTIFICADOR) {
        char buffer[128];
        TipoDato tipo = deducirTipoExprARM64(expr, astGlobal);
        if (tipo == TIPO_INT) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr w%d, [x%d]\n", *regIndex, expr->valor, *regIndex, *regIndex);
        } else if (tipo == TIPO_CHAR) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldrb w%d, [x%d]\n", *regIndex, expr->valor, *regIndex, *regIndex);
        } else if (tipo == TIPO_BOOLEAN) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldrb w%d, [x%d]\n", *regIndex, expr->valor, *regIndex, *regIndex);
        } else if (tipo == TIPO_STRING) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr x%d, [x%d]\n", *regIndex, expr->valor, *regIndex, *regIndex);
        } else if (tipo == TIPO_FLOAT) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr s%d, [x%d]\n", *regIndex, expr->valor, *regIndex - 1, *regIndex);
        } else if (tipo == TIPO_DOUBLE) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr d%d, [x%d]\n", *regIndex, expr->valor, *regIndex - 1, *regIndex);
        }
        strcat(res.codigo, buffer);
        snprintf(res.resultado, sizeof(res.resultado), "%s%d", (tipo == TIPO_STRING ? "x" : "w"), *regIndex);
        res.tipo = tipo;
        (*regIndex)++;
        return res;
    }
    if (expr->tipo == AST_CADENA) {
        // String literal
        char nombreStr[64];
        snprintf(nombreStr, sizeof(nombreStr), "str_lit_%d", (*regIndex));
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s: .asciz %s\n", nombreStr, expr->valor);
        insertarEnSeccionData(gen, buffer);
        snprintf(buffer, sizeof(buffer), "    ldr x%d, =%s\n", *regIndex, nombreStr);
        strcat(res.codigo, buffer);
        snprintf(res.resultado, sizeof(res.resultado), "x%d", *regIndex);
        res.tipo = TIPO_STRING;
        (*regIndex)++;
        return res;
    }
    if (expr->tipo == AST_EQUAL) {
        // Suponiendo que tus hijos son los dos strings a comparar
        ResultadoExprEmbebida izq = generarExprEmbebidaARM64(gen, expr->hijos[0], regIndex);
        ResultadoExprEmbebida der = generarExprEmbebidaARM64(gen, expr->hijos[1], regIndex);

        // Genera el código para comparar cadenas (puedes usar strcmp embebido o tu propia rutina)
        char buffer[256];
        strcat(res.codigo, izq.codigo);
        strcat(res.codigo, der.codigo);

        // Ejemplo usando strcmp de C embebido (ajusta si tienes tu propia rutina)
        snprintf(buffer, sizeof(buffer),
            "    mov x0, %s\n"   // dirección string 1
            "    mov x1, %s\n"   // dirección string 2
            "    bl strcmp\n"    // strcmp(x0, x1)
            "    cmp w0, #0\n"
            "    cset w%d, eq\n",
            izq.resultado, der.resultado, *regIndex);
        strcat(res.codigo, buffer);

        snprintf(res.resultado, sizeof(res.resultado), "w%d", *regIndex);
        res.tipo = TIPO_BOOLEAN;
        (*regIndex)++;
        return res;
    }

    return res;
}