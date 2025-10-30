#ifndef FUNCIONES_PREDEFINIDAS_ARM64_H
#define FUNCIONES_PREDEFINIDAS_ARM64_H

#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include <stdbool.h>

// Verifica si una función es predefinida
bool esFuncionPredefinida(const char* nombreFuncion);

// Genera el código ARM64 de una función predefinida (si aún no se ha generado)
void generarFuncionPredefinidaSiNecesario(GeneradorDeclaraciones* gen, const char* nombreFuncion);

// Resetea el estado de las funciones predefinidas
void resetearFuncionesPredefinidas();

#endif