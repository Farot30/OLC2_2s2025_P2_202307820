#ifndef EVALUADOR_ARRAY_H
#define EVALUADOR_ARRAY_H

#include "../AST/ast.h"
#include "../semantico/tipos.h"
#include "../semantico/tabla_simbolo.h"

// Función para evaluar acceso a arrays (arr[i], arr[i][j], etc.)
ValorSimbolo evaluarAccesoArrays(NodoAST* nodo);

#endif // EVALUADOR_ARRAY_H