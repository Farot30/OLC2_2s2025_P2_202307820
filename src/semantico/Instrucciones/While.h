#ifndef WHILE_H
#define WHILE_H

#include "../../AST/ast.h"
#include "../../AST/ejecutar_ast.h"

typedef struct {
    NodoAST* condicion;
    NodoAST* bloque;
} SentenciaWhile;

SentenciaWhile* crearSentenciaWhile(NodoAST* condicion, NodoAST* bloque);
void liberarSentenciaWhile(SentenciaWhile* sentencia);


ResultadoEjecucion ejecutarSetenciaWhile(NodoAST* nodo);

#endif 