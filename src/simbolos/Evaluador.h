#ifndef EVALUADOR_H
#define EVALUADOR_H
#include "../AST/ast.h"
#include "../semantico/tipos.h"          
#include "../semantico/tabla_simbolo.h"

ValorSimbolo evaluarExpr(NodoAST* expr);
ValorSimbolo makeNum(TipoDato tipo, double valor);
ValorSimbolo convertirValor(ValorSimbolo origen, TipoDato tipoDestino);
int esNum(TipoDato t);
ValorSimbolo evaluarOperaciones(NodoAST* e, ValorSimbolo a, ValorSimbolo b);
#endif