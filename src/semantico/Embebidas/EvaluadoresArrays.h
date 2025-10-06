#ifndef EVALUADORES_ARRAYS_H
#define EVALUADORES_ARRAYS_H

#include "../../AST/ast.h"
#include "../tabla_simbolo.h"

ValorSimbolo evaluarIndexof(NodoAST* e);

// Función para evaluar Array.length
ValorSimbolo evaluarLength(NodoAST* e);

// Función para evaluar Array.add
ValorSimbolo evaluarAdd(NodoAST* e);

void obtenerDimensionesArray(Simbolo* simbolo, int* dimensiones, int* numDimensiones);
#endif