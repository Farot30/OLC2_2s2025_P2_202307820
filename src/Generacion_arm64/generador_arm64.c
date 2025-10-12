#include "generador_arm64.h"
#include "registros_arm64.h"
#include "InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "InstruccionesNorm_arm64/declaracionConst_arm64.h"
#include "InstruccionesNorm_arm64/Asignacion_arm64.h"
#include <stdlib.h>
#include <string.h>

// Estructura para almacenar código de carga
typedef struct CodigoCarga {
    char codigo[256];
    struct CodigoCarga* siguiente;
} CodigoCarga;

static FILE* archivoSalida = NULL;
static char* bufferCodigo = NULL;
static size_t bufferTamanio = 0;
static GeneradorARM64 generador_principal;

void inicializarGeneradorARM64(GeneradorARM64* gen, const char* nombreArchivo) {
    // Abrir archivo
    gen->archivo = fopen(nombreArchivo, "w");
    if (!gen->archivo) {
        perror("Error al abrir archivo ARM64");
        return;
    }
    
    // Inicializar buffer
    gen->buffer_size = 8192; // Aumentar tamaño inicial
    gen->buffer_codigo = malloc(gen->buffer_size);
    gen->buffer_codigo[0] = '\0';
    
    // Inicializar administrador de registros
    inicializarRegistros(&gen->admin_registros);
    
    // Inicializar generador de declaraciones
    inicializarGeneradorDeclaraciones(&gen->gen_declaraciones, 
                                     gen->archivo, 
                                     gen->buffer_codigo, 
                                     gen->buffer_size,
                                     &gen->admin_registros);

    inicializarGeneradorConstantes(&gen->gen_constantes, 
                                     gen->archivo, 
                                     gen->buffer_codigo, 
                                     gen->buffer_size,
                                     &gen->admin_registros);
    
    gen->seccion_data_creada = true;
    gen->seccion_bss_creada = true;
    gen->seccion_rodata_creada = true; // **AGREGAR ESTA LÍNEA**
    gen->seccion_text_iniciada = true;
    
    // **CREAR TODA LA ESTRUCTURA AL INICIO CON .rodata**
    const char* estructura_completa = 
        "// Código ARM64 generado por JavaLang Compiler\n"
        ".global _start\n"
        ".extern itoa\n\n"
        "\n// declaraciones de variables\n"
        ".data\n"
        "\n// constantes\n"
        ".section .rodata\n"
        "\n// seccion bss\n"
        ".bss\n"
        "\n// código ejecutable\n"
        ".text\n\n"
        "_start:\n"
        "    // Punto de entrada del programa\n"
        "    // Salida del programa por defecto\n"
        "    mov x0, #0\n"
        "    mov x8, #93\n"
        "    svc #0\n";
    
    // **SOLO AGREGAR AL BUFFER, NO ESCRIBIR AL ARCHIVO AÚN**
    strcat(gen->buffer_codigo, estructura_completa);
}

void finalizarGeneradorARM64(GeneradorARM64* gen) {
    if (gen->archivo && gen->buffer_codigo) {
        // **ESCRIBIR TODO EL BUFFER AL ARCHIVO AL FINAL**
        fprintf(gen->archivo, "%s", gen->buffer_codigo);
        fflush(gen->archivo);
        fclose(gen->archivo);
        gen->archivo = NULL;
        printf("[DEBUG] Código ARM64 escrito al archivo\n");
    }
}
bool seccion_data_iniciada = false;
bool seccion_bss_iniciada = false;

void procesarNodoAST_ARM64(GeneradorARM64* gen, NodoAST* nodo) {
    if (!gen || !nodo) return;

    switch (nodo->tipo) {
        case AST_DECLARACION:
            // NO generar secciones aquí, solo la declaración
            printf("[DEBUG] Nodo de declaración detectado\n");
            
            generarDeclaracionVariable(&gen->gen_declaraciones, nodo, &seccion_data_iniciada, &seccion_bss_iniciada);

            break;
            
        case AST_DECLARACION_CONST:
            generarDeclaracionConstante(&gen->gen_constantes, nodo, &gen->seccion_rodata_creada);

            break;

        case AST_ASIGNACION:
            printf("[DEBUG] Nodo de asignación detectado arm64\n");
            generarAsignacionARM64(&generador_principal.gen_declaraciones, nodo);
            break;

        case AST_ASIGNACION_COMPUESTA:
            printf("[DEBUG] Nodo de asignación compuesta detectado arm64\n");
            generarAsignacionCompuestaARM64(&generador_principal.gen_declaraciones, nodo);
            break;

        case AST_OPERACION:
            printf("[DEBUG] Nodo de operación detectado en ARM64\n");
            // Solo generar si es una operación independiente (no parte de una declaración/asignación)
            // Esto generalmente no debería ocurrir, pero si sucede, procesamos los hijos
            for (int i = 0; i < nodo->num_hijos; i++) {
                procesarNodoAST_ARM64(gen, nodo->hijos[i]);
            }
            break;

        default:
            // Procesar hijos recursivamente
            for (int i = 0; i < nodo->num_hijos; i++) {
                procesarNodoAST_ARM64(gen, nodo->hijos[i]);
            }
            break;
    }
}

// Funciones compatibles con la interfaz anterior
void iniciarArchivoARM64(const char* nombreArchivo) {
    inicializarGeneradorARM64(&generador_principal, nombreArchivo);
    
    // Mantener compatibilidad con variables estáticas
    archivoSalida = generador_principal.archivo;
    bufferTamanio = generador_principal.buffer_size;
    bufferCodigo = generador_principal.buffer_codigo;
}

void finalizarArchivoARM64() {
    finalizarGeneradorARM64(&generador_principal);
    
    // Limpiar variables estáticas
    archivoSalida = NULL;
    bufferCodigo = generador_principal.buffer_codigo;
    bufferTamanio = generador_principal.buffer_size;
}

void GeneradorCodigoARM64(NodoAST* nodo) {
    procesarNodoAST_ARM64(&generador_principal, nodo);
}

const char* obtenerCodigoGenerado() {
    return generador_principal.buffer_codigo;
}