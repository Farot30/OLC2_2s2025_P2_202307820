#ifndef ACCESO_ARRAYS_H
#define ACCESO_ARRAYS_H

#include "../../AST/ast.h"
#include "../tabla_simbolo.h"

int verificarAccesoArray(NodoAST* nodo);

// Evalúa el acceso a una matriz y devuelve su valor
ValorSimbolo evaluarAccesoArray(NodoAST* nodo);

#endif