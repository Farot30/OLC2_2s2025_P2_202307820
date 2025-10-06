#include "../../simbolos/Evaluador.h"
#include "../../simbolos/Operador.h"
#include "../../simbolos/EvaluadorArray.h"
#include "../semantico.h"
#include "../Vectores/acceso_vector.h"
#include "../Vectores/acceso_matriz.h"
#include "../../AST/err.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h> 
#include <limits.h>
#include <math.h>


ValorSimbolo evaluarIndexof(NodoAST* e) {
    ValorSimbolo r;
    r.inicializado = 0;
    r.tipo = TIPO_INT;
    r.v.i = -1;
    if (e->num_hijos < 2) {
        printf("[ERROR] Arrays.indexOf requiere un array y un valor a buscar\n");
        return r;
    }
            
    // Evaluar el array (solo necesitamos el identificador, no su valor)
    NodoAST* arrayNodo = e->hijos[0];
    if (arrayNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] El primer argumento de Arrays.indexOf debe ser un identificador de array\n");
        return r;
    }
            
    // Buscar el símbolo en la tabla
    Simbolo* simboloArray = buscarSimbolo(arrayNodo->valor);
    if (!simboloArray) {
        printf("[ERROR] Vector '%s' no declarado\n", arrayNodo->valor);
        return r;
    }
            
    // Verificar que sea un vector o matriz
    if (simboloArray->clase != SIMBOLO_VECTOR && simboloArray->clase != SIMBOLO_MATRIZ) {
        printf("[ERROR] '%s' no es un vector o matriz\n", arrayNodo->valor);
        return r;
    }
            
    // Verificar que esté inicializado
    if (!simboloArray->valor.inicializado || !simboloArray->valor.v.s) {
        printf("[ERROR] Vector '%s' no inicializado\n", arrayNodo->valor);
        return r;
    }
        
    // Evaluar la clave a buscar
    ValorSimbolo claveVal = evaluarExpr(e->hijos[1]);
    if (!claveVal.inicializado) {
        printf("[ERROR] Clave no inicializada en Arrays.indexOf\n");
        return r;
    }
            
    printf("[DEBUG] Buscando valor en vector '%s', formato: %s\n", 
        simboloArray->nombre, simboloArray->valor.v.s);
            
    // Ahora buscamos la clave en el array
    int indice = -1;  // Por defecto, no encontrado
            
    // Extraer los elementos del array
    char* arrayStr = strdup(simboloArray->valor.v.s);
            
    // Eliminar corchetes si existen
    if (arrayStr[0] == '[') {
        memmove(arrayStr, arrayStr + 1, strlen(arrayStr));
        int len = strlen(arrayStr);
        if (len > 0 && arrayStr[len - 1] == ']') {
                arrayStr[len - 1] = '\0';
        }
    }
            
    // Extraer los elementos del array
    char* elementos[1000];  // Max 1000 elementos
    int numElementos = 0;

    char* elementosCopy = strdup(arrayStr);
    char* token = strtok(elementosCopy, ",");
            
    while (token && numElementos < 1000) {
        // Limpiar espacios al inicio y final
        while (*token && isspace(*token)) token++;
        char* end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) *end-- = '\0';
                
        elementos[numElementos++] = strdup(token);
        token = strtok(NULL, ",");
    }
            
    printf("[DEBUG] Procesando %d elementos en vector\n", numElementos);
        
    // Convertir la clave al tipo adecuado y comparar
    for (int i = 0; i < numElementos; i++) {
        int found = 0;

        switch (simboloArray->tipo) {
            case TIPO_INT: {
                int elemVal = atoi(elementos[i]);
                int keyVal = (claveVal.tipo == TIPO_INT) ? claveVal.v.i : 
                            (claveVal.tipo == TIPO_FLOAT) ? (int)claveVal.v.f : 
                            (claveVal.tipo == TIPO_DOUBLE) ? (int)claveVal.v.d : 0;
                        
                printf("[DEBUG] Comparando elemento[%d]=%d con clave=%d\n", i, elemVal, keyVal);
                found = (elemVal == keyVal);
                break;
            }
            case TIPO_FLOAT: {
                float elemVal = strtof(elementos[i], NULL);
                float keyVal = (claveVal.tipo == TIPO_INT) ? (float)claveVal.v.i : 
                            (claveVal.tipo == TIPO_FLOAT) ? claveVal.v.f : 
                            (claveVal.tipo == TIPO_DOUBLE) ? (float)claveVal.v.d : 0.0f;
                    
                printf("[DEBUG] Comparando elemento[%d]=%f con clave=%f\n", i, elemVal, keyVal);
                found = (fabs(elemVal - keyVal) < 0.00001f);  // Tolerancia para float
                break;
            }
            case TIPO_DOUBLE: {
                double elemVal = strtod(elementos[i], NULL);
                double keyVal = (claveVal.tipo == TIPO_INT) ? (double)claveVal.v.i : 
                                (claveVal.tipo == TIPO_FLOAT) ? (double)claveVal.v.f : 
                                (claveVal.tipo == TIPO_DOUBLE) ? claveVal.v.d : 0.0;
                    
                printf("[DEBUG] Comparando elemento[%d]=%f con clave=%f\n", i, elemVal, keyVal);
                found = (fabs(elemVal - keyVal) < 0.00001);  // Tolerancia para double
                break;
            }
            case TIPO_CHAR: {
                char elemVal = elementos[i][0];
                char keyVal = (claveVal.tipo == TIPO_CHAR) ? claveVal.v.c : 
                        (claveVal.tipo == TIPO_INT) ? (char)claveVal.v.i : '\0';
                        
                printf("[DEBUG] Comparando elemento[%d]=%c con clave=%c\n", i, elemVal, keyVal);
                found = (elemVal == keyVal);
                break;
            }
            case TIPO_BOOLEAN: {
                int elemVal = (strcmp(elementos[i], "true") == 0) ? 1 : 0;
                int keyVal = (claveVal.tipo == TIPO_BOOLEAN) ? claveVal.v.b : 0;
                    
                printf("[DEBUG] Comparando elemento[%d]=%s con clave=%s\n", 
                    i, elemVal ? "true" : "false", keyVal ? "true" : "false");
                found = (elemVal == keyVal);
                break;
            }
            case TIPO_STRING: {
                if (claveVal.tipo == TIPO_STRING && claveVal.v.s) {
                    // Eliminar comillas de los elementos si existen
                    char* elemStrTemp = strdup(elementos[i]);
                    size_t len = strlen(elemStrTemp);

                    // Eliminar las comillas del principio y final si existen
                    if (len >= 2 && elemStrTemp[0] == '"' && elemStrTemp[len-1] == '"') {
                        elemStrTemp[len-1] = '\0';  // Eliminar comilla final
                        char* strippedElem = elemStrTemp + 1;  // Saltar comilla inicial
                                
                        printf("[DEBUG] Comparando elemento[%d]=%s con clave=%s\n", 
                            i, strippedElem, claveVal.v.s);
                                    
                        found = (strcmp(strippedElem, claveVal.v.s) == 0);
                    } else {
                        // Si no tiene comillas, comparar directamente
                        printf("[DEBUG] Comparando elemento[%d]=%s con clave=%s\n", 
                            i, elemStrTemp, claveVal.v.s);
                                
                        found = (strcmp(elemStrTemp, claveVal.v.s) == 0);
                    }
                            
                    free(elemStrTemp);
                }
                break;
            }
            default:
                break;
        }
                
        if (found) {
            indice = i;
            printf("[DEBUG] ¡Encontrado! en índice %d\n", i);
            break;  // Encontrado, salimos del bucle
        }
    }
            
    // Liberar memoria
    for (int i = 0; i < numElementos; i++) {
        free(elementos[i]);
    }
    free(elementosCopy);
    free(arrayStr);
            
    // Devolver el índice como un entero
    r = makeNum(TIPO_INT, (double)indice);
    r.tipo = TIPO_INT;
    printf("[DEBUG] Arrays.indexOf resultado: %d\n", indice);
            
    return r;
}
void actualizarVector(NodoAST* e, ValorSimbolo resultado) {
    if (!e || e->num_hijos < 1 || e->hijos[0]->tipo != AST_IDENTIFICADOR) {
        return;
    }
    
    NodoAST* idNodo = e->hijos[0];
    Simbolo* simboloArray = buscarSimbolo(idNodo->valor);
    if (!simboloArray) {
        return;
    }
    
    // Actualizar el valor del símbolo en la tabla
    if (simboloArray->valor.v.s) free(simboloArray->valor.v.s);
    simboloArray->valor.v.s = strdup(resultado.v.s);
    simboloArray->valor.inicializado = 1;
    simboloArray->valor.tipo = resultado.tipo;
    
    printf("[DEBUG ADD] Vector '%s' actualizado con nuevo valor: %s\n", 
           idNodo->valor, simboloArray->valor.v.s);
}

