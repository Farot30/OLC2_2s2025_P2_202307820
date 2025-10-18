#ifndef ESCAPE_SECUENCIA_ARM64_H
#define ESCAPE_SECUENCIA_ARM64_H

// Convierte caracteres literales de vuelta a secuencias de escape para ARM64
char* escaparParaARM64(const char* cadena);

// Limpia y procesa una cadena para usar en ARM64 assembly
char* limpiarCadenaParaASM(const char* cadena);

#endif