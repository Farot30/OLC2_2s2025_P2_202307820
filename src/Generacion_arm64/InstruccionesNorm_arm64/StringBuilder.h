#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include "declaraciones_arm64.h"
#include "../../semantico/tipos.h"
#include "../../AST/ast.h"
#include <stdio.h>
#include <stdbool.h>

// Estructura para manejar la generación de StringBuilder ARM64
typedef struct {
    FILE* archivo;
    char* buffer;
    size_t buffer_size;
    bool seccion_data_creada;
    bool seccion_bss_creada;
    bool seccion_text_creada;
    bool funciones_generadas;
    int contador_strings;  // Para generar nombres únicos
} GeneradorStringBuilder;

void resetearStringBuilder(); 
// Funciones principales de StringBuilder
void inicializarGeneradorStringBuilder(GeneradorStringBuilder* gen, FILE* archivo, 
                                     char* buffer, size_t buffer_size);

// Generar la infraestructura completa de StringBuilder
void generarInfraestructuraStringBuilder(GeneradorStringBuilder* gen);

// Generar concatenación de strings específica
void generarConcatenacionStrings(GeneradorStringBuilder* gen, NodoAST** strings, int numStrings);

// Generar concatenación desde operación AST (a + b + c)
void generarConcatenacionDesdeOperacion(GeneradorStringBuilder* gen, NodoAST* operacionNodo);


void generarConcatenacionStringBuilder(GeneradorDeclaraciones* gen, const char* nombreVar, NodoAST* operacionNodo);
// Funciones auxiliares
void generarFuncionesAuxiliares(GeneradorStringBuilder* gen);
void generarSeccionesStringBuilder(GeneradorStringBuilder* gen);

void insertarEnSeccionTextStringBuilder(GeneradorStringBuilder* gen, const char* codigo);

#endif // STRING_BUILDER_H