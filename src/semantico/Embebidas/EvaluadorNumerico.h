#ifndef EVALUADOR_NUMERICO_H
#define EVALUADOR_NUMERICO_H

#include "../../AST/ast.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"

ValorSimbolo evaluarParseInt(NodoAST* e);
ValorSimbolo evaluarParseFloat(NodoAST* e);
ValorSimbolo evaluarParseDouble(NodoAST* e);

#endif