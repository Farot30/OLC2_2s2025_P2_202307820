#ifndef EMBEBIDAS_ARM64_H
#define EMBEBIDAS_ARM64_H

#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../Impresion_arm64.h"
#include "../Vectores_arm64/AccesoVector_arm64.h"
#include "../../AST/ast.h"

void generarStringJoinARM64(GeneradorDeclaraciones* gen, const char* nombreDestino, NodoAST* nodo);

#endif