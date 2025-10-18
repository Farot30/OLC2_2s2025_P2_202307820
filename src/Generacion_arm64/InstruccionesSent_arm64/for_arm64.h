#ifndef FOR_ARM64_H
#define FOR_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../registros_arm64.h"

void generarCodigoFor(GeneradorDeclaraciones* gen, NodoAST* nodoFor);


#endif