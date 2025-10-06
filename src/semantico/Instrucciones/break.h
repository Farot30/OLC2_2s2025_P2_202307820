#ifndef BREAK_H
#define BREAK_H

#include "../../AST/ast.h"
#include "../../AST/ejecutar_ast.h" 

typedef struct {
    NodoAST* nodo;
} SentenciaBreak;

SentenciaBreak* crearSentenciaBreak(NodoAST* nodo);
void liberarSentenciaBreak(SentenciaBreak* sentencia);
ResultadoEjecucion ejecutarSentenciaBreak(NodoAST* nodo);

#endif