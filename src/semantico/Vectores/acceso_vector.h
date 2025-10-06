#ifndef ACCESO_VECTOR_H
#define ACCESO_VECTOR_H

#include "../../AST/ast.h"
#include "../tabla_simbolo.h" 

int verificarAccesoVector(NodoAST* nodo);

// Evalúa el acceso a un vector y devuelve su valor
ValorSimbolo evaluarAccesoVector(NodoAST* nodo);

void asignarValorVector(Simbolo* simbolo, int indice, ValorSimbolo valor);
int evaluarIndiceVector(NodoAST* acceso);

#endif