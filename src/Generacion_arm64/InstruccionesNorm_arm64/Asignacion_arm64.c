#include "Asignacion_arm64.h"
#include "Operadores_arm64.h"
#include "declaracionConst_arm64.h"
#include "../../semantico/tipos.h"
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
    } 
    // **DETECTAR SI ES ASIGNACIÓN DE VARIABLE A VARIABLE**
    else if (valorNodo->tipo == AST_IDENTIFICADOR) {
        printf("[DEBUG] Asignación de variable '%s' = '%s'\n", nombreVariable, valorNodo->valor);
        
        if (tipoVariable == TIPO_INT) {
            snprintf(codigo, sizeof(codigo),
                "    // Asignación: %s = %s\n"
                "    ldr x0, =%s\n"      // Cargar dirección de variable origen
                "    ldr w0, [x0]\n"     // Cargar valor de variable origen
                "    ldr x1, =%s\n"      // Cargar dirección de variable destino
                "    str w0, [x1]\n",    // Guardar valor en variable destino
                nombreVariable, valorNodo->valor, valorNodo->valor, nombreVariable);
        } else if (tipoVariable == TIPO_FLOAT) {
            snprintf(codigo, sizeof(codigo),
                "    // Asignación: %s = %s\n"
                "    ldr x0, =%s\n"
                "    ldr s0, [x0]\n"
                "    ldr x1, =%s\n"
                "    str s0, [x1]\n",
                nombreVariable, valorNodo->valor, valorNodo->valor, nombreVariable);
        } else if (tipoVariable == TIPO_DOUBLE) {
            snprintf(codigo, sizeof(codigo),
                "    // Asignación: %s = %s\n"
                "    ldr x0, =%s\n"
                "    ldr d0, [x0]\n"
                "    ldr x1, =%s\n"
                "    str d0, [x1]\n",
                nombreVariable, valorNodo->valor, valorNodo->valor, nombreVariable);
        } else if (tipoVariable == TIPO_CHAR || tipoVariable == TIPO_BOOLEAN) {
            snprintf(codigo, sizeof(codigo),
                "    // Asignación: %s = %s\n"
                "    ldr x0, =%s\n"
                "    ldrb w0, [x0]\n"
                "    ldr x1, =%s\n"
                "    strb w0, [x1]\n",
                nombreVariable, valorNodo->valor, valorNodo->valor, nombreVariable);
        }
        
        insertarEnSeccionText(gen, codigo);
    } else {
        // **VALOR LITERAL** (tu código existente)
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

        // Generar código para asignar el valor literal
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