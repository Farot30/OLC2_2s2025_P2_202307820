#include "acceso_arrays.h"
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

// Función auxiliar para parsear la representación de un array multidimensional
// y obtener un valor específico según los índices proporcionados
static ValorSimbolo obtenerValorDeArray(Simbolo* simbolo, int* indices, int numIndices) {
    ValorSimbolo resultado;
    memset(&resultado, 0, sizeof(ValorSimbolo));
    
    if (!simbolo || simbolo->clase != SIMBOLO_ARRAY_MULTIDIMENSIONAL || !simbolo->valor.inicializado) {
        printf("[ERROR] Símbolo no es un array multidimensional inicializado\n");
        return resultado;
    }
    
    // Verificar que tenemos una representación en string del array
    if (!simbolo->valor.v.s) {
        printf("[ERROR] Array no tiene representación inicializada\n");
        return resultado;
    }
    
    // La representación debe ser algo como [[1,2,3],[4,5,6]]
    char* arrayStr = strdup(simbolo->valor.v.s);
    if (!arrayStr) {
        printf("[ERROR] No se pudo copiar la representación del array\n");
        return resultado;
    }
    
    printf("[DEBUG] Procesando array: %s\n", arrayStr);
    printf("[DEBUG] Buscando elementos en índices: [");
    for (int i = 0; i < numIndices; i++) {
        printf("%d", indices[i]);
        if (i < numIndices - 1) printf("][");
    }
    printf("]\n");
    
    // Navegar en la estructura anidada
    char* posActual = arrayStr;
    int nivel = 0;
    
    // Para cada nivel de profundidad (dimensión)
    while (nivel < numIndices) {
        int indiceActual = indices[nivel];
        
        // Encontrar el corchete de apertura para este nivel
        char* abreCorchete = strchr(posActual, '[');
        if (!abreCorchete) {
            printf("[ERROR] Formato de array inválido, no se encontró '[' en nivel %d\n", nivel);
            free(arrayStr);
            return resultado;
        }
        
        // Avanzar al contenido dentro del corchete
        posActual = abreCorchete + 1;
        
        // Navegar hasta el índice correcto en este nivel
        int elementoActual = 0;
        int dentroComillas = 0;
        int profundidadCorchetes = 0;
        
        while (elementoActual < indiceActual && *posActual) {
            // Manejar comillas para strings
            if (*posActual == '"' && (posActual == arrayStr || *(posActual-1) != '\\')) {
                dentroComillas = !dentroComillas;
            }
            
            // Saltar contenido dentro de comillas
            if (dentroComillas) {
                posActual++;
                continue;
            }
            
            // Contar corchetes anidados
            if (*posActual == '[') {
                profundidadCorchetes++;
            } else if (*posActual == ']') {
                profundidadCorchetes--;
            }
            
            // Contar elementos (comas al nivel correcto)
            if (*posActual == ',' && profundidadCorchetes == 0) {
                elementoActual++;
            }
            
            posActual++;
        }
        
        // Verificar si encontramos suficientes elementos
        if (elementoActual < indiceActual) {
            printf("[ERROR] Índice %d fuera de rango en dimensión %d\n", indiceActual, nivel);
            free(arrayStr);
            return resultado;
        }
        
        nivel++;
    }
    
    // Ahora estamos posicionados al inicio del elemento deseado
    // Extraer el valor hasta la próxima coma o corchete de cierre
    char* inicioValor = posActual;
    char* finValor = inicioValor;
    int profundidadCorchetes = 0;
    int dentroComillas = 0;
    
    // Si es el último nivel, el valor es un elemento primitivo
    // Si no, es un subarreglo
    if (nivel == numIndices) {
        // Buscar el final del valor (próxima coma o corchete)
        while (*finValor) {
            // Manejar comillas para strings
            if (*finValor == '"' && (finValor == arrayStr || *(finValor-1) != '\\')) {
                dentroComillas = !dentroComillas;
            }
            
            // Saltar contenido dentro de comillas
            if (dentroComillas) {
                finValor++;
                continue;
            }
            
            // Contar corchetes
            if (*finValor == '[') {
                profundidadCorchetes++;
            } else if (*finValor == ']') {
                if (profundidadCorchetes == 0) {
                    break; // Final del elemento
                }
                profundidadCorchetes--;
            } else if (*finValor == ',' && profundidadCorchetes == 0) {
                break; // Final del elemento
            }
            
            finValor++;
        }
        
        // Extraer el valor
        int longitud = finValor - inicioValor;
        char* valorStr = (char*)malloc(longitud + 1);
        if (!valorStr) {
            printf("[ERROR] No se pudo asignar memoria para valor\n");
            free(arrayStr);
            return resultado;
        }
        
        strncpy(valorStr, inicioValor, longitud);
        valorStr[longitud] = '\0';
        
        // Eliminar espacios en blanco
        char* valorTrim = valorStr;
        while (isspace(*valorTrim)) valorTrim++;
        char* end = valorTrim + strlen(valorTrim) - 1;
        while (end > valorTrim && isspace(*end)) *end-- = '\0';
        
        printf("[DEBUG] Valor extraído: '%s'\n", valorTrim);
        
        // Convertir el valor según el tipo del array
        resultado.tipo = simbolo->tipo;
        resultado.inicializado = 1;
        
        switch (simbolo->tipo) {
            case TIPO_INT:
                resultado.v.i = atoi(valorTrim);
                break;
            case TIPO_FLOAT:
                resultado.v.f = atof(valorTrim);
                break;
            case TIPO_DOUBLE:
                resultado.v.d = atof(valorTrim);
                break;
            case TIPO_CHAR:
                // Verificar si es un carácter entre comillas
                if (valorTrim[0] == '\'' && strlen(valorTrim) >= 3 && valorTrim[strlen(valorTrim)-1] == '\'') {
                    // Extraer el carácter entre comillas
                    resultado.v.c = valorTrim[1];
                } else {
                    resultado.v.c = valorTrim[0];
                }
                break;
            case TIPO_BOOLEAN:
                resultado.v.b = (strcmp(valorTrim, "true") == 0 || strcmp(valorTrim, "1") == 0);
                break;
            case TIPO_STRING:
                // Verificar si es una cadena entre comillas
                if (valorTrim[0] == '"' && valorTrim[strlen(valorTrim)-1] == '"') {
                    // Extraer la cadena entre comillas
                    int len = strlen(valorTrim) - 2;
                    char* str = (char*)malloc(len + 1);
                    strncpy(str, valorTrim + 1, len);
                    str[len] = '\0';
                    resultado.v.s = str;
                } else {
                    resultado.v.s = strdup(valorTrim);
                }
                break;
            default:
                printf("[ERROR] Tipo de dato no soportado: %d\n", simbolo->tipo);
                resultado.inicializado = 0;
                break;
        }
        
        free(valorStr);
    } else {
        // Si no hemos llegado al nivel deseado, retornar error
        printf("[ERROR] No se pudo obtener valor para array de %d dimensiones con %d índices\n", 
               nivel, numIndices);
        resultado.inicializado = 0;
    }
    
    free(arrayStr);
    return resultado;
}

