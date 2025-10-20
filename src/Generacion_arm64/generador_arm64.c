#include "generador_arm64.h"
#include "registros_arm64.h"
#include "InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "InstruccionesNorm_arm64/declaracionConst_arm64.h"
#include "InstruccionesNorm_arm64/Operadores_arm64.h"
#include "InstruccionesNorm_arm64/Asignacion_arm64.h"
#include "InstruccionesNorm_arm64/StringBuilder.h"
#include "InstruccionesSent_arm64/If_arm64.h"
#include "Impresion_arm64.h"
#include "InstruccionesSent_arm64/Switch_arm64.h"
#include "InstruccionesSent_arm64/for_arm64.h"
#include "InstruccionesSent_arm64/while_arm64.h"
#include "InstruccionesSent_arm64/Break_continue_arm64.h"
#include "Vectores_arm64/declaracionesVec_arm64.h"
#include "Embebidas/String_valueOf.h"
#include <stdlib.h>
#include <string.h>

// Estructura para almacenar código de carga
typedef struct CodigoCarga {
    char codigo[256];
    struct CodigoCarga* siguiente;
} CodigoCarga;

typedef struct OperacionPendiente {
    char* nombreVariable;
    NodoAST* operacionNodo;
    TipoDato tipo;
    bool esDeclaracion;
    struct OperacionPendiente* siguiente;
} OperacionPendiente;

static OperacionPendiente* operaciones_pendientes = NULL;

static FILE* archivoSalida = NULL;
static char* bufferCodigo = NULL;
static size_t bufferTamanio = 0;
static GeneradorARM64 generador_principal;

void agregarOperacionPendiente(const char* nombreVar, NodoAST* operacion, TipoDato tipo, bool esDeclaracion) {
    OperacionPendiente* nueva = malloc(sizeof(OperacionPendiente));
    nueva->nombreVariable = strdup(nombreVar);
    nueva->operacionNodo = operacion;
    nueva->tipo = tipo;
    nueva->esDeclaracion = esDeclaracion;
    nueva->siguiente = operaciones_pendientes;
    operaciones_pendientes = nueva;
    
    printf("[DEBUG] Operación pendiente agregada: %s (tipo: %d)\n", nombreVar, tipo);
}

// **FUNCIÓN PARA PROCESAR OPERACIONES EN ORDEN CORRECTO**
void procesarOperacionesPendientes(GeneradorARM64* gen) {
    // Contar operaciones para procesar en orden inverso
    int count = 0;
    OperacionPendiente* temp = operaciones_pendientes;
    while (temp) {
        count++;
        temp = temp->siguiente;
    }
    
    // Crear array para procesar en orden correcto
    OperacionPendiente** array = malloc(count * sizeof(OperacionPendiente*));
    temp = operaciones_pendientes;
    for (int i = 0; i < count; i++) {
        array[i] = temp;
        temp = temp->siguiente;
    }
    
    // Procesar desde el último hacia el primero (orden correcto de dependencias)
    for (int i = 0; i < count; i++) {
        OperacionPendiente* op = array[i];
        printf("[DEBUG] Procesando operación para variable: %s\n", op->nombreVariable);
        
        if (op->esDeclaracion) {
            generarCodigoOperacionARM64(&gen->gen_declaraciones, op->nombreVariable, op->operacionNodo, op->tipo);
        } else {
            // Para asignaciones futuras
            generarCodigoOperacionARM64(&gen->gen_declaraciones, op->nombreVariable, op->operacionNodo, op->tipo);
        }
    }
    
    // Limpiar memoria
    free(array);
    while (operaciones_pendientes) {
        OperacionPendiente* temp = operaciones_pendientes;
        operaciones_pendientes = operaciones_pendientes->siguiente;
        free(temp->nombreVariable);
        free(temp);
    }
}

// **FUNCIÓN PARA VERIFICAR SI UNA VARIABLE DEPENDE DE OTRA**
bool verificarDependencia(NodoAST* nodo, const char* variable) {
    if (!nodo || !variable) return false;
    
    if (nodo->tipo == AST_IDENTIFICADOR && strcmp(nodo->valor, variable) == 0) {
        return true;
    }
    
    for (int i = 0; i < nodo->num_hijos; i++) {
        if (verificarDependencia(nodo->hijos[i], variable)) {
            return true;
        }
    }
    
    return false;
}

