#ifndef CONTINUE_H
#define CONTINUE_H

#include "../../AST/ast.h"
#include "../../AST/ejecutar_ast.h" 

typedef struct {
    NodoAST* nodo;
} SentenciaContinue;

SentenciaContinue* crearSentenciaContinue(NodoAST* nodo);
void liberarSentenciaContinue(SentenciaContinue* sentencia);
ResultadoEjecucion ejecutarSentenciaContinue(NodoAST* nodo);

#endif