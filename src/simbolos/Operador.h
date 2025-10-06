#ifndef OPERADOR_H
#define OPERADOR_H
#include "Evaluador.h"
ValorSimbolo evaluarOperacion(NodoAST* e, ValorSimbolo a, ValorSimbolo b);
char* valorToString(ValorSimbolo val);
double convertirANumero(ValorSimbolo v);
#endif