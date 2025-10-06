#include "acceso_matriz.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include "../../AST/err.h"
#include "../../AST/errorTIpos.h"
#include "../../simbolos/Evaluador.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Función para analizar las dimensiones de una matriz
void analizarDimensionesMatriz(const char* matrixStr, int* numFilas, int* numColumnas) {
    char* matrixCopy = strdup(matrixStr);
    if (!matrixCopy) return;
    
    // Contar filas (corchetes internos)
    *numFilas = 0;
    *numColumnas = 0;
    char* p = matrixCopy;
    
    // Contar filas (separadas por ],[)
    int nivel = 0;
    while (*p) {
        if (*p == '[') {
            nivel++;
            if (nivel == 2) (*numFilas)++; // Solo contar apertura de segundo nivel
        }
        else if (*p == ']') nivel--;
        p++;
    }
    
    // Si no se encontraron filas, posiblemente sea una matriz vacía o mal formateada
    if (*numFilas == 0) *numFilas = 1;
    
    // Contar columnas en la primera fila
    if (*numFilas > 0) {
        p = strchr(matrixCopy, '[');
        if (p) {
            p++; // Saltar el primer corchete
            char* endRow = strchr(p, ']');
            if (endRow) {
                // Contar comas dentro de la primera fila + 1
                *numColumnas = 1; // Al menos un elemento
                while (p < endRow) {
                    if (*p == ',') (*numColumnas)++;
                    p++;
                }
            }
        }
    }
    
    free(matrixCopy);
}

// Función para validar la estructura básica del nodo de acceso a matriz
static int validarEstructuraNodoMatriz(NodoAST* nodo, NodoAST** idNodo, NodoAST** dimensionesNodo) {
    if (!nodo) {
        printf("[ERROR] Nodo de acceso a matriz es NULL\n");
        return 0;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] Nodo no tiene suficientes hijos para un acceso a matriz\n");
        return 0;
    }

    *idNodo = nodo->hijos[0];             // Identificador
    *dimensionesNodo = nodo->hijos[1];    // Nodo de dimensiones

    // Verificar que los nodos no sean NULL
    if (!(*idNodo) || !(*dimensionesNodo)) {
        printf("[ERROR] Uno de los nodos hijos es NULL en acceso a matriz\n");
        return 0;
    }

    // Verificar que el valor del identificador no sea NULL
    if (!(*idNodo)->valor) {
        printf("[ERROR] Identificador de matriz es NULL\n");
        return 0;
    }

    // Verificar que el nodo de dimensiones sea del tipo correcto
    if ((*dimensionesNodo)->tipo != AST_DIMENSIONES) {
        printf("[ERROR] Se esperaba un nodo de dimensiones para matriz, se encontró tipo %d\n", 
               (*dimensionesNodo)->tipo);
        return 0;
    }

    return 1;
}

// Función para validar el símbolo de matriz
static int validarSimboloMatriz(NodoAST* idNodo, Simbolo** simbolo) {
    // Buscar el símbolo en la tabla
    *simbolo = buscarSimbolo(idNodo->valor);
    if (!(*simbolo)) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Matriz '%s' no declarada", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return 0;
    }

    // Verificar que el símbolo sea una matriz
    if ((*simbolo)->clase != SIMBOLO_MATRIZ) {
        char msg[128];
        if ((*simbolo)->clase == SIMBOLO_VECTOR) {
            snprintf(msg, sizeof(msg), 
                    "Variable '%s' es un vector, no una matriz. Use un solo índice para acceder", 
                    idNodo->valor);
        } else {
            snprintf(msg, sizeof(msg), "Variable '%s' no es una matriz (clase: %d)", 
                    idNodo->valor, (*simbolo)->clase);
        }
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return 0;
    }

    return 1;
}

