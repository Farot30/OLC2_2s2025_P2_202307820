#ifndef WHILE_ARM64_H
#define WHILE_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../registros_arm64.h"

void generarCodigoWhile(GeneradorDeclaraciones* gen, NodoAST* nodoWhile);

#endif