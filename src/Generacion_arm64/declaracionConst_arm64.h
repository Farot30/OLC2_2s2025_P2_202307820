#ifndef DECLARACION_CONST_ARM64_H
#define DECLARACION_CONST_ARM64_H

#include "registros_arm64.h"
#include "../AST/ast.h"
#include <stdio.h>

typedef struct {
    FILE* archivo;
    char* buffer;
    size_t buffer_size;
    AdministradorRegistros* admin_registros;
} GeneradorConstantesARM64;

// Funciones para generar declaraciones ARM64
void inicializarGeneradorConstantes(GeneradorConstantesARM64* gen, FILE* archivo, 
                                      char* buffer, size_t buffer_size,
                                      AdministradorRegistros* admin);
                                      
void generarDeclaracionConstante(GeneradorConstantesARM64* gen, NodoAST* nodo, bool* seccion_text_iniciada);
void generarSeccionRoData(GeneradorConstantesARM64* gen);
void agregarCodigoAlBufferConstantes(GeneradorConstantesARM64* gen, const char* codigo);
void generarCargaEnSeccionTextoConstantes(GeneradorConstantesARM64* gen, const char* nombreConstante, int registro, bool* seccion_text_iniciada);

#endif