#ifndef FUNCION_H
#define FUNCION_H

#include "../../AST/ast.h"
#include "../tabla_simbolo.h"
#include "../../AST/ejecutar_ast.h"

ResultadoEjecucion DeclararFuncion(NodoAST* nodo) ;
int esTipoVector(const char* tipo);
int esTipoMatriz(const char* tipo);
#endif