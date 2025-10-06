#include "acceso_vector.h"
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

// Función para validar la estructura básica del nodo de acceso a vector
static int validarEstructuraNodoVector(NodoAST* nodo, NodoAST** idNodo, NodoAST** dimensionesNodo) {
    if (!nodo) {
        printf("[ERROR] Nodo de acceso a vector es NULL\n");
        return 0;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] Nodo no tiene suficientes hijos para un acceso a vector\n");
        return 0;
    }

    *idNodo = nodo->hijos[0];             // Identificador
    *dimensionesNodo = nodo->hijos[1];    // Nodo de dimensiones

    // Verificar que los nodos no sean NULL
    if (!(*idNodo) || !(*dimensionesNodo)) {
        printf("[ERROR] Uno de los nodos hijos es NULL en acceso a vector\n");
        return 0;
    }

    // Verificar que el valor del identificador no sea NULL
    if (!(*idNodo)->valor) {
        printf("[ERROR] Identificador de vector es NULL\n");
        return 0;
    }

    // Verificar que el nodo de dimensiones sea del tipo correcto
    if ((*dimensionesNodo)->tipo != AST_DIMENSIONES) {
        printf("[ERROR] Se esperaba un nodo de dimensiones para vector, se encontró tipo %d\n", 
               (*dimensionesNodo)->tipo);
        return 0;
    }

    return 1;
}

// Función para validar el símbolo de vector
static int validarSimboloVector(NodoAST* nodo, NodoAST* idNodo, Simbolo** simbolo) {
    // Buscar el símbolo en la tabla
    *simbolo = buscarSimbolo(idNodo->valor);
    if (!(*simbolo)) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Vector '%s' no declarado", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        idNodo->valido = 0;
        nodo->valido = 0;
        return 0;
    }

    // Verificar que el símbolo sea un vector
    if ((*simbolo)->clase != SIMBOLO_VECTOR) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Variable '%s' no es un vector", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        idNodo->valido = 0;
        nodo->valido = 0;
        return 0;
    }

    return 1;
}

// Función para obtener la longitud de un vector
static int obtenerLongitudVector(Simbolo* simbolo) {
    int longitud = 0;
    
    if (simbolo->valor.inicializado && simbolo->valor.v.s) {
        char* valorCopy = strdup(simbolo->valor.v.s);
        
        // Eliminar corchetes si existen
        if (valorCopy[0] == '[') {
            memmove(valorCopy, valorCopy + 1, strlen(valorCopy));
            int len = strlen(valorCopy);
            if (len > 0 && valorCopy[len - 1] == ']') {
                valorCopy[len - 1] = '\0';
            }
        }
        
        // Contar comas para determinar número de elementos
        longitud = 1; // Al menos un elemento
        for (char* p = valorCopy; *p; p++) {
            if (*p == ',') longitud++;
        }
        
        free(valorCopy);
    }
    
    return longitud;
}

// Función para validar el índice de acceso a vector
static int validarIndiceVector(NodoAST* nodo, NodoAST* idNodo, NodoAST* exprNodo, int longitudVector) {
    ValorSimbolo valor = evaluarExpr(exprNodo);
    
    // Verificar que el índice sea entero
    if (!valor.inicializado || valor.tipo != TIPO_INT) {
        char msg[128];
        snprintf(msg, sizeof(msg), "El índice de '%s' debe ser entero", idNodo->valor);
        addError(msg, ambitoActual(), exprNodo->linea, exprNodo->columna);
        printf("[ERROR] %s\n", msg);
        nodo->valido = 0;
        return 0;
    }
    
    // Verificar que el índice esté dentro de los límites
    int max_idx = longitudVector - 1;
    if (max_idx < 0) max_idx = 0; // Evitar rangos negativos
    
    if (valor.v.i < 0 || valor.v.i > max_idx) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Índice fuera de rango: %d (rango válido: 0-%d)", 
                valor.v.i, max_idx);
        addError(msg, ambitoActual(), exprNodo->linea, exprNodo->columna);
        printf("[ERROR] %s\n", msg);
        return 0;
    }
    
    printf("[DEBUG] Índice de vector: %d (en rango: 0-%d)\n", valor.v.i, max_idx);
    
    return 1;
}

