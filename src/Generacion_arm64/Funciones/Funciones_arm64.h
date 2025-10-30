#ifndef FUNCIONES_ARM64_H
#define FUNCIONES_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "FuncionesVoid_arm64.h"
#include <stdbool.h>


typedef struct {
    AmbitoFuncion ambito_base;  // Reutilizar estructura de funciones void
    TipoDato tipoRetorno;       // Tipo de retorno de la función
    bool tieneReturn;           // Flag para validar que tenga al menos un return
    char etiquetaRetorno[64];   // Etiqueta única para el return
} AmbitoFuncionRetorno;

typedef struct {
    AmbitoFuncionRetorno ambitos[MAX_AMBITOS];
    int tope;
} PilaAmbitosRetorno;

void inicializarPilaAmbitosRetorno(PilaAmbitosRetorno* pila);
void pushAmbitoRetorno(PilaAmbitosRetorno* pila, const char* nombreFuncion, TipoDato tipoRetorno);
AmbitoFuncionRetorno* popAmbitoRetorno(PilaAmbitosRetorno* pila);
AmbitoFuncionRetorno* getAmbitoRetornoActual(PilaAmbitosRetorno* pila);

void generarDeclaracionFuncionRetorno(GeneradorDeclaraciones* gen, NodoAST* nodo,
                                      PilaAmbitosRetorno* pilaRetorno, NodoAST* raizAST);

void generarPrologoFuncionRetorno(GeneradorDeclaraciones* gen, AmbitoFuncionRetorno* ambito);


void generarEpilogoFuncionRetorno(GeneradorDeclaraciones* gen, AmbitoFuncionRetorno* ambito);


void generarCargaParametrosRetorno(GeneradorDeclaraciones* gen, AmbitoFuncionRetorno* ambito);


void procesarCuerpoFuncionRetorno(GeneradorDeclaraciones* gen, NodoAST* cuerpoNodo,
                                  PilaAmbitosRetorno* pilaRetorno, NodoAST* raizAST);


void generarReturnStatement(GeneradorDeclaraciones* gen, NodoAST* returnNodo,
                           AmbitoFuncionRetorno* ambito);


const char* obtenerRegistroRetorno(TipoDato tipo);


void generarEtiquetaRetorno(const char* nombreFunc, char* buffer, size_t bufferSize);

#endif 