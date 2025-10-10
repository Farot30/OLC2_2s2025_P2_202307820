#include "Asignacion_arm64.h"
#include "Operadores_arm64.h"
#include "declaracionConst_arm64.h"
#include "../semantico/tipos.h"
#include <stdio.h>
#include <string.h>

// Función para generar asignaciones en ARM64
void generarAsignacionARM64(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_ASIGNACION) {
        printf("[DEBUG] Nodo inválido o tipo incorrecto en generarAsignacionARM64\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[DEBUG] Nodo de asignación con menos de 2 hijos\n");
        return;
    }

    NodoAST* idNodo = nodo->hijos[0];  // Nodo con el identificador de la variable
    NodoAST* valorNodo = nodo->hijos[1];  // Nodo con el valor a asignar

    if (!idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[DEBUG] Nodo identificador inválido en generarAsignacionARM64\n");
        return;
    }

    const char* nombreVariable = idNodo->valor;  // Nombre de la variable
    TipoDato tipoVariable = deducirTipoExpr(valorNodo);  // Deducir el tipo del valor a asignar

    char codigo[512] = "";

    // **DETECTAR SI EL VALOR ES UNA OPERACIÓN**
    if (valorNodo->tipo == AST_OPERACION) {
        printf("[DEBUG] Detectada operación en asignación de '%s'\n", nombreVariable);
        generarCodigoOperacionARM64(gen, nombreVariable, valorNodo, tipoVariable);
    } else {
        // **VALOR LITERAL**
        char valorStr[64] = "0";

        if (tipoVariable == TIPO_BOOLEAN) {
            if (strcmp(valorNodo->valor, "true") == 0) {
                strncpy(valorStr, "1", sizeof(valorStr));
            } else if (strcmp(valorNodo->valor, "false") == 0) {
                strncpy(valorStr, "0", sizeof(valorStr));
            }
        } else if (tipoVariable == TIPO_STRING) {
            if (valorNodo->valor[0] == '"' && valorNodo->valor[strlen(valorNodo->valor) - 1] == '"') {
                strncpy(valorStr, valorNodo->valor, sizeof(valorStr) - 1);
            } else {
                snprintf(valorStr, sizeof(valorStr), "\"%s\"", valorNodo->valor);
            }
        } else if (tipoVariable == TIPO_CHAR) {
            if (valorNodo->valor[0] == '\'' && valorNodo->valor[strlen(valorNodo->valor) - 1] == '\'') {
                strncpy(valorStr, valorNodo->valor, sizeof(valorStr) - 1);
            } else if (strlen(valorNodo->valor) == 1) {
                snprintf(valorStr, sizeof(valorStr), "'%s'", valorNodo->valor);
            } else {
                printf("[DEBUG] Valor de char inválido para variable '%s'\n", nombreVariable);
                strncpy(valorStr, "'\\0'", sizeof(valorStr));
            }
        } else {
            strncpy(valorStr, valorNodo->valor, sizeof(valorStr) - 1);
        }

        valorStr[sizeof(valorStr) - 1] = '\0';

        // Generar código para asignar el valor
        if (tipoVariable == TIPO_INT) {
            snprintf(codigo, sizeof(codigo),
                "    mov w0, #%s\n"
                "    ldr x1, =%s\n"
                "    str w0, [x1]\n",
                valorStr, nombreVariable);
        } else if (tipoVariable == TIPO_FLOAT) {
            snprintf(codigo, sizeof(codigo),
                "    fmov s0, #%s\n"
                "    ldr x1, =%s\n"
                "    str s0, [x1]\n",
                valorStr, nombreVariable);
        } else if (tipoVariable == TIPO_DOUBLE) {
            snprintf(codigo, sizeof(codigo),
                "    fmov d0, #%s\n"
                "    ldr x1, =%s\n"
                "    str d0, [x1]\n",
                valorStr, nombreVariable);
        } else if (tipoVariable == TIPO_CHAR) {
            snprintf(codigo, sizeof(codigo),
                "    mov w0, #%d\n"
                "    ldr x1, =%s\n"
                "    strb w0, [x1]\n",
                valorNodo->valor[0], nombreVariable);
        } else if (tipoVariable == TIPO_STRING) {
            char nombreConst[64];
            snprintf(nombreConst, sizeof(nombreConst), "str_const_%d", gen->admin_registros->contador_constantes++);
            snprintf(codigo, sizeof(codigo),
                "    ldr x0, =%s\n"
                "    ldr x1, =%s\n"
                "    str x0, [x1]\n",
                nombreConst, nombreVariable);
            insertarEnSeccionData(gen, nombreConst);
        }

        // Insertar el código generado en la sección .text
        insertarEnSeccionText(gen, codigo);
    }
}