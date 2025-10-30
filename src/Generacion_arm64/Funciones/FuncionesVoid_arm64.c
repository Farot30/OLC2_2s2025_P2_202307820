#include "FuncionesVoid_arm64.h"
#include "../Impresion_arm64.h"
#include "../InstruccionesNorm_arm64/Asignacion_arm64.h"
#include "../InstruccionesSent_arm64/If_arm64.h"
#include "../InstruccionesSent_arm64/for_arm64.h"
#include "../InstruccionesSent_arm64/while_arm64.h"
#include "../InstruccionesSent_arm64/Switch_arm64.h"
#include "../Vectores_arm64/EscrituraVector_arm64.h"
#include "../../semantico/tipos.h"
#include "../generador_arm64.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ==================== GESTIÓN DE ÁMBITOS ====================

void inicializarPilaAmbitos(PilaAmbitos* pila) {
    pila->tope = -1;
    memset(pila->ambitos, 0, sizeof(pila->ambitos));
}

void pushAmbito(PilaAmbitos* pila, const char* nombreFuncion, bool esVoid) {
    if (pila->tope >= MAX_AMBITOS - 1) {
        printf("[ERROR] Desbordamiento de pila de ámbitos\n");
        return;
    }
    
    pila->tope++;
    AmbitoFuncion* ambito = &pila->ambitos[pila->tope];
    
    strncpy(ambito->nombreFuncion, nombreFuncion, MAX_NOMBRE - 1);
    ambito->nombreFuncion[MAX_NOMBRE - 1] = '\0';
    ambito->numParametros = 0;
    ambito->numVariables = 0;
    ambito->tamanioStackFrame = 0;
    ambito->esVoid = esVoid;
    
    printf("[DEBUG AMBITO] Push: '%s' (void=%d), tope=%d\n", 
           nombreFuncion, esVoid, pila->tope);
}

AmbitoFuncion* popAmbito(PilaAmbitos* pila) {
    if (pila->tope < 0) {
        printf("[ERROR] Intento de pop en pila vacía\n");
        return NULL;
    }
    
    AmbitoFuncion* ambito = &pila->ambitos[pila->tope];
    printf("[DEBUG AMBITO] Pop: '%s', tope=%d->%d\n", 
           ambito->nombreFuncion, pila->tope, pila->tope - 1);
    
    pila->tope--;
    return ambito;
}

AmbitoFuncion* getAmbitoActual(PilaAmbitos* pila) {
    if (pila->tope < 0) return NULL;
    return &pila->ambitos[pila->tope];
}


bool esTipoPorReferencias(TipoDato tipo, bool esVector, bool esMatriz) {
    // Por referencia: String y arrays
    if (tipo == TIPO_STRING) return true;
    if (esVector || esMatriz) return true;
    
    // Por valor: tipos primitivos (int, float, double, char, boolean)
    return false;
}

void agregarParametroAmbito(PilaAmbitos* pila, const char* nombre, TipoDato tipo,
                           bool esVector, bool esMatriz) {
    AmbitoFuncion* ambito = getAmbitoActual(pila);
    if (!ambito || ambito->numParametros >= MAX_PARAMS) {
        printf("[ERROR] No se puede agregar parámetro '%s'\n", nombre);
        return;
    }
    
    ParametroInfo* param = &ambito->parametros[ambito->numParametros];
    strncpy(param->nombre, nombre, MAX_NOMBRE - 1);
    param->nombre[MAX_NOMBRE - 1] = '\0';
    param->tipo = tipo;
    param->esVector = esVector;
    param->esMatriz = esMatriz;
    param->esPorReferencia = esTipoPorReferencias(tipo, esVector, esMatriz);
    
    ambito->numParametros++;
    
    printf("[DEBUG AMBITO] Parámetro agregado: '%s', tipo=%d, vector=%d, matriz=%d, porRef=%d\n",
           nombre, tipo, esVector, esMatriz, param->esPorReferencia);
}

void agregarVariableLocalAmbito(PilaAmbitos* pila, const char* nombre, TipoDato tipo,
                               bool esVector, bool esMatriz) {
    AmbitoFuncion* ambito = getAmbitoActual(pila);
    if (!ambito) {
        printf("[ERROR] No hay ámbito actual para agregar variable '%s'\n", nombre);
        return;
    }
    
    VariableLocal* var = &ambito->variables[ambito->numVariables];
    strncpy(var->nombre, nombre, MAX_NOMBRE - 1);
    var->nombre[MAX_NOMBRE - 1] = '\0';
    var->tipo = tipo;
    var->esVector = esVector;
    var->esMatriz = esMatriz;
    var->offset_stack = ambito->tamanioStackFrame;
    
    // Calcular espacio necesario
    int tamanio = obtenerTamanioTipo(tipo);
    if (esVector || esMatriz) tamanio = 8; // Puntero
    
    ambito->tamanioStackFrame += (tamanio + 7) & ~7; // Alinear a 8 bytes
    ambito->numVariables++;
    
    printf("[DEBUG AMBITO] Variable local agregada: '%s', tipo=%d, offset=%d\n",
           nombre, tipo, var->offset_stack);
}

