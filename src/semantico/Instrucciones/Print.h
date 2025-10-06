#ifndef PRINT_H
#define PRINT_H

#include "../../AST/ast.h"
#include "../tipos.h"

void imprimirTexto(const char* mensaje);
void imprimirValor(TipoDato tipo, const char* valor);
void imprimirEvaluado(NodoAST* expr);

#endif
