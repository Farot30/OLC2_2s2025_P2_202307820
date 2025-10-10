#ifndef OPERADORES_ARM64_H
#define OPERADORES_ARM64_H

#include "../AST/ast.h"
#include "declaraciones_arm64.h"
#include "registros_arm64.h"
#include "../semantico/tipos.h"
#include <stdio.h>


void generarCodigoOperacionARM64(GeneradorDeclaraciones* gen, const char* nombreVar, NodoAST* operacionNodo, TipoDato tipoVar);

// Función recursiva para sumas/restas/multis/divisiones de enteros
void generarCodigoRecursivoOperacion(NodoAST* nodo, char* codigoOperacion, int* esPrimera);

// Insertar constantes en .rodata
void insertarConstanteDoubleRoData(GeneradorDeclaraciones* gen, const char* nombre, double valor);
void insertarConstanteFloatRoData(GeneradorDeclaraciones* gen, const char* nombre, float valor);
void insertarConstanteCharRoData(GeneradorDeclaraciones* gen, const char* nombre, char valor);
void insertarConstanteIntRoData(GeneradorDeclaraciones* gen, const char* nombre, int valor);

#endif