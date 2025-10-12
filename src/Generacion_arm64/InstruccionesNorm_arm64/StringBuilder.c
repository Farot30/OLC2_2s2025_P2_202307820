#include "StringBuilder.h"
#include <string.h>
#include <stdlib.h>

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

void inicializarGeneradorStringBuilder(GeneradorStringBuilder* gen, FILE* archivo, 
                                     char* buffer, size_t buffer_size) {
    gen->archivo = archivo;
    gen->buffer = buffer;
    gen->buffer_size = buffer_size;
    gen->seccion_data_creada = false;
    gen->seccion_bss_creada = false;
    gen->seccion_text_creada = false;
    gen->funciones_generadas = false;
    gen->contador_strings = 0;
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
    if (gen->seccion_bss_creada) return;
    
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
    
    gen->seccion_bss_creada = true;
}

void generarFuncionesAuxiliares(GeneradorStringBuilder* gen) {
    if (gen->funciones_generadas) return;
    
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
        "    adr x1, sb_buffer\n"
        "    str x0, [x1]\n"
        "    \n"
        "    // Guardar capacidad\n"
        "    adr x1, sb_capacity\n"
        "    str x19, [x1]\n"
        "    \n"
        "    // Longitud inicial = 0\n"
        "    adr x1, sb_length\n"
        "    str xzr, [x1]\n"
        "    \n"
        "    // Poner null terminator\n"
        "    adr x1, sb_buffer\n"
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
        "    adr x0, sb_length\n"
        "    ldr x0, [x0]\n"
        "    add x0, x0, x20             // longitud_total = actual + nueva\n"
        "    add x0, x0, #1              // +1 para null\n"
        "    \n"
        "    adr x1, sb_capacity\n"
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
        "    adr x0, sb_buffer\n"
        "    ldr x0, [x0]\n"
        "    adr x1, sb_length\n"
        "    ldr x1, [x1]\n"
        "    add x0, x0, x1              // posición final\n"
        "    \n"
        "    mov x1, x19                 // string a copiar\n"
        "    bl strcpy\n"
        "    \n"
        "    // Actualizar longitud\n"
        "    adr x0, sb_length\n"
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
        "    adr x1, sb_buffer\n"
        "    ldr x1, [x1]\n"
        "    bl strcpy\n"
        "    \n"
        "    // Liberar buffer antiguo\n"
        "    adr x0, sb_buffer\n"
        "    ldr x0, [x0]\n"
        "    adr x1, sb_capacity\n"
        "    ldr x1, [x1]\n"
        "    bl sb_free_buffer\n"
        "    \n"
        "    // Actualizar puntero y capacidad\n"
        "    adr x0, sb_buffer\n"
        "    str x20, [x0]\n"
        "    adr x0, sb_capacity\n"
        "    str x19, [x0]\n"
        "    \n"
        "    ldr x20, [sp, #24]\n"
        "    ldr x19, [sp, #16]\n"
        "    ldp x29, x30, [sp], #32\n"
        "    ret\n\n"
        "// Obtener string final\n"
        "sb_get_string:\n"
        "    adr x0, sb_buffer\n"
        "    ldr x0, [x0]\n"
        "    ret\n\n"
        "// Liberar StringBuilder\n"
        "sb_free:\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    \n"
        "    adr x0, sb_buffer\n"
        "    ldr x0, [x0]\n"
        "    adr x1, sb_capacity\n"
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
    
    gen->funciones_generadas = true;
}

void generarInfraestructuraStringBuilder(GeneradorStringBuilder* gen) {
    generarSeccionesStringBuilder(gen);
    generarFuncionesAuxiliares(gen);
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