// Función principal para verificar acceso a vector
int verificarAccesoVector(NodoAST* nodo) {
    printf("[DEBUG verificarAccesoVector] Iniciando verificación de acceso a vector\n");

    NodoAST* idNodo = NULL;
    NodoAST* dimensionesNodo = NULL;
    Simbolo* simbolo = NULL;
    
    // Validar estructura básica del nodo
    if (!validarEstructuraNodoVector(nodo, &idNodo, &dimensionesNodo)) {
        return 0;
    }
    
    // Validar el símbolo del vector
    if (!validarSimboloVector(nodo, idNodo, &simbolo)) {
        return 0;
    }
    
    // Verificar que el acceso tenga exactamente una dimensión
    int numDimensionesAcceso = 0;
    for (int i = 0; i < dimensionesNodo->num_hijos; i++) {
        if (dimensionesNodo->hijos[i]->tipo == AST_1DIMENSION) {
            numDimensionesAcceso++;
        }
    }
    
    if (numDimensionesAcceso != 1) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Vector '%s' requiere exactamente 1 índice, se proporcionaron %d", 
                 idNodo->valor, numDimensionesAcceso);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        nodo->valido = 0;
        return 0;
    }
    
    // Obtener longitud del vector
    int longitudVector = obtenerLongitudVector(simbolo);
    
    // Validar el índice de acceso
    for (int i = 0; i < dimensionesNodo->num_hijos; i++) {
        if (dimensionesNodo->hijos[i]->tipo == AST_1DIMENSION) {
            NodoAST* dimNodo = dimensionesNodo->hijos[i];
            if (dimNodo->num_hijos < 1) {
                printf("[ERROR] Nodo de dimensión sin expresión en vector\n");
                nodo->valido = 0;
                return 0;
            }
            
            NodoAST* exprNodo = dimNodo->hijos[0];
            if (!validarIndiceVector(nodo, idNodo, exprNodo, longitudVector)) {
                return 0;
            }
            
            break; // Solo necesitamos validar la primera dimensión para vectores
        }
    }
    
    // Asignar el tipo del elemento al nodo de acceso
    nodo->columna = simbolo->tipo;
    
    printf("[DEBUG verificarAccesoVector] Acceso a vector '%s' válido, tipo: %s\n", 
           idNodo->valor, nombreTipo(simbolo->tipo));
    
    return 1;
}

// Función para evaluar acceso a vector y devolver su valor
ValorSimbolo evaluarAccesoVector(NodoAST* nodo) {
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    
    NodoAST* idNodo = NULL;
    NodoAST* dimensionesNodo = NULL;
    
    // Validar estructura básica del nodo
    if (!validarEstructuraNodoVector(nodo, &idNodo, &dimensionesNodo)) {
        return r;
    }
    
    // Buscar el símbolo del vector
    Simbolo* simbolo = buscarSimbolo(idNodo->valor);
    if (!simbolo || simbolo->clase != SIMBOLO_VECTOR || !simbolo->valor.inicializado) {
        return r;
    }
    
    // Evaluar el índice
    int indice = -1;
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
            
            indice = idxVal.v.i;
            break;
        }
    }
    
    // Verificar índice válido
    if (indice < 0) {
        return r;
    }
    
    // Obtener valor en el índice especificado
    r.tipo = simbolo->tipo;
    r.inicializado = 1;
    
    char* valCopy = strdup(simbolo->valor.v.s);
    char* valores = valCopy;
    
    // Eliminar corchetes si existen
    if (valores[0] == '[') {
        memmove(valores, valores + 1, strlen(valores));
        int len = strlen(valores);
        if (len > 0 && valores[len - 1] == ']') {
            valores[len - 1] = '\0';
        }
    }
    
    // Extraer elementos
    char* valoresCopy = strdup(valores);
    char* elementos[256]; // Máximo 256 elementos
    int numElementos = 0;
    
    char* token = strtok(valoresCopy, ",");
    while (token && numElementos < 256) {
        // Limpiar espacios
        while (*token && isspace(*token)) token++;
        char* end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) *end-- = '\0';
        
        elementos[numElementos++] = strdup(token);
        token = strtok(NULL, ",");
    }
    
    
    // Verificar índice dentro del rango
    if (indice >= 0 && indice < numElementos) {
        // Convertir según el tipo
        switch (simbolo->tipo) {
            case TIPO_INT:
                r.v.i = atoi(elementos[indice]);
                break;
            case TIPO_FLOAT:
                r.v.f = strtof(elementos[indice], NULL);
                break;
            case TIPO_DOUBLE:
                r.v.d = strtod(elementos[indice], NULL);
                break;
            case TIPO_CHAR:
                r.v.c = elementos[indice][0];
                break;
            case TIPO_BOOLEAN:
                r.v.b = (strcmp(elementos[indice], "true") == 0);
                break;
            case TIPO_STRING:
                r.v.s = strdup(elementos[indice]);
                break;
            default:
                r.inicializado = 0;
                break;
        }
        printf("[EVALUADOR] Vector '%s[%d]' = '%s'\n", 
            simbolo->nombre, indice, elementos[indice]);
    } else {
        printf("[EVALUADOR] Índice %d fuera de rango (0-%d)\n", 
            indice, numElementos-1);
        r.inicializado = 0;
    }
    
    // Liberar memoria
    for (int i = 0; i < numElementos; i++) {
        free(elementos[i]);
    }
    free(valoresCopy);
    free(valCopy);
    
    return r;
}

