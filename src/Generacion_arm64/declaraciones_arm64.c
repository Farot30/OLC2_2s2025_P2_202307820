#include "declaraciones_arm64.h"
#include "declaracionConst_arm64.h"
#include "Operadores_arm64.h"
#include "../semantico/tipos.h"
#include "../semantico/semantico.h"
#include <string.h>
#include <stdlib.h>

void inicializarGeneradorDeclaraciones(GeneradorDeclaraciones* gen, FILE* archivo, 
                                      char* buffer, size_t buffer_size,
                                      AdministradorRegistros* admin) {
    gen->archivo = archivo;
    gen->buffer = buffer;
    gen->buffer_size = buffer_size;
    gen->admin_registros = admin;
}
void insertarEnSeccionText(GeneradorDeclaraciones* gen, const char* codigo) {
    if (!gen->buffer || !codigo) return;

    const char* start_marker = "    // Punto de entrada del programa\n";
    char* pos = strstr(gen->buffer, start_marker);
    if (pos) {
        // Insertar justo después del comentario de punto de entrada
        pos += strlen(start_marker);
        size_t offset = pos - gen->buffer;
        size_t len_actual = strlen(gen->buffer);
        size_t len_codigo = strlen(codigo);

        // Redimensionar buffer si es necesario
        if (len_actual + len_codigo + 1 >= gen->buffer_size) {
            gen->buffer_size = len_actual + len_codigo + 1024;
            gen->buffer = realloc(gen->buffer, gen->buffer_size);
        }

        // Mover el contenido para hacer espacio
        memmove(gen->buffer + offset + len_codigo, gen->buffer + offset, len_actual - offset + 1);
        memcpy(gen->buffer + offset, codigo, len_codigo);
    } else {
        // Si no encuentra el marcador, agregar al final
        agregarCodigoAlBuffer(gen, codigo);
    }
}
void insertarEnSeccionData(GeneradorDeclaraciones* gen, const char* codigo) {
    if (!gen->buffer || !codigo) return;

    const char* data_marker = ".data\n";
    char* pos = strstr(gen->buffer, data_marker);
    if (pos) {
        // Encontrar el final de la línea .data
        pos += strlen(data_marker);
        size_t offset = pos - gen->buffer;
        size_t len_actual = strlen(gen->buffer);
        size_t len_codigo = strlen(codigo);

        // Redimensionar buffer si es necesario
        if (len_actual + len_codigo + 1 >= gen->buffer_size) {
            gen->buffer_size = len_actual + len_codigo + 1024;
            gen->buffer = realloc(gen->buffer, gen->buffer_size);
        }

        // Mover el contenido después de .data para hacer espacio
        memmove(gen->buffer + offset + len_codigo, gen->buffer + offset, len_actual - offset + 1);
        memcpy(gen->buffer + offset, codigo, len_codigo);
    } else {
        // Si no existe .data, agregar al final
        agregarCodigoAlBuffer(gen, codigo);
    }
}

void BoscarSeccionStart(GeneradorDeclaraciones* gen, const char* codigo) {
    if (!gen->buffer || !codigo) return;

    const char* start_marker = "_start\n";
    char* pos = strstr(gen->buffer, start_marker);
    if (pos) {
        // Encontrar el final de la línea _start
        pos += strlen(start_marker);
        size_t offset = pos - gen->buffer;
        size_t len_actual = strlen(gen->buffer);
        size_t len_codigo = strlen(codigo);

        // Redimensionar buffer si es necesario
        if (len_actual + len_codigo + 1 >= gen->buffer_size) {
            gen->buffer_size = len_actual + len_codigo + 1024;
            gen->buffer = realloc(gen->buffer, gen->buffer_size);
        }

        // Mover el contenido después de _start para hacer espacio
        memmove(gen->buffer + offset + len_codigo, gen->buffer + offset, len_actual - offset + 1);
        memcpy(gen->buffer + offset, codigo, len_codigo);
    } else {
        // Si no existe _start, agregar al final
        agregarCodigoAlBuffer(gen, codigo);
    }
}

void insertarEnSeccionBss(GeneradorDeclaraciones* gen, const char* codigo) {
    if (!gen->buffer || !codigo) return;

    const char* bss_marker = ".bss\n";
    char* pos = strstr(gen->buffer, bss_marker);
    if (pos) {
        // Encontrar el final de la línea .bss
        pos += strlen(bss_marker);
        size_t offset = pos - gen->buffer;
        size_t len_actual = strlen(gen->buffer);
        size_t len_codigo = strlen(codigo);

        // Redimensionar buffer si es necesario
        if (len_actual + len_codigo + 1 >= gen->buffer_size) {
            gen->buffer_size = len_actual + len_codigo + 1024;
            gen->buffer = realloc(gen->buffer, gen->buffer_size);
        }

        // Mover el contenido después de .bss para hacer espacio
        memmove(gen->buffer + offset + len_codigo, gen->buffer + offset, len_actual - offset + 1);
        memcpy(gen->buffer + offset, codigo, len_codigo);
    } else {
        // Si no existe .bss, agregar al final
        agregarCodigoAlBuffer(gen, codigo);
    }
}

