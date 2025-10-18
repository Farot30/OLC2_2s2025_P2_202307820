#include "declaraciones_arm64.h"
#include "declaracionConst_arm64.h"
#include "Operadores_arm64.h"
#include "Evaluadores_arm64.h"
#include "../../semantico/tipos.h"
#include "../../semantico/semantico.h"
#include "../../AST/ast.h"
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

    // **BUSCAR LA SALIDA DEL PROGRAMA PARA INSERTAR ANTES**
    const char* exit_marker = "    // Salida del programa por defecto\n";
    char* pos = strstr(gen->buffer, exit_marker);
    
    if (pos) {
        // **INSERTAR JUSTO ANTES DE LA SALIDA DEL PROGRAMA**
        size_t offset = pos - gen->buffer;
        size_t len_actual = strlen(gen->buffer);
        size_t len_codigo = strlen(codigo);

        // Redimensionar buffer si es necesario
        if (len_actual + len_codigo + 1 >= gen->buffer_size) {
            gen->buffer_size = len_actual + len_codigo + 1024;
            gen->buffer = realloc(gen->buffer, gen->buffer_size);
            // Recalcular posición después del realloc
            pos = gen->buffer + offset;
        }

        // Mover el contenido para hacer espacio
        memmove(pos + len_codigo, pos, len_actual - offset + 1);
        memcpy(pos, codigo, len_codigo);
    } else {
        // Si no encuentra el marcador de salida, buscar el punto de entrada
        const char* start_marker = "    // Punto de entrada del programa\n";
        pos = strstr(gen->buffer, start_marker);
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
            // Si no encuentra ningún marcador, agregar al final
            agregarCodigoAlBuffer(gen, codigo);
        }
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

    if (valorNodo && (valorNodo->tipo == AST_OPERACION || valorNodo->tipo == AST_EQUAL) && strcmp(valorNodo->valor, ".equals") == 0) {
        char decl[64];
        snprintf(decl, sizeof(decl), "%s: .byte 0\n", nombreVariable);
        insertarEnSeccionData(gen, decl);

        const char* nombreVarResultado = nombreVariable;
        const char* nombreStr1 = valorNodo->hijos[0]->valor;
        const char* nombreStr2 = valorNodo->hijos[1]->valor;

        generarCodigoEqualsCompleto(gen, nombreVarResultado, nombreStr1, nombreStr2);
        return;
    }

    if (valorNodo) {
        // **DETECTAR SI ES UNA OPERACIÓN**
        if (valorNodo->tipo == AST_OPERACION) {
            es_operacion = true;
            inicializado = true;
            printf("[DEBUG] Detectada operación '%s' en declaración de '%s'\n", valorNodo->valor, nombreVariable);
        }
        // Si es un valor literal normal
        else if (valorNodo->valor) {
            if(valorNodo->tipo == AST_IDENTIFICADOR){
                 inicializado = true;
                es_operacion = true; // Para que se genere el código de asignación en .text
                printf("[DEBUG] Declaración '%s' inicializada con variable '%s'\n", nombreVariable, valorNodo->valor);
            }else{
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
        
        if (es_operacion){
            tipoDeclaracion = "skip 8";

        } else {
            tipoDeclaracion = "asciz";
        }
    }

    if (inicializado && es_operacion) {

        // Generar el código de asignación en ARM64
        if (tipo == TIPO_INT) {
            char asignacion[128];
            snprintf(asignacion, sizeof(asignacion),
                "    ldr x1, =%s\n"
                "    ldr w0, [x1]\n"
                "    ldr x1, =%s\n"
                "    str w0, [x1]\n",
                valorNodo->valor, nombreVariable);
            insertarEnSeccionText(gen, asignacion);
        } else if (tipo == TIPO_STRING) {
            if (es_operacion) {
                snprintf(codigo, sizeof(codigo), "%s: .skip 8\n", nombreVariable);
            } else {
                snprintf(codigo, sizeof(codigo), "%s: .asciz \"%s\"\n", nombreVariable, valorStr);
            }
        } else {
            snprintf(codigo, sizeof(codigo), "%s: .%s 0\n", nombreVariable, tipoDeclaracion);
        }
        insertarEnSeccionData(gen, codigo);
        // **NO GENERAR LA OPERACIÓN AQUÍ - SE HACE DESPUÉS**
        return;
    }

    if (inicializado) {
        if (es_operacion) {
            // **GENERAR DECLARACIÓN CON VALOR 0 EN .data**
            if (tipo == TIPO_STRING) {
                snprintf(codigo, sizeof(codigo), "%s: .skip 8\n", nombreVariable);
            } else {
                snprintf(codigo, sizeof(codigo), "%s: .%s 0\n", nombreVariable, tipoDeclaracion);
            }
            insertarEnSeccionData(gen, codigo);
            //if (gen->archivo) fprintf(gen->archivo, "%s", codigo);

            // **GENERAR CÓDIGO DE OPERACIÓN EN .text**
            generarCodigoOperacionARM64(gen, nombreVariable, valorNodo, tipo);
        }  else {
            // Declaración normal con valor literal
            if (tipo == TIPO_STRING) {
                // **CORREGIR: Asegurar comillas en strings literales**
                if (valorStr[0] != '"') {
                    char valorConComillas[128];
                    snprintf(valorConComillas, sizeof(valorConComillas), "\"%s\"", valorStr);
                    snprintf(codigo, sizeof(codigo), "%s: .%s %s\n", nombreVariable, tipoDeclaracion, valorConComillas);
                } else {
                    snprintf(codigo, sizeof(codigo), "%s: .%s %s\n", nombreVariable, tipoDeclaracion, valorStr);
                }
            } else {
                snprintf(codigo, sizeof(codigo), "%s: .%s %s\n", nombreVariable, tipoDeclaracion, valorStr);
            }
            insertarEnSeccionData(gen, codigo);
        }
    } else {
        // Variable no inicializada va a .bss
        size_t espacio = 4;
        if (tipo == TIPO_DOUBLE) espacio = 8;
        else if (tipo == TIPO_FLOAT) espacio = 4;
        else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) espacio = 1;
        else if (tipo == TIPO_STRING) espacio = 8;
        
        snprintf(codigo, sizeof(codigo), "%s: .space %zu\n", nombreVariable, espacio);
        insertarEnSeccionBss(gen, codigo);
        //if (gen->archivo) fprintf(gen->archivo, "%s", codigo);
    }
}
