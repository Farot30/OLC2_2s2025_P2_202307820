#ifndef SENTENCIA_FOR_H
#define SENTENCIA_FOR_H

#include "../../AST/ast.h"
#include "../../AST/ejecutar_ast.h"

typedef struct {
    NodoAST* inicializacion;
    NodoAST* condicion;
    NodoAST* incremento;
    NodoAST* bloque;
} SentenciaFor;

SentenciaFor* crearSentenciaFor(NodoAST* inicializacion, NodoAST* condicion, NodoAST* incremento, NodoAST* bloque);
void liberarSentenciaFor(SentenciaFor* sentencia);
ResultadoEjecucion ejecutarSentenciaFor(NodoAST* nodo);

#endif