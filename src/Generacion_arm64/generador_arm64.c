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
#include "Vectores_arm64/DeclaracionesMat_arm64.h"
#include "Embebidas/String_valueOf.h"
#include "InstruccionesSent_arm64/ForEach_arm64.h"
#include "Vectores_arm64/EscrituraVector_arm64.h"
#include "Vectores_arm64/EscrituraMat_arm64.h"
#include "Vectores_arm64/AccesoMatriz_arm64.h"
#include "Embebidas/Parseos_arm64.h"
#include "Embebidas/Embebidas_arm64.h"
#include "InstruccionesSent_arm64/Do_While_arm64.h"
#include "Funciones/FuncionesVoid_arm64.h"
#include "Funciones/LlamadasVoid_arm64.h"
#include "Funciones/Funciones_arm64.h"
#include "Funciones/Llamadas_arm64.h"
#include "../AST/err.h"
#include <stdlib.h>
#include <string.h>



// Pilas de ámbitos globales
PilaAmbitosRetorno pilaAmbitosRetorno;
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

extern NodoAST* astGlobal;

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

// ✅ NUEVA FUNCIÓN: Insertar código de función ANTES de _start
void insertarFuncionAntesDeStart(GeneradorDeclaraciones* gen, const char* codigo) {
    if (!gen->buffer || !codigo) return;

    // Buscar la etiqueta _start:
    const char* start_marker = "_start:\n";
    char* pos = strstr(gen->buffer, start_marker);
    
    if (pos) {
        // Insertar JUSTO ANTES de _start:
        size_t offset = pos - gen->buffer;
        size_t len_actual = strlen(gen->buffer);
        size_t len_codigo = strlen(codigo);

        // Redimensionar buffer si es necesario
        if (len_actual + len_codigo + 1 >= gen->buffer_size) {
            gen->buffer_size = len_actual + len_codigo + 2048;
            gen->buffer = realloc(gen->buffer, gen->buffer_size);
            pos = gen->buffer + offset; // Recalcular posición
        }

        // Mover el contenido para hacer espacio
        memmove(pos + len_codigo, pos, len_actual - offset + 1);
        memcpy(pos, codigo, len_codigo);
    } else {
        // Si no existe _start, agregar después de .text
        const char* text_marker = ".text\n";
        pos = strstr(gen->buffer, text_marker);
        if (pos) {
            pos += strlen(text_marker);
            size_t offset = pos - gen->buffer;
            size_t len_actual = strlen(gen->buffer);
            size_t len_codigo = strlen(codigo);

            if (len_actual + len_codigo + 1 >= gen->buffer_size) {
                gen->buffer_size = len_actual + len_codigo + 2048;
                gen->buffer = realloc(gen->buffer, gen->buffer_size);
            }

            memmove(gen->buffer + offset + len_codigo, gen->buffer + offset, len_actual - offset + 1);
            memcpy(gen->buffer + offset, codigo, len_codigo);
        } else {
            agregarCodigoAlBuffer(gen, codigo);
        }
    }
}

static NodoAST* buscarFuncionRetornoEnAST_Local(NodoAST* raiz, const char* nombreFuncion) {
    if (!raiz || !nombreFuncion) return NULL;
    
    // Si es un nodo de función con retorno (AST_FUNCION)
    if (raiz->tipo == AST_FUNCION && raiz->num_hijos >= 2) {
        NodoAST* idNodo = raiz->hijos[1]; // El nombre está en el segundo hijo
        if (idNodo && idNodo->tipo == AST_IDENTIFICADOR && 
            strcmp(idNodo->valor, nombreFuncion) == 0) {
            return raiz;
        }
    }
    
    // Buscar recursivamente en los hijos
    for (int i = 0; i < raiz->num_hijos; i++) {
        NodoAST* resultado = buscarFuncionRetornoEnAST_Local(raiz->hijos[i], nombreFuncion);
        if (resultado) return resultado;
    }
    
    return NULL;
}

/**
 * Obtiene el tipo de retorno de una función desde su nodo AST
 */
