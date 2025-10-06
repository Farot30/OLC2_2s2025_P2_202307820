#ifndef EJECUTOR_AST_H
#define EJECUTOR_AST_H

#include "ast.h"
#include "../simbolos/Evaluador.h"  

typedef enum {
    EJEC_OK,
    EJEC_BREAK,
    EJEC_CONTINUE,
    EJEC_RETURN,
} ResultadoEjecucion;

typedef struct {
    ValorSimbolo valor;
    int nivel_funcion;
} RetornoInfo;

typedef struct {
    RetornoInfo items[100];
    int tope;
} StackRetorno;

// Variables globales para manejo de retorno
extern StackRetorno stackRetorno;
extern int nivel_funcion_actual;

extern int contexto_break;
extern int contexto_continue;
extern int contexto_funcion;

// Variables para manejar el valor de retorno
extern ValorSimbolo valorRetorno;
extern int hayRetorno;

// Función principal para interpretar el programa completo
void interpretarPrograma(NodoAST* raiz);

// Función para ejecutar cada nodo del AST en orden secuencial
ResultadoEjecucion ejecutarNodoAST(NodoAST* nodo);

// Función auxiliar para verificar la estructura del AST
void verificarEstructuraAST(NodoAST* nodo, int nivel);

#endif