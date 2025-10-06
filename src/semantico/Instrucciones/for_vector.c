#include <stdlib.h>
#include "for_vector.h"
#include <string.h>
#include <stdbool.h>
#include "../../AST/ejecutar_ast.h"
#include "../../AST/err.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../../simbolos/Evaluador.h"
#include "../Vectores/acceso_matriz.h"

SentenciaForVector* crearSentenciaForVector(NodoAST* variable_iteracion, NodoAST* coleccion, NodoAST* bloque) {
    SentenciaForVector* sentencia = (SentenciaForVector*)malloc(sizeof(SentenciaForVector));
    sentencia->variable_iteracion = variable_iteracion;
    sentencia->coleccion = coleccion;
    sentencia->bloque = bloque;
    return sentencia;
}

void liberarSentenciaForVector(SentenciaForVector* sentencia) {
    if (!sentencia) return;
    free(sentencia);
}

// Función auxiliar para parsear el contenido de un vector desde string
int* parsearVectorEnteros(const char* str, int* tamanio) {
    if (!str || !tamanio) return NULL;
    
    // El formato esperado es "[10,20,30]"
    if (str[0] != '[') return NULL;
    
    // Contar elementos
    *tamanio = 0;
    int len = strlen(str);
    for (int i = 1; i < len - 1; i++) {
        if (str[i] == ',') (*tamanio)++;
    }
    if (len > 2) (*tamanio)++; // El último elemento
    
    if (*tamanio == 0) return NULL;
    
    // Allocar memoria
    int* elementos = malloc(*tamanio * sizeof(int));
    if (!elementos) return NULL;
    
    // Parsear elementos
    char* copia = strdup(str);
    char* inicio = copia + 1; // Saltar '['
    char* fin = strchr(inicio, ']');
    if (fin) *fin = '\0';
    
    char* token = strtok(inicio, ",");
    int indice = 0;
    while (token && indice < *tamanio) {
        elementos[indice] = atoi(token);
        token = strtok(NULL, ",");
        indice++;
    }
    
    free(copia);
    return elementos;
}

// Función auxiliar para parsear vectores de float
float* parsearVectorFloat(const char* str, int* tamanio) {
    if (!str || !tamanio) return NULL;
    
    if (str[0] != '[') return NULL;
    
    *tamanio = 0;
    int len = strlen(str);
    for (int i = 1; i < len - 1; i++) {
        if (str[i] == ',') (*tamanio)++;
    }
    if (len > 2) (*tamanio)++;
    
    if (*tamanio == 0) return NULL;
    
    float* elementos = malloc(*tamanio * sizeof(float));
    if (!elementos) return NULL;
    
    char* copia = strdup(str);
    char* inicio = copia + 1;
    char* fin = strchr(inicio, ']');
    if (fin) *fin = '\0';
    
    char* token = strtok(inicio, ",");
    int indice = 0;
    while (token && indice < *tamanio) {
        elementos[indice] = atof(token);
        token = strtok(NULL, ",");
        indice++;
    }
    
    free(copia);
    return elementos;
}

// Función auxiliar para parsear vectores de string
char** parsearVectorString(const char* str, int* tamanio) {
    if (!str || !tamanio) return NULL;
    
    if (str[0] != '[') return NULL;
    
    *tamanio = 0;
    int len = strlen(str);
    for (int i = 1; i < len - 1; i++) {
        if (str[i] == ',') (*tamanio)++;
    }
    if (len > 2) (*tamanio)++;
    
    if (*tamanio == 0) return NULL;
    
    char** elementos = malloc(*tamanio * sizeof(char*));
    if (!elementos) return NULL;
    
    char* copia = strdup(str);
    char* inicio = copia + 1;
    char* fin = strchr(inicio, ']');
    if (fin) *fin = '\0';
    
    char* token = strtok(inicio, ",");
    int indice = 0;
    while (token && indice < *tamanio) {
        // Remover espacios en blanco
        while (*token == ' ') token++;
        char* end = token + strlen(token) - 1;
        while (end > token && *end == ' ') {
            *end = '\0';
            end--;
        }
        elementos[indice] = strdup(token);
        token = strtok(NULL, ",");
        indice++;
    }
    
    free(copia);
    return elementos;
}

