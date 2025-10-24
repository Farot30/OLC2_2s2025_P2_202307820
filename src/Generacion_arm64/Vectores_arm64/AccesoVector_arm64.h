#ifndef ACCESO_VECTOR_ARM64_H
#define ACCESO_VECTOR_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"

// Genera código ARM64 para acceso a un vector y devuelve el resultado
// Esta función se integra con el evaluador recursivo de expresiones
ResultadoExprEmbebida generarAccesoVectorARM64(GeneradorDeclaraciones* gen, NodoAST* nodo, int* regIndex);

// Versión simplificada para asignaciones
void generarAccesoVectorParaAsignacion(GeneradorDeclaraciones* gen, NodoAST* nodo, const char* nombreDestino);
NodoAST* buscarVectorEnAST(const char* nombre);

#endif