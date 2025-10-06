#ifndef ACCESO_MATRIZ_H
#define ACCESO_MATRIZ_H
#include "../../AST/ast.h"
#include "../tabla_simbolo.h"

void analizarDimensionesMatriz(const char* matrixStr, int* numFilas, int* numColumnas);

// Verifica que el acceso a una matriz sea válido
int verificarAccesoMatriz(NodoAST* nodo);

// Evalúa el acceso a una matriz y devuelve su valor
ValorSimbolo evaluarAccesoMatriz(NodoAST* nodo);

void asignarValorMatriz(Simbolo* simbolo, int fila, int columna, ValorSimbolo valor);
int evaluarIndiceMatriz(NodoAST* acceso, int dimension);

#endif