ResultadoEjecucion ejecutarSentenciaForVector(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_FOR_VECTOR) {
        printf("[ERROR FOR_VECTOR] Nodo inválido o no es tipo FOR_VECTOR\n");
        return EJEC_OK;
    }

    if (nodo->num_hijos < 3) {
        printf("[ERROR FOR_VECTOR] FOR_VECTOR incompleto - esperados al menos 3 hijos, encontrados %d\n", nodo->num_hijos);
        return EJEC_OK;
    }

    NodoAST* variable_iteracion = nodo->hijos[0];
    NodoAST* coleccion = nodo->hijos[1];
    NodoAST* bloque = nodo->hijos[2];

    printf("[DEBUG FOR_VECTOR] === INICIANDO BUCLE FOR_VECTOR ===\n");

    // Evaluar la colección
    ValorSimbolo valorColeccion = evaluarExpr(coleccion);
    if (!valorColeccion.inicializado) {
        printf("[ERROR FOR_VECTOR] Colección no inicializada o inválida\n");
        addError("Colección no inicializada en FOR_VECTOR", ambitoActual(), coleccion->linea, coleccion->columna);
        return EJEC_OK;
    }

    // Buscar el símbolo de la colección
    Simbolo* simboloColeccion = buscarSimbolo(coleccion->valor);
    if (!simboloColeccion) {
        printf("[ERROR FOR_VECTOR] Símbolo de colección no encontrado\n");
        addError("Símbolo de colección no encontrado en FOR_VECTOR", ambitoActual(), coleccion->linea, coleccion->columna);
        return EJEC_OK;
    }

    if (simboloColeccion->clase != SIMBOLO_VECTOR && simboloColeccion->clase != SIMBOLO_MATRIZ) {
        printf("[ERROR FOR_VECTOR] La colección no es un vector ni una matriz\n");
        addError("La colección no es un vector ni una matriz en FOR_VECTOR", ambitoActual(), coleccion->linea, coleccion->columna);
        return EJEC_OK;
    }

    printf("[DEBUG FOR_VECTOR] Colección encontrada: %s, tipo: %d, clase: %d\n", 
           simboloColeccion->nombre, simboloColeccion->tipo, simboloColeccion->clase);
    printf("[DEBUG FOR_VECTOR] Valor de colección: %s\n", valorColeccion.v.s ? valorColeccion.v.s : "NULL");

    // Variables para procesamiento
    int tamanio = 0;
    char** filas = NULL;

    // Detectar si es matriz o vector y procesar adecuadamente
    if (simboloColeccion->clase == SIMBOLO_MATRIZ) {
        // Obtener dimensiones de la matriz
        int numFilas = 0, numColumnas = 0;
        analizarDimensionesMatriz(valorColeccion.v.s, &numFilas, &numColumnas);
        printf("[DEBUG FOR_VECTOR] Matriz detectada: %d filas, %d columnas\n", numFilas, numColumnas);

        // Extraer filas como strings
        filas = malloc(numFilas * sizeof(char*));
        if (!filas) {
            printf("[ERROR FOR_VECTOR] No se pudo reservar memoria para filas\n");
            return EJEC_OK;
        }

        // Inicializar todas las filas como NULL para manejo seguro
        for (int i = 0; i < numFilas; i++) {
            filas[i] = NULL;
        }

        char* matrixCopy = strdup(valorColeccion.v.s);
        if (!matrixCopy) {
            printf("[ERROR FOR_VECTOR] No se pudo duplicar string de matriz\n");
            free(filas);
            return EJEC_OK;
        }

        // Extraer cada fila - formato [[a,b],[c,d],...]
        char* p = matrixCopy;
        int filaIdx = 0;
        // Primero saltar el corchete externo
        if (*p == '[') p++;
        while (*p && filaIdx < numFilas) {
            // Buscar el inicio de la fila (corchete interno)
            while (*p && *p != '[') p++;
            if (!*p) break;
            
            char* start = p;
            int nivel = 1;
            p++;
            
            // Encontrar el fin de esta fila interna
            while (*p && nivel > 0) {
                if (*p == '[') nivel++;
                else if (*p == ']') nivel--;
                p++;
            }
            
            if (nivel == 0) {
                int filaLen = p - start;
                char* fila = (char*)malloc(filaLen + 1);
                if (!fila) {
                    printf("[ERROR FOR_VECTOR] No se pudo reservar memoria para fila %d\n", filaIdx);
                    break;
                }
                strncpy(fila, start, filaLen);
                fila[filaLen] = '\0';
                filas[filaIdx++] = fila;
                printf("[DEBUG FOR_VECTOR] Extraída fila %d: %s\n", filaIdx-1, fila);
            }
            
            // Avanzar más allá de la coma que separa filas
            while (*p && (*p == ',' || *p == ' ')) p++;
        }
        free(matrixCopy);
        tamanio = filaIdx; // Número real de filas extraídas

        // Verificar que se extrajeron todas las filas esperadas
        if (tamanio != numFilas) {
            printf("[ADVERTENCIA FOR_VECTOR] Se esperaban %d filas, se obtuvieron %d\n", numFilas, tamanio);
        }
    } else {
        // Vector normal (mantener comportamiento actual)
        // No se requiere procesamiento adicional para vectores
        tamanio = 0;
    }

    // Entrar al ámbito del for
    entrarAmbito("for_vector");
    
    // Obtener el nombre de la variable de iteración
    const char* nombre_var_iter = variable_iteracion->hijos[1]->valor;
    printf("[DEBUG FOR_VECTOR] Nombre de variable de iteración: %s\n", nombre_var_iter);
    
    // Crear la variable de iteración según el tipo de colección
    if (simboloColeccion->clase == SIMBOLO_MATRIZ) {
        // Para matrices, declarar la variable como vector
        agregarSimboloSiNoExiste(nombre_var_iter, SIMBOLO_VECTOR, simboloColeccion->tipo, 
                                ambitoActual(), variable_iteracion->linea, variable_iteracion->columna);
    } else {
        // Para vectores, declarar la variable como escalar
        agregarSimboloSiNoExiste(nombre_var_iter, SIMBOLO_VARIABLE, simboloColeccion->tipo, 
                                ambitoActual(), variable_iteracion->linea, variable_iteracion->columna);
    }
    
    Simbolo* var_iter = buscarSimboloEnAmbito(nombre_var_iter, ambitoActual());
    if (!var_iter) {
        printf("[ERROR FOR_VECTOR] No se pudo declarar la variable de iteración\n");
        salirAmbito();
        goto cleanup;
    }

    if (simboloColeccion->clase == SIMBOLO_MATRIZ) var_iter->valor.v.s = NULL;
    
    printf("[DEBUG FOR_VECTOR] Variable de iteración '%s' declarada como %s\n", 
           nombre_var_iter, 
           simboloColeccion->clase == SIMBOLO_MATRIZ ? "vector" : "variable");
    
    int limite_iteraciones = 1000;

    // BUCLE PRINCIPAL
    if (simboloColeccion->clase == SIMBOLO_MATRIZ) {
        printf("[DEBUG FOR_VECTOR] Iniciando iteraciones sobre %d filas\n", tamanio);

        for (int i = 0; i < tamanio && i < limite_iteraciones; i++) {
            printf("[DEBUG FOR_VECTOR] === ITERACIÓN %d de %d ===\n", i + 1, tamanio);

            if (!filas) {
                printf("[ERROR FOR_VECTOR] filas es NULL en iteración %d\n", i);
                break;
            }
            if (!filas[i]) {
                printf("[ERROR FOR_VECTOR] filas[%d] es NULL\n", i);
                continue;
            }

            int tamFila = 0;
            int* elementos = parsearVectorEnteros(filas[i], &tamFila);

            if (!elementos || tamFila == 0) {
                printf("[ERROR FOR_VECTOR] No se pudo parsear la fila %d: %s\n", i, filas[i]);
                continue;
            }

            char vectorStr[256] = {0};
            sprintf(vectorStr, "[");
            for (int j = 0; j < tamFila; j++) {
                char elemStr[32];
                sprintf(elemStr, "%d", elementos[j]);
                strcat(vectorStr, elemStr);
                if (j < tamFila - 1) strcat(vectorStr, ",");
            }
            strcat(vectorStr, "]");

            if (var_iter->valor.v.s) {
                free(var_iter->valor.v.s);
            }
            var_iter->valor.v.s = strdup(vectorStr);

            var_iter->valor.inicializado = 1;
            var_iter->valor.tipo = simboloColeccion->tipo;
            var_iter->clase = SIMBOLO_VECTOR;

            printf("[DEBUG FOR_VECTOR] Variable '%s' actualizada con vector: %s\n", nombre_var_iter, var_iter->valor.v.s);

            free(elementos);

            if (bloque) {
                printf("[DEBUG FOR_VECTOR] Ejecutando bloque de instrucciones\n");
                ResultadoEjecucion resBloque = ejecutarNodoAST(bloque);
                printf("[DEBUG FOR_VECTOR] Resultado ejecutarNodoAST: %d\n", resBloque);
                if (resBloque == EJEC_BREAK) {
                    printf("[DEBUG FOR_VECTOR] Break encontrado, saliendo del bucle\n");
                    break;
                } else if (resBloque == EJEC_CONTINUE) {
                    printf("[DEBUG FOR_VECTOR] Continue encontrado, siguiente iteración\n");
                    continue;
                } else if (resBloque != EJEC_OK) {
                    printf("[ERROR FOR_VECTOR] Error al ejecutar el bloque del FOR_VECTOR\n");
                    salirAmbito();
                    goto cleanup;
                }
            }

            printf("[DEBUG FOR_VECTOR] === FIN ITERACIÓN %d ===\n", i + 1);
        }
    } else {
        // Comportamiento actual para vectores regulares
        void* elementos = NULL;
        
        switch (simboloColeccion->tipo) {
            case TIPO_INT: {
                int* enteros = parsearVectorEnteros(valorColeccion.v.s, &tamanio);
                elementos = enteros;
                printf("[DEBUG FOR_VECTOR] Vector de enteros parseado: %d elementos\n", tamanio);
                break;
            }
            case TIPO_FLOAT: {
                float* flotantes = parsearVectorFloat(valorColeccion.v.s, &tamanio);
                elementos = flotantes;
                printf("[DEBUG FOR_VECTOR] Vector de float parseado: %d elementos\n", tamanio);
                break;
            }
            case TIPO_STRING: {
                char** strings = parsearVectorString(valorColeccion.v.s, &tamanio);
                elementos = strings;
                printf("[DEBUG FOR_VECTOR] Vector de strings parseado: %d elementos\n", tamanio);
                break;
            }
            default:
                printf("[ERROR FOR_VECTOR] Tipo de vector no soportado: %d\n", simboloColeccion->tipo);
                salirAmbito();
                return EJEC_OK;
        }
        
        if (!elementos || tamanio == 0) {
            printf("[ERROR FOR_VECTOR] No se pudieron parsear los elementos del vector\n");
            salirAmbito();
            return EJEC_OK;
        }
        
        printf("[DEBUG FOR_VECTOR] Iniciando iteraciones sobre %d elementos\n", tamanio);
        
        for (int i = 0; i < tamanio; i++) {
            printf("[DEBUG FOR_VECTOR] === ITERACIÓN %d de %d ===\n", i + 1, tamanio);
            
            // Asignar el valor del elemento a la variable de iteración
            switch (simboloColeccion->tipo) {
                case TIPO_INT:
                    var_iter->valor.v.i = ((int*)elementos)[i];
                    printf("[DEBUG FOR_VECTOR] Asignado int: %d\n", var_iter->valor.v.i);
                    break;
                case TIPO_FLOAT:
                    var_iter->valor.v.f = ((float*)elementos)[i];
                    printf("[DEBUG FOR_VECTOR] Asignado float: %f\n", var_iter->valor.v.f);
                    break;
                case TIPO_STRING:
                    if (var_iter->valor.v.s) free(var_iter->valor.v.s);
                    var_iter->valor.v.s = strdup(((char**)elementos)[i]);
                    printf("[DEBUG FOR_VECTOR] Asignado string: %s\n", var_iter->valor.v.s);
                    break;
                default:
                    printf("[ERROR FOR_VECTOR] Tipo de vector no soportado en asignación\n");
                    salirAmbito();
                    
                    // Liberar memoria
                    if (elementos) {
                        if (simboloColeccion->tipo == TIPO_STRING) {
                            char** strings = (char**)elementos;
                            for (int j = 0; j < tamanio; j++) {
                                if (strings[j]) free(strings[j]);
                            }
                        }
                        free(elementos);
                    }
                    
                    return EJEC_OK;
            }
            
            var_iter->valor.inicializado = 1;
            var_iter->valor.tipo = simboloColeccion->tipo;
            
            printf("[DEBUG FOR_VECTOR] Variable '%s' actualizada con nuevo valor\n", nombre_var_iter);
            
            // Ejecutar el bloque de instrucciones
            if (bloque) {
                printf("[DEBUG FOR_VECTOR] Ejecutando bloque de instrucciones\n");
                ResultadoEjecucion resBloque = ejecutarNodoAST(bloque);
                if (resBloque == EJEC_BREAK) {
                    printf("[DEBUG FOR_VECTOR] Break encontrado, saliendo del bucle\n");
                    break;
                } else if (resBloque == EJEC_CONTINUE) {
                    printf("[DEBUG FOR_VECTOR] Continue encontrado, siguiente iteración\n");
                    continue;
                } else if (resBloque != EJEC_OK) {
                    printf("[ERROR FOR_VECTOR] Error al ejecutar el bloque del FOR_VECTOR\n");
                    salirAmbito();
                    
                    // Liberar memoria
                    if (elementos) {
                        if (simboloColeccion->tipo == TIPO_STRING) {
                            char** strings = (char**)elementos;
                            for (int j = 0; j < tamanio; j++) {
                                if (strings[j]) free(strings[j]);
                            }
                        }
                        free(elementos);
                    }
                    
                    return EJEC_OK;
                }
            }
            
            printf("[DEBUG FOR_VECTOR] === FIN ITERACIÓN %d ===\n", i + 1);
        }
        
        // Liberar memoria
        if (elementos) {
            if (simboloColeccion->tipo == TIPO_STRING) {
                char** strings = (char**)elementos;
                for (int i = 0; i < tamanio; i++) {
                    if (strings[i]) free(strings[i]);
                }
            }
            free(elementos);
        }
    }
    
    salirAmbito();

cleanup:
    // Liberar memoria de las filas extraídas
    if (filas) {
        for (int i = 0; i < tamanio; i++) {
            if (filas[i]) free(filas[i]);
        }
        free(filas);
    }

    printf("[DEBUG FOR_VECTOR] === BUCLE FOR_VECTOR COMPLETADO ===\n");
    return EJEC_OK;
}