// Función para verificar el acceso a un array multidimensional
int verificarAccesoArray(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_ACCESO) {
        printf("[ERROR] Nodo inválido para acceso a array\n");
        return 0;
    }
    
    // Verificar que haya al menos un hijo para el identificador
    if (nodo->num_hijos < 1 || nodo->hijos[0]->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Estructura de acceso inválida, falta identificador\n");
        return 0;
    }
    
    // Obtener el identificador
    NodoAST* idNodo = nodo->hijos[0];
    const char* idArray = idNodo->valor;
    
    // Buscar el símbolo en la tabla
    Simbolo* simbolo = buscarSimbolo(idArray);
    if (!simbolo) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Variable '%s' no declarada", idArray);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return 0;
    }
    
    // Verificar que sea un array multidimensional
    if (simbolo->clase != SIMBOLO_ARRAY_MULTIDIMENSIONAL) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Variable '%s' no es un array multidimensional", idArray);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return 0;
    }
    
    // Verificar que el array esté inicializado
    if (!simbolo->valor.inicializado || !simbolo->valor.v.s) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Array '%s' no inicializado", idArray);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return 0;
    }
    
    // Verificar que haya nodo de dimensiones
    if (nodo->num_hijos < 2 || nodo->hijos[1]->tipo != AST_DIMENSIONES) {
        printf("[ERROR] Estructura de acceso inválida, faltan dimensiones\n");
        return 0;
    }
    
    NodoAST* dimensionesNodo = nodo->hijos[1];
    int numDimensiones = dimensionesNodo->num_hijos;
    
    // Verificar que haya al menos una dimensión
    if (numDimensiones == 0) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Acceso a array '%s' sin índices", idArray);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return 0;
    }
    
    // Verificar cada dimensión
    for (int i = 0; i < numDimensiones; i++) {
        NodoAST* dimNodo = dimensionesNodo->hijos[i];
        
        if (dimNodo->tipo != AST_1DIMENSION || dimNodo->num_hijos < 1) {
            printf("[ERROR] Formato inválido para dimensión %d\n", i+1);
            return 0;
        }
        
        // Evaluar el índice para verificar que sea un entero
        ValorSimbolo idxValor = evaluarExpr(dimNodo->hijos[0]);
        if (!idxValor.inicializado || idxValor.tipo != TIPO_INT) {
            char msg[128];
            snprintf(msg, sizeof(msg), "Índice %d debe ser entero", i+1);
            addError(msg, ambitoActual(), dimNodo->linea, dimNodo->columna);
            printf("[ERROR] %s\n", msg);
            return 0;
        }
        
        // Verificar que el índice no sea negativo
        if (idxValor.v.i < 0) {
            char msg[128];
            snprintf(msg, sizeof(msg), "Índice %d negativo (%d)", i+1, idxValor.v.i);
            addError(msg, ambitoActual(), dimNodo->linea, dimNodo->columna);
            printf("[ERROR] %s\n", msg);
            return 0;
        }
    }
    
    return 1;
}

