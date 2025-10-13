#include "Evaluadores_arm64.h"
#include "declaraciones_arm64.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// **FUNCIÓN PARA INICIALIZAR EL GENERADOR (igual que StringBuilder)**
void inicializarGeneradorEquals(GeneradorEquals* gen, FILE* archivo, 
                               char* buffer, size_t buffer_size) {
    gen->archivo = archivo;
    gen->buffer = buffer;
    gen->buffer_size = buffer_size;
    gen->seccion_data_creada = false;
    gen->seccion_bss_creada = false;
    gen->seccion_text_creada = false;
    gen->funciones_generadas = false;
    gen->contador_equals = 0;
}

// **FUNCIÓN PARA INSERTAR EN SECCIÓN TEXT (igual que StringBuilder)**
void insertarEnSeccionTextEquals(GeneradorEquals* gen, const char* codigo) {
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

// **FUNCIÓN PARA GENERAR FUNCIONES AUXILIARES (igual que StringBuilder)**
void generarFuncionesAuxiliaresEquals(GeneradorEquals* gen) {
    if (gen->funciones_generadas) return;
    
    const char* funciones_auxiliares = 
        "\n// ============================================\n"
        "// FUNCIONES EQUALS\n"
        "// ============================================\n\n"
        "string_equals:\n"
        "    mov x2, #0              // Inicializar índice\n"
        "string_equals_loop:\n"
        "    ldrb w3, [x0, x2]       // Cargar carácter del primer string\n"
        "    ldrb w4, [x1, x2]       // Cargar carácter del segundo string\n"
        "    cmp w3, w4              // Comparar los caracteres\n"
        "    bne strings_not_equal   // Si son diferentes, no son iguales\n"
        "    cbz w3, strings_equal   // Si llegamos al final ('\\0'), son iguales\n"
        "    add x2, x2, #1          // Incrementar índice\n"
        "    b string_equals_loop    // Repetir el bucle\n"
        "strings_not_equal:\n"
        "    mov w0, #0              // Retornar 0 (no son iguales)\n"
        "    ret\n"
        "strings_equal:\n"
        "    mov w0, #1              // Retornar 1 (son iguales)\n"
        "    ret\n\n";
    
    // Agregar al final del buffer
    if (strlen(gen->buffer) + strlen(funciones_auxiliares) + 1 < gen->buffer_size) {
        strcat(gen->buffer, funciones_auxiliares);
    }
    
    gen->funciones_generadas = true;
}

// **FUNCIÓN PARA GENERAR INFRAESTRUCTURA COMPLETA (igual que StringBuilder)**
void generarInfraestructuraEquals(GeneradorEquals* gen) {
    generarFuncionesAuxiliaresEquals(gen);
}

// **FUNCIÓN PRINCIPAL PARA GENERAR CÓDIGO DE EQUALS (usando la nueva estructura)**
void generarCodigoEqualsCompleto(GeneradorDeclaraciones* gen, const char* nombreVarResultado, 
                                const char* nombreStr1, const char* nombreStr2) {
    if (!gen || !nombreVarResultado || !nombreStr1 || !nombreStr2) return;

    printf("[DEBUG] Generando código equals para variable '%s'\n", nombreVarResultado);
    
    // Crear un generador de Equals
    GeneradorEquals equals_gen;
    inicializarGeneradorEquals(&equals_gen, gen->archivo, gen->buffer, gen->buffer_size);

    // Generar infraestructura del Equals
    generarInfraestructuraEquals(&equals_gen);

    char codigoOperacion[512] = "";
    char buffer[256];

    // Comentario inicial
    snprintf(buffer, sizeof(buffer), "    // Operación: %s = %s.equals(%s)\n", 
             nombreVarResultado, nombreStr1, nombreStr2);
    strcat(codigoOperacion, buffer);

    // Cargar el primer string en x0
    snprintf(buffer, sizeof(buffer), "    ldr x0, =%s\n", nombreStr1);
    strcat(codigoOperacion, buffer);

    // Cargar el segundo string en x1
    snprintf(buffer, sizeof(buffer), "    ldr x1, =%s\n", nombreStr2);
    strcat(codigoOperacion, buffer);

    // Llamar a la función string_equals
    strcat(codigoOperacion, "    bl string_equals\n");
    
    //guardar resultado del registro
    snprintf(buffer, sizeof(buffer), "    mov w19, w0           // guardar resultado en registro preservado\n");
    strcat(codigoOperacion, buffer);

    // Guardar el resultado en la variable destino (booleano)
    snprintf(buffer, sizeof(buffer),
        "    ldr x1, =%s\n"
        "    strb w0, [x1]\n", nombreVarResultado);
    strcat(codigoOperacion, buffer);

    // Insertar el código generado en la sección .text
    insertarEnSeccionTextEquals(&equals_gen, codigoOperacion);
}

// **FUNCIÓN DE COMPATIBILIDAD (para no romper código existente)**
void generarCodigoStringEqualsARM64(GeneradorDeclaraciones* gen, const char* nombreVarResultado, 
                                   const char* nombreStr1, const char* nombreStr2) {
    generarCodigoEqualsCompleto(gen, nombreVarResultado, nombreStr1, nombreStr2);
}

// **FUNCIÓN AUXILIAR PARA INSERTAR AL FINAL (igual que StringBuilder)**
void insertarFuncionAuxiliarEnText(GeneradorDeclaraciones* gen, const char* codigo) {
    if (strlen(gen->buffer) + strlen(codigo) + 1 < gen->buffer_size) {
        strcat(gen->buffer, codigo);
    }
}