// Función para validar los índices de acceso a matriz
static int validarIndicesMatriz(NodoAST* nodo, NodoAST* idNodo, NodoAST* dimensionesNodo, int numFilas, int numColumnas) {
    int filaIdx = -1;
    int colIdx = -1;
    int dimensionActual = 0;
    
    // Extraer y validar los dos índices
    for (int i = 0; i < dimensionesNodo->num_hijos; i++) {
        if (dimensionesNodo->hijos[i]->tipo == AST_1DIMENSION) {
            NodoAST* dimNodo = dimensionesNodo->hijos[i];
            if (dimNodo->num_hijos < 1) {
                printf("[ERROR] Nodo de dimensión sin expresión en matriz\n");
                nodo->valido = 0;
                return 0;
            }
            
            NodoAST* exprNodo = dimNodo->hijos[0];
            ValorSimbolo valor = evaluarExpr(exprNodo);
            
            // Verificar que el índice sea entero
            if (!valor.inicializado || valor.tipo != TIPO_INT) {
                char msg[128];
                snprintf(msg, sizeof(msg), "El índice %d de matriz '%s' debe ser entero", 
                         dimensionActual + 1, idNodo->valor);
                addError(msg, ambitoActual(), exprNodo->linea, exprNodo->columna);
                printf("[ERROR] %s\n", msg);
                nodo->valido = 0;
                return 0;
            }
            
            if (dimensionActual == 0) {
                filaIdx = valor.v.i;
                // Verificar rango de fila
                if (filaIdx < 0 || filaIdx >= numFilas) {
                    char msg[128];
                    snprintf(msg, sizeof(msg), "Índice de fila fuera de rango: %d (rango válido: 0-%d)", 
                             filaIdx, numFilas - 1);
                    addError(msg, ambitoActual(), exprNodo->linea, exprNodo->columna);
                    printf("[ERROR] %s\n", msg);
                    return 0;
                }
            } else if (dimensionActual == 1) {
                colIdx = valor.v.i;
                // Verificar rango de columna
                if (colIdx < 0 || colIdx >= numColumnas) {
                    char msg[128];
                    snprintf(msg, sizeof(msg), "Índice de columna fuera de rango: %d (rango válido: 0-%d)", 
                             colIdx, numColumnas - 1);
                    addError(msg, ambitoActual(), exprNodo->linea, exprNodo->columna);
                    printf("[ERROR] %s\n", msg);
                    return 0;
                }
            }
            
            dimensionActual++;
        }
    }
    
    // Verificar que se proporcionaron exactamente 2 índices
    if (dimensionActual != 2) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Matriz '%s' requiere exactamente 2 índices, se proporcionaron %d", 
                 idNodo->valor, dimensionActual);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        nodo->valido = 0;
        return 0;
    }
    
    printf("[DEBUG] Índices de matriz: [%d][%d] (en rango: [0-%d][0-%d])\n", 
           filaIdx, colIdx, numFilas - 1, numColumnas - 1);
    
    return 1;
}

// Función principal para verificar acceso a matriz
int verificarAccesoMatriz(NodoAST* nodo) {
    printf("[DEBUG verificarAccesoMatriz] Iniciando verificación de acceso a matriz\n");

    NodoAST* idNodo = NULL;
    NodoAST* dimensionesNodo = NULL;
    Simbolo* simbolo = NULL;
    
    // Validar estructura básica del nodo
    if (!validarEstructuraNodoMatriz(nodo, &idNodo, &dimensionesNodo)) {
        return 0;
    }
    
    // Validar el símbolo de la matriz
    if (!validarSimboloMatriz(idNodo, &simbolo)) {
        return 0;
    }
    
    // Obtener dimensiones de la matriz
    int numFilas = 0, numColumnas = 0;
    analizarDimensionesMatriz(simbolo->valor.v.s, &numFilas, &numColumnas);
    
    printf("[DEBUG] Matriz '%s' tiene %d filas y %d columnas\n", 
           idNodo->valor, numFilas, numColumnas);
    
    // Validar los índices de acceso
    if (!validarIndicesMatriz(nodo, idNodo, dimensionesNodo, numFilas, numColumnas)) {
        return 0;
    }
    
    // Asignar el tipo del elemento al nodo de acceso
    nodo->columna = simbolo->tipo;
    
    printf("[DEBUG verificarAccesoMatriz] Acceso a matriz '%s' válido, tipo: %s\n", 
           idNodo->valor, nombreTipo(simbolo->tipo));
    
    return 1;
}