void agregarCodigoAlBuffer(GeneradorDeclaraciones* gen, const char* codigo) {
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

void generarSeccionData(GeneradorDeclaraciones* gen) {
    const char* seccion_data = "\n// declaraciones de variables\n.data\n";
    if (gen->archivo) {
        fprintf(gen->archivo, "%s", seccion_data);
    }
    agregarCodigoAlBuffer(gen, seccion_data);
}

void generarSeccionBss(GeneradorDeclaraciones* gen) {
    const char* seccion_bss = "\n// seccion bss\n.bss\n";
    if (gen->archivo) {
        fprintf(gen->archivo, "%s", seccion_bss);
    }
    agregarCodigoAlBuffer(gen, seccion_bss);
}


void generarDeclaracionVariable(GeneradorDeclaraciones* gen, NodoAST* nodo, bool* seccion_data_iniciada, bool* seccion_bss_iniciada) {
    if (!gen || !nodo || nodo->tipo != AST_DECLARACION || !seccion_data_iniciada || !seccion_bss_iniciada) {
        printf("[DEBUG] Nodo inválido o tipo incorrecto en generarDeclaracionVariable\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[DEBUG] Nodo con menos de 2 hijos en generarDeclaracionVariable\n");
        return;
    }

    NodoAST* tipoNodo = nodo->hijos[0];
    NodoAST* idNodo = nodo->hijos[1];
    NodoAST* valorNodo = nodo->num_hijos > 2 ? nodo->hijos[2] : NULL;

    if (!tipoNodo || !idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[DEBUG] Nodo tipo o identificador inválido en generarDeclaracionVariable\n");
        return;
    }

    const char* nombreVariable = idNodo->valor;
    const char* tipoVariable = tipoNodo->valor;
    TipoDato tipo = obtenerTipoDesdeString(tipoVariable);

    char valorStr[64] = "0";
    bool inicializado = false;
    bool es_operacion = false;

    if (valorNodo) {
        // **DETECTAR SI ES UNA OPERACIÓN**
        if (valorNodo->tipo == AST_OPERACION) {
            es_operacion = true;
            inicializado = true;
            printf("[DEBUG] Detectada operación '%s' en declaración de '%s'\n", valorNodo->valor, nombreVariable);
        }
        // Si es un valor literal normal
        else if (valorNodo->valor) {
            inicializado = true;
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
                    printf("[DEBUG] Valor de char inválido para variable '%s'\n", nombreVariable);
                    strncpy(valorStr, "'\\0'", sizeof(valorStr));
                }
            } else {
                strncpy(valorStr, valorNodo->valor, sizeof(valorStr) - 1);
            }
            valorStr[sizeof(valorStr) - 1] = '\0';
        }
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

    if (inicializado) {
        if (es_operacion) {
            // **GENERAR DECLARACIÓN CON VALOR 0 EN .data**
            snprintf(codigo, sizeof(codigo), "%s: .%s 0\n", nombreVariable, tipoDeclaracion);
            insertarEnSeccionData(gen, codigo);
            //if (gen->archivo) fprintf(gen->archivo, "%s", codigo);

            // **GENERAR CÓDIGO DE OPERACIÓN EN .text**
            generarCodigoOperacionARM64(gen, nombreVariable, valorNodo, tipo);
        } else {
            // Declaración normal con valor literal
            snprintf(codigo, sizeof(codigo), "%s: .%s %s\n", nombreVariable, tipoDeclaracion, valorStr);
            insertarEnSeccionData(gen, codigo);
            //if (gen->archivo) fprintf(gen->archivo, "%s", codigo);
        }
    } else {
        // Variable no inicializada va a .bss
        size_t espacio = 4;
        if (tipo == TIPO_DOUBLE) espacio = 8;
        else if (tipo == TIPO_FLOAT) espacio = 4;
        else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) espacio = 1;
        else if (tipo == TIPO_STRING) espacio = 32;
        
        snprintf(codigo, sizeof(codigo), "%s: .space %zu\n", nombreVariable, espacio);
        insertarEnSeccionBss(gen, codigo);
        //if (gen->archivo) fprintf(gen->archivo, "%s", codigo);
    }
}