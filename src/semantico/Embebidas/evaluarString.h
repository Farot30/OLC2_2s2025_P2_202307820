#ifndef EVALUADOR_STRING_H
#define EVALUADOR_STRING_H

#include "../../AST/ast.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"

ValorSimbolo evaluarParseJoin(NodoAST* e);
ValorSimbolo evaluarValueOf(NodoAST* e);
ValorSimbolo evaluarEqual(NodoAST* e);

#endif