// Función para evaluar acceso a matriz y devolver su valor
ValorSimbolo evaluarAccesoMatriz(NodoAST* nodo) {
    printf("[DEBUG EVAL_MATRIZ] Iniciando evaluación de acceso a matriz\n");
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    
    NodoAST* idNodo = NULL;
    NodoAST* dimensionesNodo = NULL;
    
    
    // Validar estructura básica del nodo
    if (!validarEstructuraNodoMatriz(nodo, &idNodo, &dimensionesNodo)) {
        return r;
    }
    
    // Buscar el símbolo de la matriz
    Simbolo* simbolo = buscarSimbolo(idNodo->valor);
    printf("[DEBUG EVAL_MATRIZ] Buscando símbolo '%s': %s\n", 
           idNodo->valor, 
           simbolo ? "encontrado" : "no encontrado");
    if (!simbolo) {
        printf("[ERROR EVAL_MATRIZ] Símbolo '%s' no encontrado\n", idNodo->valor);
        return r;
    }
    int numFilas = 0, numColumnas = 0;
    analizarDimensionesMatriz(simbolo->valor.v.s, &numFilas, &numColumnas);
    
    printf("[DEBUG EVAL_MATRIZ] Clase del símbolo: %d (SIMBOLO_MATRIZ=%d)\n", 
           simbolo->clase, SIMBOLO_MATRIZ);
    
    if (simbolo->clase != SIMBOLO_MATRIZ) {
        printf("[ERROR EVAL_MATRIZ] Símbolo '%s' no es una matriz (clase=%d)\n", 
               idNodo->valor, simbolo->clase);
        return r;
    }
    
    printf("[DEBUG EVAL_MATRIZ] Inicializado: %d, Valor: %s\n", 
           simbolo->valor.inicializado,
           simbolo->valor.v.s ? simbolo->valor.v.s : "NULL");
    
    if (!simbolo->valor.inicializado) {
        printf("[ERROR EVAL_MATRIZ] Matriz '%s' no inicializada\n", idNodo->valor);
        return r;
    }

    // Evaluar los índices
    int filaIdx = -1;
    int colIdx = -1;
    int dimensionActual = 0;
    
    for (int i = 0; i < dimensionesNodo->num_hijos; i++) {
        if (dimensionesNodo->hijos[i]->tipo == AST_1DIMENSION) {
            NodoAST* dimNodo = dimensionesNodo->hijos[i];
            if (dimNodo->num_hijos < 1) {
                return r;
            }
            
            ValorSimbolo idxVal = evaluarExpr(dimNodo->hijos[0]);
            if (!idxVal.inicializado || idxVal.tipo != TIPO_INT) {
                return r;
            }
            
            if (dimensionActual == 0) {
                filaIdx = idxVal.v.i;
            } else if (dimensionActual == 1) {
                colIdx = idxVal.v.i;
            }
            
            dimensionActual++;
        }
    }
    
    // Verificar índices válidos
    if (filaIdx < 0 || filaIdx >= numFilas) {
        printf("[ERROR] Índice de fila fuera de rango: %d (rango válido: 0-%d)\n", 
            filaIdx, numFilas - 1);
        r.inicializado = 0;  // IMPORTANTE: Marcar como no inicializado
        return r;
    }

    if (colIdx < 0 || colIdx >= numColumnas) {
        printf("[ERROR] Índice de columna fuera de rango: %d (rango válido: 0-%d)\n", 
            colIdx, numColumnas - 1);
        r.inicializado = 0;  // IMPORTANTE: Marcar como no inicializado
        return r;
    }
    printf("[DEBUG EVAL_MATRIZ] Índices calculados: [%d][%d]\n", filaIdx, colIdx);
    
    // Procesar acceso a la matriz
    r.tipo = simbolo->tipo;
    r.inicializado = 1;
    
    printf("[EVALUADOR] Accediendo a matriz '%s[%d][%d]'\n", 
        simbolo->nombre, filaIdx, colIdx);
    
    char* matrixStr = strdup(simbolo->valor.v.s);
    
    // Verificar formato
    if (matrixStr[0] != '[' || matrixStr[1] != '[') {
        printf("[EVALUADOR] Formato de matriz no reconocido\n");
        free(matrixStr);
        r.inicializado = 0;
        return r;
    }
    
    // Extraer filas de la matriz
    char* matrixCopy = strdup(matrixStr);
    char* filas[50]; // Máximo 50 filas
    numFilas = 0;

    printf("[DEBUG EVAL_MATRIZ] Contenido de la matriz: %s\n", matrixStr);
    
    // Quitar corchete externo inicial
    memmove(matrixCopy, matrixCopy + 1, strlen(matrixCopy));
    
    // Quitar corchete externo final si existe
    int len = strlen(matrixCopy);
    if (len > 0 && matrixCopy[len - 1] == ']') {
        matrixCopy[len - 1] = '\0';
    }
    
    // Extraer cada fila - tienen formato [val1,val2,...]
    char* p = matrixCopy;
    char* inicioFila = p;
    int nivel = 0;
    
    while (*p) {
        if (*p == '[') nivel++;
        else if (*p == ']') {
            nivel--;
            if (nivel == 0) {
                // Fin de una fila
                *(p+1) = '\0'; // Cortar después del corchete
                filas[numFilas++] = strdup(inicioFila);
                inicioFila = p + 2; // Saltar el corchete y la coma
                p++; // Avanzar después del corchete
            }
        }
        p++;
    }
    
    printf("[EVALUADOR] Matriz analizada con %d filas\n", numFilas);
    
    // Verificar índice de fila
    if (filaIdx < 0 || filaIdx >= numFilas) {
        printf("[EVALUADOR] Índice de fila %d fuera de rango (0-%d)\n", 
            filaIdx, numFilas-1);
        // Liberar memoria
        for (int i = 0; i < numFilas; i++) {
            free(filas[i]);
        }
        free(matrixCopy);
        free(matrixStr);
        r.inicializado = 0;
        return r;
    }
    
    // Analizar la fila seleccionada
    char* filaStr = filas[filaIdx];
    
    // Quitar corchetes de la fila
    if (filaStr[0] == '[') {
        memmove(filaStr, filaStr + 1, strlen(filaStr));
        len = strlen(filaStr);
        if (len > 0 && filaStr[len - 1] == ']') {
            filaStr[len - 1] = '\0';
        }
    }
    
    // Separar elementos de la fila
    char* elementos[100]; // Máximo 100 columnas
    int numElementos = 0;
    
    char* elementosCopy = strdup(filaStr);
    char* token = strtok(elementosCopy, ",");
    while (token && numElementos < 100) {
        // Eliminar espacios al inicio y final si existen
        while (*token && isspace(*token)) token++;
        char* end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) *end-- = '\0';
        
        elementos[numElementos++] = strdup(token);
        token = strtok(NULL, ",");
    }
    
    printf("[EVALUADOR] Fila %d tiene %d elementos\n", filaIdx, numElementos);
    
    // Verificar índice de columna
    if (colIdx < 0 || colIdx >= numElementos) {
        printf("[EVALUADOR] Índice de columna %d fuera de rango (0-%d)\n", 
            colIdx, numElementos-1);
        
        // Liberar memoria
        for (int i = 0; i < numElementos; i++) {
            free(elementos[i]);
        }
        free(elementosCopy);
        
        for (int i = 0; i < numFilas; i++) {
            free(filas[i]);
        }
        free(matrixCopy);
        free(matrixStr);
        
        r.inicializado = 0;
        return r;
    }
    
    // Convertir elemento según el tipo
    printf("[EVALUADOR] Accediendo a elemento [%d][%d] = '%s'\n", 
        filaIdx, colIdx, elementos[colIdx]);
    
    switch (simbolo->tipo) {
        case TIPO_INT:
            r.v.i = atoi(elementos[colIdx]);
            break;
        case TIPO_FLOAT:
            r.v.f = strtof(elementos[colIdx], NULL);
            break;
        case TIPO_DOUBLE:
            r.v.d = strtod(elementos[colIdx], NULL);
            break;
        case TIPO_CHAR:
            r.v.c = elementos[colIdx][0];
            break;
        case TIPO_BOOLEAN:
            r.v.b = (strcmp(elementos[colIdx], "true") == 0);
            break;
        case TIPO_STRING:
            r.v.s = strdup(elementos[colIdx]);
            break;
        default:
            r.inicializado = 0;
            break;
    }
    
    // Liberar toda la memoria
    for (int i = 0; i < numElementos; i++) {
        free(elementos[i]);
    }
    free(elementosCopy);
    
    for (int i = 0; i < numFilas; i++) {
        free(filas[i]);
    }
    free(matrixCopy);
    free(matrixStr);
    
    printf("[EVALUADOR] Matriz '%s[%d][%d]' evaluada correctamente\n", 
        simbolo->nombre, filaIdx, colIdx);
    
    return r;
}