VariableLocal* buscarVariableEnAmbito(PilaAmbitos* pila, const char* nombre) {
    if (!pila || !nombre) return NULL;
    
    // Buscar desde el ámbito actual hacia atrás
    for (int i = pila->tope; i >= 0; i--) {
        AmbitoFuncion* ambito = &pila->ambitos[i];
        
        // Buscar en variables locales
        for (int j = 0; j < ambito->numVariables; j++) {
            if (strcmp(ambito->variables[j].nombre, nombre) == 0) {
                return &ambito->variables[j];
            }
        }
    }
    
    return NULL;
}

ParametroInfo* buscarParametroEnAmbito(PilaAmbitos* pila, const char* nombre) {
    AmbitoFuncion* ambito = getAmbitoActual(pila);
    if (!ambito) return NULL;
    
    for (int i = 0; i < ambito->numParametros; i++) {
        if (strcmp(ambito->parametros[i].nombre, nombre) == 0) {
            return &ambito->parametros[i];
        }
    }
    
    return NULL;
}

// ==================== ANÁLISIS DEL AST ====================

void extraerParametrosDesdeAST(NodoAST* parametrosNodo, ParametroInfo params[], int* numParams) {
    *numParams = 0;
    
    if (!parametrosNodo || parametrosNodo->tipo != AST_PARAMETROS) {
        printf("[DEBUG] No hay parámetros o nodo inválido\n");
        return;
    }
    
    printf("[DEBUG] Extrayendo parámetros, num_hijos=%d\n", parametrosNodo->num_hijos);
    
    for (int i = 0; i < parametrosNodo->num_hijos && *numParams < MAX_PARAMS; i++) {
        NodoAST* paramNodo = parametrosNodo->hijos[i];
        
        if (paramNodo->tipo != AST_PARAMETRO || paramNodo->num_hijos < 2) {
            printf("[ERROR] Nodo parámetro inválido\n");
            continue;
        }
        
        NodoAST* tipoNodo = paramNodo->hijos[0];
        NodoAST* idNodo = paramNodo->hijos[1];
        
        const char* tipoStr = tipoNodo->valor;
        const char* nombreParam = idNodo->valor;
        
        // Detectar si es vector o matriz
        bool esVector = (strstr(tipoStr, "[]") != NULL) && (strstr(tipoStr, "[][]") == NULL);
        bool esMatriz = (strstr(tipoStr, "[][]") != NULL);
        
        // Obtener tipo base
        TipoDato tipo = obtenerTipoDesdeString(tipoStr);
        
        // Agregar parámetro
        ParametroInfo* param = &params[*numParams];
        strncpy(param->nombre, nombreParam, MAX_NOMBRE - 1);
        param->nombre[MAX_NOMBRE - 1] = '\0';
        param->tipo = tipo;
        param->esVector = esVector;
        param->esMatriz = esMatriz;
        param->esPorReferencia = esTipoPorReferencias(tipo, esVector, esMatriz);
        
        (*numParams)++;
        
        printf("[DEBUG] Parámetro extraído: '%s' tipo='%s' (%d), vector=%d, matriz=%d\n",
               nombreParam, tipoStr, tipo, esVector, esMatriz);
    }
}


int calcularTamanioStackFrame(AmbitoFuncion* ambito) {
    int tamanio = 0;
    
    // ✅ SOLO espacio para parámetros (las variables locales NO están en stack)
    for (int i = 0; i < ambito->numParametros; i++) {
        ParametroInfo* param = &ambito->parametros[i];
        int tamParam = obtenerTamanioTipo(param->tipo);
        if (param->esVector || param->esMatriz) tamParam = 8;
        tamanio += (tamParam + 7) & ~7;
    }
    
    // ❌ NO agregamos tamanioStackFrame porque variables locales son globales
    
    // Alinear a 16 bytes
    tamanio = (tamanio + 15) & ~15;
    
    return tamanio;
}

