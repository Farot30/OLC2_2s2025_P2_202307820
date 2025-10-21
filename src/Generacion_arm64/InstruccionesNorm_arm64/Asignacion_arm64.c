#include "Asignacion_arm64.h"
#include "Operadores_arm64.h"
#include "declaracionConst_arm64.h"
#include "../../semantico/tipos.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include <stdio.h>
#include <string.h>

extern NodoAST* astGlobal;
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

    NodoAST* idNodo = nodo->hijos[0];
    NodoAST* valorNodo = nodo->hijos[1];

    if (!idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[DEBUG] Nodo identificador inválido en generarAsignacionARM64\n");
        return;
    }

    const char* nombreVariable = idNodo->valor;
    TipoDato tipoVariable = deducirTipoExprARM64(valorNodo, astGlobal);

    char codigo[512] = "";
    int regIndex = 1;
    ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, valorNodo, &regIndex);
    strcat(codigo, resultado.codigo);

    if (tipoVariable == TIPO_INT) {
        if (valorNodo->tipo == AST_IDENTIFICADOR) {
            snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                "    ldr x1, =%s\n"
                "    ldr w1, [x1]\n"
                "    ldr x2, =%s\n"
                "    str w1, [x2]\n",
                valorNodo->valor, nombreVariable);
        } else {
            snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                "    mov w1, %s\n"
                "    ldr x2, =%s\n"
                "    str w1, [x2]\n",
                resultado.resultado, nombreVariable);
        }
    } else if (tipoVariable == TIPO_FLOAT) {
        snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
            "    ldr x2, =%s\n"
            "    str s0, [x2]\n",
             nombreVariable);
    } else if (tipoVariable == TIPO_DOUBLE) {
        // Asignar double
        snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
            "    fmov d0, %s\n"
            "    ldr x2, =%s\n"
            "    str d0, [x2]\n",
            resultado.resultado, nombreVariable);
    } else if (tipoVariable == TIPO_CHAR) {
        if (valorNodo->tipo == AST_IDENTIFICADOR) {
        snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                "    ldr x1, =%s\n"
                "    ldrb w0, [x1]\n"
                "    ldr x2, =%s\n"
                "    strb w0, [x2]\n",
                valorNodo->valor, nombreVariable);
        } else {
            // Literal o resultado inmediato
            snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
                "    mov w0, %s\n"
                "    ldr x2, =%s\n"
                "    strb w0, [x2]\n",
                resultado.resultado, nombreVariable);
        }
    } else if (tipoVariable == TIPO_BOOLEAN) {
        // Asignar booleano
        snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
            "    mov w1, %s\n"
            "    ldr x2, =%s\n"
            "    strb w1, [x2]\n",
            resultado.resultado, nombreVariable);
    } else if (tipoVariable == TIPO_STRING) {
        // Asignar string (puntero)
        snprintf(codigo + strlen(codigo), sizeof(codigo) - strlen(codigo),
            "    ldr x2, =%s\n"      // x2 = dirección del string (resultado.resultado)
            "    ldr x1, =%s\n"      // x1 = dirección de la variable puntero
            "    str x2, [x1]\n",    // Guarda la dirección en la variable
            resultado.resultado, nombreVariable);
    }

    insertarEnSeccionText(gen, codigo);
}

void generarAsignacionCompuestaARM64(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_ASIGNACION_COMPUESTA) {
        printf("[DEBUG] Nodo inválido para asignación compuesta\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[DEBUG] Asignación compuesta con menos de 2 hijos\n");
        return;
    }

    NodoAST* idNodo = nodo->hijos[0];  // Variable destino
    NodoAST* valorNodo = nodo->hijos[1];  // Valor a operar
    
    if (!idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[DEBUG] Nodo identificador inválido en asignación compuesta\n");
        return;
    }

    const char* nombreVariable = idNodo->valor;
    const char* operador = nodo->valor;  // Operador como "+=", "-=", "*=", "/="
    
    char codigo[512];
    char buffer[256];
    
    // Extraer el operador base (+ de +=, - de -=, etc.)
    char opBase[3] = {0};
    if (strlen(operador) >= 2) {
        strncpy(opBase, operador, strlen(operador) - 1);
    }
    
    snprintf(buffer, sizeof(buffer), "    // Asignación compuesta: %s %s <valor>\n", nombreVariable, operador);
    strcpy(codigo, buffer);
    
    // **PASO 1: Cargar valor actual de la variable**
    snprintf(buffer, sizeof(buffer),
        "    ldr x1, =%s\n"
        "    ldr w0, [x1]\n", nombreVariable);
    strcat(codigo, buffer);
    
    // **PASO 2: Cargar el valor operando**
    if (valorNodo->tipo == AST_NUMERO) {
        snprintf(buffer, sizeof(buffer), "    mov w1, #%d\n", atoi(valorNodo->valor));
        strcat(codigo, buffer);
    } else if (valorNodo->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x2, =%s\n"
            "    ldr w1, [x2]\n", valorNodo->valor);
        strcat(codigo, buffer);
    } else if (valorNodo->tipo == AST_OPERACION) {
        // Para operaciones más complejas, usar función recursiva
        int regIndex = 2;
        generarCodigoRecursivoOperacion(valorNodo, codigo, &regIndex);
        snprintf(buffer, sizeof(buffer), "    mov w1, w0\n    ldr x1, =%s\n    ldr w0, [x1]\n", nombreVariable);
        strcat(codigo, buffer);
    }
    
    // **PASO 3: Realizar la operación**
    if (strcmp(opBase, "+") == 0) {
        snprintf(buffer, sizeof(buffer), "    add w0, w0, w1\n");
    } else if (strcmp(opBase, "-") == 0) {
        snprintf(buffer, sizeof(buffer), "    sub w0, w0, w1\n");
    } else if (strcmp(opBase, "*") == 0) {
        snprintf(buffer, sizeof(buffer), "    mul w0, w0, w1\n");
    } else if (strcmp(opBase, "/") == 0) {
        snprintf(buffer, sizeof(buffer), "    sdiv w0, w0, w1\n");
    } else if (strcmp(opBase, "&") == 0) {
        snprintf(buffer, sizeof(buffer), "    and w0, w0, w1\n");
    } else if (strcmp(opBase, "|") == 0) {
        snprintf(buffer, sizeof(buffer), "    orr w0, w0, w1\n");
    } else if (strcmp(opBase, "^") == 0) {
        snprintf(buffer, sizeof(buffer), "    eor w0, w0, w1\n");
    }
    strcat(codigo, buffer);
    
    // **PASO 4: Guardar resultado de vuelta en la variable**
    snprintf(buffer, sizeof(buffer),
        "    ldr x1, =%s\n"
        "    str w0, [x1]\n", nombreVariable);
    strcat(codigo, buffer);
    
    insertarEnSeccionText(gen, codigo);
}