void obtenerDimensionesArray(Simbolo* simbolo, int* dimensiones, int* numDimensiones) {
    if (!simbolo || !simbolo->valor.v.s || !dimensiones || !numDimensiones) return;
    
    // Inicializar
    *numDimensiones = 0;
    
    // Si es una matriz, procesar su formato específico
    if (simbolo->clase == SIMBOLO_MATRIZ) {
        // Formato matriz: [[val1,val2,...],[val1,val2,...],...]
        char* matrixCopy = strdup(simbolo->valor.v.s);
        if (matrixCopy) {
            // Usar la función de acceso_matriz.c para analizar dimensiones
            int numFilas = 0;
            int numColumnas = 0;
            
            // Llamar a la función correcta
            analizarDimensionesMatriz(simbolo->valor.v.s, &numFilas, &numColumnas);
            
            printf("[EVALUADOR] Matriz '%s' detectada con formato: %d filas, %d columnas\n", 
                simbolo->nombre, numFilas, numColumnas);
            
            dimensiones[0] = numFilas;
            dimensiones[1] = numColumnas;
            *numDimensiones = 2;
            
            free(matrixCopy);
        }
    } else if (simbolo->clase == SIMBOLO_VECTOR) {
        // Vector: contar elementos directamente
        char* valorCopy = strdup(simbolo->valor.v.s);
        int count = 1; // Al menos un elemento
        
        // Eliminar corchetes si existen
        if (valorCopy[0] == '[') {
            memmove(valorCopy, valorCopy + 1, strlen(valorCopy));
            int len = strlen(valorCopy);
            if (len > 0 && valorCopy[len - 1] == ']') {
                valorCopy[len - 1] = '\0';
            }
        }
        
        // Contar comas para determinar número de elementos
        for (char* p = valorCopy; *p; p++) {
            if (*p == ',') count++;
        }
        
        dimensiones[0] = count;
        *numDimensiones = 1;
        
        free(valorCopy);
        printf("[EVALUADOR] Vector '%s' con %d elementos\n", simbolo->nombre, count);
    }
}