int obtenerTamanioTipo(TipoDato tipo) {
    switch (tipo) {
        case TIPO_CHAR:
        case TIPO_BOOLEAN:
            return 1;
        case TIPO_INT:
        case TIPO_FLOAT:
            return 4;
        case TIPO_DOUBLE:
        case TIPO_STRING:
            return 8;
        default:
            return 8;
    }
}

// ==================== GENERACIÓN DE CÓDIGO ARM64 ====================

void generarNombreFuncion(const char* nombre, char* buffer, size_t bufferSize) {
    snprintf(buffer, bufferSize, "func_%s", nombre);
}

void generarPrologoFuncion(GeneradorDeclaraciones* gen, AmbitoFuncion* ambito) {
    char codigo[512];
    int frameSize = calcularTamanioStackFrame(ambito);
    
    // Guardar frame pointer y link register
    snprintf(codigo, sizeof(codigo),
        "    // Prólogo de función '%s'\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n",
        ambito->nombreFuncion);
    
    insertarFuncionAntesDeStart(gen, codigo);
    
    // Reservar espacio en el stack si es necesario
    if (frameSize > 0) {
        snprintf(codigo, sizeof(codigo),
            "    sub sp, sp, #%d\n",
            frameSize);
        insertarFuncionAntesDeStart(gen, codigo);
    }
}

