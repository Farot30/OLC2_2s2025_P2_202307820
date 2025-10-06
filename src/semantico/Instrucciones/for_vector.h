#ifndef SEMANTICO_FOR_VECTOR_H
#define SEMANTICO_FOR_VECTOR_H

#include "../../AST/ejecutar_ast.h"
#include "../../AST/err.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../../simbolos/Evaluador.h"

typedef struct {
    NodoAST* variable_iteracion;
    NodoAST* coleccion;
    NodoAST* bloque;
} SentenciaForVector;

SentenciaForVector* crearSentenciaForVector(NodoAST* variable_iteracion, NodoAST* coleccion, NodoAST* bloque);
void liberarSentenciaForVector(SentenciaForVector* sentencia);
ResultadoEjecucion ejecutarSentenciaForVector(NodoAST* nodo);

#endif