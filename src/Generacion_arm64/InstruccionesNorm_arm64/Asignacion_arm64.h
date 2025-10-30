#ifndef ASIGNACION_ARM64_H
#define ASIGNACION_ARM64_H

#include "../../AST/ast.h"
#include "declaraciones_arm64.h"
#include "../Funciones/FuncionesVoid_arm64.h"
#include "../Funciones/Funciones_arm64.h"

// Función principal para generar asignaciones
void generarAsignacionARM64(GeneradorDeclaraciones* gen, NodoAST* nodo);
void generarAsignacionCompuestaARM64(GeneradorDeclaraciones* gen, NodoAST* nodo);
void generarAsignacionCompuestaEnFuncion(GeneradorDeclaraciones* gen, NodoAST* nodo,
                                        PilaAmbitos* pilaAmbitos);
void generarAsignacionCompuestaEnFuncionRetorno(GeneradorDeclaraciones* gen, NodoAST* nodo,
                                        PilaAmbitosRetorno* pilaRetorno);

#endif