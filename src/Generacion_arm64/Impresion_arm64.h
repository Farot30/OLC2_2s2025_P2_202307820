#ifndef IMPRESION_ARM64_H
#define IMPRESION_ARM64_H

#include "../AST/ast.h"
#include "InstruccionesNorm_arm64/declaraciones_arm64.h"

// Función principal para generar impresión
void generarImpresionARM64(GeneradorDeclaraciones* gen, NodoAST* nodo);

#endif