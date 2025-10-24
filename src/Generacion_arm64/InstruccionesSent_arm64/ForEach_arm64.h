#ifndef FOREACH_ARM64_H
#define FOREACH_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"

// Estructura para manejar etiquetas del for-each
typedef struct {
    int numero_foreach;
    char etiqueta_inicio[32];
    char etiqueta_fin[32];
    char etiqueta_loop[32];
} EtiquetasForEach;

// Genera código ARM64 para un for-each (enhanced for loop)
// Estructura del nodo:
// - hijos[0]: Declaración de variable iteradora (ej: int nota)
// - hijos[1]: Vector/Array a iterar (ej: notasParaForeach)
// - hijos[2]: Bloque de código del for-each
void generarCodigoForEach(GeneradorDeclaraciones* gen, NodoAST* nodoForEach);

#endif