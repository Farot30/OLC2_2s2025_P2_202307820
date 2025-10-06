#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "../simbolos/Evaluador.h"
#include "../semantico/tipos.h"
#include "../semantico/Vectores/acceso_vector.h"
#include "../semantico/Vectores/acceso_matriz.h"
#include "../semantico/Vectores/acceso_arrays.h"
#include "../semantico/semantico.h"
NodoAST* astGlobal = NULL;

NodoAST* crearNodo(TipoNodoAST tipo, char* valor) {
    NodoAST* nodo = (NodoAST*)malloc(sizeof(NodoAST));
    nodo->tipo = tipo;
    nodo->valor = valor ? strdup(valor) : NULL;
    nodo->hijos = NULL;
    nodo->num_hijos = 0;
    nodo->linea = -1;
    nodo->columna = -1;
    nodo->valido = 1;
    
    return nodo;
}

void agregarHijo(NodoAST* padre, NodoAST* hijo) {
    padre->hijos = (NodoAST**)realloc(padre->hijos, sizeof(NodoAST*) * (padre->num_hijos + 1));
    padre->hijos[padre->num_hijos] = hijo;
    padre->num_hijos++;
}


void liberarAST(NodoAST* nodo) {
    if (!nodo) return;
    for (int i = 0; i < nodo->num_hijos; ++i)
        liberarAST(nodo->hijos[i]);
    free(nodo->hijos);
    if (nodo->valor) free(nodo->valor);
    free(nodo);
}


void imprimirAST(NodoAST* nodo, int nivel) {
    if (!nodo) return;
    for (int i = 0; i < nivel; ++i) printf("  ");
    printf("Tipo: %d", nodo->tipo);
    if (nodo->valor) printf(", Valor: %s", nodo->valor);
    printf("\n");
    for (int i = 0; i < nodo->num_hijos; ++i)
        imprimirAST(nodo->hijos[i], nivel + 1);
}
    

static const char* nombreNodoTipo(TipoNodoAST t){
    switch(t){
        case AST_PROGRAMA: return "PROGRAMA";
        case AST_NUMERO: return "NUMERO";
        case AST_IDENTIFICADOR: return "IDENT";
        case AST_DECLARACION: return "DECL";
        case AST_LISTA: return "LISTA";
        case AST_BLOQUE: return "BLOQUE";
        case AST_ASIGNACION: return "ASIG";
        case AST_ASIGNACION_COMPUESTA: return "ASIG_COMP";
        case AST_CADENA: return "CADENA";
        case AST_CARACTER: return "CARACTER";
        case AST_OPERACION: return "OP";
        case AST_CAST: return "CAST";
        case AST_PRINT: return "PRINT";
        case AST_BOOLEAN: return "BOOLEAN";
        case AST_ERROR: return "ERROR";
        case AST_STRING: return "STRING";
        case AST_EQUAL: return "EQUAL";
        case AST_IF: return "IF";
        case AST_ELSE : return "ELSE";
        case AST_ELSEIF: return "ELSEIF";
        case AST_SWITCH: return "SWITCH";
        case AST_CASE: return "CASE";
        case AST_BREAK: return "BREAK";
        case AST_DEFAULT: return "DEFAULT";
        case AST_WHILE: return "WHILE";
        case AST_CONTINUE: return "CONTINUE";
        case AST_FOR: return "FOR";
        case AST_FOR_VECTOR: return "FOR_VECTOR";
        case AST_INCREMENTO: return "INCREMENTO";
        case AST_DECREMENTO: return "DECREMENTO";
        case AST_DECLARACION_VECTOR: return "DECLARACION_VECTOR";
        case AST_DECLARACION_MATRIZ: return "DECLARACION_MATRIZ";
        case AST_DECLARACION_ARRAY_MULTIDIMENSIONAL: return "DECLARACION_ARRAY_MULTIDIMENSIONAL";
        case AST_DIMENSIONES: return "DIMENSIONES";
        case AST_TIPO: return "TIPO";
        case AST_INTERNO: return "INTERNO";
        case AST_LISTA_ELEMENTOS: return "LISTA_ELEMENTOS";
        case AST_1DIMENSION: return "1DIMENSION";
        case AST_ACCESO: return "ACCESO";
        case AST_PARSEINT: return "PARSEINT";
        case AST_PARSEFLOAT: return "PARSEFLOAT";
        case AST_PARSEDOUBLE: return "PARSEDOUBLE";
        case AST_PARSEJOIN: return "PARSEJOIN";
        case AST_VALUEOF: return "VALUEOF";
        case AST_INDEXOF: return "INDEXOF";
        case AST_LENGHT: return "LENGHT";
        case AST_ADD: return "ADD";
        case AST_ACCESO_ESCRITURA: return "ACCESO_ESCRITURA";
        case AST_FUNCION_VOID: return "FUNCION_VOID";
        case AST_LLAMADA_VOID: return "LLAMADA_VOID";
        case AST_FUNCION: return "FUNCION";
        case AST_LLAMADA: return "LLAMADA";
        case AST_RETURN: return "RETURN";
        case AST_LISTA_PARAMETROS: return "LISTA_PARAMETROS";
        case AST_PARAMETROS: return "PARAMETROS";
        case AST_PARAMETRO: return "PARAMETRO";
        case AST_METODO_MAIN: return "METODO_MAIN";
        case AST_NULL: return "NULL";
        case AST_DECLARACION_CONST: return "DECLARACION_CONST";
        case AST_DECLARACION_VECTOR_TEMP: return "DECLARACION_VECTOR_TEMP";
        
         // Agregar más casos según sea necesario
        default: return "UNK";
    }
}


