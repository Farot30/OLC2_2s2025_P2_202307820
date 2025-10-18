#include "StringBuilder.h"
#include <string.h>
#include <stdlib.h>

static void procesarNodosConcatenacion(NodoAST* nodo, GeneradorDeclaraciones* gen, GeneradorStringBuilder* sb_gen, char* codigoOperacion);
static int contador_global_strings = 0;
static bool sb_secciones_generadas = false;
static bool sb_funciones_generadas = false;

// **FUNCIÓN PARA GENERAR NOMBRE ÚNICO DE STRING**
static char* generarNombreStringUnico() {
    static char nombre[32];
    snprintf(nombre, sizeof(nombre), "str%d", contador_global_strings++);
    return nombre;
}

// **FUNCIÓN PARA VERIFICAR SI UNA VARIABLE YA EXISTE EN .data**
static bool variableExisteEnData(GeneradorStringBuilder* gen, const char* valor) {
    if (!gen->buffer || !valor) return false;
    
    char patron[256];
    // Buscar el patrón ": .asciz valor" en el buffer
    snprintf(patron, sizeof(patron), ".asciz %s", valor);
    return strstr(gen->buffer, patron) != NULL;
}

// **FUNCIÓN PARA OBTENER NOMBRE DE VARIABLE EXISTENTE**
static char* obtenerNombreVariableExistente(GeneradorStringBuilder* gen, const char* valor) {
    if (!gen->buffer || !valor) return NULL;
    
    static char nombre[32];
    char patron[256];
    snprintf(patron, sizeof(patron), ".asciz %s", valor);
    
    char* pos = strstr(gen->buffer, patron);
    if (pos) {
        // Buscar hacia atrás para encontrar el nombre de la variable
        char* inicio = pos;
        while (inicio > gen->buffer && *inicio != '\n') {
            inicio--;
        }
        if (*inicio == '\n') inicio++; // Saltar el \n
        
        // Extraer el nombre hasta el ':'
        char* fin = strchr(inicio, ':');
        if (fin) {
            int len = fin - inicio;
            if (len < 32) {
                strncpy(nombre, inicio, len);
                nombre[len] = '\0';
                return nombre;
            }
        }
    }
    return NULL;
}

