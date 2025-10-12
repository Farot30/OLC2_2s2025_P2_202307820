#ifndef FUNCIONES_LLAMADAS_H
#define FUNCIONES_LLAMADAS_H

#include "../../AST/ast.h"
#include "../tabla_simbolo.h"
#include "../../AST/ejecutar_ast.h"

ResultadoEjecucion EjecutarLlamadas(NodoAST* nodo);
ValorSimbolo EvaluarLlamadaFuncion(NodoAST* nodo);
ValorSimbolo buscarRespaldoGenerico(const char* nombre_variable, const char* nombre_funcion);
void copiarVectorComoParametro(Simbolo* destino, Simbolo* origen);
extern int generic_recursion_depth;

void copiarMatrizComoParametro(Simbolo* destino, Simbolo* origen);

#endif