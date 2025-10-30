#include "declaracion_matriz.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include "../../AST/err.h"
#include "../../AST/errorTIpos.h"
#include "../../simbolos/Evaluador.h" 
#include "../Funciones/Llamadas.h"
#include "acceso_matriz.h"
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 

int verificarDeclaracionMatriz(NodoAST* nodo) {
    printf("[DEBUG verificarDeclaracionMatriz] Iniciando verificación\n");

    if (!nodo) {
        printf("[ERROR] Nodo es NULL\n");
        return 0;
    }

    if (nodo->num_hijos < 3) {
        printf("[ERROR] Nodo no tiene suficientes hijos para una declaración de matriz\n");
        return 0;
    }

    NodoAST* tipoNodo = nodo->hijos[0];  // Tipo declarado
    NodoAST* idNodo = nodo->hijos[1];    // Identificador
    NodoAST* tercer_nodo = nodo->hijos[2]; // Puede ser tipoNew o lista de elementos

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

    printf("[DEBUG] Declarando matriz '%s': tipo declarado=%s\n",
           idNodo->valor, nombreTipo(tipo));

    // Verificar si el identificador ya está declarado
    if (buscarSimbolo(idNodo->valor)) {
        char msg[128];
        snprintf(msg, sizeof(msg), "Variable '%s' ya declarada", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("%s\n", msg);
        idNodo->valido = 0;
        nodo->valido = 0;
        return 0;
    }

    // **NUEVO ORDEN: Caso 1 - Declaración con resultado de función (DEBE IR PRIMERO)**
    if (tercer_nodo->tipo == AST_LLAMADA) {
        printf("[DEBUG] Procesando matriz con resultado de función\n");
        
        // Evaluar la llamada a función
        ValorSimbolo resultadoFuncion = EvaluarLlamadaFuncion(tercer_nodo);
        
        if (!resultadoFuncion.inicializado) {
            printf("[ERROR] La función no retornó un valor válido\n");
            return 0;
        }
        
        // **CORREGIR: Detectar matriz por contenido, NO por tipo**
        int esMatrizRetornada = (resultadoFuncion.v.s && 
                                resultadoFuncion.v.s[0] == '[' && 
                                resultadoFuncion.v.s[1] == '[');
        
        if (!esMatrizRetornada) {
            printf("[ERROR] La función no retornó una matriz válida, tipo: %d, contenido: %s\n", 
                resultadoFuncion.tipo, 
                resultadoFuncion.v.s ? resultadoFuncion.v.s : "NULL");
            return 0;
        }
        
        printf("[DEBUG] Función retornó matriz: %s (tipo base: %s)\n", 
            resultadoFuncion.v.s, nombreTipo(resultadoFuncion.tipo));
        
        // Agregar el símbolo a la tabla
        agregarSimboloDetallado(
            idNodo->valor,
            SIMBOLO_MATRIZ,
            tipo,  // tipo del elemento (int), no el tipo de retorno
            ambitoActual(),
            idNodo->linea,
            idNodo->columna
        );
        
        // Inicializar con el resultado de la función
        Simbolo* simbolo = buscarSimbolo(idNodo->valor);
        if (simbolo) {
            simbolo->valor.v.s = strdup(resultadoFuncion.v.s);
            simbolo->valor.inicializado = 1;
            simbolo->valor.tipo = TIPO_STRING; // **Almacenamiento interno como STRING**
            
            // **EXTRAER Y ESTABLECER DIMENSIONES**
            int numFilas = 0;
            int numColumnas = 0;
            analizarDimensionesMatriz(resultadoFuncion.v.s, &numFilas, &numColumnas);
            simbolo->filas = numFilas;
            simbolo->columnas = numColumnas;
            
            printf("[DEBUG] Matriz '%s' inicializada con resultado de función: %dx%d\n",
                idNodo->valor, numFilas, numColumnas);
            printf("[DEBUG] Contenido de la matriz: %s\n", simbolo->valor.v.s);
        }
        
        // **NO liberar resultadoFuncion.v.s aquí, ya se copió con strdup**
        
        printf("[DEBUG verificarDeclaracionMatriz] Finalizando verificación exitosamente\n");
        return 1;
    }

    // Caso 1: Declaración con lista de elementos
    if (tercer_nodo->tipo == AST_LISTA_ELEMENTOS) {
        printf("[DEBUG] Procesando matriz con lista de elementos\n");
        
        // Obtener la lista de filas
        NodoAST* listaFilas = NULL;
        
        if (tercer_nodo->num_hijos == 1 && 
            (tercer_nodo->hijos[0]->tipo == AST_LISTA_ELEMENTOS || 
             tercer_nodo->hijos[0]->tipo == AST_INTERNO)) {
            listaFilas = tercer_nodo->hijos[0];
        } else {
            listaFilas = tercer_nodo;
        }
        
        // Contar filas y verificar consistencia de columnas
        int numFilas = listaFilas->num_hijos;
        int numColumnas = -1;
        
        //printf("[DEBUG] Procesando %d filas\n", numFilas);
        
        // Matriz para almacenar los valores
        char*** valores_matriz = malloc(numFilas * sizeof(char**));
        if (!valores_matriz) {
            printf("[ERROR] No se pudo asignar memoria para la matriz\n");
            return 0;
        }
        
        for (int i = 0; i < numFilas; i++) {
            NodoAST* fila = listaFilas->hijos[i];
            
            // La fila puede ser un nodo interno con la lista real de elementos
            NodoAST* elementosFila = fila;
            if (fila->tipo == AST_INTERNO && fila->num_hijos == 1) {
                elementosFila = fila->hijos[0];
            }
            
            int columnas = elementosFila->num_hijos;
            //printf("[DEBUG] Fila %d tiene %d elementos\n", i, columnas);
            
            if (numColumnas == -1) {
                numColumnas = columnas;
            } else if (numColumnas != columnas) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Inconsistencia en número de columnas en matriz '%s': fila %d tiene %d elementos, se esperaban %d", 
                         idNodo->valor, i, columnas, numColumnas);
                addError(msg, ambitoActual(), fila->linea, fila->columna);
                printf("[ERROR] %s\n", msg);
                
                // Liberar memoria asignada hasta ahora
                for (int j = 0; j < i; j++) {
                    for (int k = 0; k < numColumnas; k++) {
                        free(valores_matriz[j][k]);
                    }
                    free(valores_matriz[j]);
                }
                free(valores_matriz);
                
                return 0;
            }
            
            // Asignar memoria para esta fila
            valores_matriz[i] = malloc(numColumnas * sizeof(char*));
            if (!valores_matriz[i]) {
                printf("[ERROR] No se pudo asignar memoria para la fila %d\n", i);
                
                // Liberar memoria asignada hasta ahora
                for (int j = 0; j < i; j++) {
                    for (int k = 0; k < numColumnas; k++) {
                        free(valores_matriz[j][k]);
                    }
                    free(valores_matriz[j]);
                }
                free(valores_matriz);
                
                return 0;
            }
            
            // Procesar cada elemento de la fila
            for (int j = 0; j < columnas; j++) {
                NodoAST* elemento = elementosFila->hijos[j];
                
                // Manejar cada tipo de elemento según su tipo de nodo
                if (elemento->tipo == AST_NUMERO && elemento->valor) {
                    // Número literal
                    switch (tipo) {
                        case TIPO_INT: {
                            int val = atoi(elemento->valor);
                            char buffer[16];
                            snprintf(buffer, sizeof(buffer), "%d", val);
                            valores_matriz[i][j] = strdup(buffer);
                            break;
                        }
                        case TIPO_FLOAT:
                        case TIPO_DOUBLE: {
                            double val = atof(elemento->valor);
                            char buffer[32];
                            snprintf(buffer, sizeof(buffer), "%f", val);
                            valores_matriz[i][j] = strdup(buffer);
                            break;
                        }
                        default: {
                            valores_matriz[i][j] = strdup(elemento->valor);
                            break;
                        }
                    }
                } else if (elemento->tipo == AST_CADENA && elemento->valor) {
                    // Cadena literal
                    if (tipo == TIPO_STRING) {
                        char buffer[1024]; // Tamaño suficiente para cadenas típicas
                        snprintf(buffer, sizeof(buffer), "\"%s\"", elemento->valor);
                        valores_matriz[i][j] = strdup(buffer);
                    } else {
                        printf("[ERROR] Tipo incompatible: string en matriz de tipo %s\n", 
                            nombreTipo(tipo));
                        
                        // Liberar memoria asignada hasta ahora
                        for (int x = 0; x <= i; x++) {
                            for (int y = 0; y < (x == i ? j : numColumnas); y++) {
                                if (valores_matriz[x][y]) free(valores_matriz[x][y]);
                            }
                            free(valores_matriz[x]);
                        }
                        free(valores_matriz);
                        
                        return 0;
                    }
                } else if (elemento->tipo == AST_CARACTER && elemento->valor) {
                    // Carácter literal
                    if (tipo == TIPO_CHAR) {
                        char buffer[16];
                        snprintf(buffer, sizeof(buffer), "'%s'", elemento->valor);
                        valores_matriz[i][j] = strdup(buffer);
                    } else {
                        printf("[ERROR] Tipo incompatible: char en matriz de tipo %s\n", 
                            nombreTipo(tipo));
                        
                        // Liberar memoria asignada hasta ahora
                        for (int x = 0; x <= i; x++) {
                            for (int y = 0; y < (x == i ? j : numColumnas); y++) {
                                if (valores_matriz[x][y]) free(valores_matriz[x][y]);
                            }
                            free(valores_matriz[x]);
                        }
                        free(valores_matriz);
                        
                        return 0;
                    }
                } else if (elemento->tipo == AST_BOOLEAN && elemento->valor) {
                    // Booleano literal
                    if (tipo == TIPO_BOOLEAN) {
                        valores_matriz[i][j] = strdup(elemento->valor); // true o false
                    } else {
                        printf("[ERROR] Tipo incompatible: boolean en matriz de tipo %s\n", 
                            nombreTipo(tipo));
                        
                        // Liberar memoria asignada hasta ahora
                        for (int x = 0; x <= i; x++) {
                            for (int y = 0; y < (x == i ? j : numColumnas); y++) {
                                if (valores_matriz[x][y]) free(valores_matriz[x][y]);
                            }
                            free(valores_matriz[x]);
                        }
                        free(valores_matriz);
                        
                        return 0;
                    }
                } else {
                    // Evaluar el elemento usando evaluarExpr
                    ValorSimbolo valorElemento = evaluarExpr(elemento);
                    if (!valorElemento.inicializado) {
                        printf("[ERROR] Elemento [%d][%d] no está inicializado\n", i, j);
                        
                        // Liberar memoria asignada hasta ahora
                        for (int x = 0; x <= i; x++) {
                            for (int y = 0; y < (x == i ? j : numColumnas); y++) {
                                if (valores_matriz[x][y]) free(valores_matriz[x][y]);
                            }
                            free(valores_matriz[x]);
                        }
                        free(valores_matriz);
                        
                        return 0;
                    }
                    
                    // Convertir el valor a string según el tipo y con verificación de compatibilidad
                    char buffer[1024]; // Buffer más grande para strings
                    switch (tipo) {
                        case TIPO_INT:
                            if (valorElemento.tipo != TIPO_INT && !esWidening(valorElemento.tipo, TIPO_INT)) {
                                printf("[ERROR] Tipo incompatible en matriz: %s no es compatible con %s\n",
                                    nombreTipo(valorElemento.tipo), nombreTipo(TIPO_INT));
                                
                                // Liberar memoria
                                for (int x = 0; x <= i; x++) {
                                    for (int y = 0; y < (x == i ? j : numColumnas); y++) {
                                        if (valores_matriz[x][y]) free(valores_matriz[x][y]);
                                    }
                                    free(valores_matriz[x]);
                                }
                                free(valores_matriz);
                                
                                return 0;
                            }
                            snprintf(buffer, sizeof(buffer), "%d", valorElemento.v.i);
                            break;
                        case TIPO_FLOAT:
                            if (valorElemento.tipo != TIPO_FLOAT && !esWidening(valorElemento.tipo, TIPO_FLOAT)) {
                                printf("[ERROR] Tipo incompatible en matriz: %s no es compatible con %s\n",
                                    nombreTipo(valorElemento.tipo), nombreTipo(TIPO_FLOAT));
                                
                                // Liberar memoria
                                for (int x = 0; x <= i; x++) {
                                    for (int y = 0; y < (x == i ? j : numColumnas); y++) {
                                        if (valores_matriz[x][y]) free(valores_matriz[x][y]);
                                    }
                                    free(valores_matriz[x]);
                                }
                                free(valores_matriz);
                                
                                return 0;
                            }
                            snprintf(buffer, sizeof(buffer), "%f", valorElemento.v.f);
                            break;
                        case TIPO_DOUBLE:
                            if (valorElemento.tipo != TIPO_DOUBLE && !esWidening(valorElemento.tipo, TIPO_DOUBLE)) {
                                printf("[ERROR] Tipo incompatible en matriz: %s no es compatible con %s\n",
                                    nombreTipo(valorElemento.tipo), nombreTipo(TIPO_DOUBLE));
                                
                                // Liberar memoria
                                for (int x = 0; x <= i; x++) {
                                    for (int y = 0; y < (x == i ? j : numColumnas); y++) {
                                        if (valores_matriz[x][y]) free(valores_matriz[x][y]);
                                    }
                                    free(valores_matriz[x]);
                                }
                                free(valores_matriz);
                                
                                return 0;
                            }
                            snprintf(buffer, sizeof(buffer), "%lf", valorElemento.v.d);
                            break;
                        case TIPO_CHAR:
                            if (valorElemento.tipo != TIPO_CHAR) {
                                printf("[ERROR] Tipo incompatible en matriz: %s no es compatible con %s\n",
                                    nombreTipo(valorElemento.tipo), nombreTipo(TIPO_CHAR));
                                
                                // Liberar memoria
                                for (int x = 0; x <= i; x++) {
                                    for (int y = 0; y < (x == i ? j : numColumnas); y++) {
                                        if (valores_matriz[x][y]) free(valores_matriz[x][y]);
                                    }
                                    free(valores_matriz[x]);
                                }
                                free(valores_matriz);
                                
                                return 0;
                            }
                            snprintf(buffer, sizeof(buffer), "'%c'", valorElemento.v.c);
                            break;
                        case TIPO_BOOLEAN:
                            if (valorElemento.tipo != TIPO_BOOLEAN) {
                                printf("[ERROR] Tipo incompatible en matriz: %s no es compatible con %s\n",
                                    nombreTipo(valorElemento.tipo), nombreTipo(TIPO_BOOLEAN));
                                
                                // Liberar memoria
                                for (int x = 0; x <= i; x++) {
                                    for (int y = 0; y < (x == i ? j : numColumnas); y++) {
                                        if (valores_matriz[x][y]) free(valores_matriz[x][y]);
                                    }
                                    free(valores_matriz[x]);
                                }
                                free(valores_matriz);
                                
                                return 0;
                            }
                            snprintf(buffer, sizeof(buffer), "%s", valorElemento.v.b ? "true" : "false");
                            break;
                        case TIPO_STRING:
                            if (valorElemento.tipo != TIPO_STRING) {
                                printf("[ERROR] Tipo incompatible en matriz: %s no es compatible con %s\n",
                                    nombreTipo(valorElemento.tipo), nombreTipo(TIPO_STRING));
                                
                                // Liberar memoria
                                for (int x = 0; x <= i; x++) {
                                    for (int y = 0; y < (x == i ? j : numColumnas); y++) {
                                        if (valores_matriz[x][y]) free(valores_matriz[x][y]);
                                    }
                                    free(valores_matriz[x]);
                                }
                                free(valores_matriz);
                                
                                return 0;
                            }
                            if (valorElemento.v.s) {
                                snprintf(buffer, sizeof(buffer), "\"%s\"", valorElemento.v.s);
                            } else {
                                snprintf(buffer, sizeof(buffer), "\"\"");
                            }
                            break;
                        default:
                            buffer[0] = '\0';
                            break;
                    }
                    valores_matriz[i][j] = strdup(buffer);
                }
                
                //printf("[DEBUG] Elemento [%d][%d] = %s\n", i, j, valores_matriz[i][j]);
            }
        }
        
        // Agregar el símbolo a la tabla
        agregarSimboloDetallado(
            idNodo->valor,
            SIMBOLO_MATRIZ,
            tipo,
            ambitoActual(),
            idNodo->linea,
            idNodo->columna
        );
        
        // Crear una representación en string de la matriz
        Simbolo* simbolo = buscarSimbolo(idNodo->valor);
        if (simbolo) {
            // Formato: [[val1,val2],[val3,val4]]
            size_t buffer_size = numFilas * numColumnas * 32 + numFilas * 4 + 4;
            char* matriz_str = malloc(buffer_size);
            if (!matriz_str) {
                printf("[ERROR] No se pudo asignar memoria para la representación de la matriz\n");
                return 0;
            }
            
            matriz_str[0] = '\0';
            strcat(matriz_str, "[");
            
            for (int i = 0; i < numFilas; i++) {
                strcat(matriz_str, "[");
                
                for (int j = 0; j < numColumnas; j++) {
                    strcat(matriz_str, valores_matriz[i][j]);
                    if (j < numColumnas - 1) strcat(matriz_str, ",");
                    
                    free(valores_matriz[i][j]); // Liberar la memoria del elemento
                }
                
                strcat(matriz_str, "]");
                if (i < numFilas - 1) strcat(matriz_str, ",");
                
                free(valores_matriz[i]); // Liberar la memoria de la fila
            }
            
            strcat(matriz_str, "]");
            free(valores_matriz); // Liberar la matriz
            
            // Guardar la representación en el símbolo
            simbolo->valor.v.s = matriz_str;
            simbolo->valor.inicializado = 1;
            simbolo->valor.tipo = TIPO_STRING;
            
            printf("[DEBUG] Matriz '%s' inicializada con %d filas y %d columnas: %s\n",
                   idNodo->valor, numFilas, numColumnas, matriz_str);
        }
        
    } else if (nodo->num_hijos >= 4) {
        // Caso 2: Declaración con 'new' y dimensiones explícitas
        NodoAST* tipoNew = tercer_nodo; // Tipo después de 'new'
        
        NodoAST* nodo_dimensiones = nodo->hijos[3]; // Puede ser un nodo de dimensiones o directamente filas
        NodoAST* filas = NULL;
        NodoAST* columnas = NULL;
        
        // Verificar si es un nodo de dimensiones
        if (nodo_dimensiones->tipo == AST_DIMENSIONES && nodo_dimensiones->num_hijos >= 2) {
            filas = nodo_dimensiones->hijos[0];
            columnas = nodo_dimensiones->hijos[1];
            printf("[DEBUG] Usando nodo de dimensiones con %d hijos\n", nodo_dimensiones->num_hijos);
        } else {
            printf("[ERROR] Formato de dimensiones incorrecto\n");
            return 0;
        }
        
        // Verificar que filas y columnas existan
        if (!filas || !columnas) {
            printf("[ERROR] No se encontraron dimensiones válidas para la matriz\n");
            return 0;
        }
        
        // Verificar que los tipos coincidan
        TipoDato tipoNewEnum = obtenerTipoDesdeString(tipoNew->valor);
        if (tipo != tipoNewEnum) {
            char msg[128];
            snprintf(msg, sizeof(msg), "El tipo '%s' en 'new' no coincide con el tipo declarado '%s' en '%s'",
                     nombreTipo(tipoNewEnum), nombreTipo(tipo), idNodo->valor);
            addError(msg, ambitoActual(), tipoNew->linea, tipoNew->columna);
            printf("[ERROR] %s\n", msg);
            nodo->valido = 0;
            return 0;
        }
        
        printf("[DEBUG] Tipos coinciden: '%s' == '%s'\n", nombreTipo(tipo), nombreTipo(tipoNewEnum));
        
        // Evaluar las dimensiones
        ValorSimbolo valorFilas = evaluarExpr(filas);
        ValorSimbolo valorColumnas = evaluarExpr(columnas);
        
        if (!valorFilas.inicializado || !valorColumnas.inicializado ||
            valorFilas.v.i <= 0 || valorColumnas.v.i <= 0) {
            char msg[128];
            snprintf(msg, sizeof(msg), "Dimensiones de matriz '%s' deben ser mayores a 0", idNodo->valor);
            addError(msg, ambitoActual(), filas->linea, filas->columna);
            printf("[ERROR] %s\n", msg);
            nodo->valido = 0;
            return 0;
        }
        
        int numFilas = valorFilas.v.i;
        int numColumnas = valorColumnas.v.i;
        
        // Agregar el símbolo a la tabla
        agregarSimboloDetallado(
            idNodo->valor,
            SIMBOLO_MATRIZ,
            tipo,
            ambitoActual(),
            idNodo->linea,
            idNodo->columna
        );
        
        // Inicializar la matriz con valores por defecto
        Simbolo* simbolo = buscarSimbolo(idNodo->valor);
        if (simbolo) {
            // Formato: [[val1,val2],[val3,val4]]
            size_t buffer_size = numFilas * numColumnas * 16 + numFilas * 4 + 4;
            char* matriz_str = malloc(buffer_size);
            if (!matriz_str) {
                printf("[ERROR] No se pudo asignar memoria para la matriz\n");
                return 0;
            }
            
            matriz_str[0] = '\0';
            strcat(matriz_str, "[");
            
            for (int i = 0; i < numFilas; i++) {
                strcat(matriz_str, "[");
                
                for (int j = 0; j < numColumnas; j++) {
                    // Valor por defecto según el tipo
                    switch (tipo) {
                        case TIPO_INT:
                            strcat(matriz_str, "0");
                            break;
                        case TIPO_FLOAT:
                            strcat(matriz_str, "0.0");
                            break;
                        case TIPO_DOUBLE:
                            strcat(matriz_str, "0.0");
                            break;
                        case TIPO_CHAR:
                            strcat(matriz_str, "\\0");
                            break;
                        case TIPO_BOOLEAN:
                            strcat(matriz_str, "false");
                            break;
                        case TIPO_STRING:
                            strcat(matriz_str, "\"\"");
                            break;
                        default:
                            strcat(matriz_str, "0");
                            break;
                    }
                    
                    if (j < numColumnas - 1) strcat(matriz_str, ",");
                }
                
                strcat(matriz_str, "]");
                if (i < numFilas - 1) strcat(matriz_str, ",");
            }
            
            strcat(matriz_str, "]");
            
            // Guardar la representación en el símbolo
            simbolo->valor.v.s = matriz_str;
            simbolo->valor.inicializado = 1;
            simbolo->valor.tipo = TIPO_STRING;
            
            printf("[DEBUG] Matriz '%s' inicializada con %d filas y %d columnas: %s\n",
                   idNodo->valor, numFilas, numColumnas, matriz_str);
        }
    } else {
        printf("[ERROR] Estructura de nodo no reconocida para declaración de matriz: tipo=%d\n", 
               tercer_nodo->tipo);
        return 0;
    }
    
    printf("[DEBUG verificarDeclaracionMatriz] Finalizando verificación exitosamente\n");
    return 1;
}