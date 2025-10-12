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

// En src/semantico/Embebidas/EvaluadoresArrays.c - agregar estas funciones auxiliares

static char* extraerSubarrayComoString(const char* arrayStr, int* indices, int numIndices) {
    if (!arrayStr || numIndices <= 0) return NULL;
    
    char* arrayCopy = strdup(arrayStr);
    char* posActual = arrayCopy;
    
    printf("[DEBUG extraerSubarray] Procesando: %s con %d índices\n", arrayStr, numIndices);
    
    // Para cada nivel de profundidad (dimensión)
    for (int nivel = 0; nivel < numIndices; nivel++) {
        int indiceActual = indices[nivel];
        printf("[DEBUG extraerSubarray] Nivel %d, buscando índice %d\n", nivel, indiceActual);
        
        // Encontrar el corchete de apertura para este nivel
        char* abreCorchete = strchr(posActual, '[');
        if (!abreCorchete) {
            printf("[ERROR extraerSubarray] No se encontró '[' en nivel %d\n", nivel);
            free(arrayCopy);
            return NULL;
        }
        
        // Avanzar al contenido dentro del corchete
        posActual = abreCorchete + 1;
        
        // Navegar hasta el índice correcto en este nivel
        int elementoActual = 0;
        int profundidadCorchetes = 0;
        
        while (elementoActual < indiceActual && *posActual) {
            if (*posActual == '[') {
                profundidadCorchetes++;
            } else if (*posActual == ']') {
                profundidadCorchetes--;
            } else if (*posActual == ',' && profundidadCorchetes == 0) {
                elementoActual++;
            }
            posActual++;
        }
        
        // Verificar si encontramos suficientes elementos
        if (elementoActual < indiceActual) {
            printf("[ERROR extraerSubarray] Índice %d fuera de rango en nivel %d\n", indiceActual, nivel);
            free(arrayCopy);
            return NULL;
        }
    }
    
    // Ahora extraer el elemento/subarray completo
    char* inicioSubarray = posActual;
    char* finSubarray = inicioSubarray;
    int profundidadCorchetes = 0;
    
    // Si empieza con '[', es un subarray
    if (*inicioSubarray == '[') {
        profundidadCorchetes = 1;
        finSubarray++;
        
        while (*finSubarray && profundidadCorchetes > 0) {
            if (*finSubarray == '[') {
                profundidadCorchetes++;
            } else if (*finSubarray == ']') {
                profundidadCorchetes--;
            }
            finSubarray++;
        }
    } else {
        // Es un elemento primitivo
        while (*finSubarray && *finSubarray != ',' && *finSubarray != ']') {
            finSubarray++;
        }
    }
    
    // Extraer el subarray
    int longitud = finSubarray - inicioSubarray;
    char* resultado = (char*)malloc(longitud + 1);
    if (resultado) {
        strncpy(resultado, inicioSubarray, longitud);
        resultado[longitud] = '\0';
        
        // Limpiar espacios
        char* trim = resultado;
        while (isspace(*trim)) trim++;
        if (trim != resultado) {
            memmove(resultado, trim, strlen(trim) + 1);
        }
    }
    
    free(arrayCopy);
    printf("[DEBUG extraerSubarray] Resultado: '%s'\n", resultado ? resultado : "(null)");
    return resultado;
}