static TipoDato obtenerTipoRetornoFuncions(NodoAST* nodoFuncion) {
    if (!nodoFuncion || nodoFuncion->tipo != AST_FUNCION || nodoFuncion->num_hijos < 1) {
        return TIPO_DESCONOCIDO;
    }
    
    // El tipo de retorno está en el primer hijo
    NodoAST* tipoRetornoNodo = nodoFuncion->hijos[0];
    if (!tipoRetornoNodo || tipoRetornoNodo->tipo != AST_TIPO) {
        return TIPO_DESCONOCIDO;
    }
    
    return obtenerTipoDesdeString(tipoRetornoNodo->valor);
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
    crearArchivosParseo();

    
    // Inicializar buffer
    gen->buffer_size = 65536; // Aumentar tamaño inicial
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
        ".extern parse_int\n"      
        ".extern parse_double\n"   
        ".extern parse_float\n"    
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
    inicializarPilaAmbitos(&gen->pila_ambitos);
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

            // Detectar parseos
            if (valorNodo) {
                if (valorNodo->tipo == AST_PARSEINT) {
                    // 1. Declarar la variable en .data
                    char decl[64];
                    if (tipo == TIPO_INT) {
                        snprintf(decl, sizeof(decl), "%s: .word 0\n", nombreVariable);
                    } else {
                        snprintf(decl, sizeof(decl), "%s: .word 0\n", nombreVariable); // Por defecto int
                    }
                    insertarEnSeccionData(&gen->gen_declaraciones, decl);

                    // 2. Generar el código de parseo y asignación
                    int regIndex = 1;
                    ResultadoExprEmbebida resultado = generarParseIntARM64(&gen->gen_declaraciones, valorNodo, &regIndex);
                    insertarEnSeccionText(&gen->gen_declaraciones, resultado.codigo);
                    char codigo[128];
                    snprintf(codigo, sizeof(codigo),
                        "    ldr x1, =%s\n"
                        "    str w0, [x1]\n", nombreVariable);
                    insertarEnSeccionText(&gen->gen_declaraciones, codigo);
                    return;
                }
                else if (valorNodo->tipo == AST_PARSEDOUBLE) {
                    char decl[64];
                    snprintf(decl, sizeof(decl), "%s: .double 0.0\n", nombreVariable);
                    insertarEnSeccionData(&gen->gen_declaraciones, decl);
                    int regIndex = 1;
                    ResultadoExprEmbebida resultado = generarParseDoubleARM64(&gen->gen_declaraciones, valorNodo, &regIndex);
                    insertarEnSeccionText(&gen->gen_declaraciones, resultado.codigo);
                    char codigo[128];
                    snprintf(codigo, sizeof(codigo),
                        "    ldr x1, =%s\n"
                        "    str d0, [x1]\n", nombreVariable);
                    insertarEnSeccionText(&gen->gen_declaraciones, codigo);
                    return;
                }
                else if (valorNodo->tipo == AST_PARSEFLOAT) {
                     char decl[64];
                    snprintf(decl, sizeof(decl), "%s: .float 0.0\n", nombreVariable);
                    insertarEnSeccionData(&gen->gen_declaraciones, decl);
                    int regIndex = 1;
                    ResultadoExprEmbebida resultado = generarParseFloatARM64(&gen->gen_declaraciones, valorNodo, &regIndex);
                    insertarEnSeccionText(&gen->gen_declaraciones, resultado.codigo);
                    char codigo[128];
                    snprintf(codigo, sizeof(codigo),
                        "    ldr x1, =%s\n"
                        "    str s0, [x1]\n", nombreVariable);
                    insertarEnSeccionText(&gen->gen_declaraciones, codigo);
                    return;
                }
                else if (valorNodo->tipo == AST_OPERACION){
                    printf("[DEBUG] Agregando operación pendiente para: %s\n", nombreVariable);
                    
                    // **GENERAR LA DECLARACIÓN CORRECTA SEGÚN EL TIPO**
                    char decl[64];
                    if (tipo == TIPO_STRING) {
                        snprintf(decl, sizeof(decl), "%s: .skip 8\n", nombreVariable);
                    } else if (tipo == TIPO_DOUBLE) {
                        snprintf(decl, sizeof(decl), "%s: .double 0.0\n", nombreVariable);
                    } else if (tipo == TIPO_FLOAT) {
                        snprintf(decl, sizeof(decl), "%s: .float 0.0\n", nombreVariable);
                    } else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) {
                        snprintf(decl, sizeof(decl), "%s: .byte 0\n", nombreVariable);
                    } else {
                        snprintf(decl, sizeof(decl), "%s: .word 0\n", nombreVariable);
                    }
                    insertarEnSeccionData(&gen->gen_declaraciones, decl);
                    
                    // **AGREGAR LA OPERACIÓN A PENDIENTES**
                    agregarOperacionPendiente(nombreVariable, valorNodo, tipo, true);

                    procesarOperacionesPendientes(gen);
                }else if (valorNodo->tipo == AST_INDEXOF){
                    printf("[DEBUG DECLARACION ARM64] Arrays.indexOf para '%s'\n", nombreVariable);
                    char codigo[128];
                    // 1. Declaración en .data (int para el resultado)
                    snprintf(codigo, sizeof(codigo), "%s: .word -1\n", nombreVariable);
                    insertarEnSeccionData(&gen->gen_declaraciones, codigo);
                    
                    // 2. Generar código de Arrays.indexOf
                    generarArraysIndexOfARM64(&gen->gen_declaraciones, nombreVariable, valorNodo);
                    
                    return;
                } else if (valorNodo->tipo == AST_ACCESO) {
                     printf("[DEBUG DECLARACION ARM64] Acceso a variable para '%s'\n", nombreVariable);

                    // **DETECTAR SI ES VECTOR O MATRIZ**
                    int numDimensiones = contarDimensionesAcceso(valorNodo);
                    
                    char codigo[256];
                    
                    // 1. Declaración en .data según el tipo
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
                    insertarEnSeccionData(&gen->gen_declaraciones, codigo);
                    
                    // 2. Generar código de acceso Y asignación según dimensiones
                    if (numDimensiones == 2) {
                        // **ACCESO A MATRIZ**
                        printf("[DEBUG DECLARACION] Acceso a matriz en asignación a '%s'\n", nombreVariable);
                        generarAccesoMatrizParaAsignacion(&gen->gen_declaraciones, valorNodo, nombreVariable);
                    } else if (numDimensiones == 1) {
                        // **ACCESO A VECTOR**
                        printf("[DEBUG DECLARACION] Acceso a vector en asignación a '%s'\n", nombreVariable);
                        generarAccesoVectorParaAsignacion(&gen->gen_declaraciones, valorNodo, nombreVariable);
                    } else {
                        printf("[ERROR] Número de dimensiones no soportado: %d\n", numDimensiones);
                    }
                    
                    return;
                } else if (valorNodo->tipo == AST_PARSEJOIN){
                    printf("[DEBUG] haciendo el parseJOIN");
                    char codigo[256];
                    if (tipo == TIPO_STRING) {
                        snprintf(codigo, sizeof(codigo),
                            "%s: .quad str_lit_%s\n"
                            "str_lit_%s: .asciz \"\"\n",
                            nombreVariable, nombreVariable, nombreVariable);
                    } else if (tipo == TIPO_DOUBLE) {
                        snprintf(codigo, sizeof(codigo), "%s: .double 0.0\n", nombreVariable);
                    } else if (tipo == TIPO_FLOAT) {
                        snprintf(codigo, sizeof(codigo), "%s: .float 0.0\n", nombreVariable);
                    } else if (tipo == TIPO_INT) {
                        snprintf(codigo, sizeof(codigo), "%s: .word 0\n", nombreVariable);
                    } else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) {
                        snprintf(codigo, sizeof(codigo), "%s: .byte 0\n", nombreVariable);
                    }
                    insertarEnSeccionData(gen, codigo);

                    generarStringJoinARM64(&gen->gen_declaraciones, nombreVariable, valorNodo);
                    return;
                } else if(valorNodo->tipo == AST_LENGHT){
                    printf("[DEBUG DECLARACION ARM64] Array.length para '%s'\n", nombreVariable);
                    
                    // 1. Declaración en .data (int para el resultado)
                    char codigo[128];
                    snprintf(codigo, sizeof(codigo), "%s: .word 0\n", nombreVariable);
                    insertarEnSeccionData(&gen->gen_declaraciones, codigo);
                    
                    // 2. Generar código de Array.length
                    int regIndex = 1;
                    ResultadoExprEmbebida resultado = generarArrayLengthARM64(&gen->gen_declaraciones, valorNodo, &regIndex);
                    
                    if (resultado.codigo[0] != '\0') {
                        insertarEnSeccionText(&gen->gen_declaraciones, resultado.codigo);
                        
                        // 3. Asignar el resultado a la variable
                        snprintf(codigo, sizeof(codigo),
                            "    ldr x2, =%s\n"
                            "    str %s, [x2]\n",
                            nombreVariable, resultado.resultado); // Usa x2 para la dirección, w1/w2 para el valor
                        insertarEnSeccionText(&gen->gen_declaraciones, codigo);
                    }
                    
                    return;
                
                } else if (valorNodo->tipo == AST_LLAMADA){
                    printf("[DEBUG] Declaración con llamada a función para '%s'\n", nombreVariable);
            
                    // Obtener el nombre de la función desde el nodo de llamada
                    const char* nombreFuncion = NULL;
                    if (valorNodo->num_hijos >= 1 && valorNodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                        nombreFuncion = valorNodo->hijos[0]->valor;
                    }
                    
                    if (!nombreFuncion) {
                        printf("[ERROR] No se pudo obtener el nombre de la función\n");
                        break;
                    }
                    
                    // ✅ BUSCAR LA FUNCIÓN EN EL AST (no en la tabla de símbolos)
                    NodoAST* funcionNodo = buscarFuncionRetornoEnAST_Local(astGlobal, nombreFuncion);
                    
                    if (!funcionNodo) {
                        printf("[ERROR] Función '%s' no encontrada en el AST\n", nombreFuncion);
                        
                        // Declarar la variable sin inicializar
                        char decl[64];
                        if (tipo == TIPO_INT || tipo == TIPO_BOOLEAN || tipo == TIPO_CHAR) {
                            snprintf(decl, sizeof(decl), "%s: .word 0\n", nombreVariable);
                        } else if (tipo == TIPO_FLOAT) {
                            snprintf(decl, sizeof(decl), "%s: .float 0.0\n", nombreVariable);
                        } else if (tipo == TIPO_DOUBLE) {
                            snprintf(decl, sizeof(decl), "%s: .double 0.0\n", nombreVariable);
                        } else if (tipo == TIPO_STRING) {
                            snprintf(decl, sizeof(decl), "%s: .quad 0\n", nombreVariable);
                        }
                        insertarEnSeccionData(&gen->gen_declaraciones, decl);
                        break;
                    }
                    
                    // ✅ OBTENER EL TIPO DE RETORNO DESDE EL AST
                    TipoDato tipoRetorno = obtenerTipoRetornoFuncions(funcionNodo);
                    
                    printf("[DEBUG] Función '%s' encontrada en AST, tipo de retorno: %s\n", 
                        nombreFuncion, obtenerNombreTipo(tipoRetorno));
                    
                    // ✅ VERIFICAR QUE EL TIPO DE RETORNO COINCIDA CON EL TIPO DECLARADO
                    if (tipoRetorno != tipo && !esWidening(tipoRetorno, tipo)) {
                        printf("[ERROR] Tipo de retorno de '%s' (%s) no compatible con tipo declarado (%s)\n",
                            nombreFuncion, obtenerNombreTipo(tipoRetorno), obtenerNombreTipo(tipo));
                    }
                    
                    // 1. Declarar la variable
                    char decl[64];
                    if (tipo == TIPO_INT || tipo == TIPO_BOOLEAN || tipo == TIPO_CHAR) {
                        snprintf(decl, sizeof(decl), "%s: .word 0\n", nombreVariable);
                    } else if (tipo == TIPO_FLOAT) {
                        snprintf(decl, sizeof(decl), "%s: .float 0.0\n", nombreVariable);
                    } else if (tipo == TIPO_DOUBLE) {
                        snprintf(decl, sizeof(decl), "%s: .double 0.0\n", nombreVariable);
                    } else if (tipo == TIPO_STRING) {
                        snprintf(decl, sizeof(decl), "%s: .quad 0\n", nombreVariable);
                    }
                    insertarEnSeccionData(&gen->gen_declaraciones, decl);
                    
                    // 2. Generar la llamada a la función
                    generarLlamadaFuncionRetorno(&gen->gen_declaraciones, valorNodo,
                                                &pilaAmbitosRetorno, astGlobal);
                    
                    // 3. Guardar el resultado en la variable
                    char codigo[128];
                    if (tipo == TIPO_INT || tipo == TIPO_BOOLEAN || tipo == TIPO_CHAR) {
                        snprintf(codigo, sizeof(codigo),
                            "    ldr x1, =%s\n"
                            "    str w0, [x1]\n", nombreVariable);
                    } else if (tipo == TIPO_FLOAT) {
                        snprintf(codigo, sizeof(codigo),
                            "    ldr x1, =%s\n"
                            "    str s0, [x1]\n", nombreVariable);
                    } else if (tipo == TIPO_DOUBLE) {
                        snprintf(codigo, sizeof(codigo),
                            "    ldr x1, =%s\n"
                            "    str d0, [x1]\n", nombreVariable);
                    } else if (tipo == TIPO_STRING) {
                        snprintf(codigo, sizeof(codigo),
                            "    ldr x1, =%s\n"
                            "    str x0, [x1]\n", nombreVariable);
                    }
                    insertarEnSeccionText(&gen->gen_declaraciones, codigo);
                    return; // ✅ SALIR AQUÍ
                }
                else {
                    generarDeclaracionVariable(&gen->gen_declaraciones, nodo, &seccion_data_iniciada, &seccion_bss_iniciada);
                }
            } else {
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

        case AST_DO_WHILE:
            printf("[DEBUG]");
            generarCodigoDoWhile(&generador_principal.gen_declaraciones, nodo);
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
        case AST_FOR_VECTOR:
            printf("[DEBUG] Nodo FOR-EACH detectado arm64\n");
            generarCodigoForEach(&gen->gen_declaraciones, nodo);
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

            if (nodo->num_hijos >= 2) {
                NodoAST* tipoNdo = nodo->hijos[0];
                NodoAST* idNdo = nodo->hijos[1];
                NodoAST* valorNodo = nodo->num_hijos > 2 ? nodo->hijos[2] : NULL;

                const char* nombreVariable = idNdo->valor;
                const char* tipoVariable = tipoNdo->valor;

                TipoDato tipoDestino = obtenerTipoDesdeString(tipoVariable);

                if (valorNodo) {
                    if (valorNodo->tipo == AST_LISTA_ELEMENTOS) {
                        NodoAST* lista = valorNodo;
                        
                        // Verificar si es Arrays.add
                        if (lista->num_hijos == 1 && lista->hijos[0]->tipo == AST_ADD) {
                            NodoAST* addNodo = lista->hijos[0];
                            printf("[DEBUG] Nodo Arrays.add detectado en declaración de vector '%s'\n", nombreVariable);

                            // Verificar tipo del vector origen
                            if (addNodo->num_hijos >= 1 && addNodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                                const char* nombreVectorOrigen = addNodo->hijos[0]->valor;
                                
                                // Buscar el vector origen en el AST
                                NodoAST* vectorOrigenDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_VECTOR, nombreVectorOrigen);
                                if (!vectorOrigenDecl) {
                                    printf("[ERROR] Vector origen '%s' no encontrado en el AST\n", nombreVectorOrigen);
                                    return;
                                }

                                // Obtener el tipo del vector origen
                                TipoDato tipoOrigen = obtenerTipoVectorDesdeAST(vectorOrigenDecl);

                                // **VERIFICAR QUE LOS TIPOS COINCIDAN**
                                if (tipoDestino != tipoOrigen) {
                                    printf("[ERROR SEMÁNTICO] El vector '%s' es de tipo '%s', pero el vector origen '%s' es de tipo '%s'. Los tipos no coinciden.\n",
                                        nombreVariable, tipoVariable, nombreVectorOrigen, obtenerNombreTipo(tipoOrigen));
                                    
                                    // Opcional: agregar error semántico a la lista de errores
                                    char msgError[256];
                                    snprintf(msgError, sizeof(msgError), 
                                            "El vector '%s' (%s) no puede ser asignado desde '%s' (%s). Tipos incompatibles.",
                                            nombreVariable, tipoVariable, nombreVectorOrigen, obtenerNombreTipo(tipoOrigen));
                                    addError(msgError, "global", nodo->linea, nodo->columna);
                                    return;
                                }

                                printf("[DEBUG] Verificación de tipos exitosa: '%s' y '%s' son ambos de tipo '%s'\n",
                                    nombreVariable, nombreVectorOrigen, tipoVariable);

                                // 2. Generar el código para Arrays.add
                                generarArraysAddOfArm64(&gen->gen_declaraciones, nombreVariable, addNodo);
                            } else {
                                printf("[ERROR] El primer argumento de Arrays.add debe ser un identificador de vector\n");
                                return;
                            }
                        } else {
                            // Generar la declaración normal del vector
                            generarDeclaracionVectorARM64(&gen->gen_declaraciones, nodo);
                        }
                    } else {
                        // Generar la declaración normal del vector
                        generarDeclaracionVectorARM64(&gen->gen_declaraciones, nodo);
                    }
                }
            }
            break;
        case AST_ACCESO_ESCRITURA:
            printf("[DEBUG] Nodo de acceso con escritura detectado\n");
    
            // **DETECTAR SI ES VECTOR O MATRIZ**
            if (nodo->num_hijos >= 1 && nodo->hijos[0]) {
                NodoAST* accesoNodo = nodo->hijos[0];
                
                if (accesoNodo->tipo == AST_ACCESO) {
                    int numDimensiones = contarDimensionesAcceso(accesoNodo);
                    
                    if (numDimensiones == 2) {
                        // **ESCRITURA EN MATRIZ**
                        printf("[DEBUG] Escritura en matriz detectada\n");
                        generarEscrituraMatrizARM64(&gen->gen_declaraciones, nodo);
                    } else if (numDimensiones == 1) {
                        // **ESCRITURA EN VECTOR**
                        printf("[DEBUG] Escritura en vector detectada\n");
                        generarEscrituraVectorARM64(&gen->gen_declaraciones, nodo);
                    } else {
                        printf("[ERROR] Número de dimensiones no soportado: %d\n", numDimensiones);
                    }
                } else {
                    printf("[ERROR] Nodo de acceso inválido en escritura\n");
                }
            }
            break;
        case AST_DECLARACION_MATRIZ:
            printf("[DEBUG] Nodo de declaración de matriz detectado\n");
            generarDeclaracionMatrizARM64(&gen->gen_declaraciones, nodo);
            break;

        case AST_FUNCION_VOID:
            printf("[DEBUG] Nodo FUNCION_VOID detectado arm64\n");
            generarDeclaracionFuncionVoid(&gen->gen_declaraciones, nodo, 
                                       &gen->pila_ambitos, astGlobal);
            break;
    
        case AST_LLAMADA_VOID:
            printf("[DEBUG] Nodo LLAMADA_VOID detectado arm64\n");
            generarLlamadaFuncionVoid(&gen->gen_declaraciones, nodo, 
                                    &gen->pila_ambitos, astGlobal);

            break;

        case AST_LLAMADA:
            // Puede ser llamada a void o con retorno
            printf("[DEBUG] Nodo LLAMADA detectado arm64\n");
            
            // Verificar si es una función void
            if (nodo->num_hijos >= 1 && nodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                NodoAST* funcionDecl = buscarFuncionVoidEnAST(astGlobal, nodo->hijos[0]->valor);
                if (funcionDecl) {
                    // Es función void
                    generarLlamadaFuncionVoid(&gen->gen_declaraciones, nodo, 
                                            &gen->pila_ambitos, astGlobal);
                } else {
                    generarLlamadaFuncionRetorno(&gen->gen_declaraciones, nodo,
                                        &pilaAmbitosRetorno, astGlobal);
                    printf("[WARNING] Llamada a función con retorno no implementada aún\n");
                }
            }
            break;

        case AST_FUNCION:
            printf("[DEBUG] Procesando función con retorno: %s -> %s\n",
                   nodo->hijos[1] ? nodo->hijos[1]->valor : "unknown",
                   nodo->hijos[0] ? nodo->hijos[0]->valor : "unknown");
            generarDeclaracionFuncionRetorno(&gen->gen_declaraciones, nodo,
                                            &pilaAmbitosRetorno, astGlobal);
            break;
        case AST_RETURN:
            printf("[DEBUG] Nodo RETURN detectado arm64\n");

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