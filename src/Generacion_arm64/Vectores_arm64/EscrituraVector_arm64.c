#include "EscrituraVector_arm64.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include "../../semantico/tipos.h"
#include "../../semantico/semantico.h"
#include "../../AST/ast.h"
#include "AccesoVector_arm64.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern NodoAST* astGlobal;

void generarEscrituraVectorARM64(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_ACCESO_ESCRITURA) {
        printf("[ERROR] Nodo inválido para escritura en vector\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] Escritura en vector sin destino o valor\n");
        return;
    }
    //printf("[DEBUG ESCRITURA VECTOR] verificación de hijos pasado\n");

    // Extraer nodo de acceso y valor
    NodoAST* accesoNodo = nodo->hijos[0];
    NodoAST* valorNodo = nodo->hijos[1];

    if (accesoNodo->tipo != AST_ACCESO || accesoNodo->num_hijos < 2) {
        printf("[ERROR] Estructura de acceso inválida en escritura\n");
        return;
    }

    //printf("[DEBUG ESCRITURA VECTOR] verificación de estructura de acceso pasado\n");

    // Extraer identificador del vector
    NodoAST* idNodo = accesoNodo->hijos[0];
    NodoAST* dimensionesNodo = accesoNodo->hijos[1];

    //printf("[DEBUG ESCRITURA VECTOR] identificador y dimensiones extraídos\n");
    if (idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Identificador inválido en escritura de vector\n");
        return;
    }

    const char* nombreVector = idNodo->valor;

    // ✅ BUSCAR EL VECTOR EN EL AST EN LUGAR DE LA TABLA DE SÍMBOLOS
    NodoAST* vectorNodo = buscarVectorEnAST(nombreVector);
    if (!vectorNodo) {
        printf("[ERROR] Vector '%s' no encontrado en el AST\n", nombreVector);
        return;
    }

    //printf("[DEBUG ACCESO VECTOR] Vector '%s' encontrado en AST\n", nombreVector);

    // ✅ OBTENER EL TIPO BASE DEL VECTOR DESDE EL AST
    TipoDato tipoVector = obtenerTipoVectorDesdeAST(vectorNodo);
    if (tipoVector == TIPO_DESCONOCIDO) {  // ✅ Cambiar verificación
        printf("[ERROR] No se pudo determinar el tipo del vector '%s'\n", nombreVector);
        return;
    }
    //printf("[DEBUG ACCESO VECTOR] Vector '%s' encontrado en AST, tipo: %d\n", nombreVector, tipoVector);

    printf("[DEBUG ACCESO VECTOR] Generando acceso a '%s'\n", nombreVector);

    // Obtener el índice
    if (dimensionesNodo->tipo != AST_DIMENSIONES || dimensionesNodo->num_hijos < 1) {
        printf("[ERROR] Dimensiones inválidas en escritura\n");
        return;
    }

    NodoAST* dimNodo = NULL;
    for (int i = 0; i < dimensionesNodo->num_hijos; i++) {
        if (dimensionesNodo->hijos[i]->tipo == AST_1DIMENSION) {
            dimNodo = dimensionesNodo->hijos[i];
            break;
        }
    }

    if (!dimNodo || dimNodo->num_hijos < 1) {
        printf("[ERROR] No se encontró dimensión válida en escritura\n");
        return;
    }

    NodoAST* indiceNodo = dimNodo->hijos[0];

    printf("[DEBUG ESCRITURA VECTOR] Escribiendo en '%s[índice]'\n", nombreVector);

    // Determinar el tamaño del elemento según el tipo
    int tamElemento = 4;
    const char* instruccionStore = "str";
    const char* sufijoStore = "w";
    int shift = 2;

    TipoDato tipo = tipoVector;

    if (tipo == TIPO_DOUBLE) {
        tamElemento = 8;
        sufijoStore = "d";
        shift = 3;
    } else if (tipo == TIPO_FLOAT) {
        tamElemento = 4;
        sufijoStore = "s";
        shift = 2;
    } else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) {
        tamElemento = 1;
        instruccionStore = "strb";
        sufijoStore = "w";
        shift = 0;
    } else if (tipo == TIPO_STRING) {
        tamElemento = 8;
        sufijoStore = "x";
        shift = 3;
    }

    char buffer[512];
    char codigo[2048] = "";

    // **PASO 1: Evaluar el índice recursivamente**
    snprintf(buffer, sizeof(buffer),
        "    // Escritura en vector: %s[índice] = valor\n",
        nombreVector);
    strcat(codigo, buffer);

    int regIndex = 1;
    ResultadoExprEmbebida indiceResultado = generarExprEmbebidaARM64(gen, indiceNodo, &regIndex);
    strcat(codigo, indiceResultado.codigo);

    // El índice está en el registro indicado por indiceResultado.resultado
    char regIndiceStr[16];
    if (indiceResultado.resultado[0] == 'w') {
        strncpy(regIndiceStr, indiceResultado.resultado, sizeof(regIndiceStr) - 1);
    } else {
        snprintf(regIndiceStr, sizeof(regIndiceStr), "w%d", regIndex++);
        snprintf(buffer, sizeof(buffer),
            "    mov %s, %s\n", regIndiceStr, indiceResultado.resultado);
        strcat(codigo, buffer);
    }

    // **PASO 2: Evaluar el valor a asignar**
    ResultadoExprEmbebida valorResultado = generarExprEmbebidaARM64(gen, valorNodo, &regIndex);
    strcat(codigo, valorResultado.codigo);

    // **PASO 3: Verificar compatibilidad de tipos**
    TipoDato tipoValor = valorResultado.tipo;
    
    if (tipoValor != tipo) {
        // Intentar conversión automática si es widening
        if (esWidening(tipoValor, tipo)) {
            printf("[DEBUG ESCRITURA VECTOR] Conversión automática de %d a %d\n", tipoValor, tipo);
            
            // Generar código de conversión
            if (tipoValor == TIPO_INT && tipo == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer),
                    "    scvtf s%d, %s\n", regIndex, valorResultado.resultado);
                strcat(codigo, buffer);
                snprintf(valorResultado.resultado, sizeof(valorResultado.resultado), "s%d", regIndex);
                regIndex++;
            } else if (tipoValor == TIPO_INT && tipo == TIPO_DOUBLE) {
                snprintf(buffer, sizeof(buffer),
                    "    scvtf d%d, %s\n", regIndex, valorResultado.resultado);
                strcat(codigo, buffer);
                snprintf(valorResultado.resultado, sizeof(valorResultado.resultado), "d%d", regIndex);
                regIndex++;
            } else if (tipoValor == TIPO_FLOAT && tipo == TIPO_DOUBLE) {
                snprintf(buffer, sizeof(buffer),
                    "    fcvt d%d, %s\n", regIndex, valorResultado.resultado);
                strcat(codigo, buffer);
                snprintf(valorResultado.resultado, sizeof(valorResultado.resultado), "d%d", regIndex);
                regIndex++;
            }
        } else {
            printf("[ERROR] Tipo incompatible en asignación a vector: %d vs %d\n", tipoValor, tipo);
            return;
        }
    }

    // **PASO 4: Cargar dirección base y almacenar**
    int regBase = regIndex++;
    snprintf(buffer, sizeof(buffer),
        "    ldr x%d, =%s           // Dirección base del vector\n",
        regBase, nombreVector);
    strcat(codigo, buffer);

    // Almacenar con indexación escalada
    if (shift > 0) {
        snprintf(buffer, sizeof(buffer),
            "    %s %s, [x%d, %s, SXTW #%d]  // Almacenar valor\n",
            instruccionStore, valorResultado.resultado, regBase, regIndiceStr, shift);
    } else {
        snprintf(buffer, sizeof(buffer),
            "    %s %s, [x%d, %s, SXTW]      // Almacenar valor\n",
            instruccionStore, valorResultado.resultado, regBase, regIndiceStr);
    }
    strcat(codigo, buffer);

    // Insertar código completo
    insertarEnSeccionText(gen, codigo);

    /*
    int regBase = regIndex++;
    
    // **✅ VERIFICAR SI ES UN PUNTERO**
    char patronPuntero[128];
    snprintf(patronPuntero, sizeof(patronPuntero), "%s: .quad", nombreVector);
    
    if (strstr(gen->buffer, patronPuntero)) {
        // Es un puntero
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s           // Dirección del puntero\n"
            "    ldr x%d, [x%d]          // ✅ Desreferenciar\n",
            regBase, nombreVector, regBase, regBase);
    } else {
        // Vector directo
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s           // Dirección base del vector\n",
            regBase, nombreVector);
    }
    strcat(codigo, buffer);

    // Almacenar con indexación escalada
    if (shift > 0) {
        snprintf(buffer, sizeof(buffer),
            "    %s %s, [x%d, %s, SXTW #%d]  // Almacenar valor\n",
            instruccionStore, valorResultado.resultado, regBase, regIndiceStr, shift);
    } else {
        snprintf(buffer, sizeof(buffer),
            "    %s %s, [x%d, %s, SXTW]      // Almacenar valor\n",
            instruccionStore, valorResultado.resultado, regBase, regIndiceStr);
    }
    strcat(codigo, buffer);*/

    printf("[DEBUG ESCRITURA VECTOR] Código generado para escritura en '%s'\n", nombreVector);
}