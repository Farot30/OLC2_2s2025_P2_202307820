#include "declaracionConst_arm64.h"
#include "../../semantico/tabla_simbolo.h"
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

void insertarEnSeccionRoData(GeneradorConstantesARM64* gen, const char* codigo) {
    if (!gen->buffer || !codigo) return;

    const char* rodata_marker = ".section .rodata\n";
    char* pos = strstr(gen->buffer, rodata_marker);
    if (pos) {
        // Encontrar el final de la línea .rodata
        pos += strlen(rodata_marker);
        size_t offset = pos - gen->buffer;
        size_t len_actual = strlen(gen->buffer);
        size_t len_codigo = strlen(codigo);

        // Redimensionar buffer si es necesario
        if (len_actual + len_codigo + 1 >= gen->buffer_size) {
            gen->buffer_size = len_actual + len_codigo + 1024;
            gen->buffer = realloc(gen->buffer, gen->buffer_size);
        }

        // Mover el contenido después de .rodata para hacer espacio
        memmove(gen->buffer + offset + len_codigo, gen->buffer + offset, len_actual - offset + 1);
        memcpy(gen->buffer + offset, codigo, len_codigo);
    } else {
        // Si no existe .rodata, agregar al final
        agregarCodigoAlBufferConstantes(gen, codigo);
    }
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

void generarDeclaracionConstante(GeneradorConstantesARM64* gen, NodoAST* nodo, bool* seccion_rodata_iniciada) {
    if (!gen || !nodo || !seccion_rodata_iniciada) {
        printf("[DEBUG] Nodo inválido o tipo incorrecto en generarDeclaracionConstante\n");
        return;
    }

    // Extraer información de la declaración
    if (nodo->num_hijos < 3) {
        printf("[DEBUG] Nodo con menos de 3 hijos en generarDeclaracionConstante\n");
        return;
    }

    NodoAST* tipoNodo = nodo->hijos[0];  // Tipo de la constante
    NodoAST* idNodo = nodo->hijos[1];    // Identificador
    NodoAST* valorNodo = nodo->hijos[2]; // Valor inicial

    if (!tipoNodo || !idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[DEBUG] Nodo tipo o identificador inválido en generarDeclaracionConstante\n");
        return;
    }

    const char* nombreConstante = idNodo->valor;
    const char* tipoConstante = tipoNodo->valor;

    printf("[DEBUG] Procesando constante '%s' de tipo '%s'\n", nombreConstante, tipoConstante);

    // Obtener el tipo de dato
    TipoDato tipo = obtenerTipoDesdeString(tipoConstante);

    char valorStr[64] = "0";
    if (valorNodo && valorNodo->valor) {
        if (tipo == TIPO_BOOLEAN) {
            if (strcmp(valorNodo->valor, "true") == 0) {
                strncpy(valorStr, "1", sizeof(valorStr));
            } else if (strcmp(valorNodo->valor, "false") == 0) {
                strncpy(valorStr, "0", sizeof(valorStr));
            }
        } else if (tipo == TIPO_STRING){
            if (valorNodo->valor[0] == '"' && valorNodo->valor[strlen(valorNodo->valor)-1] == '"') {
                strncpy(valorStr, valorNodo->valor, sizeof(valorStr) - 1);
            } else {
                snprintf(valorStr, sizeof(valorStr), "\"%s\"", valorNodo->valor);
            }
        } else if (tipo == TIPO_CHAR) {
            if (valorNodo->valor[0] == '\'' && valorNodo->valor[strlen(valorNodo->valor)-1] == '\'') {
                strncpy(valorStr, valorNodo->valor, sizeof(valorStr) - 1);
            } else if (strlen(valorNodo->valor) == 1) {
                snprintf(valorStr, sizeof(valorStr), "'%s'", valorNodo->valor);
            } else {
                strncpy(valorStr, "'\\0'", sizeof(valorStr));
            }
        } else if (tipo == TIPO_FLOAT) {
            // Quitar la 'f' o 'F' si viene al final
            strncpy(valorStr, valorNodo->valor, sizeof(valorStr) - 1);
            valorStr[sizeof(valorStr) - 1] = '\0';
            size_t len = strlen(valorStr);
            if (len > 0 && (valorStr[len-1] == 'f' || valorStr[len-1] == 'F')) {
                valorStr[len-1] = '\0';
            }
        } else if (tipo == TIPO_DOUBLE) {
            // Quitar la 'd', 'D', 'f', 'F' si viene al final
            strncpy(valorStr, valorNodo->valor, sizeof(valorStr) - 1);
            valorStr[sizeof(valorStr) - 1] = '\0';
            size_t len = strlen(valorStr);
            if (len > 0 && (valorStr[len-1] == 'd' || valorStr[len-1] == 'D' || valorStr[len-1] == 'f' || valorStr[len-1] == 'F')) {
                valorStr[len-1] = '\0';
            }
        } else {
            strncpy(valorStr, valorNodo->valor, sizeof(valorStr) - 1);
        }
        valorStr[sizeof(valorStr) - 1] = '\0';
    }

    char codigo[512];
    const char* tipoDeclaracion = "word";
    if (tipo == TIPO_DOUBLE) {
        tipoDeclaracion = "double";
    } else if (tipo == TIPO_FLOAT) {
        tipoDeclaracion = "float";
    } else if (tipo == TIPO_CHAR) {
        tipoDeclaracion = "byte";
    } else if (tipo == TIPO_BOOLEAN) {
        tipoDeclaracion = "byte";
    } else if (tipo == TIPO_STRING) {
        tipoDeclaracion = "asciz";
    }
    snprintf(codigo, sizeof(codigo), "%s: .%s %s\n", nombreConstante, tipoDeclaracion, valorStr);

    insertarEnSeccionRoData(gen, codigo);

}