void inicializarGeneradorARM64(GeneradorARM64* gen, const char* nombreArchivo) {
    // Abrir archivo
    gen->archivo = fopen(nombreArchivo, "w");
    if (!gen->archivo) {
        perror("Error al abrir archivo ARM64");
        return;
    }

    crearArchivoStringValueOfInt("String_valueOfint.s");
    crearArchivoStringValueOfBool("String_valueOfbool.s");
    crearArchivoStringValueOffloat("String_valueOffloat.s");
    crearArchivoStringValueOfDouble("String_valueOfdouble.s");
    crearArchivoValueLong("String_valueOflong.s");

    
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
        ".extern itoa\n"
        ".extern valuesOf_int\n"
        ".extern valueOf_bool\n"
        ".extern valueOf_float\n"
        ".extern valueOf_double\n"
        ".extern valueOf_long\n"
        ".extern printf\n\n"
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
    // **PROCESAR OPERACIONES PENDIENTES ANTES DE ESCRIBIR AL ARCHIVO**
    printf("[DEBUG] Procesando operaciones pendientes antes de finalizar...\n");
    procesarOperacionesPendientes(gen);
    
    // **AHORA SÍ ESCRIBIR TODO AL ARCHIVO**
    if (gen->archivo && gen->buffer_codigo) {
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
            printf("[DEBUG] Nodo de declaración detectado\n");
            
            if (nodo->num_hijos >= 2) {
                NodoAST* tipoNodo = nodo->hijos[0];
                NodoAST* idNodo = nodo->hijos[1];
                NodoAST* valorNodo = nodo->num_hijos > 2 ? nodo->hijos[2] : NULL;
                
                const char* nombreVariable = idNodo->valor;
                const char* tipoVariable = tipoNodo->valor;
                TipoDato tipo = obtenerTipoDesdeString(tipoVariable);
                
                // **SI TIENE OPERACIÓN, GENERAR DECLARACIÓN Y AGREGAR A PENDIENTES**
                if (valorNodo && valorNodo->tipo == AST_OPERACION) {
                    printf("[DEBUG] Agregando operación pendiente para: %s\n", nombreVariable);
                    
                    // **GENERAR LA DECLARACIÓN CON .skip 8 INMEDIATAMENTE**
                    char decl[64];
                    if (tipo == TIPO_STRING) {
                        snprintf(decl, sizeof(decl), "%s: .skip 8\n", nombreVariable);
                    } else {
                        snprintf(decl, sizeof(decl), "%s: .word 0\n", nombreVariable);
                    }
                    insertarEnSeccionData(&gen->gen_declaraciones, decl);
                    
                    // **AGREGAR LA OPERACIÓN A PENDIENTES**
                    agregarOperacionPendiente(nombreVariable, valorNodo, tipo, true);

                    procesarOperacionesPendientes(gen);
                } else {
                    // Declaración simple, generar inmediatamente
                    generarDeclaracionVariable(&gen->gen_declaraciones, nodo, &seccion_data_iniciada, &seccion_bss_iniciada);
                }
            }
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
        case AST_IF:
            printf("[DEBUG] Nodo IF detectado arm64\n");
            if (nodo->num_hijos == 2) {
                // IF simple
                generarCodigoIF(&gen->gen_declaraciones, nodo);
            } else if (nodo->num_hijos == 3) {
                // IF-ELSE
                generarCodigoIFElse(&gen->gen_declaraciones, nodo);
            } else if (nodo->num_hijos >= 4) {
                // IF-ELSE IF
                generarCodigoIFElseIF(&gen->gen_declaraciones, nodo);
            }
            break;
        case AST_ELSEIF:
            printf("[DEBUG] Nodo ELSEIF detectado arm64\n");
            generarCodigoIFElseIF(&gen->gen_declaraciones, nodo);
            break;
        case AST_PRINT:
            printf("[DEBUG] Nodo PRINT detectado arm64\n");
            generarImpresionARM64(&gen->gen_declaraciones, nodo);
            break;
        case AST_INCREMENTO:
            printf("[DEBUG] Nodo INCREMENTO detectado arm64\n");
            if (nodo->num_hijos > 0 && nodo->hijos[0] && nodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                const char* nombreVar = nodo->hijos[0]->valor;
                char codigo[128];
                snprintf(codigo, sizeof(codigo),
                    "    // Incremento: %s++\n"
                    "    ldr x1, =%s\n"
                    "    ldr w0, [x1]\n"
                    "    add w0, w0, #1\n"
                    "    str w0, [x1]\n",
                    nombreVar, nombreVar);
                insertarEnSeccionText(&gen->gen_declaraciones, codigo);
            }
            break;
        case AST_DECREMENTO:
            printf("[DEBUG] Nodo DECREMENTO detectado arm64\n");
            if (nodo->num_hijos > 0 && nodo->hijos[0] && nodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                const char* nombreVar = nodo->hijos[0]->valor;
                char codigo[128];
                snprintf(codigo, sizeof(codigo),
                    "    // Decremento: %s--\n"
                    "    ldr x1, =%s\n"
                    "    ldr w0, [x1]\n"
                    "    sub w0, w0, #1\n"
                    "    str w0, [x1]\n",
                    nombreVar, nombreVar);
                insertarEnSeccionText(&gen->gen_declaraciones, codigo);
            }
            break;
        case AST_SWITCH:
            printf("[DEBUG] Nodo SWITCH detectado arm64\n");
            generarCodigoSwitch(&gen->gen_declaraciones, nodo);
            break;

        case AST_FOR:
            printf("[DEBUG] Nodo FOR detectado arm64\n");
            generarCodigoFor(&gen->gen_declaraciones, nodo);
            break;
        case AST_WHILE:
            printf("[DEBUG] Nodo WHILE detectado arm64\n");
            generarCodigoWhile(&gen->gen_declaraciones, nodo);
            break;
        case AST_BREAK:
            printf("[DEBUG] Nodo BREAK detectado arm64\n");
            generarCodigoBreak(&gen->gen_declaraciones, nodo);
            break;
        case AST_CONTINUE:
            printf("[DEBUG] Nodo CONTINUE detectado arm64\n");
            generarCodigoContinue(&gen->gen_declaraciones, nodo);
            break;
        case AST_DECLARACION_VECTOR:
            printf("[DEBUG] Nodo de declaración de vector detectado\n");
            generarDeclaracionVectorARM64(&gen->gen_declaraciones, nodo);
            break;
        default:
            // Procesar hijos recursivamente
            for (int i = 0; i < nodo->num_hijos; i++) {
                procesarNodoAST_ARM64(gen, nodo->hijos[i]);
            }

            procesarOperacionesPendientes(gen);
            break;
    }
}

// Funciones compatibles con la interfaz anterior
void iniciarArchivoARM64(const char* nombreArchivo) {
    resetearStringBuilder(); 
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