ValorSimbolo evaluarLength(NodoAST* e) {
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    r.tipo = TIPO_INT;
    r.v.i = 0;

    if (e->num_hijos < 1) {
        printf("[ERROR] Array.length requiere un identificador o acceso\n");
        return r;
    }

    NodoAST* idNodo = e->hijos[0];

    // Caso 1: Identificador directo (vector o matriz)
    if (idNodo->tipo == AST_IDENTIFICADOR) {
        Simbolo* simbolo = buscarSimbolo(idNodo->valor);
        if (!simbolo) {
            printf("[ERROR] Variable '%s' no declarada\n", idNodo->valor);
            return r;
        }
        if (simbolo->clase != SIMBOLO_VECTOR && simbolo->clase != SIMBOLO_MATRIZ && simbolo->clase != SIMBOLO_ARRAY_MULTIDIMENSIONAL) {
            printf("[ERROR] Array.length solo se puede aplicar a arreglos\n");
            return r;
        }
        int dimensiones[10] = {0};
        int numDimensiones = 0;
        obtenerDimensionesArray(simbolo, dimensiones, &numDimensiones);
        int len = dimensiones[0];
        r = makeNum(TIPO_INT, (double)len);
        r.tipo = TIPO_INT;
        r.inicializado = 1;
        printf("[DEBUG] Array.length de '%s' = %d\n", idNodo->valor, len);
        return r;
    }

    // Caso 2: Acceso a matriz/vector (ejemplo: matriz[exp].length)
    else if (idNodo->tipo == AST_ACCESO) {
        // Obtener el identificador base
        if (idNodo->num_hijos < 2 || idNodo->hijos[0]->tipo != AST_IDENTIFICADOR) {
            printf("[ERROR] Acceso inválido para length\n");
            return r;
        }
        NodoAST* idBase = idNodo->hijos[0];
        Simbolo* simbolo = buscarSimbolo(idBase->valor);
        if (!simbolo) {
            printf("[ERROR] Variable '%s' no declarada\n", idBase->valor);
            return r;
        }
        if (simbolo->clase != SIMBOLO_MATRIZ) {
            printf("[ERROR] Array.length solo se puede aplicar a filas de matrices\n");
            return r;
        }
        // Obtener el índice de fila
        NodoAST* dims = idNodo->hijos[1];
        if (!dims || dims->num_hijos < 1 || dims->hijos[0]->num_hijos < 1) {
            printf("[ERROR] Acceso sin dimensiones\n");
            return r;
        }
        ValorSimbolo idx = evaluarExpr(dims->hijos[0]->hijos[0]);
        if (!idx.inicializado) {
            printf("[ERROR] Índice no inicializado en acceso para length\n");
            return r;
        }
        int fila = idx.v.i;
        // Obtener dimensiones de la matriz
        int numFilas = 0, numColumnas = 0;
        analizarDimensionesMatriz(simbolo->valor.v.s, &numFilas, &numColumnas);
        if (fila < 0 || fila >= numFilas) {
            printf("[ERROR] Índice de fila fuera de rango en length: %d (rango: 0-%d)\n", fila, numFilas-1);
            return r;
        }
        // Devolver el número de columnas de esa fila (todas las filas tienen igual cantidad en tu modelo)
        r = makeNum(TIPO_INT, (double)numColumnas);
        r.tipo = TIPO_INT;
        r.inicializado = 1;
        printf("[DEBUG] Array.length de '%s[%d]' = %d\n", idBase->valor, fila, numColumnas);
        return r;
    }

    // Caso no soportado
    printf("[ERROR] Array.length solo se puede aplicar a identificadores o accesos\n");
    return r;
}