static void escaparLabel(const char* src, char* dst, size_t max){
    size_t j=0;
    for(size_t i=0; src && src[i] && j+6 < max; ++i){
        if(src[i]=='|'){
            strcpy(&dst[j], "&#124;");
            j += 6;
        } else if(src[i]=='<'){
            strcpy(&dst[j], "&lt;");
            j += 4;
        } else if(src[i]=='>'){
            strcpy(&dst[j], "&gt;");
            j += 4;
        } else if(src[i]=='"'){
            dst[j++]='\\';
            dst[j++]=src[i];
        } else {
            dst[j++]=src[i];
        }
    }
    dst[j]=0;
}

static void graphvizRec(NodoAST* n, FILE* f) {
    if (!n || !n->valido) return;

    char valorEsc[128];
    escaparLabel(n->valor ? n->valor : "NULL", valorEsc, sizeof(valorEsc));

    // Mostrar el valor evaluado en el nodo PRINT
    if (n->tipo == AST_PRINT && n->num_hijos > 0 && n->hijos[0]) {
        // Evaluar la expresión a imprimir
        ValorSimbolo valorEvaluado = evaluarExpr(n->hijos[0]);
        char valorPrint[256] = "sin valor";
        
        if (valorEvaluado.inicializado) {
            switch (valorEvaluado.tipo) {
                case TIPO_INT:
                    snprintf(valorPrint, sizeof(valorPrint), "%d", valorEvaluado.v.i);
                    break;
                case TIPO_FLOAT:
                    snprintf(valorPrint, sizeof(valorPrint), "%f", valorEvaluado.v.f);
                    break;
                case TIPO_DOUBLE:
                    snprintf(valorPrint, sizeof(valorPrint), "%lf", valorEvaluado.v.d);
                    break;
                case TIPO_CHAR:
                    snprintf(valorPrint, sizeof(valorPrint), "%c", valorEvaluado.v.c);
                    break;
                case TIPO_BOOLEAN:
                    snprintf(valorPrint, sizeof(valorPrint), "%s", valorEvaluado.v.b ? "true" : "false");
                    break;
                case TIPO_STRING:
                    if (valorEvaluado.v.s) {
                        // Para strings, limitar a 40 caracteres para no sobrecargar el gráfico
                        if (strlen(valorEvaluado.v.s) > 40) {
                            char temp[44];
                            strncpy(temp, valorEvaluado.v.s, 40);
                            temp[40] = '\0';
                            snprintf(valorPrint, sizeof(valorPrint), "%s...", temp);
                        } else {
                            snprintf(valorPrint, sizeof(valorPrint), "%s", valorEvaluado.v.s);
                        }
                    } else {
                        snprintf(valorPrint, sizeof(valorPrint), "null");
                    }
                    break;
                default:
                    snprintf(valorPrint, sizeof(valorPrint), "?");
                    break;
            }
        }
        
        // Escapar el valor para Graphviz
        char valorPrintEsc[256];
        escaparLabel(valorPrint, valorPrintEsc, sizeof(valorPrintEsc));
        
        fprintf(f, "  n%p [label=\"{Tipo: %s|Imprime: %s}\"];\n",
                (void*)n, nombreNodoTipo(n->tipo), valorPrintEsc);
    } else {
        fprintf(f, "  n%p [label=\"{Tipo: %s|Valor: %s}\"];\n",
                (void*)n, nombreNodoTipo(n->tipo), valorEsc);
    }

    for (int i = 0; i < n->num_hijos; ++i) {
        if (n->hijos[i] && n->hijos[i]->valido) {
            fprintf(f, "  n%p -> n%p;\n", (void*)n, (void*)n->hijos[i]);
            graphvizRec(n->hijos[i], f);
        }
    }
}

