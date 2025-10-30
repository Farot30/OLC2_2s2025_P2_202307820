#ifndef LLAMADAS_ARM64_H
#define LLAMADAS_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "Funciones_arm64.h"
#include <stdbool.h>

// Genera código para llamar a una función con retorno
void generarLlamadaFuncionRetorno(GeneradorDeclaraciones* gen, NodoAST* llamadaNodo,
                                  PilaAmbitosRetorno* pilaRetorno, NodoAST* raizAST);

// Busca la declaración de una función con retorno en el AST
NodoAST* buscarFuncionRetornoEnAST(NodoAST* raiz, const char* nombreFuncion);

// Obtiene el tipo de retorno de una función
TipoDato obtenerTipoRetornoFuncion(NodoAST* funcionNodo);

// Genera código para pasar argumentos a una función
void generarPasarArgumentosRetorno(GeneradorDeclaraciones* gen, 
                                   NodoAST* argumentosNodo,
                                   ParametroInfo* parametros,
                                   int numParametros);

#endif // LLAMADAS_ARM64_H