ValorSimbolo evaluarAdd(NodoAST* e) {
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    r.tipo = TIPO_DESCONOCIDO;
    r.inicializado = 0;
    
    // Verificamos que tengamos identificador como primer hijo
    if (e->num_hijos < 1 || e->hijos[0]->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Array.add requiere un identificador de array\n");
        return r;
    }
    
    // Verificar si hay un elemento para añadir
    if (e->num_hijos < 2) {
        printf("[ERROR] Array.add requiere un elemento para añadir\n");
        return r;
    }
    
    // Buscar el símbolo del array
    NodoAST* idNodo = e->hijos[0];
    Simbolo* simboloArray = buscarSimbolo(idNodo->valor);
    if (!simboloArray) {
        printf("[ERROR] Array '%s' no declarado\n", idNodo->valor);
        return r;
    }
    
    // Verificar que sea un vector
    if (simboloArray->clase != SIMBOLO_VECTOR) {
        printf("[ERROR] '%s' no es un vector\n", idNodo->valor);
        return r;
    }
    
    // Verificar que esté inicializado
    if (!simboloArray->valor.inicializado || !simboloArray->valor.v.s) {
        printf("[ERROR] Vector '%s' no inicializado\n", idNodo->valor);
        return r;
    }
    
    // Evaluar el elemento a añadir
    ValorSimbolo nuevoElemento = evaluarExpr(e->hijos[1]);
    if (!nuevoElemento.inicializado) {
        printf("[ERROR] Elemento a añadir no inicializado\n");
        return r;
    }
    
    // Verificar compatibilidad de tipos
    if (nuevoElemento.tipo != simboloArray->tipo) {
        printf("[ERROR] Tipo incompatible: intentando añadir %s a un array de %s\n", 
            nombreTipo(nuevoElemento.tipo), nombreTipo(simboloArray->tipo));
        return r;
    }
    
    // Obtener la representación del array actual
    char* arrayActual = strdup(simboloArray->valor.v.s);
    
    // Convertir el nuevo elemento a string según su tipo
    char elementoStr[256];
    switch (nuevoElemento.tipo) {
        case TIPO_INT:
            snprintf(elementoStr, sizeof(elementoStr), "%d", nuevoElemento.v.i);
            break;
        case TIPO_FLOAT:
            snprintf(elementoStr, sizeof(elementoStr), "%f", nuevoElemento.v.f);
            break;
        case TIPO_DOUBLE:
            snprintf(elementoStr, sizeof(elementoStr), "%f", nuevoElemento.v.d);
            break;
        case TIPO_CHAR:
            snprintf(elementoStr, sizeof(elementoStr), "'%c'", nuevoElemento.v.c);
            break;
        case TIPO_BOOLEAN:
            snprintf(elementoStr, sizeof(elementoStr), "%s", nuevoElemento.v.b ? "true" : "false");
            break;
        case TIPO_STRING:
            snprintf(elementoStr, sizeof(elementoStr), "\"%s\"", nuevoElemento.v.s ? nuevoElemento.v.s : "");
            break;
        default:
            elementoStr[0] = '\0';
            break;
    }
    
    // Crear el nuevo array con el elemento añadido
    char nuevoArray[4096]; // Tamaño suficiente para la mayoría de los arrays
    
    // Si el array actual está vacío o es [0], crear uno nuevo con solo el elemento
    if (strcmp(arrayActual, "[]") == 0 || strcmp(arrayActual, "[0]") == 0) {
        snprintf(nuevoArray, sizeof(nuevoArray), "[%s]", elementoStr);
    } else {
        // Eliminar el corchete de cierre, añadir el elemento y cerrar el array
        size_t len = strlen(arrayActual);
        if (len > 0 && arrayActual[len-1] == ']') {
            arrayActual[len-1] = '\0';
            snprintf(nuevoArray, sizeof(nuevoArray), "%s,%s]", arrayActual, elementoStr);
        } else {
            // Formato inválido, usar formato por defecto
            snprintf(nuevoArray, sizeof(nuevoArray), "[%s]", elementoStr);
        }
    }
    
    // Crear el resultado como un nuevo array del mismo tipo
    r.tipo = simboloArray->tipo;
    r.inicializado = 1;
    r.v.s = strdup(nuevoArray);
    
    printf("[DEBUG] Array.add: %s + %s = %s\n", 
        simboloArray->valor.v.s, elementoStr, nuevoArray);

    actualizarVector(e, r);
    
    free(arrayActual);
    return r;
}