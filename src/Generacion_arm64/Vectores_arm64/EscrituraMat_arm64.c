#include "EscrituraMat_arm64.h"
#include "AccesoMatriz_arm64.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include "../../semantico/tipos.h"
#include "../../semantico/semantico.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern NodoAST* astGlobal;

void generarEscrituraMatrizARM64(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_ACCESO_ESCRITURA) {
        printf("[ERROR] Nodo inválido para escritura de matriz\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] Estructura de escritura incompleta\n");
        return;
    }

    NodoAST* accesoNodo = nodo->hijos[0];  // El acceso matriz[fila][columna]
    NodoAST* valorNodo = nodo->hijos[1];   // El valor a asignar

    if (!accesoNodo || accesoNodo->tipo != AST_ACCESO) {
        printf("[ERROR] Nodo de acceso inválido en escritura\n");
        return;
    }

    // **VERIFICAR QUE SEA ACCESO A MATRIZ (2 DIMENSIONES)**
    int numDimensiones = contarDimensionesAcceso(accesoNodo);
    if (numDimensiones != 2) {
        printf("[ERROR] Se esperaba acceso a matriz (2 dimensiones), encontrado: %d\n", numDimensiones);
        return;
    }

    // Obtener el nombre de la matriz
    if (accesoNodo->num_hijos < 2) {
        printf("[ERROR] Estructura de acceso inválida\n");
        return;
    }

    NodoAST* idNodo = accesoNodo->hijos[0];
    if (!idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Identificador inválido en acceso\n");
        return;
    }

    const char* nombreMatriz = idNodo->valor;
    printf("[DEBUG ESCRITURA MATRIZ] Generando escritura en matriz '%s'\n", nombreMatriz);

    // Buscar la declaración de la matriz en el AST
    NodoAST* matrizDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_MATRIZ, nombreMatriz);
    if (!matrizDecl) {
        printf("[ERROR] Matriz '%s' no encontrada en el AST\n", nombreMatriz);
        return;
    }

    // Obtener tipo base y dimensiones
    TipoDato tipoMatriz = obtenerTipoMatrizDesdeAST(matrizDecl);
    int numFilas = 0, numColumnas = 0;
    obtenerDimensionesMatriz(matrizDecl, &numFilas, &numColumnas);

    if (numColumnas == 0) {
        printf("[ERROR] No se pudieron determinar las dimensiones de la matriz '%s'\n", nombreMatriz);
        return;
    }

    printf("[DEBUG ESCRITURA MATRIZ] Tipo: %d, Dimensiones: [%d][%d]\n", 
           tipoMatriz, numFilas, numColumnas);

    // Obtener los índices de fila y columna
    NodoAST* dimensionesNodo = accesoNodo->hijos[1];
    if (dimensionesNodo->num_hijos < 2) {
        printf("[ERROR] Se esperaban 2 dimensiones para escritura en matriz\n");
        return;
    }

    NodoAST* dimFila = dimensionesNodo->hijos[0];
    NodoAST* dimColumna = dimensionesNodo->hijos[1];

    if (dimFila->tipo != AST_1DIMENSION || dimColumna->tipo != AST_1DIMENSION) {
        printf("[ERROR] Formato de dimensiones inválido\n");
        return;
    }

    NodoAST* indiceFila = dimFila->hijos[0];
    NodoAST* indiceColumna = dimColumna->hijos[0];

    char codigo[1024] = "";
    char buffer[256];

    // Comentario descriptivo
    snprintf(buffer, sizeof(buffer),
        "    // Escritura en matriz: %s[fila][columna] = valor\n",
        nombreMatriz);
    strcat(codigo, buffer);

    // Determinar tamaño del elemento según el tipo
    int tamElemento = 4; // Por defecto: int
    const char* instrStore = "str w";

    switch (tipoMatriz) {
        case TIPO_INT:
            tamElemento = 4;
            instrStore = "str w";
            break;
        case TIPO_FLOAT:
            tamElemento = 4;
            instrStore = "str s";
            break;
        case TIPO_DOUBLE:
            tamElemento = 8;
            instrStore = "str d";
            break;
        case TIPO_CHAR:
        case TIPO_BOOLEAN:
            tamElemento = 1;
            instrStore = "strb w";
            break;
        case TIPO_STRING:
            tamElemento = 8; // Puntero
            instrStore = "str x";
            break;
        default:
            tamElemento = 4;
            instrStore = "str w";
    }

    // **PASO 1: EVALUAR ÍNDICE DE FILA**
    int regIndex = 10; // Empezar con registros altos para evitar conflictos
    int regFila = regIndex++;

    if (indiceFila->tipo == AST_NUMERO) {
        snprintf(buffer, sizeof(buffer),
            "    mov w%d, #%s\n",
            regFila, indiceFila->valor);
    } else if (indiceFila->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s\n"
            "    ldr w%d, [x%d]\n",
            regFila, indiceFila->valor, regFila, regFila);
    } else {
        // Expresión compleja
        ResultadoExprEmbebida resFila = generarExprEmbebidaARM64(gen, indiceFila, &regIndex);
        strcat(codigo, resFila.codigo);
        snprintf(buffer, sizeof(buffer),
            "    mov w%d, %s\n",
            regFila, resFila.resultado);
    }
    strcat(codigo, buffer);

    // **PASO 2: EVALUAR ÍNDICE DE COLUMNA**
    int regColumna = regIndex++;

    if (indiceColumna->tipo == AST_NUMERO) {
        snprintf(buffer, sizeof(buffer),
            "    mov w%d, #%s\n",
            regColumna, indiceColumna->valor);
    } else if (indiceColumna->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s\n"
            "    ldr w%d, [x%d]\n",
            regColumna, indiceColumna->valor, regColumna, regColumna);
    } else {
        // Expresión compleja
        ResultadoExprEmbebida resCol = generarExprEmbebidaARM64(gen, indiceColumna, &regIndex);
        strcat(codigo, resCol.codigo);
        snprintf(buffer, sizeof(buffer),
            "    mov w%d, %s\n",
            regColumna, resCol.resultado);
    }
    strcat(codigo, buffer);

    // **PASO 3: CALCULAR OFFSET = (fila * numColumnas + columna) * tamElemento**
    int regOffset = regIndex++;
    int regTemp = regIndex++;

    snprintf(buffer, sizeof(buffer),
        "    // Calcular offset: (fila * %d + columna) * %d\n"
        "    mov w%d, #%d\n"           // w_temp = numColumnas
        "    mul w%d, w%d, w%d\n"      // offset = fila * numColumnas
        "    add w%d, w%d, w%d\n"      // offset = offset + columna
        "    mov w%d, #%d\n"           // w_temp = tamElemento
        "    mul w%d, w%d, w%d\n",     // offset = offset * tamElemento
        numColumnas, tamElemento,
        regTemp, numColumnas,
        regOffset, regFila, regTemp,
        regOffset, regOffset, regColumna,
        regTemp, tamElemento,
        regOffset, regOffset, regTemp);
    strcat(codigo, buffer);

    // **PASO 4: EVALUAR EL VALOR A ASIGNAR**
    int regValor = regIndex++;
    TipoDato tipoValor = deducirTipoExprARM64(valorNodo, astGlobal);

    if (valorNodo->tipo == AST_NUMERO) {
        if (tipoMatriz == TIPO_FLOAT) {
            // Crear constante float en .data
            char nombreConst[64];
            snprintf(nombreConst, sizeof(nombreConst), "float_const_%d", regValor);
            snprintf(buffer, sizeof(buffer), "%s: .float %s\n", nombreConst, valorNodo->valor);
            insertarEnSeccionData(gen, buffer);
            
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr s%d, [x%d]\n",
                regValor, nombreConst, regValor, regValor);
        } else if (tipoMatriz == TIPO_DOUBLE) {
            // Crear constante double en .data
            char nombreConst[64];
            snprintf(nombreConst, sizeof(nombreConst), "double_const_%d", regValor);
            snprintf(buffer, sizeof(buffer), "%s: .double %s\n", nombreConst, valorNodo->valor);
            insertarEnSeccionData(gen, buffer);
            
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr d%d, [x%d]\n",
                regValor, nombreConst, regValor, regValor);
        } else {
            // Entero, char, boolean
            snprintf(buffer, sizeof(buffer),
                "    mov w%d, #%s\n",
                regValor, valorNodo->valor);
        }
    } else if (valorNodo->tipo == AST_IDENTIFICADOR) {
        if (tipoMatriz == TIPO_FLOAT) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr s%d, [x%d]\n",
                regValor, valorNodo->valor, regValor, regValor);
        } else if (tipoMatriz == TIPO_DOUBLE) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr d%d, [x%d]\n",
                regValor, valorNodo->valor, regValor, regValor);
        } else if (tipoMatriz == TIPO_CHAR || tipoMatriz == TIPO_BOOLEAN) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldrb w%d, [x%d]\n",
                regValor, valorNodo->valor, regValor, regValor);
        } else if (tipoMatriz == TIPO_STRING) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr x%d, [x%d]\n",
                regValor, valorNodo->valor, regValor, regValor);
        } else {
            // TIPO_INT
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    ldr w%d, [x%d]\n",
                regValor, valorNodo->valor, regValor, regValor);
        }
    } else {
        // Expresión compleja o acceso a otro vector/matriz
        ResultadoExprEmbebida resValor = generarExprEmbebidaARM64(gen, valorNodo, &regIndex);
        strcat(codigo, resValor.codigo);
        
        if (tipoMatriz == TIPO_FLOAT) {
            snprintf(buffer, sizeof(buffer),
                "    fmov s%d, %s\n",
                regValor, resValor.resultado);
        } else if (tipoMatriz == TIPO_DOUBLE) {
            snprintf(buffer, sizeof(buffer),
                "    fmov d%d, %s\n",
                regValor, resValor.resultado);
        } else if (tipoMatriz == TIPO_STRING) {
            snprintf(buffer, sizeof(buffer),
                "    mov x%d, %s\n",
                regValor, resValor.resultado);
        } else {
            snprintf(buffer, sizeof(buffer),
                "    mov w%d, %s\n",
                regValor, resValor.resultado);
        }
    }
    strcat(codigo, buffer);

    // **PASO 5: CALCULAR DIRECCIÓN Y ALMACENAR**
    int regDireccion = regIndex++;

    snprintf(buffer, sizeof(buffer),
        "    ldr x%d, =%s\n"
        "    add x%d, x%d, w%d, sxtw\n",
        regDireccion, nombreMatriz,
        regDireccion, regDireccion, regOffset);
    strcat(codigo, buffer);

    // Almacenar según el tipo
    if (tipoMatriz == TIPO_FLOAT) {
        snprintf(buffer, sizeof(buffer),
            "    str s%d, [x%d]\n",
            regValor, regDireccion);
    } else if (tipoMatriz == TIPO_DOUBLE) {
        snprintf(buffer, sizeof(buffer),
            "    str d%d, [x%d]\n",
            regValor, regDireccion);
    } else if (tipoMatriz == TIPO_CHAR || tipoMatriz == TIPO_BOOLEAN) {
        snprintf(buffer, sizeof(buffer),
            "    strb w%d, [x%d]\n",
            regValor, regDireccion);
    } else if (tipoMatriz == TIPO_STRING) {
        snprintf(buffer, sizeof(buffer),
            "    str x%d, [x%d]\n",
            regValor, regDireccion);
    } else {
        // TIPO_INT
        snprintf(buffer, sizeof(buffer),
            "    str w%d, [x%d]\n",
            regValor, regDireccion);
    }
    strcat(codigo, buffer);

    // Insertar el código generado en la sección .text
    insertarEnSeccionText(gen, codigo);
    
    printf("[DEBUG ESCRITURA MATRIZ] Código generado para escritura en '%s'\n", nombreMatriz);
}