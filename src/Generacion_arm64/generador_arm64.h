#ifndef GENERADOR_ARM64_H
#define GENERADOR_ARM64_H

#include "../AST/ast.h"
#include "registros_arm64.h"
#include "declaraciones_arm64.h"
#include "declaracionConst_arm64.h"
#include <stdio.h>

// Estructura principal del generador ARM64
typedef struct {
    FILE* archivo;
    char* buffer_codigo;
    size_t buffer_size;
    AdministradorRegistros admin_registros;
    GeneradorDeclaraciones gen_declaraciones;
    GeneradorConstantesARM64 gen_constantes;
    bool seccion_data_creada;
    bool seccion_text_iniciada;
} GeneradorARM64;

// Funciones principales
void iniciarArchivoARM64(const char* nombreArchivo);
void finalizarArchivoARM64();
void generarDeclaracionARM64(NodoAST* nodo);
void generarDeclaracionConstanteARM64(NodoAST* nodo);
const char* obtenerCodigoGenerado();

// Funciones del generador mejorado
void inicializarGeneradorARM64(GeneradorARM64* gen, const char* nombreArchivo);
void finalizarGeneradorARM64(GeneradorARM64* gen);
void procesarNodoAST_ARM64(GeneradorARM64* gen, NodoAST* nodo);

#endif // GENERADOR_ARM64_H