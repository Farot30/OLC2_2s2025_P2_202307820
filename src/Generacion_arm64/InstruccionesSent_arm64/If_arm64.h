#ifndef IF_ARM64_H
#define IF_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../registros_arm64.h"
#include "../../semantico/tipos.h"
#include <stdio.h>

// Contador global para etiquetas únicas
extern int contador_etiquetas_if;

// Estructura para manejar etiquetas de if
typedef struct {
    char etiqueta_if[32];
    char etiqueta_endif[32];
    char etiqueta_else[32];
    char etiqueta_elseif[32];
    int numero_if;
} EtiquetasIF;

// Funciones para generar código IF en ARM64
void generarCodigoIF(GeneradorDeclaraciones* gen, NodoAST* nodoIF);
void generarCodigoIFElse(GeneradorDeclaraciones* gen, NodoAST* nodoIF);
void generarCodigoIFElseIF(GeneradorDeclaraciones* gen, NodoAST* nodoIF);

// Funciones auxiliares
EtiquetasIF crearEtiquetasIF();
void evaluarCondicionBooleanaMejorada(GeneradorDeclaraciones* gen, NodoAST* condicion, char* codigoResultado);
void generarBloqueMejorado(GeneradorDeclaraciones* gen, NodoAST* bloque);

#endif 