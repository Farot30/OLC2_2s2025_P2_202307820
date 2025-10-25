#ifndef AST_H
#define AST_H

#include <stdio.h>
#include "../comun/strutil.h"
typedef enum {
    AST_PROGRAMA,
    AST_NUMERO,
    AST_IDENTIFICADOR,
    AST_DECLARACION,
    AST_1DIMENSION,
    AST_LISTA_ELEMENTOS,
    AST_LISTA,
    AST_BLOQUE,
    AST_ASIGNACION,
    AST_ASIGNACION_COMPUESTA,
    AST_CADENA,
    AST_CARACTER,
    AST_OPERACION,
    AST_CAST,
    AST_PRINT,
    AST_BOOLEAN,
    AST_ERROR,
    AST_STRING,
    AST_EQUAL,
    AST_IF,
    AST_ELSE,
    AST_ELSEIF,
    AST_SWITCH,
    AST_CASE,
    AST_BREAK,
    AST_DEFAULT,
    AST_WHILE,
    AST_CONTINUE,
    AST_FOR,
    AST_FOR_VECTOR,
    AST_INCREMENTO,
    AST_DECREMENTO,
    AST_DECLARACION_VECTOR,
    AST_DECLARACION_MATRIZ,
    AST_DECLARACION_ARRAY_MULTIDIMENSIONAL,
    AST_DIMENSIONES,
    AST_TIPO,
    AST_INTERNO,
    AST_ACCESO,
    AST_PARSEINT,
    AST_PARSEDOUBLE,
    AST_PARSEFLOAT,
    AST_PARSEJOIN,
    AST_VALUEOF,
    AST_INDEXOF,
    AST_LENGHT,
    AST_ADD,
    AST_ACCESO_ESCRITURA,
    AST_FUNCION_VOID,
    AST_LLAMADA_VOID,
    AST_FUNCION,
    AST_LLAMADA,
    AST_PARAMETROS,
    AST_RETURN,
    AST_LISTA_PARAMETROS,
    AST_PARAMETRO,
    AST_METODO_MAIN,
    AST_NULL,
    AST_DECLARACION_CONST,
    AST_DECLARACION_VECTOR_TEMP,
    AST_DO_WHILE,
    // Por si se me olvida, aquí es agregar más tipos
} TipoNodoAST;

typedef struct NodoAST {
    TipoNodoAST tipo;
    char* valor;
    struct NodoAST** hijos;
    int num_hijos;
    int linea;
    int columna;
    int valido;  // 1 = nodo válido, 0 = nodo con errores
} NodoAST;

// Funciones para crear nodos
NodoAST* crearNodo(TipoNodoAST tipo, char* valor);
void agregarHijo(NodoAST* padre, NodoAST* hijo);
void liberarAST(NodoAST* nodo);
void imprimirAST(NodoAST* nodo, int nivel);
void graphvizAST(NodoAST* nodo);
void graphvizAST_ToFile(NodoAST* nodo, FILE* f);
int exportarAST(NodoAST* nodo, const char* rutaDot, const char* rutaPNG);
void interpretarAccesoEscritura(NodoAST* nodo);
NodoAST* buscarDeclaracionPorIdentificador(NodoAST* raiz, TipoNodoAST tipoDecl, const char* nombreId);
NodoAST* buscarNodoEnAST(NodoAST* raiz, TipoNodoAST tipoBuscad, const char* valorBuscad);

#endif