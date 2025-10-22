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

bool esConcatenacionSimple(NodoAST* expr) {
    if (!expr || expr->tipo != AST_OPERACION || strcmp(expr->valor, "+") != 0) return false;
    if (expr->num_hijos != 2) return false;
    TipoDato tipoIzq = deducirTipoExprARM64(expr->hijos[0], astGlobal);
    TipoDato tipoDer = deducirTipoExprARM64(expr->hijos[1], astGlobal);

    bool izqEsCadena = (expr->hijos[0]->tipo == AST_CADENA) ||
                       (expr->hijos[0]->tipo == AST_IDENTIFICADOR && tipoIzq == TIPO_STRING);
    bool derEsCadena = (expr->hijos[1]->tipo == AST_CADENA) ||
                       (expr->hijos[1]->tipo == AST_IDENTIFICADOR && tipoDer == TIPO_STRING);

    return (izqEsCadena && derEsCadena);
}

static void insertarSeccionValue(GeneradorDeclaraciones* gen) {
    if (!esVariableAscizImpresion(gen, "temp_str_valueof")) {
        insertarEnSeccionBss(gen, "temp_str_valueof: .skip 32\n");
    }
}


ResultadoExprEmbebida generarExprEmbebidaARM64(GeneradorDeclaraciones* gen, NodoAST* expr, int* regIndex) {
    ResultadoExprEmbebida res = {0};
    if (!expr) return res;

    if (esConcatenacionSimple(expr)) {
        printf("[DEBUG CONCAT SIMPLE] Se genera printf optimizado\n");
        char nombreFormato[64];
        snprintf(nombreFormato, sizeof(nombreFormato), "format_concat_%d", *regIndex);

        char formato[256] = "";
        if (expr->hijos[0]->tipo == AST_CADENA)
            snprintf(formato, sizeof(formato), "%s%%s\\n", expr->hijos[0]->valor);
        else if (expr->hijos[1]->tipo == AST_CADENA)
            snprintf(formato, sizeof(formato), "%%s%s\\n", expr->hijos[1]->valor);
        else
            snprintf(formato, sizeof(formato), "%%s%%s\\n");

        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%s\"\n", nombreFormato, formato);
        insertarEnSeccionData(gen, buffer);

        ResultadoExprEmbebida resStrIzq = generarExprEmbebidaARM64(gen, expr->hijos[0], regIndex);
        ResultadoExprEmbebida resStrDer = generarExprEmbebidaARM64(gen, expr->hijos[1], regIndex);

        if (expr->hijos[0]->tipo == AST_IDENTIFICADOR && expr->hijos[1]->tipo == AST_IDENTIFICADOR) {
            snprintf(res.codigo, sizeof(res.codigo),
                "    ldr x1, =%s\n"
                "    ldr x1, [x1]\n"
                "    ldr x2, =%s\n"
                "    ldr x2, [x2]\n"
                "    ldr x0, =%s\n"
                "    bl printf\n",
                resStrIzq.resultado, resStrDer.resultado, nombreFormato);
        } else {
            ResultadoExprEmbebida resStr = (expr->hijos[0]->tipo == AST_IDENTIFICADOR || expr->hijos[0]->tipo == AST_CADENA) ? resStrDer : resStrIzq;
            snprintf(res.codigo, sizeof(res.codigo),
                "    ldr x1, =%s\n"
                "    ldr x1, [x1]\n"
                "    ldr x0, =%s\n"
                "    bl printf\n",
                resStr.resultado, nombreFormato);
        }
        strcpy(res.resultado, "");
        res.tipo = TIPO_STRING;
        (*regIndex)++;
        return res;
    }

    // Concatenación compleja: más de dos partes o anidada
    if (expr->tipo == AST_OPERACION && contieneStringEnOperacion(expr) && !esConcatenacionSimple(expr) && strcmp(expr->valor, "+") == 0) {
        // Solo usa StringBuilder si NO es concatenación simple
        char nombreTemp[64];
        snprintf(nombreTemp, sizeof(nombreTemp), "__temp_concat_%d", *regIndex);
        printf("[DEBUG CONCAT COMPLEJA] Generando StringBuilder para variable '%s'\n", nombreTemp);
        generarConcatenacionStringBuilder(gen, nombreTemp, expr);
        snprintf(res.codigo, sizeof(res.codigo),
            "    ldr x%d, =%s\n"
            "    ldr x%d, [x%d]\n", *regIndex, nombreTemp, *regIndex, *regIndex);
        snprintf(res.resultado, sizeof(res.resultado), "x%d", *regIndex);
        res.tipo = TIPO_STRING;
        (*regIndex)++;
        return res;
    }
    // 1. Operaciones aritméticas o concatenación
    if (expr->tipo == AST_OPERACION) {

            // --- OPERACIÓN ARITMÉTICA ---
            char buffer[256];
            ResultadoExprEmbebida izq = generarExprEmbebidaARM64(gen, expr->hijos[0], regIndex);
            ResultadoExprEmbebida der = generarExprEmbebidaARM64(gen, expr->hijos[1], regIndex);
            strcat(res.codigo, izq.codigo);
            strcat(res.codigo, der.codigo);
            int regRes = *regIndex;
            // Puedes mejorar esto para soportar otros operadores
            if (strcmp(expr->valor, "+") == 0)
                snprintf(buffer, sizeof(buffer), "    add w%d, %s, %s\n", regRes-2, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "-") == 0)
                snprintf(buffer, sizeof(buffer), "    sub w%d, %s, %s\n", regRes-2, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "*") == 0)
                snprintf(buffer, sizeof(buffer), "    mul w%d, %s, %s\n", regRes-2, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "/") == 0)
                snprintf(buffer, sizeof(buffer), "    sdiv w%d, %s, %s\n", regRes-2, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "%") == 0) {
                // ARM64: a % b = a - (a / b) * b
                // Usamos w1 para a, w2 para b, w3 para resultado
                snprintf(buffer, sizeof(buffer),
                    "    udiv w3, %s, %s\n"   // w3 = a / b
                    "    msub w1, w3, %s, %s\n", // w1 = a - (a / b) * b
                    izq.resultado, der.resultado, der.resultado, izq.resultado);
                strcat(res.codigo, buffer);
                snprintf(res.resultado, sizeof(res.resultado), "w1");
                res.tipo = TIPO_INT;
                (*regIndex)++;
                return res;
            }else if (strcmp(expr->valor, "&") == 0)
                snprintf(buffer, sizeof(buffer), "    and w%d, %s, %s\n", regRes-2, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "|") == 0)
                snprintf(buffer, sizeof(buffer), "    orr w%d, %s, %s\n", regRes-2, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "^") == 0)
                snprintf(buffer, sizeof(buffer), "    eor w%d, %s, %s\n", regRes-2, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "<<") == 0)
                snprintf(buffer, sizeof(buffer), "    lsl w%d, %s, %s\n", regRes-2, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, ">>") == 0)
                snprintf(buffer, sizeof(buffer), "    lsr w%d, %s, %s\n", regRes-2, izq.resultado, der.resultado);
            else if (strcmp(expr->valor, "==") == 0) {
                if (izq.tipo == TIPO_STRING && der.tipo == TIPO_STRING) {
                    char buffer[256];

                    const char* izq_nombre = expr->hijos[0]->tipo == AST_IDENTIFICADOR ? expr->hijos[0]->valor : izq.resultado;
                    const char* der_nombre = expr->hijos[1]->tipo == AST_IDENTIFICADOR ? expr->hijos[1]->valor : der.resultado;

                    snprintf(buffer, sizeof(buffer),
                        "    ldr x0, =%s\n"
                        "    ldr x0, [x0]\n"
                        "    ldr x1, =%s\n"
                        "    ldr x1, [x1]\n"
                        "    bl strcmp\n"
                        "    cmp w0, #0\n"
                        "    cset w%d, eq\n"
                        "    mov w0, w%d\n",
                        izq_nombre, der_nombre, *regIndex, *regIndex);
                }
                    snprintf(buffer, sizeof(buffer),
                        "    cmp %s, %s\n"
                        "    cset w%d, eq\n", izq.resultado, der.resultado, regRes-2);
                    strcat(res.codigo, buffer);
                    snprintf(res.resultado, sizeof(res.resultado), "w%d", regRes-2);
                    res.tipo = TIPO_BOOLEAN;
                    (*regIndex)++;
                    return res;
                }
            else if (strcmp(expr->valor, "!=") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp %s, %s\n"
                    "    cset w%d, ne\n", izq.resultado, der.resultado, regRes-2);
            strcat(res.codigo, buffer);
            snprintf(res.resultado, sizeof(res.resultado), "w%d", regRes-2);
            res.tipo = TIPO_BOOLEAN;
            (*regIndex)++;
            return res;
            }
            else if (strcmp(expr->valor, "<") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp %s, %s\n"
                    "    cset w%d, lt\n", izq.resultado, der.resultado, regRes-2);
                strcat(res.codigo, buffer);
                snprintf(res.resultado, sizeof(res.resultado), "w%d", regRes-2);
                res.tipo = TIPO_BOOLEAN;
                (*regIndex)++;
                return res;
            }
            else if (strcmp(expr->valor, "<=") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp %s, %s\n"
                    "    cset w%d, le\n", izq.resultado, der.resultado, regRes-2);
                strcat(res.codigo, buffer);
                snprintf(res.resultado, sizeof(res.resultado), "w%d", regRes-2);
                res.tipo = TIPO_BOOLEAN;
                (*regIndex)++;
                return res;
            }
            else if (strcmp(expr->valor, ">") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp %s, %s\n"
                    "    cset w%d, gt\n", izq.resultado, der.resultado, regRes-2);
                strcat(res.codigo, buffer);
                snprintf(res.resultado, sizeof(res.resultado), "w%d", regRes-2);
                res.tipo = TIPO_BOOLEAN;
                (*regIndex)++;
                return res;
            }
            else if (strcmp(expr->valor, ">=") == 0) {
                snprintf(buffer, sizeof(buffer),
                    "    cmp %s, %s\n"
                    "    cset w%d, ge\n", izq.resultado, der.resultado, regRes-2);
                strcat(res.codigo, buffer);
                snprintf(res.resultado, sizeof(res.resultado), "w%d", regRes-2);
                res.tipo = TIPO_BOOLEAN;
                (*regIndex)++;
                return res;
            }else if (strcmp(expr->valor, "&&") == 0) {
                // wA = izq.resultado, wB = der.resultado, wN = resultado
                snprintf(buffer, sizeof(buffer),
                    "    and w%d, %s, %s\n"
                    "    cmp w%d, #0\n"
                    "    cset w%d, ne\n",
                    regRes-2, izq.resultado, der.resultado,
                    regRes-2, regRes-2);
                strcat(res.codigo, buffer);
                snprintf(res.resultado, sizeof(res.resultado), "w%d", regRes-2);
                res.tipo = TIPO_BOOLEAN;
                (*regIndex)++;
                return res;
            } else if (strcmp(expr->valor, "||") == 0) {
                // wA = izq.resultado, wB = der.resultado, wN = resultado
                snprintf(buffer, sizeof(buffer),
                    "    orr w%d, %s, %s\n"
                    "    cmp w%d, #0\n"
                    "    cset w%d, ne\n",
                    regRes-2, izq.resultado, der.resultado,
                    regRes-2, regRes-2);
                strcat(res.codigo, buffer);
                snprintf(res.resultado, sizeof(res.resultado), "w%d", regRes-2);
                res.tipo = TIPO_BOOLEAN;
                (*regIndex)++;
                return res;
            } else if (strcmp(expr->valor, "!") == 0) {
                // wA = resultado del hijo, wN = resultado
                ResultadoExprEmbebida hijo = generarExprEmbebidaARM64(gen, expr->hijos[0], regIndex);
                strcat(res.codigo, hijo.codigo);
                snprintf(buffer, sizeof(buffer),
                    "    cmp %s, #0\n"
                    "    cset w%d, eq\n",
                    hijo.resultado, *regIndex);
                strcat(res.codigo, buffer);
                snprintf(res.resultado, sizeof(res.resultado), "w%d", *regIndex);
                res.tipo = TIPO_BOOLEAN;
                (*regIndex)++;
                return res;
            }
            else
                snprintf(buffer, sizeof(buffer), "    // Operador no soportado: %s\n", expr->valor);
            strcat(res.codigo, buffer);
            snprintf(res.resultado, sizeof(res.resultado), "w%d", regRes-2);
            res.tipo = TIPO_INT; // Puedes deducir el tipo real si lo necesitas
            (*regIndex)++;
            return res;
        
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
            snprintf(res.resultado, sizeof(res.resultado), "s%d", *regIndex);
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
            snprintf(res.resultado, sizeof(res.resultado), "d0%d", *regIndex);
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
        snprintf(nombreStr, sizeof(nombreStr), "str_lit_%d", gen->contador_literales_string++);
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s: .asciz \"%s\"\n", nombreStr, expr->valor);
        insertarEnSeccionData(gen, buffer);
        res.codigo[0] = '\0'; // No necesitas código para cargar el literal
        snprintf(res.resultado, sizeof(res.resultado), "%s", nombreStr); // Devuelve el nombre del literal
        res.tipo = TIPO_STRING;
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