int evaluarIndiceMatriz(NodoAST* acceso, int dimension) {
    NodoAST* indiceNodo = acceso->hijos[1]->hijos[dimension]; // Dimensión específica
    ValorSimbolo indice = evaluarExpr(indiceNodo);
    if (indice.tipo != TIPO_INT) {
        printf("[ERROR] Índice debe ser entero\n");
        return -1;
    }
    return indice.v.i;
}

void asignarValorMatriz(Simbolo* simbolo, int fila, int columna, ValorSimbolo valor) {
    if (!simbolo || simbolo->clase != SIMBOLO_MATRIZ || !simbolo->valor.inicializado) {
        printf("[ERROR] Símbolo no es una matriz inicializada\n");
        return;
    }
    
    // Obtener dimensiones de la matriz
    int numFilas = 0, numColumnas = 0;
    analizarDimensionesMatriz(simbolo->valor.v.s, &numFilas, &numColumnas);
    
    // Verificar que los índices estén dentro de los límites
    if (fila < 0 || fila >= numFilas || columna < 0 || columna >= numColumnas) {
        printf("[ERROR] Índices [%d][%d] fuera de rango para matriz '%s' [0-%d][0-%d]\n", 
               fila, columna, simbolo->nombre, numFilas-1, numColumnas-1);
        return;
    }
    
    // Extraer la representación de la matriz
    char* matrizStr = strdup(simbolo->valor.v.s);
    if (!matrizStr) {
        printf("[ERROR] No se pudo obtener el valor de la matriz '%s'\n", simbolo->nombre);
        return;
    }
    
    // Extraer filas de la matriz
    char* matrixCopy = strdup(matrizStr);
    char* filas[50]; // Máximo 50 filas
    int numFilasReal = 0;
    
    // Verificar que la matriz comience con [
    if (matrixCopy[0] != '[') {
        printf("[ERROR] Formato de matriz inválido: %s\n", matrixCopy);
        free(matrizStr);
        free(matrixCopy);
        return;
    }
    
    // Quitar corchete externo inicial
    memmove(matrixCopy, matrixCopy + 1, strlen(matrixCopy));
    
    // Quitar corchete externo final si existe
    int len = strlen(matrixCopy);
    if (len > 0 && matrixCopy[len - 1] == ']') {
        matrixCopy[len - 1] = '\0';
    }
    
    // Extraer cada fila - tienen formato [val1,val2,...]
    char* p = matrixCopy;
    char* inicioFila = p;
    int nivel = 0;
    
    while (*p) {
        if (*p == '[') nivel++;
        else if (*p == ']') {
            nivel--;
            if (nivel == 0) {
                // Fin de una fila
                *(p+1) = '\0'; // Cortar después del corchete
                filas[numFilasReal++] = strdup(inicioFila);
                inicioFila = p + 2; // Saltar el corchete y la coma
                p++; // Avanzar después del corchete
            }
        }
        p++;
    }
    
    // Verificar que tengamos suficientes filas
    if (numFilasReal <= fila) {
        printf("[ERROR] Índice de fila %d fuera de rango (0-%d)\n", fila, numFilasReal-1);
        free(matrizStr);
        free(matrixCopy);
        for (int i = 0; i < numFilasReal; i++) {
            free(filas[i]);
        }
        return;
    }
    
    // Obtener la fila específica
    char* filaStr = filas[fila];
    
    // Quitar corchetes de la fila si existen
    if (filaStr[0] == '[') {
        memmove(filaStr, filaStr + 1, strlen(filaStr));
        len = strlen(filaStr);
        if (len > 0 && filaStr[len - 1] == ']') {
            filaStr[len - 1] = '\0';
        }
    }
    
    // Separar elementos de la fila por comas
    char* elementos[100]; // Máximo 100 columnas
    int numElementos = 0;
    
    char* filaCopy = strdup(filaStr);
    char* elementoToken = strtok(filaCopy, ",");
    while (elementoToken && numElementos < 100) {
        // Limpiar espacios
        while (*elementoToken && isspace(*elementoToken)) elementoToken++;
        char* end = elementoToken + strlen(elementoToken) - 1;
        while (end > elementoToken && isspace(*end)) *end-- = '\0';
        
        elementos[numElementos++] = strdup(elementoToken);
        elementoToken = strtok(NULL, ",");
    }
    
    // Verificar que tengamos suficientes columnas
    if (numElementos <= columna) {
        printf("[ERROR] Índice de columna %d fuera de rango (0-%d)\n", columna, numElementos-1);
        free(matrizStr);
        free(matrixCopy);
        free(filaCopy);
        for (int i = 0; i < numFilasReal; i++) {
            free(filas[i]);
        }
        for (int i = 0; i < numElementos; i++) {
            free(elementos[i]);
        }
        return;
    }
    
    // Convertir el nuevo valor a string según el tipo
    char nuevoValorStr[64];
    switch (simbolo->tipo) {
        case TIPO_INT:
            snprintf(nuevoValorStr, sizeof(nuevoValorStr), "%d", valor.v.i);
            break;
        case TIPO_FLOAT:
            snprintf(nuevoValorStr, sizeof(nuevoValorStr), "%f", valor.v.f);
            break;
        case TIPO_DOUBLE:
            snprintf(nuevoValorStr, sizeof(nuevoValorStr), "%lf", valor.v.d);
            break;
        case TIPO_CHAR:
            snprintf(nuevoValorStr, sizeof(nuevoValorStr), "%c", valor.v.c);
            break;
        case TIPO_BOOLEAN:
            snprintf(nuevoValorStr, sizeof(nuevoValorStr), "%s", valor.v.b ? "true" : "false");
            break;
        case TIPO_STRING:
            if (valor.v.s)
                strncpy(nuevoValorStr, valor.v.s, sizeof(nuevoValorStr)-1);
            else
                strcpy(nuevoValorStr, "");
            nuevoValorStr[sizeof(nuevoValorStr)-1] = '\0';
            break;
        default:
            strcpy(nuevoValorStr, "");
            break;
    }
    
    // Reemplazar el valor en la posición correspondiente
    free(elementos[columna]);
    elementos[columna] = strdup(nuevoValorStr);
    
    // Reconstruir la fila con el nuevo valor
    char nuevaFila[1024] = "[";
    for (int i = 0; i < numElementos; i++) {
        strcat(nuevaFila, elementos[i]);
        if (i < numElementos - 1) {
            strcat(nuevaFila, ",");
        }
    }
    strcat(nuevaFila, "]");
    
    // Actualizar la fila en el array de filas
    free(filas[fila]);
    filas[fila] = strdup(nuevaFila);
    
    // Reconstruir la matriz completa
    char nuevaMatriz[4096] = "[";
    
    // Añadir todas las filas
    for (int i = 0; i < numFilasReal; i++) {
        strcat(nuevaMatriz, filas[i]);
        if (i < numFilasReal - 1) {
            strcat(nuevaMatriz, ",");
        }
    }
    
    // Cerrar corchete de matriz
    strcat(nuevaMatriz, "]");
    
    // Actualizar el valor en el símbolo
    if (simbolo->valor.v.s) {
        free(simbolo->valor.v.s);
    }
    simbolo->valor.v.s = strdup(nuevaMatriz);
    
    printf("[DEBUG] Matriz '%s[%d][%d]' actualizada con valor '%s'\n", 
           simbolo->nombre, fila, columna, nuevoValorStr);
    
    // Limpiar memoria
    for (int i = 0; i < numElementos; i++) {
        free(elementos[i]);
    }
    free(filaCopy);
    
    for (int i = 0; i < numFilasReal; i++) {
        free(filas[i]);
    }
    free(matrizStr);
    free(matrixCopy);
}