static int contarElementosEnSubarray(const char* subarrayStr) {
    if (!subarrayStr || strlen(subarrayStr) == 0) return 0;
    
    // Si no empieza con '[', es un elemento primitivo
    if (subarrayStr[0] != '[') return 1;
    
    // Es un array, contar elementos
    char* str = strdup(subarrayStr);
    
    // Remover corchetes externos
    if (str[0] == '[') {
        memmove(str, str + 1, strlen(str));
        int len = strlen(str);
        if (len > 0 && str[len - 1] == ']') {
            str[len - 1] = '\0';
        }
    }
    
    if (strlen(str) == 0) {
        free(str);
        return 0;
    }
    
    // Contar elementos separados por comas al nivel 0
    int count = 0;
    int nivel = 0;
    int enString = 0;
    char* p = str;
    int hayElemento = 0;
    
    while (*p) {
        if (*p == '"' && (p == str || *(p-1) != '\\')) {
            enString = !enString;
        } else if (!enString) {
            if (*p == '[') {
                nivel++;
                hayElemento = 1;
            } else if (*p == ']') {
                nivel--;
            } else if (*p == ',' && nivel == 0) {
                if (hayElemento) count++;
                hayElemento = 0;
            } else if (!isspace(*p)) {
                hayElemento = 1;
            }
        }
        p++;
    }
    
    // Contar el último elemento si hay
    if (hayElemento) count++;
    
    free(str);
    
    printf("[DEBUG contarElementos] String: '%s' -> %d elementos\n", subarrayStr, count);
    return count;
}

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

    // ===== CASO 1: Identificador directo (vector, matriz o array) =====
    if (idNodo->tipo == AST_IDENTIFICADOR) {
        Simbolo* simbolo = buscarSimbolo(idNodo->valor);
        if (!simbolo) {
            printf("[ERROR] Variable '%s' no declarada\n", idNodo->valor);
            return r;
        }
        
        // --- SUBCASO 1A: VECTOR ---
        if (simbolo->clase == SIMBOLO_VECTOR) {
            // **PRIMERA PRIORIDAD: Usar la longitud del símbolo si está establecida**
            if (simbolo->longitud > 0) {
                r = makeNum(TIPO_INT, (double)simbolo->longitud);
                r.tipo = TIPO_INT;
                r.inicializado = 1;
                printf("[DEBUG] Vector.length de '%s' usando símbolo.longitud = %d\n", 
                    idNodo->valor, simbolo->longitud);
                return r;
            }
            
            // **SEGUNDA PRIORIDAD: Calcular desde el string del vector**
            if (simbolo->valor.inicializado && simbolo->valor.v.s) {
                char* vectorStr = strdup(simbolo->valor.v.s);
                int len = 0;
                
                // Remover corchetes si los hay
                if (vectorStr[0] == '[') {
                    memmove(vectorStr, vectorStr + 1, strlen(vectorStr));
                    int strLen = strlen(vectorStr);
                    if (strLen > 0 && vectorStr[strLen - 1] == ']') {
                        vectorStr[strLen - 1] = '\0';
                    }
                }
                
                // Contar elementos separados por comas
                if (strlen(vectorStr) > 0) {
                    len = 1; // Al menos un elemento si hay contenido
                    for (char* p = vectorStr; *p; p++) {
                        if (*p == ',') len++;
                    }
                }
                
                free(vectorStr);
                
                // **ACTUALIZAR la longitud del símbolo para uso futuro**
                simbolo->longitud = len;
                
                r = makeNum(TIPO_INT, (double)len);
                r.tipo = TIPO_INT;
                r.inicializado = 1;
                printf("[DEBUG] Vector.length de '%s' calculado = %d\n", 
                    idNodo->valor, len);
                return r;
            }
            
            printf("[ERROR] Vector '%s' no está inicializado\n", idNodo->valor);
            return r;
        }
        
        // --- SUBCASO 1B: MATRIZ ---
        else if (simbolo->clase == SIMBOLO_MATRIZ) {
            // **PRIORIDAD 1: Usar filas almacenadas en el símbolo**
            if (simbolo->filas > 0) {
                r = makeNum(TIPO_INT, (double)simbolo->filas);
                r.tipo = TIPO_INT;
                r.inicializado = 1;
                printf("[DEBUG] Matriz.length de '%s' usando símbolo.filas = %d\n", 
                    idNodo->valor, simbolo->filas);
                return r;
            }
            
            // **PRIORIDAD 2: Analizar desde el string de la matriz**
            if (simbolo->valor.inicializado && simbolo->valor.v.s) {
                int numFilas = 0;
                int numColumnas = 0;
                analizarDimensionesMatriz(simbolo->valor.v.s, &numFilas, &numColumnas);
                
                // **ACTUALIZAR dimensiones del símbolo**
                simbolo->filas = numFilas;
                simbolo->columnas = numColumnas;
                
                r = makeNum(TIPO_INT, (double)numFilas);
                r.tipo = TIPO_INT;
                r.inicializado = 1;
                printf("[DEBUG] Matriz.length de '%s' calculado = %d filas\n", 
                    idNodo->valor, numFilas);
                return r;
            }
            
            printf("[ERROR] Matriz '%s' no está inicializada\n", idNodo->valor);
            return r;
        }
        
        // --- SUBCASO 1C: ARRAY MULTIDIMENSIONAL ---
        else if (simbolo->clase == SIMBOLO_ARRAY_MULTIDIMENSIONAL) {
            // Obtener la primera dimensión (número de "bloques" principales)
            if (simbolo->valor.inicializado && simbolo->valor.v.s) {
                char* arrayStr = strdup(simbolo->valor.v.s);
                int dimension1 = 0;
                
                // Contar corchetes de apertura en el primer nivel
                int nivel = 0;
                int contandoPrimerNivel = 1;
                
                for (char* p = arrayStr; *p; p++) {
                    if (*p == '[') {
                        nivel++;
                        if (nivel == 2 && contandoPrimerNivel) {
                            dimension1++;
                        }
                    } else if (*p == ']') {
                        nivel--;
                        if (nivel == 0) {
                            contandoPrimerNivel = 0;
                        }
                    } else if (*p == ',' && nivel == 1) {
                        // Ignorar comas en el primer nivel
                    }
                }
                
                free(arrayStr);
                
                r = makeNum(TIPO_INT, (double)dimension1);
                r.tipo = TIPO_INT;
                r.inicializado = 1;
                printf("[DEBUG] Array[n].length de '%s' = %d\n", 
                    idNodo->valor, dimension1);
                return r;
            }
            
            printf("[ERROR] Array '%s' no está inicializado\n", idNodo->valor);
            return r;
        }
        
        else {
            printf("[ERROR] '.length' solo se puede aplicar a vectores, matrices o arrays\n");
            return r;
        }
    }
    
    // ===== CASO 2: Acceso a subarray (ejemplo: matriz[0].length) =====
    else if (idNodo->tipo == AST_ACCESO) {
        printf("[DEBUG] Evaluando .length sobre acceso\n");
        
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
        
        printf("[DEBUG] Símbolo '%s' encontrado, clase=%d\n", 
            idBase->valor, simbolo->clase);
        
        // --- SUBCASO 2A: Acceso a fila de MATRIZ (matriz[i].length) ---
        if (simbolo->clase == SIMBOLO_MATRIZ) {
            // Obtener las dimensiones del acceso
            NodoAST* dims = idNodo->hijos[1];
            if (!dims || dims->tipo != AST_DIMENSIONES || dims->num_hijos < 1) {
                printf("[ERROR] Acceso sin dimensiones válidas\n");
                return r;
            }
            
            // Debe tener exactamente 1 dimensión para matriz[i].length
            if (dims->num_hijos != 1) {
                printf("[ERROR] .length sobre matriz requiere exactamente 1 índice (ej: matriz[0].length)\n");
                return r;
            }
            
            // Evaluar el índice de fila
            NodoAST* dimNodo = dims->hijos[0];
            if (!dimNodo || dimNodo->tipo != AST_1DIMENSION || dimNodo->num_hijos < 1) {
                printf("[ERROR] Dimensión sin expresión\n");
                return r;
            }
            
            ValorSimbolo idx = evaluarExpr(dimNodo->hijos[0]);
            if (!idx.inicializado) {
                printf("[ERROR] Índice no inicializado en acceso para length\n");
                return r;
            }
            
            int fila = idx.v.i;
            printf("[DEBUG] Índice de fila: %d\n", fila);
            
            // **PRIORIDAD 1: Usar columnas almacenadas en el símbolo**
            if (simbolo->columnas > 0) {
                // Verificar rango de fila
                if (simbolo->filas > 0 && (fila < 0 || fila >= simbolo->filas)) {
                    printf("[ERROR] Índice de fila fuera de rango: %d (rango: 0-%d)\n", 
                        fila, simbolo->filas - 1);
                    return r;
                }
                
                r.tipo = TIPO_INT;
                r.inicializado = 1;
                r.v.i = simbolo->columnas;
                printf("[DEBUG] Matriz[%d].length de '%s' usando símbolo.columnas = %d\n", 
                    fila, idBase->valor, simbolo->columnas);
                return r;
            }
            
            // **PRIORIDAD 2: Analizar desde el string**
            if (simbolo->valor.inicializado && simbolo->valor.v.s) {
                int numFilas = 0;
                int numColumnas = 0;
                analizarDimensionesMatriz(simbolo->valor.v.s, &numFilas, &numColumnas);
                
                // Actualizar dimensiones del símbolo
                simbolo->filas = numFilas;
                simbolo->columnas = numColumnas;
                
                // Verificar rango
                if (fila < 0 || fila >= numFilas) {
                    printf("[ERROR] Índice de fila fuera de rango: %d (rango: 0-%d)\n", 
                        fila, numFilas - 1);
                    return r;
                }
                
                r.tipo = TIPO_INT;
                r.inicializado = 1;
                r.v.i = numColumnas;
                printf("[DEBUG] Matriz[%d].length de '%s' calculado = %d columnas\n", 
                    fila, idBase->valor, numColumnas);
                return r;
            }
            
            printf("[ERROR] Matriz '%s' no está inicializada\n", idBase->valor);
            return r;
        }

        // --- SUBCASO 2B: Acceso a subarray de ARRAY MULTIDIMENSIONAL ---
        else if (simbolo->clase == SIMBOLO_ARRAY_MULTIDIMENSIONAL) {
            // Para arrays de 3+ dimensiones: array[i][j].length
            NodoAST* dims = idNodo->hijos[1];
            if (!dims || dims->tipo != AST_DIMENSIONES) {
                printf("[ERROR] Acceso sin dimensiones válidas\n");
                return r;
            }
            
            int numIndices = dims->num_hijos;
            printf("[DEBUG] Array[n] con %d índices solicitando .length\n", numIndices);
            
            // **NUEVA IMPLEMENTACIÓN: Manejar directamente el string del subarray**
            if (!simbolo->valor.inicializado || !simbolo->valor.v.s) {
                printf("[ERROR] Array '%s' no está inicializado\n", idBase->valor);
                return r;
            }
            
            // Evaluar cada índice
            int indices[10]; // Máximo 10 dimensiones
            for (int i = 0; i < numIndices && i < 10; i++) {
                NodoAST* dimNodo = dims->hijos[i];
                if (dimNodo->tipo != AST_1DIMENSION || dimNodo->num_hijos < 1) {
                    printf("[ERROR] Formato inválido para dimensión %d\n", i+1);
                    return r;
                }
                ValorSimbolo idxValor = evaluarExpr(dimNodo->hijos[0]);
                if (!idxValor.inicializado || idxValor.tipo != TIPO_INT) {
                    printf("[ERROR] Índice %d debe ser entero\n", i+1);
                    return r;
                }
                indices[i] = idxValor.v.i;
            }
            
            // **CORREGIR: Extraer directamente el subarray como string**
            char* subarrayStr = extraerSubarrayComoString(simbolo->valor.v.s, indices, numIndices);
            
            if (!subarrayStr) {
                printf("[ERROR] No se pudo extraer el subarray\n");
                return r;
            }
            
            printf("[DEBUG] Subarray extraído: '%s'\n", subarrayStr);
            
            // Contar elementos en el subarray
            int len = contarElementosEnSubarray(subarrayStr);
            
            free(subarrayStr);
            
            r.tipo = TIPO_INT;
            r.inicializado = 1;
            r.v.i = len;
            printf("[DEBUG] Array[...].length calculado = %d\n", len);
            return r;
        }
        
        else {
            printf("[ERROR] '.length' sobre acceso solo válido para matrices o arrays multidimensionales\n");
            return r;
        }
    }

    // Caso no soportado
    printf("[ERROR] '.length' solo se puede aplicar a identificadores o accesos válidos\n");
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