void generarEpilogoFuncion(GeneradorDeclaraciones* gen, AmbitoFuncion* ambito) {
    char codigo[512];
    int frameSize = calcularTamanioStackFrame(ambito);
    
    snprintf(codigo, sizeof(codigo),
        "func_%s_exit:\n"
        "    // Epílogo de función '%s'\n",
        ambito->nombreFuncion, ambito->nombreFuncion);
    
    insertarFuncionAntesDeStart(gen, codigo);
    
    // Restaurar stack pointer
    if (frameSize > 0) {
        snprintf(codigo, sizeof(codigo),
            "    add sp, sp, #%d\n",
            frameSize);
        insertarFuncionAntesDeStart(gen, codigo);
    }
    
    // Restaurar frame pointer y retornar
    snprintf(codigo, sizeof(codigo),
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n\n");
    
    insertarFuncionAntesDeStart(gen, codigo);
}

void generarCargaParametros(GeneradorDeclaraciones* gen, AmbitoFuncion* ambito) {
    if (ambito->numParametros == 0) return;
    
    char codigo[512];
    
    insertarFuncionAntesDeStart(gen, "    // Cargar parámetros\n");
    
    // Los primeros 8 parámetros vienen en x0-x7
    // Si son por referencia, ya son direcciones
    // Si son por valor, necesitamos guardarlos en el stack
    
    int offset = 0;
    for (int i = 0; i < ambito->numParametros && i < 8; i++) {
        ParametroInfo* param = &ambito->parametros[i];
        
        if (param->esPorReferencia) {
            // Por referencia: guardar la dirección (puntero)
            snprintf(codigo, sizeof(codigo),
                "    str x%d, [sp, #%d]  // param '%s' (por referencia)\n",
                i, offset, param->nombre);
        } else {
            // Por valor: guardar el valor según el tipo
            if (param->tipo == TIPO_INT || param->tipo == TIPO_BOOLEAN) {
                snprintf(codigo, sizeof(codigo),
                    "    str w%d, [sp, #%d]  // param '%s' (int/bool)\n",
                    i, offset, param->nombre);
            } else if (param->tipo == TIPO_FLOAT) {
                snprintf(codigo, sizeof(codigo),
                    "    str s%d, [sp, #%d]  // param '%s' (float)\n",
                    i, offset, param->nombre);
            } else if (param->tipo == TIPO_DOUBLE) {
                snprintf(codigo, sizeof(codigo),
                    "    str d%d, [sp, #%d]  // param '%s' (double)\n",
                    i, offset, param->nombre);
            } else if (param->tipo == TIPO_CHAR) {
                snprintf(codigo, sizeof(codigo),
                    "    strb w%d, [sp, #%d]  // param '%s' (char)\n",
                    i, offset, param->nombre);
            }
        }
        
        insertarFuncionAntesDeStart(gen, codigo);
        
        int tamParam = obtenerTamanioTipo(param->tipo);
        if (param->esVector || param->esMatriz) tamParam = 8;
        offset += (tamParam + 7) & ~7;
    }
}

// ==================== PROCESAMIENTO DEL CUERPO ====================

void procesarCuerpoFuncion(GeneradorDeclaraciones* gen, NodoAST* cuerpoNodo,
                          PilaAmbitos* pilaAmbitos, NodoAST* raizAST) {
    if (!cuerpoNodo) return;
    
    AmbitoFuncion* ambito = getAmbitoActual(pilaAmbitos);
    if (!ambito) {
        printf("[ERROR] No hay ámbito actual\n");
        return;
    }
    
    printf("[DEBUG] Procesando cuerpo de función '%s'\n", ambito->nombreFuncion);
    
    gen->dentro_de_funcion = true;
    strncpy(gen->nombre_funcion_actual, ambito->nombreFuncion, sizeof(gen->nombre_funcion_actual) - 1);
    
    // Procesar cada hijo del cuerpo
    for (int i = 0; i < cuerpoNodo->num_hijos; i++) {
        NodoAST* nodo = cuerpoNodo->hijos[i];
        if (!nodo) continue;
        
        switch (nodo->tipo) {
            case AST_BLOQUE:
            case AST_LISTA:
                procesarCuerpoFuncion(gen, nodo, pilaAmbitos, raizAST);
                break;
                
            case AST_DECLARACION: {
                // ✅ DECLARAR VARIABLES LOCALES COMO GLOBALES (en .data/.bss)
                printf("[DEBUG] Declaración de variable local como global en función '%s'\n", 
                       ambito->nombreFuncion);
                
                // Generar un nombre único para evitar conflictos
                if (nodo->num_hijos >= 2) {
                    NodoAST* tipoNodo = nodo->hijos[0];
                    NodoAST* idNodo = nodo->hijos[1];
                    NodoAST* valorNodo = nodo->num_hijos > 2 ? nodo->hijos[2] : NULL;
                    
                    // Crear nombre único: nombreFuncion_nombreVariable
                    char nombreUnico[128];
                    snprintf(nombreUnico, sizeof(nombreUnico), "%s_%s", 
                            ambito->nombreFuncion, idNodo->valor);
                    
                    // Crear un nodo temporal con el nombre único
                    NodoAST nodoTemp = *nodo;
                    NodoAST idTemp = *idNodo;
                    idTemp.valor = nombreUnico;
                    nodoTemp.hijos[1] = &idTemp;
                    
                    // Usar la función de declaración global existente
                    bool dummy_data = true, dummy_bss = true;
                    generarDeclaracionVariable(gen, &nodoTemp, &dummy_data, &dummy_bss);
                    
                    // Registrar en el ámbito para referencia
                    TipoDato tipo = obtenerTipoDesdeString(tipoNodo->valor);
                    VariableLocal* var = &ambito->variables[ambito->numVariables];
                    strncpy(var->nombre, idNodo->valor, MAX_NOMBRE - 1);
                    var->nombre[MAX_NOMBRE - 1] = '\0';
                    var->tipo = tipo;
                    var->esVector = false;
                    var->esMatriz = false;
                    var->offset_stack = -1; // Marcar como NO en stack
                    ambito->numVariables++;
                }
                break;
            }
                
            case AST_PRINT:
                printf("[DEBUG] Generando PRINT dentro de función\n");
                generarImpresionARM64(gen, nodo);
                break;
                
            case AST_ASIGNACION:
                printf("[DEBUG] Generando ASIGNACION dentro de función\n");
                generarAsignacionARM64(gen, nodo);
                break;
                
            case AST_IF:
                printf("[DEBUG] Generando IF dentro de función\n");
                if (nodo->num_hijos == 2) {
                    generarCodigoIF(gen, nodo);
                } else if (nodo->num_hijos == 3) {
                    generarCodigoIFElse(gen, nodo);
                } else {
                    generarCodigoIFElseIF(gen, nodo);
                }
                break;
                
            case AST_FOR:
                printf("[DEBUG] Generando FOR dentro de función\n");
                generarCodigoFor(gen, nodo);
                break;
                
            case AST_WHILE:
                printf("[DEBUG] Generando WHILE dentro de función\n");
                generarCodigoWhile(gen, nodo);
                break;
                
            case AST_SWITCH:
                printf("[DEBUG] Generando SWITCH dentro de función\n");
                generarCodigoSwitch(gen, nodo);
                break;
                
            case AST_INCREMENTO:
            case AST_DECREMENTO:
                printf("[DEBUG] Generando INC/DEC dentro de función\n");
                break;
                
            case AST_ACCESO_ESCRITURA:
                printf("[DEBUG] Generando ACCESO_ESCRITURA dentro de función\n");
                generarEscrituraVectorARM64(gen, nodo);
                break;

            case AST_ASIGNACION_COMPUESTA:
                printf("[DEBUG] Generando ASIGNACION_COMPUESTA dentro de función\n");
                generarAsignacionCompuestaEnFuncion(gen, nodo, pilaAmbitos);
                break;
                
            default:
                printf("[DEBUG] Tipo de nodo no manejado: %d\n", nodo->tipo);
                break;
        }
    }
    
    gen->dentro_de_funcion = false;
    gen->nombre_funcion_actual[0] = '\0';
}


void generarDeclaracionFuncionVoid(GeneradorDeclaraciones* gen, NodoAST* nodo,
                                  PilaAmbitos* pilaAmbitos, NodoAST* raizAST) {
    if (!gen || !nodo || !pilaAmbitos) {
        printf("[ERROR] Parámetros inválidos en generarDeclaracionFuncionVoid\n");
        return;
    }
    
    if (nodo->tipo != AST_FUNCION_VOID || nodo->num_hijos < 3) {
        printf("[ERROR] Nodo no es función void o estructura inválida\n");
        return;
    }
    
    NodoAST* idNodo = nodo->hijos[0];
    NodoAST* parametrosNodo = nodo->hijos[1];
    NodoAST* cuerpoNodo = nodo->hijos[2];
    
    const char* nombreFuncion = idNodo->valor;
    
    printf("[DEBUG VOID] Generando función void: '%s'\n", nombreFuncion);
    
    // Crear ámbito de la función
    pushAmbito(pilaAmbitos, nombreFuncion, true);
    
    // Extraer y agregar parámetros al ámbito
    ParametroInfo params[MAX_PARAMS];
    int numParams = 0;
    extraerParametrosDesdeAST(parametrosNodo, params, &numParams);
    
    for (int i = 0; i < numParams; i++) {
        agregarParametroAmbito(pilaAmbitos, params[i].nombre, params[i].tipo,
                              params[i].esVector, params[i].esMatriz);
    }
    
    AmbitoFuncion* ambito = getAmbitoActual(pilaAmbitos);

    gen->buffer_funcion_len = 0;
    gen->buffer_funcion[0] = '\0';
    
    // ✅ MARCAR QUE ESTAMOS EN UNA FUNCIÓN
    gen->dentro_de_funcion = true;
    strncpy(gen->nombre_funcion_actual, nombreFuncion, sizeof(gen->nombre_funcion_actual) - 1);
    gen->nombre_funcion_actual[sizeof(gen->nombre_funcion_actual) - 1] = '\0';
    
    // Generar etiqueta de la función
    char codigo[546];
    char nombreFunc[128];
    generarNombreFuncion(nombreFuncion, nombreFunc, sizeof(nombreFunc));
    
    snprintf(codigo, sizeof(codigo),
        "// ==================== FUNCIÓN VOID: %s ====================\n"
        ".global %s\n"
        "%s:\n",
        nombreFuncion, nombreFunc, nombreFunc);
    
    insertarFuncionAntesDeStart(gen, codigo);
    
    // Generar prólogo
    generarPrologoFuncion(gen, ambito);
    
    // Cargar parámetros desde registros al stack
    generarCargaParametros(gen, ambito);
    
    // ✅ PROCESAR EL CUERPO DE LA FUNCIÓN
    procesarCuerpoFuncion(gen, cuerpoNodo, pilaAmbitos, raizAST);
    
    // Generar epílogo
    generarEpilogoFuncion(gen, ambito);

    if (gen->buffer_funcion_len > 0) {
        char epilogo_marker[256];
        snprintf(epilogo_marker, sizeof(epilogo_marker), "func_%s_exit:\n", nombreFuncion);
        char* pos = strstr(gen->buffer, epilogo_marker);
        if (pos) {
            size_t offset = pos - gen->buffer;
            size_t len_actual = strlen(gen->buffer);
            size_t len_codigo = gen->buffer_funcion_len;

            if (len_actual + len_codigo + 1 >= gen->buffer_size) {
                gen->buffer_size = len_actual + len_codigo + 2048;
                gen->buffer = realloc(gen->buffer, gen->buffer_size);
                pos = gen->buffer + offset;
            }
            memmove(pos + len_codigo, pos, len_actual - offset + 1);
            memcpy(pos, gen->buffer_funcion, len_codigo);
        }
        gen->buffer_funcion_len = 0;
        gen->buffer_funcion[0] = '\0';
    }
    
    // ✅ DESMARCAR AL FINALIZAR
    gen->dentro_de_funcion = false;
    gen->nombre_funcion_actual[0] = '\0';
    
    // Salir del ámbito
    popAmbito(pilaAmbitos);
    
    printf("[DEBUG VOID] Función '%s' generada exitosamente\n", nombreFuncion);
}

