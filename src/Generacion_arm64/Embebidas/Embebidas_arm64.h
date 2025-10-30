#ifndef EMBEBIDAS_ARM64_H
#define EMBEBIDAS_ARM64_H

#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../Impresion_arm64.h"
#include "../Vectores_arm64/AccesoVector_arm64.h"
#include "../../AST/ast.h"
#include "EvaluadorExpr_arm64.h"

void generarStringJoinARM64(GeneradorDeclaraciones* gen, const char* nombreDestino, NodoAST* nodo);
void generarArraysIndexOfARM64(GeneradorDeclaraciones* gen, const char* nombreDestino, NodoAST* nodo);
void generarArraysAddOfArm64(GeneradorDeclaraciones* gen, const char* nombreDestino, NodoAST* nodo);

ResultadoExprEmbebida generarArrayLengthARM64(GeneradorDeclaraciones* gen, NodoAST* nodo, int* regIndex);

int obtenerTamanoVectorDesdeAST_Length(NodoAST* vectorDecl);

void obtenerDimensionesMatrizDesdeAST_Length(NodoAST* matrizDecl, int* filas, int* columnas);

#endif