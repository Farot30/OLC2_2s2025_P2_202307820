#ifndef BREAK_CONTINUE_ARM64_H
#define BREAK_CONTINUE_ARM64_H

#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"

// Estructura para el stack de contextos de bucles
typedef struct ContextoBucle {
    char etiqueta_break[32];     // Etiqueta para break (fin del bucle)
    char etiqueta_continue[32];  // Etiqueta para continue (inicio del bucle)
    char tipo_contexto[16];      // "for", "while", "switch"
    struct ContextoBucle* siguiente;
} ContextoBucle;

// Stack global de contextos
extern ContextoBucle* stack_contextos_bucle;

// Funciones para manejar el stack de contextos
void pushContextoBucle(const char* etiq_break, const char* etiq_continue, const char* tipo);
void popContextoBucle();
ContextoBucle* getContextoActual();

// Funciones para generar código break y continue
void generarCodigoBreak(GeneradorDeclaraciones* gen, NodoAST* nodoBreak);
void generarCodigoContinue(GeneradorDeclaraciones* gen, NodoAST* nodoContinue);

#endif