void graphvizAST(NodoAST* raiz){
    if(!raiz){
        printf("digraph AST { /* vacío */ }\n");
        return;
    }
    printf("digraph AST {\n");
    printf("  node [shape=record,fontname=\"Helvetica\"];\n");
    graphvizRec(raiz, stdout);
    printf("}\n");
}


static void graphvizRecFile(NodoAST* n, FILE* f){
    if(!n || !n->valido) return;

    char valorEsc[128];
    escaparLabel(n->valor ? n->valor : "NULL", valorEsc, sizeof(valorEsc));
    
    // Mostrar el valor evaluado en el nodo PRINT (igual que en graphvizRec)
    if (n->tipo == AST_PRINT && n->num_hijos > 0 && n->hijos[0]) {
        // Evaluar la expresión a imprimir
        ValorSimbolo valorEvaluado = evaluarExpr(n->hijos[0]);
        char valorPrint[256] = "sin valor";
        
        if (valorEvaluado.inicializado) {
            switch (valorEvaluado.tipo) {
                case TIPO_INT:
                    snprintf(valorPrint, sizeof(valorPrint), "%d", valorEvaluado.v.i);
                    break;
                case TIPO_FLOAT:
                    snprintf(valorPrint, sizeof(valorPrint), "%f", valorEvaluado.v.f);
                    break;
                case TIPO_DOUBLE:
                    snprintf(valorPrint, sizeof(valorPrint), "%lf", valorEvaluado.v.d);
                    break;
                case TIPO_CHAR:
                    snprintf(valorPrint, sizeof(valorPrint), "%c", valorEvaluado.v.c);
                    break;
                case TIPO_BOOLEAN:
                    snprintf(valorPrint, sizeof(valorPrint), "%s", valorEvaluado.v.b ? "true" : "false");
                    break;
                case TIPO_STRING:
                    if (valorEvaluado.v.s) {
                        // Para strings, limitar a 40 caracteres para no sobrecargar el gráfico
                        if (strlen(valorEvaluado.v.s) > 40) {
                            char temp[44];
                            strncpy(temp, valorEvaluado.v.s, 40);
                            temp[40] = '\0';
                            snprintf(valorPrint, sizeof(valorPrint), "%s...", temp);
                        } else {
                            snprintf(valorPrint, sizeof(valorPrint), "%s", valorEvaluado.v.s);
                        }
                    } else {
                        snprintf(valorPrint, sizeof(valorPrint), "null");
                    }
                    break;
                default:
                    snprintf(valorPrint, sizeof(valorPrint), "?");
                    break;
            }
        }
        
        // Escapar el valor para Graphviz
        char valorPrintEsc[256];
        escaparLabel(valorPrint, valorPrintEsc, sizeof(valorPrintEsc));
        
        fprintf(f, "  n%p [label=\"{Tipo: %s|Imprime: %s}\"];\n",
                (void*)n, nombreNodoTipo(n->tipo), valorPrintEsc);
    } else {
        fprintf(f, "  n%p [label=\"{Tipo: %s|Valor: %s}\"];\n",
                (void*)n, nombreNodoTipo(n->tipo), valorEsc);
    }

    for(int i=0; i<n->num_hijos; ++i){
        if(n->hijos[i] && n->hijos[i]->valido) {
            fprintf(f, "  n%p -> n%p;\n", (void*)n, (void*)n->hijos[i]);
            graphvizRecFile(n->hijos[i], f);
        }
    }
}

