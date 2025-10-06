#include "declaracionConst_arm64.h"
#include "../semantico/tabla_simbolo.h"
#include <string.h>
#include <stdlib.h>

void inicializarGeneradorConstantes(GeneradorConstantesARM64* gen, FILE* archivo, 
                                      char* buffer, size_t buffer_size,
                                      AdministradorRegistros* admin) {
    gen->archivo = archivo;
    gen->buffer = buffer;
    gen->buffer_size = buffer_size;
    gen->admin_registros = admin;
}

void agregarCodigoAlBufferConstantes(GeneradorConstantesARM64* gen, const char* codigo) {
    if (gen->buffer && codigo) {
        size_t len_actual = strlen(gen->buffer);
        size_t len_codigo = strlen(codigo);
        
        if (len_actual + len_codigo + 1 < gen->buffer_size) {
            strcat(gen->buffer, codigo);
        } else {
            // Redimensionar buffer si es necesario
            gen->buffer_size = (len_actual + len_codigo + 1024);
            gen->buffer = realloc(gen->buffer, gen->buffer_size);
            if (gen->buffer) {
                strcat(gen->buffer, codigo);
            }
        }
    }
}

void generarSeccionRoData(GeneradorConstantesARM64* gen) {
    const char* seccion_rodata = "\n// declaraciones de constantes\n.section .rodata\n";
    if (gen->archivo) {
        fprintf(gen->archivo, "%s", seccion_rodata);
    }
    agregarCodigoAlBufferConstantes(gen, seccion_rodata);
}

void generarDeclaracionConstante(GeneradorConstantesARM64* gen, NodoAST* nodo, bool* seccion_text_iniciada) {
    if (!gen || !nodo || nodo->tipo != AST_DECLARACION_CONST || !seccion_text_iniciada) return;

    // Extraer información de la declaración
    if (nodo->num_hijos < 3) return;

    NodoAST* tipoNodo = nodo->hijos[0];  // Tipo de la constante
    NodoAST* idNodo = nodo->hijos[1];    // Identificador
    NodoAST* valorNodo = nodo->num_hijos > 2 ? nodo->hijos[2] : NULL; // Valor inicial

    if (!tipoNodo || !idNodo || idNodo->tipo != AST_IDENTIFICADOR || !valorNodo) return;

    const char* nombreVariable = idNodo->valor;
    const char* tipoVariable = tipoNodo->valor;

    // Obtener el tipo de dato
    TipoDato tipo = obtenerTipoDesdeString(tipoVariable);

    // Buscar el símbolo en la tabla para obtener el valor
    Simbolo* simbolo = buscarSimbolo(nombreVariable);
    if (!simbolo || !simbolo->es_constante) return;

    // Obtener el valor como cadena
    char valorStr[64] = "0";
    if (simbolo->valor.inicializado) {
        switch (simbolo->tipo) {
            case TIPO_INT:
                snprintf(valorStr, sizeof(valorStr), "%d", simbolo->valor.v.i);
                break;
            case TIPO_CHAR:
                snprintf(valorStr, sizeof(valorStr), "%d", (int)simbolo->valor.v.c);
                break;
            case TIPO_FLOAT:
                snprintf(valorStr, sizeof(valorStr), "%.6f", simbolo->valor.v.f);
                break;
            case TIPO_DOUBLE:
                snprintf(valorStr, sizeof(valorStr), "%.15lf", simbolo->valor.v.d);
                break;
            case TIPO_BOOLEAN:
                snprintf(valorStr, sizeof(valorStr), "%d", simbolo->valor.v.b ? 1 : 0);
                break;
            default:
                strcpy(valorStr, "0");
                break;
        }
    } else if (valorNodo && valorNodo->valor) {
        strncpy(valorStr, valorNodo->valor, sizeof(valorStr) - 1);
        valorStr[sizeof(valorStr) - 1] = '\0';
    }

    // Asignar registro a la variable
    int registro = asignarRegistroAVariable(gen->admin_registros, nombreVariable, tipo);

    // Generar código ARM64 - Solo declaración en .rodata
    char codigo[512];
    snprintf(codigo, sizeof(codigo),
             "%s: .%s %s\n",
             nombreVariable,
             (tipo == TIPO_INT || tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) ? "word" : "float",
             valorStr);

    if (gen->archivo) {
        fprintf(gen->archivo, "%s", codigo);
    }
    agregarCodigoAlBufferConstantes(gen, codigo);

    printf("[DEBUG ARM64] Constante '%s' declarada con valor %s en registro %s\n",
           nombreVariable, valorStr, obtenerNombreRegistro(registro));
}

void generarCargaEnSeccionTextoConstantes(GeneradorConstantesARM64* gen, const char* nombreVariable, int registro, bool* seccion_text_iniciada) {
    if (!gen || !nombreVariable || registro < 0 || !seccion_text_iniciada) return;

    // Cambiar a la sección .text si no está activa
    if (!(*seccion_text_iniciada)) {
        const char* seccion_texto = "\n// código ejecutable\n.text\n\n_start:\n    // Punto de entrada del programa\n";
        if (gen->archivo) {
            fprintf(gen->archivo, "%s", seccion_texto);
        }
        agregarCodigoAlBufferConstantes(gen, seccion_texto);
        *seccion_text_iniciada = true;
    }

    // Generar el código de carga
    char codigo[256];
    snprintf(codigo, sizeof(codigo),
             "    // Cargar %s en registro %s\n"
             "    adr x1, %s\n"
             "    ldr %s, [x1]\n",
             nombreVariable,
             obtenerNombreRegistro(registro),
             nombreVariable,
             obtenerNombreRegistro(registro));

    if (gen->archivo) {
        fprintf(gen->archivo, "%s", codigo);
    }
    agregarCodigoAlBufferConstantes(gen, codigo);
}