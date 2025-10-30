#ifndef ACCESO_MATRIZ_ARM64_H
#define ACCESO_MATRIZ_ARM64_H

#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include "../../AST/ast.h"
#include "../../semantico/tipos.h"

int contarDimensionesAcceso(NodoAST* accesoNodo);


void obtenerDimensionesMatriz(NodoAST* matrizDecl, int* filas, int* columnas);


ResultadoExprEmbebida generarAccesoMatrizARM64(GeneradorDeclaraciones* gen, NodoAST* accesoNodo, int* regIndex);

void generarAccesoMatrizParaAsignacion(GeneradorDeclaraciones* gen, NodoAST* nodo, const char* nombreDestino);

TipoDato obtenerTipoMatrizDesdeAST(NodoAST* matrizDecl);

#endif // ACCESO_MATRIZ_ARM64_H