#ifndef EVALUADORES_ARM64_H
#define EVALUADORES_ARM64_H

#include "declaraciones_arm64.h"
#include "../../semantico/tipos.h"
#include "../../AST/ast.h"
#include <stdio.h>
#include <stdbool.h>

// Estructura para manejar la generación de Evaluadores ARM64 (igual que StringBuilder)
typedef struct {
    FILE* archivo;
    char* buffer;
    size_t buffer_size;
    bool seccion_data_creada;
    bool seccion_bss_creada;
    bool seccion_text_creada;
    bool funciones_generadas;
    int contador_equals;  // Para generar nombres únicos si es necesario
} GeneradorEquals;

// Funciones principales de Evaluadores
void inicializarGeneradorEquals(GeneradorEquals* gen, FILE* archivo, 
                               char* buffer, size_t buffer_size);

// Generar la infraestructura completa de Equals
void generarInfraestructuraEquals(GeneradorEquals* gen);

// Función principal para generar código de equals
void generarCodigoEqualsCompleto(GeneradorDeclaraciones* gen, const char* nombreVarResultado, 
                                const char* nombreStr1, const char* nombreStr2);

// Funciones auxiliares
void generarFuncionesAuxiliaresEquals(GeneradorEquals* gen);
void insertarEnSeccionTextEquals(GeneradorEquals* gen, const char* codigo);

#endif // EVALUADORES_ARM64_H