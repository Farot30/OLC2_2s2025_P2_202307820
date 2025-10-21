#include "registros_arm64.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void inicializarRegistros(AdministradorRegistros* admin) {
    for (int i = 0; i < 32; i++) {
        snprintf(admin->registros[i].nombre, sizeof(admin->registros[i].nombre), "x%d", i);
        admin->registros[i].ocupado = false;
        admin->registros[i].variable = NULL;
        admin->registros[i].tipo = TIPO_DESCONOCIDO;
    }
    
    // Reservar registros especiales
    // x0-x7: parámetros y valores de retorno
    // x8: número de syscall
    // x29: frame pointer
    // x30: link register
    admin->registros[8].ocupado = true;   // syscall
    admin->registros[29].ocupado = true;  // frame pointer
    admin->registros[30].ocupado = true;  // link register
    
    admin->contador_temp = 0;
    admin->contador_constantes = 0;
    admin->contador_formatos = 0;
}

int obtenerRegistroLibre(AdministradorRegistros* admin) {
    // Buscar desde x9 hacia arriba (evitar registros de parámetros y especiales)
    for (int i = 9; i < 29; i++) {
        if (!admin->registros[i].ocupado) {
            admin->registros[i].ocupado = true;
            return i;
        }
    }
    return -1; // No hay registros libres
}

void liberarRegistro(AdministradorRegistros* admin, int indice) {
    if (indice >= 0 && indice < 32) {
        admin->registros[indice].ocupado = false;
        if (admin->registros[indice].variable) {
            free(admin->registros[indice].variable);
            admin->registros[indice].variable = NULL;
        }
        admin->registros[indice].tipo = TIPO_DESCONOCIDO;
    }
}

int asignarRegistroAVariable(AdministradorRegistros* admin, const char* variable, TipoDato tipo) {
    // Primero verificar si la variable ya tiene un registro asignado
    int registro_existente = buscarRegistroPorVariable(admin, variable);
    if (registro_existente != -1) {
        return registro_existente;
    }
    
    // Obtener un nuevo registro libre
    int registro = obtenerRegistroLibre(admin);
    if (registro != -1) {
        admin->registros[registro].variable = strdup(variable);
        admin->registros[registro].tipo = tipo;
        printf("[DEBUG REGISTRO] Variable '%s' asignada a registro %s\n", 
               variable, admin->registros[registro].nombre);
    }
    return registro;
}

int buscarRegistroPorVariable(AdministradorRegistros* admin, const char* variable) {
    for (int i = 0; i < 32; i++) {
        if (admin->registros[i].variable && 
            strcmp(admin->registros[i].variable, variable) == 0) {
            return i;
        }
    }
    return -1;
}

const char* obtenerNombreRegistro(int indice) {
    static char nombre[8];
    if (indice >= 0 && indice < 32) {
        snprintf(nombre, sizeof(nombre), "x%d", indice);
        return nombre;
    }
    return "x0"; // fallback
}