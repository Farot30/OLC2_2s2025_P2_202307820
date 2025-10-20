#ifndef EVALUADOR_EXPR_EMBEBIDAS_ARM64_H
#define EVALUADOR_EXPR_EMBEBIDAS_ARM64_H

#include "../InstruccionesNorm_arm64/Operadores_arm64.h"
#include "../Embebidas/String_valueOf.h"
#include <stdio.h>
#include <string.h>

// Estructura de resultado
typedef struct {
    char codigo[1024];
    char resultado[32]; // registro o dirección del resultado
    TipoDato tipo;
} ResultadoExprEmbebida;

// Función recursiva principal
ResultadoExprEmbebida generarExprEmbebidaARM64(GeneradorDeclaraciones* gen, NodoAST* expr, int* regIndex);

#endif