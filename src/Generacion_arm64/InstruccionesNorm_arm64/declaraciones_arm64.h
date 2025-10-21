#ifndef DECLARACIONES_ARM64_H
#define DECLARACIONES_ARM64_H

#include "../../AST/ast.h"
#include "../registros_arm64.h"
#include <stdio.h>

// Estructura para manejar la generación de declaraciones
typedef struct {
    FILE* archivo;
    char* buffer;
    size_t buffer_size;
    AdministradorRegistros* admin_registros;
    bool temp_str_valueof_bss_insertado;
    int contador_literales_string;
} GeneradorDeclaraciones;

// Funciones para generar declaraciones ARM64
void inicializarGeneradorDeclaraciones(GeneradorDeclaraciones* gen, FILE* archivo, 
                                      char* buffer, size_t buffer_size,
                                      AdministradorRegistros* admin);

void generarDeclaracionVariable(GeneradorDeclaraciones* gen, NodoAST* nodo, bool* seccion_data_iniciada, bool* seccion_bss_iniciada);
void generarSeccionData(GeneradorDeclaraciones* gen);
void generarSeccionBss(GeneradorDeclaraciones* gen);
void agregarCodigoAlBuffer(GeneradorDeclaraciones* gen, const char* codigo);

void insertarEnSeccionText(GeneradorDeclaraciones* gen, const char* codigo);
void insertarEnSeccionData(GeneradorDeclaraciones* gen, const char* codigo);
void insertarEnSeccionBss(GeneradorDeclaraciones* gen, const char* codigo);

#endif