int evaluarIndiceVector(NodoAST* acceso) {
    NodoAST* indiceNodo = acceso->hijos[1]->hijos[0]; // Primer hijo de las dimensiones
    ValorSimbolo indice = evaluarExpr(indiceNodo);
    if (indice.tipo != TIPO_INT) {
        printf("[ERROR] Índice debe ser entero\n");
        return -1;
    }
    return indice.v.i;
}

void asignarValorVector(Simbolo* simbolo, int indice, ValorSimbolo valor) {
    if (!simbolo || simbolo->clase != SIMBOLO_VECTOR || !simbolo->valor.inicializado) {
        printf("[ERROR] Símbolo no es un vector inicializado\n");
        return;
    }
    
    // Determinar la longitud del vector
    int longitud = obtenerLongitudVector(simbolo);
    
    // Verificar que el índice sea válido
    if (indice < 0 || indice >= longitud) {
        printf("[ERROR] Índice %d fuera de rango para vector '%s' (0-%d)\n", 
               indice, simbolo->nombre, longitud - 1);
        return;
    }
    
    // Extraer la representación del vector
    char* vectorStr = strdup(simbolo->valor.v.s);
    if (!vectorStr) {
        printf("[ERROR] No se pudo obtener el valor del vector '%s'\n", simbolo->nombre);
        return;
    }
    
    // Tokenizar para encontrar valores
    char* valores[256]; // Máximo 256 elementos
    int numValores = 0;
    
    // Eliminar corchetes si existen
    if (vectorStr[0] == '[') {
        memmove(vectorStr, vectorStr + 1, strlen(vectorStr));
        int len = strlen(vectorStr);
        if (len > 0 && vectorStr[len - 1] == ']') {
            vectorStr[len - 1] = '\0';
        }
    }
    
    // Hacer una copia para tokenizar
    char* valuesCopy = strdup(vectorStr);
    char* token = strtok(valuesCopy, ",");
    
    while (token && numValores < 256) {
        // Limpiar espacios
        while (*token && isspace(*token)) token++;
        char* end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) *end-- = '\0';
        
        valores[numValores++] = strdup(token);
        token = strtok(NULL, ",");
    }
    
    // Verificar que el índice esté dentro del rango
    if (indice >= numValores) {
        printf("[ERROR] Índice %d fuera de rango (0-%d)\n", indice, numValores-1);
        
        // Limpiar memoria
        for (int i = 0; i < numValores; i++)
            free(valores[i]);
        free(vectorStr);
        free(valuesCopy);
        
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
    free(valores[indice]);
    valores[indice] = strdup(nuevoValorStr);
    
    // Reconstruir el string del vector
    char nuevoVectorStr[2048] = "[";
    
    // Añadir los valores separados por comas
    for (int i = 0; i < numValores; i++) {
        strcat(nuevoVectorStr, valores[i]);
        if (i < numValores - 1)
            strcat(nuevoVectorStr, ",");
    }
    strcat(nuevoVectorStr, "]");
    
    // Actualizar el valor del símbolo
    if (simbolo->valor.v.s)
        free(simbolo->valor.v.s);
    simbolo->valor.v.s = strdup(nuevoVectorStr);
    
    printf("[DEBUG] Vector '%s[%d]' actualizado con valor '%s'\n", 
           simbolo->nombre, indice, nuevoValorStr);
    
    // Limpiar memoria
    for (int i = 0; i < numValores; i++)
        free(valores[i]);
    free(vectorStr);
    free(valuesCopy);
}