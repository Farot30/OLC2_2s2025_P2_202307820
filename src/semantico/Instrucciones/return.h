#ifndef RETURN_H
#define RETURN_H

#include "../../AST/ast.h"
#include "../../AST/ejecutar_ast.h" 
#include "../../simbolos/Evaluador.h"

typedef struct {
    NodoAST* nodo;
    ValorSimbolo valor;
} SentenciaReturn;

SentenciaReturn* crearSentenciaReturn(NodoAST* nodo);
void liberarSentenciaReturn(SentenciaReturn* sentencia);
ResultadoEjecucion ejecutarSentenciaReturn(NodoAST* nodo);

#endif