// Evalúa el acceso a un array multidimensional y devuelve su valor
ValorSimbolo evaluarAccesoArray(NodoAST* nodo) {
    ValorSimbolo resultado;
    memset(&resultado, 0, sizeof(ValorSimbolo));
    
    // Verificar el acceso
    if (!verificarAccesoArray(nodo)) {
        return resultado;
    }
    
    // Obtener el identificador y el símbolo
    NodoAST* idNodo = nodo->hijos[0];
    Simbolo* simbolo = buscarSimbolo(idNodo->valor);
    
    // Obtener las dimensiones
    NodoAST* dimensionesNodo = nodo->hijos[1];
    int numDimensiones = dimensionesNodo->num_hijos;
    
    // Evaluar cada índice
    int indices[10]; // Máximo 10 dimensiones
    
    for (int i = 0; i < numDimensiones && i < 10; i++) {
        NodoAST* dimNodo = dimensionesNodo->hijos[i];
        ValorSimbolo idxValor = evaluarExpr(dimNodo->hijos[0]);
        indices[i] = idxValor.v.i;
    }
    
    // Obtener el valor del array
    resultado = obtenerValorDeArray(simbolo, indices, numDimensiones);
    
    // Verificar que obtuvimos un valor válido
    if (!resultado.inicializado) {
        char msg[128];
        snprintf(msg, sizeof(msg), "No se pudo obtener valor del array '%s' en la posición especificada", 
                 idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
    } else {
        printf("[DEBUG] Valor obtenido de '%s': ", idNodo->valor);
        switch (resultado.tipo) {
            case TIPO_INT:
                printf("%d\n", resultado.v.i);
                break;
            case TIPO_FLOAT:
                printf("%f\n", resultado.v.f);
                break;
            case TIPO_DOUBLE:
                printf("%lf\n", resultado.v.d);
                break;
            case TIPO_CHAR:
                printf("'%c'\n", resultado.v.c);
                break;
            case TIPO_BOOLEAN:
                printf("%s\n", resultado.v.b ? "true" : "false");
                break;
            case TIPO_STRING:
                printf("\"%s\"\n", resultado.v.s ? resultado.v.s : "(null)");
                break;
            default:
                printf("(tipo desconocido)\n");
                break;
        }
    }
    
    return resultado;
}


void asignarElementoArrayMultidimensional(Simbolo* simbolo, int* indices, int numIndices, ValorSimbolo valor) {
    if (!simbolo || simbolo->clase != SIMBOLO_ARRAY_MULTIDIMENSIONAL || !simbolo->valor.inicializado) {
        printf("[ERROR] Símbolo no es un array multidimensional inicializado\n");
        return;
    }
    
    // Verificar que tenemos una representación en string del array
    if (!simbolo->valor.v.s) {
        printf("[ERROR] Array no tiene representación inicializada\n");
        return;
    }
    
    // La representación ahora es algo como [[1,2,3],[4,5,6]]
    char* arrayStr = strdup(simbolo->valor.v.s);
    if (!arrayStr) {
        printf("[ERROR] No se pudo copiar la representación del array\n");
        return;
    }
    
    printf("[DEBUG] Procesando array para escritura: %s\n", arrayStr);
    
    // Navegar en la estructura anidada para encontrar la posición a modificar
    char* posActual = arrayStr;
    int nivel = 0;
    
    // Para cada nivel de profundidad (dimensión)
    while (nivel < numIndices) {
        int indiceActual = indices[nivel];
        
        // Encontrar el corchete de apertura para este nivel
        char* abreCorchete = strchr(posActual, '[');
        if (!abreCorchete) {
            printf("[ERROR] Formato de array inválido, no se encontró '[' en nivel %d\n", nivel);
            free(arrayStr);
            return;
        }
        
        // Avanzar al contenido dentro del corchete
        posActual = abreCorchete + 1;
        
        // Navegar hasta el índice correcto en este nivel
        int elementoActual = 0;
        int dentroComillas = 0;
        int profundidadCorchetes = 0;
        
        while (elementoActual < indiceActual && *posActual) {
            // Manejar comillas para strings
            if (*posActual == '"' && (posActual == arrayStr || *(posActual-1) != '\\')) {
                dentroComillas = !dentroComillas;
            }
            
            // Saltar contenido dentro de comillas
            if (dentroComillas) {
                posActual++;
                continue;
            }
            
            // Contar corchetes anidados
            if (*posActual == '[') {
                profundidadCorchetes++;
            } else if (*posActual == ']') {
                profundidadCorchetes--;
            }
            
            // Contar elementos (comas al nivel correcto)
            if (*posActual == ',' && profundidadCorchetes == 0) {
                elementoActual++;
            }
            
            posActual++;
        }
        
        // Verificar si encontramos suficientes elementos
        if (elementoActual < indiceActual) {
            printf("[ERROR] Índice %d fuera de rango en dimensión %d\n", indiceActual, nivel);
            free(arrayStr);
            return;
        }
        
        nivel++;
    }
    
    // Ahora estamos posicionados al inicio del elemento a modificar
    // Si es el último nivel, reemplazar el valor
    if (nivel == numIndices) {
        // Encontrar los límites del valor actual (hasta la próxima coma o corchete)
        char* inicioValor = posActual;
        char* finValor = inicioValor;
        int profundidadCorchetes = 0;
        int dentroComillas = 0;
        
        // Buscar el final del valor (próxima coma o corchete)
        while (*finValor) {
            // Manejar comillas para strings
            if (*finValor == '"' && (finValor == arrayStr || *(finValor-1) != '\\')) {
                dentroComillas = !dentroComillas;
            }
            
            // Saltar contenido dentro de comillas
            if (dentroComillas) {
                finValor++;
                continue;
            }
            
            // Contar corchetes
            if (*finValor == '[') {
                profundidadCorchetes++;
            } else if (*finValor == ']') {
                if (profundidadCorchetes == 0) {
                    break; // Final del elemento
                }
                profundidadCorchetes--;
            } else if (*finValor == ',' && profundidadCorchetes == 0) {
                break; // Final del elemento
            }
            
            finValor++;
        }
        
        // Convertir el nuevo valor a string según el tipo
        char nuevoValorStr[128];
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
                snprintf(nuevoValorStr, sizeof(nuevoValorStr), "'%c'", valor.v.c);
                break;
            case TIPO_BOOLEAN:
                snprintf(nuevoValorStr, sizeof(nuevoValorStr), "%s", valor.v.b ? "true" : "false");
                break;
            case TIPO_STRING:
                snprintf(nuevoValorStr, sizeof(nuevoValorStr), "\"%s\"", 
                         valor.v.s ? valor.v.s : "");
                break;
            default:
                strcpy(nuevoValorStr, "null");
                break;
        }
        
        // Construir la nueva representación del array reemplazando el valor
        char* nuevoArray = (char*)malloc(strlen(arrayStr) + strlen(nuevoValorStr) + 1);
        if (!nuevoArray) {
            printf("[ERROR] No se pudo asignar memoria para el nuevo array\n");
            free(arrayStr);
            return;
        }
        
        // Copiar la parte antes del valor
        size_t preLen = inicioValor - arrayStr;
        strncpy(nuevoArray, arrayStr, preLen);
        nuevoArray[preLen] = '\0';
        
        // Añadir el nuevo valor
        strcat(nuevoArray, nuevoValorStr);
        
        // Añadir la parte después del valor original
        strcat(nuevoArray, finValor);
        
        // Actualizar el array en el símbolo
        if (simbolo->valor.v.s) {
            free(simbolo->valor.v.s);
        }
        simbolo->valor.v.s = nuevoArray;
        
        free(arrayStr);
        
        printf("[DEBUG] Valor en array '%s[", simbolo->nombre);
        for (int i = 0; i < numIndices; i++) {
            printf("%d", indices[i]);
            if (i < numIndices - 1) printf("][");
        }
        printf("]' actualizado a: %s\n", nuevoValorStr);
    } else {
        printf("[ERROR] No se llegó al nivel de profundidad necesario en el array\n");
        free(arrayStr);
    }
}