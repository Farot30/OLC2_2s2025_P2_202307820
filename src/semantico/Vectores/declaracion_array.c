#include "declaracion_array.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include "../../AST/err.h"
#include "../../AST/errorTIpos.h"
#include "../../simbolos/Evaluador.h" 
#include "../../simbolos/Operador.h"
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 

// Función auxiliar para generar una representación anidada de un array multidimensional
char* generarArrayAnidado(int* dimensiones, int numDimensiones, const char* valorDefecto, int nivel) {
    if (nivel >= numDimensiones) {
        // Caso base: retornar el valor por defecto
        return strdup(valorDefecto);
    }
    
    int tamañoActual = dimensiones[nivel];
    
    // Calcular el tamaño del buffer necesario
    // Para cada elemento: valor + posible coma + posibles corchetes
    // Más corchetes externos y posibles espacios adicionales
    size_t bufsize = tamañoActual * (strlen(valorDefecto) + 10) + 10;
    char* buffer = malloc(bufsize);
    if (!buffer) {
        printf("[ERROR] No se pudo asignar memoria para buffer\n");
        return NULL;
    }
    
    strcpy(buffer, "[");
    
    for (int i = 0; i < tamañoActual; i++) {
        char* subarreglo = generarArrayAnidado(dimensiones, numDimensiones, valorDefecto, nivel + 1);
        if (!subarreglo) {
            free(buffer);
            return NULL;
        }
        
        // Verificar si necesitamos ampliar el buffer
        size_t lenActual = strlen(buffer);
        size_t lenSubarreglo = strlen(subarreglo);
        if (lenActual + lenSubarreglo + 3 > bufsize) {
            bufsize = lenActual + lenSubarreglo + 100; // Espacio extra
            char* newbuf = realloc(buffer, bufsize);
            if (!newbuf) {
                free(buffer);
                free(subarreglo);
                printf("[ERROR] No se pudo reasignar memoria para buffer\n");
                return NULL;
            }
            buffer = newbuf;
        }
        
        strcat(buffer, subarreglo);
        free(subarreglo);
        
        if (i < tamañoActual - 1) {
            strcat(buffer, ",");
        }
    }
    
    strcat(buffer, "]");
    return buffer;
}

// Función auxiliar para procesar bloques anidados
static char* procesarBloqueNestado(NodoAST* nodo, TipoDato tipo, int nivel) {
    if (!nodo) return NULL;
    
    printf("[DEBUG] Procesando bloque nivel %d, tipo=%d\n", nivel, nodo->tipo);
    
    // Caso base: encontramos un valor primitivo
    if (nodo->tipo == AST_NUMERO || nodo->tipo == AST_CADENA || 
        nodo->tipo == AST_CARACTER || nodo->tipo == AST_BOOLEAN || 
        nodo->tipo == AST_IDENTIFICADOR) {
        
        ValorSimbolo valor = evaluarExpr(nodo);
        
        if (!valor.inicializado) {
            printf("[ERROR] Valor no inicializado en nivel %d\n", nivel);
            return NULL;
        }
        
        // Verificar compatibilidad de tipo
        if (!esWidening(valor.tipo, tipo) && valor.tipo != tipo) {
            printf("[ERROR] Tipo incompatible en nivel %d: se esperaba %s, se obtuvo %s\n",
                   nivel, nombreTipo(tipo), nombreTipo(valor.tipo));
            return NULL;
        }
        
        // Convertir valor a string
        char* valStr = valorToString(valor);
        if (!valStr) {
            printf("[ERROR] No se pudo convertir valor a string\n");
            return NULL;
        }
        
        return valStr;
    }
    
    // Caso recursivo: procesamos un bloque (lista de elementos)
    size_t bufsize = 1024; // Tamaño inicial
    char* buffer = malloc(bufsize);
    if (!buffer) {
        printf("[ERROR] No se pudo asignar memoria para buffer\n");
        return NULL;
    }
    
    buffer[0] = '[';
    buffer[1] = '\0';
    
    // Determinar el número real de hijos a procesar
    int numHijos = nodo->num_hijos;
    NodoAST* nodoActual = nodo;
    
    // Si el nodo tiene un solo hijo que también es un nodo interno, 
    // usamos ese hijo como el nodo actual
    if (numHijos == 1 && (nodo->hijos[0]->tipo == AST_INTERNO || 
                         nodo->hijos[0]->tipo == AST_LISTA_ELEMENTOS)) {
        nodoActual = nodo->hijos[0];
        numHijos = nodoActual->num_hijos;
    }
    
    for (int i = 0; i < numHijos; i++) {
        char* elemento;
        
        if (nodoActual->hijos[i]->tipo == AST_INTERNO || 
            nodoActual->hijos[i]->tipo == AST_LISTA_ELEMENTOS) {
            // Procesar recursivamente el bloque anidado
            elemento = procesarBloqueNestado(nodoActual->hijos[i], tipo, nivel + 1);
        } else {
            // Procesar valor primitivo
            elemento = procesarBloqueNestado(nodoActual->hijos[i], tipo, nivel + 1);
        }
        
        if (!elemento) {
            free(buffer);
            return NULL;
        }
        
        // Verificar si necesitamos ampliar el buffer
        if (strlen(buffer) + strlen(elemento) + 3 > bufsize) {
            bufsize *= 2;
            char* newbuf = realloc(buffer, bufsize);
            if (!newbuf) {
                free(buffer);
                free(elemento);
                printf("[ERROR] No se pudo reasignar memoria para buffer\n");
                return NULL;
            }
            buffer = newbuf;
        }
        
        strcat(buffer, elemento);
        free(elemento);
        
        if (i < numHijos - 1) {
            strcat(buffer, ",");
        }
    }
    
    strcat(buffer, "]");
    return buffer;
}