static bool esVariableAsciz(GeneradorDeclaraciones* gen, const char* nombreVar) {
    if (!gen->buffer || !nombreVar) return false;
    
    // Buscar la declaración exacta de la variable
    char patron[128];
    snprintf(patron, sizeof(patron), "%s:", nombreVar);
    
    char* pos = strstr(gen->buffer, patron);
    if (pos) {
        // Verificar que sea el inicio de una línea (o después de \n)
        if (pos != gen->buffer && *(pos - 1) != '\n') {
            // Buscar la siguiente ocurrencia
            pos = strstr(pos + 1, patron);
        }
        
        if (pos) {
            // Buscar el tipo de declaración en la misma línea
            char* finLinea = strchr(pos, '\n');
            if (finLinea) {
                // Crear una subcadena de solo esa línea
                size_t longLinea = finLinea - pos;
                char linea[256];
                strncpy(linea, pos, longLinea);
                linea[longLinea] = '\0';
                
                // Verificar si contiene .asciz
                if (strstr(linea, ".asciz")) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

// Función auxiliar para procesar nodos de string (movida fuera)
static void procesarNodoString(NodoAST* nodo, GeneradorStringBuilder* gen, char* codigo) {
    if (!nodo) return;
    
    if (nodo->tipo == AST_OPERACION && strcmp(nodo->valor, "+") == 0) {
        // Procesar recursivamente los hijos
        procesarNodoString(nodo->hijos[0], gen, codigo);
        procesarNodoString(nodo->hijos[1], gen, codigo);
    } else if (nodo->tipo == AST_CADENA || nodo->tipo == AST_IDENTIFICADOR) {
        char buffer[256];
        if (nodo->tipo == AST_CADENA) {
            // Declarar string literal si es necesario
            char nombreStr[32];
            snprintf(nombreStr, sizeof(nombreStr), "str%d", gen->contador_strings++);
            
            // Agregar a .data
            char declaracionStr[128];
            if (nodo->valor[0] == '"' && nodo->valor[strlen(nodo->valor)-1] == '"') {
                // Ya tiene comillas
                snprintf(declaracionStr, sizeof(declaracionStr), 
                        "%s: .asciz %s\n", nombreStr, nodo->valor);
            } else {
                // Agregar comillas
                snprintf(declaracionStr, sizeof(declaracionStr), 
                        "%s: .asciz \"%s\"\n", nombreStr, nodo->valor);
            }
            snprintf(declaracionStr, sizeof(declaracionStr), 
                    "%s: .asciz %s\n", nombreStr, nodo->valor);
            
            // Insertar en .data del buffer principal
            const char* data_marker = ".data\n";
            char* pos = strstr(gen->buffer, data_marker);
            if (pos) {
                pos += strlen(data_marker);
                size_t offset = pos - gen->buffer;
                size_t len_actual = strlen(gen->buffer);
                size_t len_decl = strlen(declaracionStr);
                
                // Redimensionar buffer si es necesario
                if (len_actual + len_decl + 1 >= gen->buffer_size) {
                    gen->buffer_size = len_actual + len_decl + 1024;
                    gen->buffer = realloc(gen->buffer, gen->buffer_size);
                }
                
                memmove(gen->buffer + offset + len_decl, gen->buffer + offset, len_actual - offset + 1);
                memcpy(gen->buffer + offset, declaracionStr, len_decl);
            }
            
            snprintf(buffer, sizeof(buffer),
                    "    adr x0, %s\n"
                    "    bl sb_append\n", nombreStr);
        } else {
            // Variable
            snprintf(buffer, sizeof(buffer),
                    "    adr x0, %s\n"
                    "    bl sb_append\n", nodo->valor);
        }
        strcat(codigo, buffer);
    }
}

static void procesarNodosConcatenacion(NodoAST* nodo, GeneradorDeclaraciones* gen, GeneradorStringBuilder* sb_gen, char* codigoOperacion) {
    if (!nodo) return;

    if (nodo->tipo == AST_OPERACION && strcmp(nodo->valor, "+") == 0) {
        if (nodo->num_hijos >= 2) {
            procesarNodosConcatenacion(nodo->hijos[0], gen, sb_gen, codigoOperacion);
            procesarNodosConcatenacion(nodo->hijos[1], gen, sb_gen, codigoOperacion);
        }
    } else if (nodo->tipo == AST_CADENA || nodo->tipo == AST_IDENTIFICADOR) {
        char buffer[256];
        if (nodo->tipo == AST_CADENA) {
            char* nombreStr = NULL;
            
            if (variableExisteEnData(sb_gen, nodo->valor)) {
                nombreStr = obtenerNombreVariableExistente(sb_gen, nodo->valor);
                printf("[DEBUG] Reutilizando string existente: %s para valor %s\n", nombreStr, nodo->valor);
            } else {
                nombreStr = generarNombreStringUnico();
                printf("[DEBUG] Creando nuevo string: %s para valor %s\n", nombreStr, nodo->valor);
                
                char declaracionStr[128];
                if (nodo->valor[0] == '"' && nodo->valor[strlen(nodo->valor) - 1] == '"') {
                    snprintf(declaracionStr, sizeof(declaracionStr), "%s: .asciz %s\n", nombreStr, nodo->valor);
                } else {
                    snprintf(declaracionStr, sizeof(declaracionStr), "%s: .asciz \"%s\"\n", nombreStr, nodo->valor);
                }

                const char* data_marker = ".data\n";
                char* pos = strstr(gen->buffer, data_marker);
                if (pos) {
                    pos += strlen(data_marker);
                    size_t offset = pos - gen->buffer;
                    size_t len_actual = strlen(gen->buffer);
                    size_t len_decl = strlen(declaracionStr);

                    if (len_actual + len_decl + 1 >= gen->buffer_size) {
                        gen->buffer_size = len_actual + len_decl + 1024;
                        gen->buffer = realloc(gen->buffer, gen->buffer_size);
                    }

                    memmove(gen->buffer + offset + len_decl, gen->buffer + offset, len_actual - offset + 1);
                    memcpy(gen->buffer + offset, declaracionStr, len_decl);
                }
            }

            snprintf(buffer, sizeof(buffer), 
            "    ldr x0, =%s\n"          // CAMBIADO: era adrp/add
            "    bl sb_append\n", nombreStr);
        } else {
            // **USAR LA NUEVA FUNCIÓN MEJORADA**
            bool esAsciz = esVariableAsciz(gen, nodo->valor);
            
            printf("[DEBUG] Variable '%s' detectada como: %s\n", nodo->valor, esAsciz ? ".asciz" : ".skip");
            printf("[DEBUG] Buffer contiene para '%s':\n", nodo->valor);
            char* debug_pos = strstr(gen->buffer, nodo->valor);
            if (debug_pos) {
                // Mostrar contexto
                char contexto[200];
                strncpy(contexto, debug_pos, 199);
                contexto[199] = '\0';
                printf("[DEBUG] Contexto: %s\n", contexto);
            }
            
            if (esAsciz) {
                // Es string literal (.asciz)
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =%s\n"
                    "    bl sb_append\n", nodo->valor);
            } else {
                // Es puntero (.skip 8)
                snprintf(buffer, sizeof(buffer),
                    "    ldr x0, =%s\n"
                    "    ldr x0, [x0]\n"
                    "    bl sb_append\n", nodo->valor, nodo->valor);
            }
        }
        strcat(codigoOperacion, buffer);
    }
}

void generarConcatenacionStringBuilder(GeneradorDeclaraciones* gen, const char* nombreVar, NodoAST* operacionNodo) {
    if (!gen || !nombreVar || !operacionNodo) return;
    
    printf("[DEBUG] Generando concatenación StringBuilder para variable '%s'\n", nombreVar);
    
    // Crear un generador de StringBuilder
    GeneradorStringBuilder sb_gen;
    inicializarGeneradorStringBuilder(&sb_gen, gen->archivo, gen->buffer, gen->buffer_size);

    // Generar infraestructura del StringBuilder
    generarInfraestructuraStringBuilder(&sb_gen);

    char codigoOperacion[2048] = "";
    char buffer[256];

    // Comentario inicial
    snprintf(buffer, sizeof(buffer), "    // Operación: %s = <expresión>\n", nombreVar);
    strcat(codigoOperacion, buffer);

    // Código para inicializar el StringBuilder
    strcat(codigoOperacion, "    // Concatenación de strings usando StringBuilder\n");
    strcat(codigoOperacion, "    mov x0, #64\n");  // Capacidad inicial
    strcat(codigoOperacion, "    bl sb_init\n");

    // **FUNCIÓN AUXILIAR MEJORADA PARA PROCESAR NODOS**
    procesarNodosConcatenacion(operacionNodo, gen, &sb_gen, codigoOperacion);

    // **OBTENER EL RESULTADO ANTES DE LIBERAR**
    strcat(codigoOperacion, "\n    // Obtener resultado final\n");
    strcat(codigoOperacion, "    bl sb_get_string\n");
    strcat(codigoOperacion, "    mov x19, x0                 // x19 tiene el string concatenado\n");

    // **RESERVAR MEMORIA PERMANENTE PARA LA COPIA**
    strcat(codigoOperacion, "\n    // Calcular longitud del string\n");
    strcat(codigoOperacion, "    mov x0, x19\n");
    strcat(codigoOperacion, "    bl strlen\n");
    strcat(codigoOperacion, "    add x0, x0, #1              // +1 para null terminator\n");
    strcat(codigoOperacion, "    mov x20, x0                 // x20 = longitud\n");
    
    // **RESERVAR MEMORIA PERMANENTE**
    strcat(codigoOperacion, "\n    // Reservar memoria permanente\n");
    strcat(codigoOperacion, "    bl sb_malloc\n");
    strcat(codigoOperacion, "    mov x21, x0                 // x21 = nueva memoria\n");
    
    // **COPIAR EL STRING A LA NUEVA MEMORIA**
    strcat(codigoOperacion, "\n    // Copiar string a memoria permanente\n");
    strcat(codigoOperacion, "    mov x0, x21                 // destino\n");
    strcat(codigoOperacion, "    mov x1, x19                 // origen\n");
    strcat(codigoOperacion, "    bl strcpy\n");

    // **AHORA SÍ LIBERAR EL STRINGBUILDER**
    strcat(codigoOperacion, "\n    // Limpiar StringBuilder\n");
    strcat(codigoOperacion, "    bl sb_free\n");
    
    // **GUARDAR EL PUNTERO PERMANENTE EN LA VARIABLE**
    strcat(codigoOperacion, "\n    // Guardar puntero permanente\n");
    snprintf(buffer, sizeof(buffer),
        "    ldr x1, =%s\n"
        "    str x21, [x1]               // Guardar puntero permanente\n", nombreVar);
    strcat(codigoOperacion, buffer);

    // Insertar el código generado en la sección .text
    insertarEnSeccionText(gen, codigoOperacion);
}

void inicializarGeneradorStringBuilder(GeneradorStringBuilder* gen, FILE* archivo, 
                                     char* buffer, size_t buffer_size) {
    gen->archivo = archivo;
    gen->buffer = buffer;
    gen->buffer_size = buffer_size;
    gen->seccion_data_creada = false;
    gen->seccion_bss_creada = false;
    gen->seccion_text_creada = false;
    gen->funciones_generadas = false;
    gen->contador_strings = contador_global_strings; // Usar contador global
}

void insertarEnSeccionTextStringBuilder(GeneradorStringBuilder* gen, const char* codigo) {
    if (!gen->buffer || !codigo) return;

    const char* start_marker = "_start:\n";
    char* pos = strstr(gen->buffer, start_marker);
    if (pos) {
        // Buscar el final de la línea _start y el comentario siguiente
        pos += strlen(start_marker);
        
        // Buscar el primer comentario después de _start
        char* comentario = strstr(pos, "    //");
        if (comentario) {
            // Encontrar el final de la línea del comentario
            char* fin_comentario = strchr(comentario, '\n');
            if (fin_comentario) {
                pos = fin_comentario + 1; // Posición después del comentario
            }
        }
        
        size_t offset = pos - gen->buffer;
        size_t len_actual = strlen(gen->buffer);
        size_t len_codigo = strlen(codigo);

        // Redimensionar buffer si es necesario
        if (len_actual + len_codigo + 1 >= gen->buffer_size) {
            gen->buffer_size = len_actual + len_codigo + 1024;
            gen->buffer = realloc(gen->buffer, gen->buffer_size);
            if (!gen->buffer) return;
            
            // Recalcular pos después del realloc
            pos = gen->buffer + offset;
        }

        // Mover el contenido para hacer espacio
        memmove(pos + len_codigo, pos, len_actual - offset + 1);
        memcpy(pos, codigo, len_codigo);
    } else {
        // Si no encuentra _start, agregar al final
        if (strlen(gen->buffer) + strlen(codigo) + 1 < gen->buffer_size) {
            strcat(gen->buffer, codigo);
        }
    }
}

void generarSeccionesStringBuilder(GeneradorStringBuilder* gen) {
    // **SOLO GENERAR UNA VEZ USANDO BANDERA GLOBAL**
    if (sb_secciones_generadas) return;
    
    const char* secciones_sb = 
        "    .align 3\n"
        "    sb_buffer:      .skip 8     // Puntero al buffer\n"
        "    sb_capacity:    .skip 8     // Capacidad total\n"
        "    sb_length:      .skip 8     // Longitud actual\n\n";
    
    // Insertar en la sección .bss del buffer principal
    const char* bss_marker = ".bss\n";
    char* pos = strstr(gen->buffer, bss_marker);
    if (pos) {
        pos += strlen(bss_marker);
        size_t offset = pos - gen->buffer;
        size_t len_actual = strlen(gen->buffer);
        size_t len_codigo = strlen(secciones_sb);
        
        // Redimensionar buffer si es necesario
        if (len_actual + len_codigo + 1 >= gen->buffer_size) {
            gen->buffer_size = len_actual + len_codigo + 1024;
            gen->buffer = realloc(gen->buffer, gen->buffer_size);
        }
        
        // Insertar el código
        memmove(gen->buffer + offset + len_codigo, gen->buffer + offset, len_actual - offset + 1);
        memcpy(gen->buffer + offset, secciones_sb, len_codigo);
    }
    
    sb_secciones_generadas = true; // **MARCAR COMO GENERADO**
    gen->seccion_bss_creada = true;
}

void generarFuncionesAuxiliares(GeneradorStringBuilder* gen) {
    if (sb_funciones_generadas) return;
    
    const char* funciones_auxiliares = 
        "\n// ============================================\n"
        "// FUNCIONES STRINGBUILDER\n"
        "// ============================================\n\n"
        "// Inicializar StringBuilder\n"
        "sb_init:\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x19, x0                 // guardar capacidad\n"
        "    \n"
        "    // Reservar memoria\n"
        "    bl sb_malloc\n"
        "    ldr x1, =sb_buffer\n"         // CAMBIADO: era adr
        "    str x0, [x1]\n"
        "    \n"
        "    // Guardar capacidad\n"
        "    ldr x1, =sb_capacity\n"       // CAMBIADO: era adr
        "    str x19, [x1]\n"
        "    \n"
        "    // Longitud inicial = 0\n"
        "    ldr x1, =sb_length\n"         // CAMBIADO: era adr
        "    str xzr, [x1]\n"
        "    \n"
        "    // Poner null terminator\n"
        "    ldr x1, =sb_buffer\n"         // CAMBIADO: era adr
        "    ldr x1, [x1]\n"
        "    strb wzr, [x1]\n"
        "    \n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n\n"
        "// Agregar string al StringBuilder\n"
        "sb_append:\n"
        "    stp x29, x30, [sp, #-32]!\n"
        "    str x19, [sp, #16]\n"
        "    str x20, [sp, #24]\n"
        "    \n"
        "    mov x19, x0                 // guardar string a agregar\n"
        "    \n"
        "    // Calcular longitud del string\n"
        "    bl strlen\n"
        "    mov x20, x0                 // x20 = longitud a agregar\n"
        "    \n"
        "    // Verificar si necesitamos expandir\n"
        "    ldr x0, =sb_length\n"         // CAMBIADO: era adr
        "    ldr x0, [x0]\n"
        "    add x0, x0, x20             // longitud_total = actual + nueva\n"
        "    add x0, x0, #1              // +1 para null\n"
        "    \n"
        "    ldr x1, =sb_capacity\n"       // CAMBIADO: era adr
        "    ldr x1, [x1]\n"
        "    cmp x0, x1\n"
        "    ble sb_append_copy          // si cabe, copiar\n"
        "    \n"
        "    // Necesitamos expandir\n"
        "    lsl x0, x0, #1              // duplicar tamaño\n"
        "    bl sb_resize\n"
        "    \n"
        "sb_append_copy:\n"
        "    // Copiar string al final\n"
        "    ldr x0, =sb_buffer\n"         // CAMBIADO: era adr
        "    ldr x0, [x0]\n"
        "    ldr x1, =sb_length\n"         // CAMBIADO: era adr
        "    ldr x1, [x1]\n"
        "    add x0, x0, x1              // posición final\n"
        "    \n"
        "    mov x1, x19                 // string a copiar\n"
        "    bl strcpy\n"
        "    \n"
        "    // Actualizar longitud\n"
        "    ldr x0, =sb_length\n"         // CAMBIADO: era adr
        "    ldr x1, [x0]\n"
        "    add x1, x1, x20\n"
        "    str x1, [x0]\n"
        "    \n"
        "    ldr x20, [sp, #24]\n"
        "    ldr x19, [sp, #16]\n"
        "    ldp x29, x30, [sp], #32\n"
        "    ret\n\n"
        "// Redimensionar buffer\n"
        "sb_resize:\n"
        "    stp x29, x30, [sp, #-32]!\n"
        "    str x19, [sp, #16]\n"
        "    str x20, [sp, #24]\n"
        "    \n"
        "    mov x19, x0                 // nueva capacidad\n"
        "    \n"
        "    // Reservar nuevo buffer\n"
        "    mov x0, x19\n"
        "    bl sb_malloc\n"
        "    mov x20, x0                 // nuevo buffer\n"
        "    \n"
        "    // Copiar contenido antiguo\n"
        "    mov x0, x20\n"
        "    ldr x1, =sb_buffer\n"         // CAMBIADO: era adr
        "    ldr x1, [x1]\n"
        "    bl strcpy\n"
        "    \n"
        "    // Liberar buffer antiguo\n"
        "    ldr x0, =sb_buffer\n"         // CAMBIADO: era adr
        "    ldr x0, [x0]\n"
        "    ldr x1, =sb_capacity\n"       // CAMBIADO: era adr
        "    ldr x1, [x1]\n"
        "    bl sb_free_buffer\n"
        "    \n"
        "    // Actualizar puntero y capacidad\n"
        "    ldr x0, =sb_buffer\n"         // CAMBIADO: era adr
        "    str x20, [x0]\n"
        "    ldr x0, =sb_capacity\n"       // CAMBIADO: era adr
        "    str x19, [x0]\n"
        "    \n"
        "    ldr x20, [sp, #24]\n"
        "    ldr x19, [sp, #16]\n"
        "    ldp x29, x30, [sp], #32\n"
        "    ret\n\n"
        "// Obtener string final\n"
        "sb_get_string:\n"
        "    ldr x0, =sb_buffer\n"         // CAMBIADO: era adr
        "    ldr x0, [x0]\n"
        "    ret\n\n"
        "// Liberar StringBuilder\n"
        "sb_free:\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    \n"
        "    ldr x0, =sb_buffer\n"         // CAMBIADO: era adr
        "    ldr x0, [x0]\n"
        "    ldr x1, =sb_capacity\n"       // CAMBIADO: era adr
        "    ldr x1, [x1]\n"
        "    bl sb_free_buffer\n"
        "    \n"
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n\n"
        "// malloc usando mmap\n"
        "sb_malloc:\n"
        "    mov x8, #222\n"
        "    mov x1, x0\n"
        "    mov x0, #0\n"
        "    mov x2, #3\n"
        "    mov x3, #34\n"
        "    mov x4, #-1\n"
        "    mov x5, #0\n"
        "    svc #0\n"
        "    ret\n\n"
        "// free usando munmap\n"
        "sb_free_buffer:\n"
        "    mov x8, #215\n"
        "    svc #0\n"
        "    ret\n\n"
        "// strlen - loop iterativo\n"
        "strlen:\n"
        "    mov x2, #0\n"
        "strlen_loop:\n"
        "    ldrb w3, [x0, x2]\n"
        "    cbz w3, strlen_end\n"
        "    add x2, x2, #1\n"
        "    b strlen_loop\n"
        "strlen_end:\n"
        "    mov x0, x2\n"
        "    ret\n\n"
        "// strcpy - loop iterativo\n"
        "strcpy:\n"
        "    mov x2, #0\n"
        "strcpy_loop:\n"
        "    ldrb w3, [x1, x2]\n"
        "    strb w3, [x0, x2]\n"
        "    cbz w3, strcpy_end\n"
        "    add x2, x2, #1\n"
        "    b strcpy_loop\n"
        "strcpy_end:\n"
        "    ret\n\n";
    
    // Agregar al final del buffer
    if (strlen(gen->buffer) + strlen(funciones_auxiliares) + 1 < gen->buffer_size) {
        strcat(gen->buffer, funciones_auxiliares);
    }
    
    sb_funciones_generadas = true;
    gen->funciones_generadas = true;
}

void generarInfraestructuraStringBuilder(GeneradorStringBuilder* gen) {
    generarSeccionesStringBuilder(gen);
    generarFuncionesAuxiliares(gen);
}
void resetearStringBuilder() {
    contador_global_strings = 0;
    sb_secciones_generadas = false;
    sb_funciones_generadas = false;
}

void generarConcatenacionDesdeOperacion(GeneradorStringBuilder* gen, NodoAST* operacionNodo) {
    if (!gen || !operacionNodo || operacionNodo->tipo != AST_OPERACION) return;
    
    // Generar infraestructura si no existe
    generarInfraestructuraStringBuilder(gen);
    
    char codigo[1024];
    snprintf(codigo, sizeof(codigo),
        "    // Concatenación de strings usando StringBuilder\n"
        "    mov x0, #64                 // Capacidad inicial\n"
        "    bl sb_init\n\n");
    
    // Procesar la operación usando la función auxiliar
    procesarNodoString(operacionNodo, gen, codigo);
    
    // Finalizar StringBuilder
    strcat(codigo,
        "\n    // Obtener resultado final\n"
        "    bl sb_get_string\n"
        "    mov x19, x0                 // x19 tiene el string concatenado\n"
        "    \n"
        "    // Limpiar StringBuilder\n"
        "    bl sb_free\n\n");
    
    // Insertar en la sección .text usando la nueva función
    insertarEnSeccionTextStringBuilder(gen, codigo);
}

void generarConcatenacionStrings(GeneradorStringBuilder* gen, NodoAST** strings, int numStrings) {
    if (!gen || !strings || numStrings <= 0) return;
    
    generarInfraestructuraStringBuilder(gen);
    
    char codigo[1024];
    snprintf(codigo, sizeof(codigo),
        "    // Concatenación de %d strings\n"
        "    mov x0, #64                 // Capacidad inicial\n"
        "    bl sb_init\n\n", numStrings);
    
    // Agregar cada string
    for (int i = 0; i < numStrings; i++) {
        char buffer[256];
        if (strings[i]->tipo == AST_CADENA) {
            char nombreStr[32];
            snprintf(nombreStr, sizeof(nombreStr), "str%d", gen->contador_strings++);
            
            // Declarar string en .data
            char declaracionStr[128];
            snprintf(declaracionStr, sizeof(declaracionStr), 
                    "%s: .asciz %s\n", nombreStr, strings[i]->valor);
            
            // Insertar en .data del buffer principal
            const char* data_marker = ".data\n";
            char* pos = strstr(gen->buffer, data_marker);
            if (pos) {
                pos += strlen(data_marker);
                size_t offset = pos - gen->buffer;
                size_t len_actual = strlen(gen->buffer);
                size_t len_decl = strlen(declaracionStr);
                
                memmove(gen->buffer + offset + len_decl, gen->buffer + offset, len_actual - offset + 1);
                memcpy(gen->buffer + offset, declaracionStr, len_decl);
            }
            
            snprintf(buffer, sizeof(buffer),
                    "    adr x0, %s\n"
                    "    bl sb_append\n", nombreStr);
        } else {
            snprintf(buffer, sizeof(buffer),
                    "    adr x0, %s\n"
                    "    bl sb_append\n", strings[i]->valor);
        }
        strcat(codigo, buffer);
    }
    
    strcat(codigo,
        "\n    // Obtener resultado final\n"
        "    bl sb_get_string\n"
        "    mov x19, x0                 // x19 tiene el string concatenado\n\n");
    
    // Insertar en .text usando la nueva función
    insertarEnSeccionTextStringBuilder(gen, codigo);
}