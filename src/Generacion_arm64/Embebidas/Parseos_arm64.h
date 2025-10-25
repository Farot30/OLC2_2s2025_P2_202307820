#ifndef PARSEOS_ARM64_H
#define PARSEOS_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "EvaluadorExpr_arm64.h"

// Genera código ARM64 para Integer.parseInt(string)
// Convierte un string a int usando nuestra implementación custom
ResultadoExprEmbebida generarParseIntARM64(GeneradorDeclaraciones* gen, NodoAST* nodo, int* regIndex);

// Genera código ARM64 para Double.parseDouble(string)
// Convierte un string a double usando nuestra implementación custom
ResultadoExprEmbebida generarParseDoubleARM64(GeneradorDeclaraciones* gen, NodoAST* nodo, int* regIndex);

// Genera código ARM64 para Float.parseFloat(string)
// Convierte un string a float usando nuestra implementación custom
ResultadoExprEmbebida generarParseFloatARM64(GeneradorDeclaraciones* gen, NodoAST* nodo, int* regIndex);

// Crea los archivos .s con las funciones de parseo implementadas en ARM64 puro
void crearArchivoParseInt(const char* nombreArchivo);
void crearArchivoParseDouble(const char* nombreArchivo);
void crearArchivoParseFloat(const char* nombreArchivo);

// Función principal que crea todos los archivos de parseo
void crearArchivosParseo();

#endif