#ifndef REGISTROS_ARM64_H
#define REGISTROS_ARM64_H

#include <stdbool.h>
#include "../semantico/tipos.h"

// Estructura para manejar registros ARM64
typedef struct {
    char nombre[8];     // Nombre del registro (x0, x1, etc.)
    bool ocupado;       // Si está en uso
    char* variable;     // Variable asociada (si existe)
    TipoDato tipo;      // Tipo de dato almacenado
} RegistroARM64;

// Administrador de registros
typedef struct {
    RegistroARM64 registros[32];  // x0 a x31
    int contador_temp;            // Para variables temporales
} AdministradorRegistros;

// Funciones para manejo de registros
void inicializarRegistros(AdministradorRegistros* admin);
int obtenerRegistroLibre(AdministradorRegistros* admin);
void liberarRegistro(AdministradorRegistros* admin, int indice);
int asignarRegistroAVariable(AdministradorRegistros* admin, const char* variable, TipoDato tipo);
int buscarRegistroPorVariable(AdministradorRegistros* admin, const char* variable);
const char* obtenerNombreRegistro(int indice);

#endif // REGISTROS_ARM64_H