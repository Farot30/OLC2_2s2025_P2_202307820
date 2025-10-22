#include "declaraciones_arm64.h"
#include "declaracionConst_arm64.h"
#include "Operadores_arm64.h"
#include "Evaluadores_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
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
    gen->temp_str_valueof_bss_insertado = false;
    gen->contador_literales_string = 0;
}
extern NodoAST* astGlobal; // Declaración externa del nodo raíz del AST

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

        // **VERIFICAR SI LA POSICIÓN ANTERIOR ES UNA ETIQUETA SIN SALTO DE LÍNEA**
        if (offset > 0 && *(pos - 1) != '\n') {
            // Insertar salto de línea antes del código
            char codigoConSalto[len_codigo + 2];
            snprintf(codigoConSalto, sizeof(codigoConSalto), "\n%s", codigo);
            size_t lenNueva = strlen(codigoConSalto);
            
            if (len_actual + lenNueva + 1 >= gen->buffer_size) {
                gen->buffer_size = len_actual + lenNueva + 1024;
                gen->buffer = realloc(gen->buffer, gen->buffer_size);
                pos = gen->buffer + offset;
            }
            
            memmove(pos + lenNueva, pos, len_actual - offset + 1);
            memcpy(pos, codigoConSalto, lenNueva);
        } else {
            // Mover el contenido para hacer espacio
            memmove(pos + len_codigo, pos, len_actual - offset + 1);
            memcpy(pos, codigo, len_codigo);
        }
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

    char codigo[512];
    const char* tipoDeclaracion = "word";
    if (tipo == TIPO_DOUBLE) tipoDeclaracion = "double";
    else if (tipo == TIPO_FLOAT) tipoDeclaracion = "float";
    else if (tipo == TIPO_CHAR) tipoDeclaracion = "byte";
    else if (tipo == TIPO_BOOLEAN) tipoDeclaracion = "byte";
    else if (tipo == TIPO_STRING) tipoDeclaracion = "skip 8";

    if (valorNodo && valorNodo->tipo == AST_IDENTIFICADOR) {
        TipoDato tipoOrigen = deducirTipoExprARM64(valorNodo, astGlobal);
        
        // **Generar declaración sin valor inicial**
        if (tipo == TIPO_STRING) {
            snprintf(codigo, sizeof(codigo),
                "%s: .quad str_lit_%s\n"
                "str_lit_%s: .asciz \"\"\n",
                nombreVariable, nombreVariable, nombreVariable);
        } else if (tipo == TIPO_FLOAT) {
            snprintf(codigo, sizeof(codigo), "%s: .float 0.0\n", nombreVariable);
        } else if (tipo == TIPO_DOUBLE) {
            snprintf(codigo, sizeof(codigo), "%s: .double 0.0\n", nombreVariable);
        } else if (tipo == TIPO_CHAR) {
            snprintf(codigo, sizeof(codigo), "%s: .byte 0\n", nombreVariable);
        } else {
            snprintf(codigo, sizeof(codigo), "%s: .word 0\n", nombreVariable);
        }
        insertarEnSeccionData(gen, codigo);
        
        // **Generar código ARM64 para cargar y convertir el valor**
        char codigoCarga[512] = "";
        
        if (esWidening(tipoOrigen, tipo)) {
            // **Widening Casting: char -> int -> float -> double**
            if (tipoOrigen == TIPO_CHAR && tipo == TIPO_INT) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    // Widening: char -> int\n"
                    "    ldr x1, =%s\n"
                    "    ldrb w0, [x1]\n"
                    "    ldr x1, =%s\n"
                    "    str w0, [x1]\n",
                    valorNodo->valor, nombreVariable);
            } else if (tipoOrigen == TIPO_INT && tipo == TIPO_FLOAT) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    // Widening: int -> float\n"
                    "    ldr x1, =%s\n"
                    "    ldr w0, [x1]\n"
                    "    scvtf s0, w0\n"
                    "    ldr x1, =%s\n"
                    "    str s0, [x1]\n",
                    valorNodo->valor, nombreVariable);
            } else if (tipoOrigen == TIPO_FLOAT && tipo == TIPO_DOUBLE) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    // Widening: float -> double\n"
                    "    ldr x1, =%s\n"
                    "    ldr s0, [x1]\n"
                    "    fcvt d0, s0\n"
                    "    ldr x1, =%s\n"
                    "    str d0, [x1]\n",
                    valorNodo->valor, nombreVariable);
            } else if (tipoOrigen == TIPO_CHAR && tipo == TIPO_FLOAT) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    // Widening: char -> float\n"
                    "    ldr x1, =%s\n"
                    "    ldrb w0, [x1]\n"
                    "    scvtf s0, w0\n"
                    "    ldr x1, =%s\n"
                    "    str s0, [x1]\n",
                    valorNodo->valor, nombreVariable);
            } else if (tipoOrigen == TIPO_INT && tipo == TIPO_DOUBLE) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    // Widening: int -> double\n"
                    "    ldr x1, =%s\n"
                    "    ldr w0, [x1]\n"
                    "    scvtf d0, w0\n"
                    "    ldr x1, =%s\n"
                    "    str d0, [x1]\n",
                    valorNodo->valor, nombreVariable);
            } else if (tipoOrigen == TIPO_CHAR && tipo == TIPO_DOUBLE) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    // Widening: char -> double\n"
                    "    ldr x1, =%s\n"
                    "    ldrb w0, [x1]\n"
                    "    scvtf d0, w0\n"
                    "    ldr x1, =%s\n"
                    "    str d0, [x1]\n",
                    valorNodo->valor, nombreVariable);
            }
        }else if (tipoOrigen == tipo) {
            // **Mismo tipo: solo copiar**
            if (tipo == TIPO_CHAR) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    ldr x1, =%s\n"
                    "    ldrb w0, [x1]\n"
                    "    ldr x1, =%s\n"
                    "    strb w0, [x1]\n",
                    valorNodo->valor, nombreVariable);
            } else if (tipo == TIPO_FLOAT) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    ldr x1, =%s\n"
                    "    ldr s0, [x1]\n"
                    "    ldr x1, =%s\n"
                    "    str s0, [x1]\n",
                    valorNodo->valor, nombreVariable);
            } else if (tipo == TIPO_DOUBLE) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    ldr x1, =%s\n"
                    "    ldr d0, [x1]\n"
                    "    ldr x1, =%s\n"
                    "    str d0, [x1]\n",
                    valorNodo->valor, nombreVariable);
            } else {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    ldr x1, =%s\n"
                    "    ldr w0, [x1]\n"
                    "    ldr x1, =%s\n"
                    "    str w0, [x1]\n",
                    valorNodo->valor, nombreVariable);
            }
        }
        
        insertarEnSeccionText(gen, codigoCarga);
        return;
    }

    // --- INTEGRACIÓN DE EVALUADOR RECURSIVO ---
    if (valorNodo && (valorNodo->tipo == AST_OPERACION || valorNodo->tipo == AST_VALUEOF)) {
        // 1. Declaración en .data
        if (tipo == TIPO_DOUBLE) {
            snprintf(codigo, sizeof(codigo), "%s: .double 0.0\n", nombreVariable);
        } else if (tipo == TIPO_FLOAT) {
            snprintf(codigo, sizeof(codigo), "%s: .float 0.0\n", nombreVariable);
        } else if (tipo == TIPO_INT) {
            snprintf(codigo, sizeof(codigo), "%s: .word 0\n", nombreVariable);
        } else {
            snprintf(codigo, sizeof(codigo), "%s: .%s 0\n", nombreVariable, tipoDeclaracion);
        }
        insertarEnSeccionData(gen, codigo);

        // 2. Evaluar la expresión recursivamente
        int regIndex = 1;
        ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, valorNodo, &regIndex);

        // 3. Insertar el código generado por la expresión
        insertarEnSeccionText(gen, resultado.codigo);

        // 4. Asignar el resultado a la variable
        if (tipo == TIPO_STRING) {
            snprintf(codigo, sizeof(codigo),
                "    ldr x1, =%s\n"
                "    str %s, [x1]\n", nombreVariable, resultado.resultado);
        } else if (tipo == TIPO_INT || tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) {
            snprintf(codigo, sizeof(codigo),
                "    ldr x1, =%s\n"
                "    str %s, [x1]\n", nombreVariable, resultado.resultado);
        } else if (tipo == TIPO_DOUBLE || tipo == TIPO_FLOAT) {
            snprintf(codigo, sizeof(codigo),
                "    ldr x1, =%s\n"
                "    str d0, [x1]\n", nombreVariable); // Ajusta si usas otro registro para double/float
        }
        insertarEnSeccionText(gen, codigo);
        return;
    }

    

    // --- CASO: LITERALES O VARIABLES ---
    char valorStr[64] = "0";
    bool inicializado = false;

    if (valorNodo && valorNodo->tipo == AST_CAST) {
         printf("[DEBUG DECLARACION] Narrowing para %s\n", nombreVariable);
        // 1. Declaración en .data
        if (tipo == TIPO_FLOAT) {
            snprintf(codigo, sizeof(codigo), "%s: .float 0.0\n", nombreVariable);
            insertarEnSeccionData(gen, codigo);
        } else if (tipo == TIPO_DOUBLE) {
            snprintf(codigo, sizeof(codigo), "%s: .double 0.0\n", nombreVariable);
            insertarEnSeccionData(gen, codigo);
        } else if (tipo == TIPO_INT) {
            snprintf(codigo, sizeof(codigo), "%s: .word 0\n", nombreVariable);
            insertarEnSeccionData(gen, codigo);
        } else if (tipo == TIPO_CHAR) {
            snprintf(codigo, sizeof(codigo), "%s: .byte 0\n", nombreVariable);
            insertarEnSeccionData(gen, codigo);
        }

        // 2. Generar Narrowing ARM64
        const char* nombreOrigen = NULL;
        if (valorNodo->num_hijos >= 1) {
            NodoAST* nodoOrigen = valorNodo->hijos[0];
            if (nodoOrigen && nodoOrigen->tipo == AST_IDENTIFICADOR) {
                nombreOrigen = nodoOrigen->valor;
                printf("[DEBUG DECLARACION] Narrowing para %s viendo en %s\n", nombreVariable, nombreOrigen);
            }
        }
        char codigoCarga[512] = "";
        if (nombreOrigen) {
            printf("[DEBUG DECLARACION] Narrowing para %s asignando en el .text\n", nombreVariable);
            if (tipo == TIPO_FLOAT) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    // Narrowing: double -> float\n"
                    "    ldr x1, =%s\n"
                    "    ldr d0, [x1]\n"
                    "    fcvt s0, d0\n"
                    "    ldr x1, =%s\n"
                    "    str s0, [x1]\n",
                    nombreOrigen, nombreVariable);
            } else if (tipo == TIPO_INT) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    // Narrowing: float -> int\n"
                    "    ldr x1, =%s\n"
                    "    ldr s0, [x1]\n"
                    "    fcvtzs w0, s0\n"
                    "    ldr x1, =%s\n"
                    "    str w0, [x1]\n",
                    nombreOrigen, nombreVariable);
            } else if (tipo == TIPO_CHAR) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    // Narrowing: int -> char\n"
                    "    ldr x1, =%s\n"
                    "    ldr w0, [x1]\n"
                    "    ldr x1, =%s\n"
                    "    strb w0, [x1]\n",
                    nombreOrigen, nombreVariable);
            } else if (tipo == TIPO_INT && valorNodo->hijos[0]->tipo == TIPO_DOUBLE) {
                snprintf(codigoCarga, sizeof(codigoCarga),
                    "    // Narrowing: double -> int\n"
                    "    ldr x1, =%s\n"
                    "    ldr d0, [x1]\n"
                    "    fcvtzs w0, d0\n"
                    "    ldr x1, =%s\n"
                    "    str w0, [x1]\n",
                    nombreOrigen, nombreVariable);
            }
            printf("[DEBUG ARM64] Insertando Narrowing: %s\n", codigoCarga);
            insertarEnSeccionText(gen, codigoCarga);
        }
        return;
    }

    if (valorNodo && valorNodo->valor) {
        inicializado = true;
        if (tipo == TIPO_BOOLEAN) {
            if (strcmp(valorNodo->valor, "true") == 0) strncpy(valorStr, "1", sizeof(valorStr));
            else if (strcmp(valorNodo->valor, "false") == 0) strncpy(valorStr, "0", sizeof(valorStr));
        } else if (tipo == TIPO_STRING) {
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

    if (inicializado) {
        if (tipo == TIPO_STRING) {
            char nombreLiteral[64];
            snprintf(nombreLiteral, sizeof(nombreLiteral), "str_lit_%s", nombreVariable);
            char patron[128];
            snprintf(patron, sizeof(patron), "%s:", nombreLiteral);
            // Inserta el literal solo si no existe
            if (!strstr(gen->buffer, patron)) {
                snprintf(codigo, sizeof(codigo), "%s: .asciz %s\n", nombreLiteral, valorStr);
                insertarEnSeccionData(gen, codigo);
            }
            // Inserta la variable puntero solo una vez
            snprintf(codigo, sizeof(codigo), "%s: .quad %s\n", nombreVariable, nombreLiteral);
            insertarEnSeccionData(gen, codigo);
            return; // <-- IMPORTANTE: evita que siga insertando más abajo
        } else if(tipo == TIPO_FLOAT){
            // Quitar la 'f' si viene en el literal (opcional)
            char valorFloat[32];
            strncpy(valorFloat, valorStr, sizeof(valorFloat));
            valorFloat[sizeof(valorFloat)-1] = '\0';
            size_t len = strlen(valorFloat);
            if (valorFloat[len-1] == 'f' || valorFloat[len-1] == 'F') {
                valorFloat[len-1] = '\0';
            }
            snprintf(codigo, sizeof(codigo), "%s: .float %s\n", nombreVariable, valorFloat);
            insertarEnSeccionData(gen, codigo);
        } else {
            snprintf(codigo, sizeof(codigo), "%s: .%s %s\n", nombreVariable, tipoDeclaracion, valorStr);
            insertarEnSeccionData(gen, codigo);
        }
    } else {
        if (tipo == TIPO_STRING) {
            // CORRECCIÓN: inicializa como puntero a cadena vacía
            snprintf(codigo, sizeof(codigo),
                "%s: .quad str_lit_%s\n"
                "str_lit_%s: .asciz \"\"\n",
                nombreVariable, nombreVariable, nombreVariable);
            insertarEnSeccionData(gen, codigo);
        } else {
            // Variable no inicializada va a .bss
            size_t espacio = 4;
            if (tipo == TIPO_DOUBLE) espacio = 8;
            else if (tipo == TIPO_FLOAT) espacio = 4;
            else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) espacio = 1;
            snprintf(codigo, sizeof(codigo), "%s: .space %zu\n", nombreVariable, espacio);
            insertarEnSeccionBss(gen, codigo);
        }
    }
}