void interpretarAccesoEscritura(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_ACCESO_ESCRITURA) {
        printf("[ERROR] Nodo inválido para acceso de escritura\n");
        return;
    }

    // Obtener el nodo de acceso (identificador + dimensiones)
    NodoAST* acceso = nodo->hijos[0];
    NodoAST* valorExpr = nodo->hijos[1];

    if (acceso->tipo != AST_ACCESO || acceso->num_hijos < 2) {
        printf("[ERROR] Estructura de acceso inválida en escritura\n");
        return;
    }

    // Extraer el identificador y las dimensiones
    NodoAST* idNodo = acceso->hijos[0];
    NodoAST* dimensionesNodo = acceso->hijos[1];

    if (idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Identificador inválido en acceso de escritura\n");
        return;
    }

    // Buscar el símbolo
    Simbolo* simbolo = buscarSimbolo(idNodo->valor);
    if (!simbolo) {
        printf("[ERROR] Variable '%s' no declarada\n", idNodo->valor);
        return;
    }

    // Verificar si es un vector o matriz
    if (simbolo->clase == SIMBOLO_VECTOR) {
        printf("[DEBUG] Escritura en vector '%s'\n", simbolo->nombre);
        
        // Verificar que el acceso sea válido
        if (!verificarAccesoVector(acceso)) {
            printf("[ERROR] Acceso inválido al vector '%s' en escritura\n", simbolo->nombre);
            return;
        }
        
        // Obtener el índice del vector
        // - dimensionesNodo contiene la lista de dimensiones
        // - Para vectores, debe tener un hijo de tipo AST_1DIMENSION
        if (dimensionesNodo->tipo != AST_DIMENSIONES || dimensionesNodo->num_hijos < 1) {
            printf("[ERROR] Dimensiones inválidas para vector en escritura\n");
            return;
        }
        
        // Buscar la primera dimensión
        NodoAST* dimNodo = NULL;
        for (int i = 0; i < dimensionesNodo->num_hijos; i++) {
            if (dimensionesNodo->hijos[i]->tipo == AST_1DIMENSION) {
                dimNodo = dimensionesNodo->hijos[i];
                break;
            }
        }
        
        if (!dimNodo || dimNodo->num_hijos < 1) {
            printf("[ERROR] No se encontró la dimensión para el vector en escritura\n");
            return;
        }
        
        // Evaluar el índice
        ValorSimbolo indiceVal = evaluarExpr(dimNodo->hijos[0]);
        if (!indiceVal.inicializado || indiceVal.tipo != TIPO_INT) {
            printf("[ERROR] Índice debe ser entero\n");
            return;
        }
        
        int indice = indiceVal.v.i;
        printf("[DEBUG] Índice para asignación en vector: %d\n", indice);
        
        // Evaluar el valor a asignar
        ValorSimbolo valorAsignar = evaluarExpr(valorExpr);
        if (!valorAsignar.inicializado) {
            printf("[ERROR] Valor no inicializado para asignación en vector\n");
            return;
        }
        
        // Verificar compatibilidad de tipos
        if (valorAsignar.tipo != simbolo->tipo && !esWidening(valorAsignar.tipo, simbolo->tipo)) {
            printf("[ERROR] Tipo incompatible en asignación: %s vs %s\n", 
                   nombreTipo(simbolo->tipo), nombreTipo(valorAsignar.tipo));
            return;
        }
        
        // Asignar el valor al vector
        asignarValorVector(simbolo, indice, valorAsignar);
        
        // Verificar que la asignación funcionó
        printf("[DEBUG] Vector después de asignación: %s\n", simbolo->valor.v.s);
    } 
    else if (simbolo->clase == SIMBOLO_MATRIZ) {
        printf("[DEBUG] Escritura en matriz '%s'\n", simbolo->nombre);
        
        // Verificar que el acceso sea válido
        if (!verificarAccesoMatriz(acceso)) {
            printf("[ERROR] Acceso inválido a la matriz '%s' en escritura\n", simbolo->nombre);
            return;
        }
        
        // Obtener los índices (fila y columna)
        if (dimensionesNodo->tipo != AST_DIMENSIONES || dimensionesNodo->num_hijos < 2) {
            printf("[ERROR] Dimensiones inválidas para matriz en escritura\n");
            return;
        }
        
        // Buscar las dos dimensiones
        NodoAST* dimFila = NULL;
        NodoAST* dimCol = NULL;
        int dimensionesEncontradas = 0;
        
        for (int i = 0; i < dimensionesNodo->num_hijos && dimensionesEncontradas < 2; i++) {
            if (dimensionesNodo->hijos[i]->tipo == AST_1DIMENSION) {
                if (dimensionesEncontradas == 0) {
                    dimFila = dimensionesNodo->hijos[i];
                } else {
                    dimCol = dimensionesNodo->hijos[i];
                }
                dimensionesEncontradas++;
            }
        }
        
        if (!dimFila || !dimCol || dimFila->num_hijos < 1 || dimCol->num_hijos < 1) {
            printf("[ERROR] No se encontraron las dimensiones para la matriz en escritura\n");
            return;
        }
        
        // Evaluar los índices
        ValorSimbolo filaVal = evaluarExpr(dimFila->hijos[0]);
        ValorSimbolo colVal = evaluarExpr(dimCol->hijos[0]);
        
        if (!filaVal.inicializado || filaVal.tipo != TIPO_INT ||
            !colVal.inicializado || colVal.tipo != TIPO_INT) {
            printf("[ERROR] Índices deben ser enteros\n");
            return;
        }
        
        int fila = filaVal.v.i;
        int columna = colVal.v.i;
        printf("[DEBUG] Índices para asignación en matriz: [%d][%d]\n", fila, columna);
        
        // Evaluar el valor a asignar
        ValorSimbolo valorAsignar = evaluarExpr(valorExpr);
        if (!valorAsignar.inicializado) {
            printf("[ERROR] Valor no inicializado para asignación en matriz\n");
            return;
        }
        
        // Verificar compatibilidad de tipos
        if (valorAsignar.tipo != simbolo->tipo && !esWidening(valorAsignar.tipo, simbolo->tipo)) {
            printf("[ERROR] Tipo incompatible en asignación: %s vs %s\n", 
                   nombreTipo(simbolo->tipo), nombreTipo(valorAsignar.tipo));
            return;
        }
        
        // Asignar el valor a la matriz
        asignarValorMatriz(simbolo, fila, columna, valorAsignar);
        
        // Verificar que la asignación funcionó
        printf("[DEBUG] Matriz después de asignación: %s\n", simbolo->valor.v.s);
    } else if (simbolo->clase == SIMBOLO_ARRAY_MULTIDIMENSIONAL) {
        printf("[DEBUG] Escritura en array multidimensional '%s'\n", simbolo->nombre);
        
        // Verificar que el acceso sea válido
        if (!verificarAccesoArray(acceso)) {
            printf("[ERROR] Acceso inválido al array '%s' en escritura\n", simbolo->nombre);
            return;
        }
        
        // Obtener los índices para todas las dimensiones
        if (dimensionesNodo->tipo != AST_DIMENSIONES || dimensionesNodo->num_hijos < 1) {
            printf("[ERROR] Dimensiones inválidas para array en escritura\n");
            return;
        }
        
        // Extraer todos los índices en un arreglo
        int indices[10] = {0}; // Máximo 10 dimensiones
        int numIndices = 0;
        
        for (int i = 0; i < dimensionesNodo->num_hijos && numIndices < 10; i++) {
            NodoAST* dimNodo = dimensionesNodo->hijos[i];
            
            if (dimNodo->tipo != AST_1DIMENSION || dimNodo->num_hijos < 1) {
                printf("[ERROR] Formato inválido para dimensión %d\n", i+1);
                return;
            }
            
            // Evaluar el índice
            ValorSimbolo idxValor = evaluarExpr(dimNodo->hijos[0]);
            if (!idxValor.inicializado || idxValor.tipo != TIPO_INT) {
                printf("[ERROR] Índice %d debe ser entero\n", i+1);
                return;
            }
            
            // Verificar que el índice no sea negativo
            if (idxValor.v.i < 0) {
                printf("[ERROR] Índice %d negativo (%d)\n", i+1, idxValor.v.i);
                return;
            }
            
            indices[numIndices++] = idxValor.v.i;
        }
        
        printf("[DEBUG] Índices para escritura en array: [");
        for (int i = 0; i < numIndices; i++) {
            printf("%d", indices[i]);
            if (i < numIndices - 1) printf("][");
        }
        printf("]\n");
        
        // Evaluar el valor a asignar
        ValorSimbolo valorAsignar = evaluarExpr(valorExpr);
        if (!valorAsignar.inicializado) {
            printf("[ERROR] Valor no inicializado para asignación en array\n");
            return;
        }
        
        // Verificar compatibilidad de tipos
        if (valorAsignar.tipo != simbolo->tipo && !esWidening(valorAsignar.tipo, simbolo->tipo)) {
            printf("[ERROR] Tipo incompatible en asignación: %s vs %s\n", 
                   nombreTipo(simbolo->tipo), nombreTipo(valorAsignar.tipo));
            return;
        }
        
        // Si es necesario, aplicar conversión de tipo
        if (valorAsignar.tipo != simbolo->tipo && esWidening(valorAsignar.tipo, simbolo->tipo)) {
            ValorSimbolo valorConvertido = convertirValor(valorAsignar, simbolo->tipo);
            
            // Liberar memoria si necesario
            if (valorAsignar.tipo == TIPO_STRING && valorAsignar.v.s) {
                free(valorAsignar.v.s);
            }
            
            valorAsignar = valorConvertido;
            printf("[DEBUG] Valor convertido de %s a %s\n", 
                   nombreTipo(valorAsignar.tipo), nombreTipo(simbolo->tipo));
        }
        
        // Asignar el valor al array multidimensional
        asignarElementoArrayMultidimensional(simbolo, indices, numIndices, valorAsignar);
        
        // Mostrar el nuevo valor para depuración
        printf("[DEBUG] Array '%s' después de asignación: ", simbolo->nombre);
        if (simbolo->valor.v.s) {
            printf("%.*s\n", 100, simbolo->valor.v.s); // Mostrar solo los primeros 100 caracteres
        } else {
            printf("(null)\n");
        }
    } else {
        printf("[ERROR] Variable '%s' no es un vector ni una matriz (clase: %d)\n", 
               simbolo->nombre, simbolo->clase);
    }
}

void graphvizAST_ToFile(NodoAST* raiz, FILE* f){
    if(!f) return;
    if(!raiz){
        fprintf(f, "digraph AST { /* vacío */ }\n");
        return;
    }
    fprintf(f, "digraph AST {\n");
    fprintf(f, "  node [shape=record,fontname=\"Helvetica\"];\n");
    graphvizRecFile(raiz, f);
    fprintf(f, "}\n");
}

int exportarAST(NodoAST* raiz, const char* rutaDot, const char* rutaPNG){
    if(!rutaDot) return 1;
    FILE* f = fopen(rutaDot, "w");
    if(!f){
        perror("fopen DOT");
        return 2;
    }
    graphvizAST_ToFile(raiz, f);
    fclose(f);
    if(rutaPNG){
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "dot -Tpng \"%s\" -o \"%s\"", rutaDot, rutaPNG);
        int r = system(cmd);
        if(r != 0){
            fprintf(stderr, "Error ejecutando Graphviz (dot). Código=%d\n", r);
            return 3;
        }
    }
    return 0;
}