int verificarDeclaracionArray(NodoAST* nodo) {
    printf("[DEBUG verificarDeclaracionArray] Iniciando verificación\n");

    if (!nodo) {
        printf("[ERROR] Nodo es NULL\n");
        return 0;
    }

    if (nodo->num_hijos < 3) {
        printf("[ERROR] Nodo no tiene suficientes hijos para una declaración de array\n");
        return 0;
    }

    // Depuración de la estructura del nodo
    printf("[DEBUG] Estructura del nodo con %d hijos:\n", nodo->num_hijos);
    for (int i = 0; i < nodo->num_hijos; i++) {
        if (nodo->hijos[i]) {
            printf("[DEBUG] Hijo %d: tipo=%d, valor=%s, num_hijos=%d\n", 
                   i, nodo->hijos[i]->tipo, 
                   nodo->hijos[i]->valor ? nodo->hijos[i]->valor : "NULL",
                   nodo->hijos[i]->num_hijos);
        } else {
            printf("[DEBUG] Hijo %d: NULL\n", i);
        }
    }

    NodoAST* tipoNodo = nodo->hijos[0];      // Tipo declarado
    NodoAST* idNodo = nodo->hijos[1];        // Identificador
    NodoAST* tercer_nodo = nodo->hijos[2];   // Puede ser tipoNew o lista de bloques

    // Verificar que los nodos no sean NULL
    if (!tipoNodo || !idNodo || !tercer_nodo) {
        printf("[ERROR] Uno de los nodos hijos es NULL\n");
        return 0;
    }

    // Verificar que los valores no sean NULL
    if (!tipoNodo->valor || !idNodo->valor) {
        printf("[ERROR] Tipo o ID es NULL\n");
        return 0;
    }

    TipoDato tipo = obtenerTipoDesdeString(tipoNodo->valor);

    printf("[DEBUG] Declarando array multidimensional '%s': tipo declarado=%s\n",
           idNodo->valor, nombreTipo(tipo));

    // Verificar si el identificador ya está declarado
    if (buscarSimbolo(idNodo->valor)) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Variable '%s' ya declarada", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        idNodo->valido = 0;
        nodo->valido = 0;
        return 0;
    }

    // Caso 1: Declaración con new
    if (tercer_nodo->tipo == AST_TIPO) {
        printf("[DEBUG] Procesando array con 'new' y dimensiones\n");
        
        if (nodo->num_hijos < 4) {
            printf("[ERROR] Faltan dimensiones en la declaración con 'new'\n");
            return 0;
        }
        
        // Verificar que los tipos coincidan
        TipoDato tipoNewEnum = obtenerTipoDesdeString(tercer_nodo->valor);
        if (tipo != tipoNewEnum) {
            char msg[128];
            snprintf(msg, sizeof(msg), "El tipo '%s' en 'new' no coincide con el tipo declarado '%s' en '%s'",
                     nombreTipo(tipoNewEnum), nombreTipo(tipo), idNodo->valor);
            addError(msg, ambitoActual(), tercer_nodo->linea, tercer_nodo->columna);
            printf("[ERROR] %s\n", msg);
            nodo->valido = 0;
            return 0;
        }
        
        printf("[DEBUG] Tipos coinciden: '%s' == '%s'\n", nombreTipo(tipo), nombreTipo(tipoNewEnum));
        
        // Procesar dimensiones
        NodoAST* dimensionesNodo = nodo->hijos[3]; // Nodo de dimensiones
        
        if (dimensionesNodo->tipo != AST_DIMENSIONES) {
            printf("[ERROR] Se esperaba un nodo de dimensiones, se encontró tipo %d\n", 
                   dimensionesNodo->tipo);
            return 0;
        }
        
        // Contar dimensiones y verificar que sean válidas
        int numDimensiones = dimensionesNodo->num_hijos;
        int dimensiones[10]; // Máximo 10 dimensiones por simplicidad
        
        if (numDimensiones == 0) {
            printf("[ERROR] El array debe tener al menos una dimensión\n");
            return 0;
        }
        
        if (numDimensiones > 10) {
            printf("[ERROR] Se excedió el número máximo de dimensiones (10)\n");
            return 0;
        }
        
        printf("[DEBUG] Procesando %d dimensiones\n", numDimensiones);
        
        // Evaluar cada dimensión
        for (int i = 0; i < numDimensiones; i++) {
            NodoAST* dimNodo = dimensionesNodo->hijos[i];
            ValorSimbolo dimValor = evaluarExpr(dimNodo);
            
            if (!dimValor.inicializado || dimValor.tipo != TIPO_INT || dimValor.v.i <= 0) {
                printf("[ERROR] La dimensión %d debe ser un entero positivo\n", i + 1);
                return 0;
            }
            
            dimensiones[i] = dimValor.v.i;
            printf("[DEBUG] Dimensión %d = %d\n", i + 1, dimensiones[i]);
        }
        
        // Valor por defecto según el tipo
        const char* valorDefecto = valorPorDefectoTipo(tipo);
        
        // Generar la representación anidada del array
        char* formatoFinal = generarArrayAnidado(dimensiones, numDimensiones, valorDefecto, 0);
        if (!formatoFinal) {
            printf("[ERROR] No se pudo generar la representación del array\n");
            return 0;
        }
        
        // Agregar a tabla de símbolos
        agregarSimboloDetallado(
            idNodo->valor,
            SIMBOLO_ARRAY_MULTIDIMENSIONAL,
            tipo,
            ambitoActual(),
            idNodo->linea,
            idNodo->columna
        );
        
        // Inicializar con el formato
        Simbolo* simbolo = buscarSimbolo(idNodo->valor);
        if (simbolo) {
            simbolo->valor.v.s = formatoFinal; // No necesitas strdup aquí porque generarArrayAnidado ya hace un malloc
            simbolo->valor.inicializado = 1;
            simbolo->valor.tipo = TIPO_STRING;
            
            printf("[DEBUG] Array multidimensional '%s' inicializado: %s\n",
                   idNodo->valor, formatoFinal);
        } else {
            free(formatoFinal);
            printf("[ERROR] No se pudo encontrar el símbolo recién agregado\n");
            return 0;
        }
    }
    // Caso 2: Declaración con lista de bloques anidados
    else if (tercer_nodo->tipo == AST_INTERNO || tercer_nodo->tipo == AST_LISTA_ELEMENTOS) {
        printf("[DEBUG] Procesando array con lista de bloques anidados\n");
        
        // Procesar el bloque raíz
        char* representacion = procesarBloqueNestado(tercer_nodo, tipo, 0);
        
        if (!representacion) {
            printf("[ERROR] Error al procesar bloques anidados\n");
            return 0;
        }
        
        // Agregar a tabla de símbolos
        agregarSimboloDetallado(
            idNodo->valor,
            SIMBOLO_ARRAY_MULTIDIMENSIONAL,
            tipo,
            ambitoActual(),
            idNodo->linea,
            idNodo->columna
        );
        
        // Inicializar con la representación obtenida
        Simbolo* simbolo = buscarSimbolo(idNodo->valor);
        if (simbolo) {
            simbolo->valor.v.s = representacion;
            simbolo->valor.inicializado = 1;
            simbolo->valor.tipo = TIPO_STRING;
            
            printf("[DEBUG] Array multidimensional '%s' inicializado: %s\n",
                   idNodo->valor, representacion);
        } else {
            free(representacion);
            printf("[ERROR] No se pudo encontrar el símbolo recién agregado\n");
            return 0;
        }
    } else {
        printf("[ERROR] Tipo de nodo no reconocido para declaración de array: %d\n", 
               tercer_nodo->tipo);
        return 0;
    }
    
    printf("[DEBUG verificarDeclaracionArray] Finalizando verificación exitosamente\n");
    return 1;
}