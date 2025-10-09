#include "generador_arm64.h"
#include "registros_arm64.h"
#include "declaraciones_arm64.h"
#include "declaracionConst_arm64.h"
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
static CodigoCarga* codigo_carga_head = NULL;

void inicializarGeneradorARM64(GeneradorARM64* gen, const char* nombreArchivo) {
    // Abrir archivo
    gen->archivo = fopen(nombreArchivo, "w");
    if (!gen->archivo) {
        perror("Error al abrir archivo ARM64");
        return;
    }
    
    // Inicializar buffer
    gen->buffer_size = 4096;
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
    
    gen->seccion_data_creada = false;
    gen->seccion_text_iniciada = false;
    
    // Escribir encabezado inicial con estructura correcta
    const char* encabezado = "// Código ARM64 generado por JavaLang Compiler\n"
                           ".global _start\n\n";
    
    fprintf(gen->archivo, "%s", encabezado);
    strcat(gen->buffer_codigo, encabezado);
}

void finalizarGeneradorARM64(GeneradorARM64* gen) {
    if (gen->archivo) {
        
        // Generar sección .text
        const char* seccion_text = "\n// código ejecutable\n.text\n\n_start:\n    // Punto de entrada del programa\n";
        fprintf(gen->archivo, "%s", seccion_text);
        strcat(gen->buffer_codigo, seccion_text);
        
        // Escribir todo el código de carga de variables
        CodigoCarga* actual = codigo_carga_head;
        while (actual) {
            fprintf(gen->archivo, "%s", actual->codigo);
            strcat(gen->buffer_codigo, actual->codigo);
            CodigoCarga* temp = actual;
            actual = actual->siguiente;
            free(temp);
        }
        codigo_carga_head = NULL;
        
        // Finalización del programa por defecto
        const char* finalizacion = "    // Salida del programa por defecto\n"
                                  "    mov x0, #0\n"
                                  "    mov x8, #93\n"
                                  "    svc #0\n";
        
        fprintf(gen->archivo, "%s", finalizacion);
        strcat(gen->buffer_codigo, finalizacion);
        
        fclose(gen->archivo);
        gen->archivo = NULL;
    }
}

void agregarCodigoCarga(const char* nombreVariable, int registro) {
    CodigoCarga* nuevo = malloc(sizeof(CodigoCarga));
    snprintf(nuevo->codigo, sizeof(nuevo->codigo),
             "    // Cargar %s en registro %s\n"
             "    adr x1, %s\n"
             "    ldr %s, [x1]\n",
             nombreVariable,
             obtenerNombreRegistro(registro),
             nombreVariable,
             obtenerNombreRegistro(registro));
    
    nuevo->siguiente = codigo_carga_head;
    codigo_carga_head = nuevo;
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
            generarDeclaracionConstante(&gen->gen_constantes, nodo, &gen->seccion_text_iniciada);

            break;

        // Aquí se pueden agregar más casos para otros tipos de nodos
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
    
    // GENERAR SOLO .data AL INICIO (las declaraciones se escribirán aquí)
    generarSeccionData(&generador_principal.gen_declaraciones);
    generarSeccionBss(&generador_principal.gen_declaraciones);

    generador_principal.seccion_data_creada = true;
    
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

void generarDeclaracionARM64(NodoAST* nodo) {
    procesarNodoAST_ARM64(&generador_principal, nodo);
}

void generarDeclaracionConstanteARM64(NodoAST* nodo) {
    procesarNodoAST_ARM64(&generador_principal, nodo);
}

const char* obtenerCodigoGenerado() {
    return generador_principal.buffer_codigo;
}