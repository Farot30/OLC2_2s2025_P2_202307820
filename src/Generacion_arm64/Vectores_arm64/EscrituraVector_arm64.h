#ifndef ESCRITURA_VECTOR_ARM64_H
#define ESCRITURA_VECTOR_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"

// Genera código ARM64 para escritura en un vector
// Estructura del nodo AST_ACCESO_ESCRITURA:
// - hijos[0]: Nodo AST_ACCESO (identificador + dimensiones)
//   - hijos[0][0]: Identificador del vector
//   - hijos[0][1]: Dimensiones (AST_DIMENSIONES con AST_1DIMENSION)
// - hijos[1]: Valor a asignar (expresión, literal, etc.)
void generarEscrituraVectorARM64(GeneradorDeclaraciones* gen, NodoAST* nodo);

#endif