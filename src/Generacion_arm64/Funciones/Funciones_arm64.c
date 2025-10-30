#include "Funciones_arm64.h"
#include "../Impresion_arm64.h"
#include "../InstruccionesNorm_arm64/Asignacion_arm64.h"
#include "../InstruccionesSent_arm64/If_arm64.h"
#include "../InstruccionesSent_arm64/for_arm64.h"
#include "../InstruccionesSent_arm64/while_arm64.h"
#include "../InstruccionesSent_arm64/Switch_arm64.h"
#include "../Vectores_arm64/EscrituraVector_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include "../../semantico/tipos.h"
#include "../generador_arm64.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ==================== GESTIÓN DE ÁMBITOS PARA RETORNO ====================

void inicializarPilaAmbitosRetorno(PilaAmbitosRetorno* pila) {
    pila->tope = -1;
    memset(pila->ambitos, 0, sizeof(pila->ambitos));
}

void pushAmbitoRetorno(PilaAmbitosRetorno* pila, const char* nombreFuncion, TipoDato tipoRetorno) {
    if (pila->tope >= MAX_AMBITOS - 1) {
        printf("[ERROR] Desbordamiento de pila de ámbitos de retorno\n");
        return;
    }
    
    pila->tope++;
    AmbitoFuncionRetorno* ambito = &pila->ambitos[pila->tope];
    
    // Inicializar ambito base
    strncpy(ambito->ambito_base.nombreFuncion, nombreFuncion, MAX_NOMBRE - 1);
    ambito->ambito_base.nombreFuncion[MAX_NOMBRE - 1] = '\0';
    ambito->ambito_base.numParametros = 0;
    ambito->ambito_base.numVariables = 0;
    ambito->ambito_base.tamanioStackFrame = 0;
    ambito->ambito_base.esVoid = false;
    
    // Inicializar información de retorno
    ambito->tipoRetorno = tipoRetorno;
    ambito->tieneReturn = false;
    generarEtiquetaRetorno(nombreFuncion, ambito->etiquetaRetorno, sizeof(ambito->etiquetaRetorno));
    
    printf("[DEBUG AMBITO RETORNO] Push: '%s' (tipo retorno=%d), tope=%d\n", 
           nombreFuncion, tipoRetorno, pila->tope);
}

AmbitoFuncionRetorno* popAmbitoRetorno(PilaAmbitosRetorno* pila) {
    if (pila->tope < 0) {
        printf("[ERROR] Intento de pop en pila vacía de retorno\n");
        return NULL;
    }
    
    AmbitoFuncionRetorno* ambito = &pila->ambitos[pila->tope];
    printf("[DEBUG AMBITO RETORNO] Pop: '%s', tope=%d->%d\n", 
           ambito->ambito_base.nombreFuncion, pila->tope, pila->tope - 1);
    
    pila->tope--;
    return ambito;
}

AmbitoFuncionRetorno* getAmbitoRetornoActual(PilaAmbitosRetorno* pila) {
    if (pila->tope < 0) return NULL;
    return &pila->ambitos[pila->tope];
}

// ==================== UTILIDADES ====================

const char* obtenerRegistroRetorno(TipoDato tipo) {
    switch (tipo) {
        case TIPO_DOUBLE:
            return "d0";
        case TIPO_FLOAT:
            return "s0";
        case TIPO_STRING:
            return "x0";
        case TIPO_INT:
        case TIPO_CHAR:
        case TIPO_BOOLEAN:
        default:
            return "w0";
    }
}

void generarEtiquetaRetorno(const char* nombreFunc, char* buffer, size_t bufferSize) {
    snprintf(buffer, bufferSize, ".L_return_%s", nombreFunc);
}

// ==================== GENERACIÓN DE CÓDIGO ====================

void generarPrologoFuncionRetorno(GeneradorDeclaraciones* gen, AmbitoFuncionRetorno* ambito) {
    char codigo[512];
    
    // Calcular tamaño SOLO para parámetros
    int frameSize = 0;
    for (int i = 0; i < ambito->ambito_base.numParametros; i++) {
        int tamParam = obtenerTamanioTipo(ambito->ambito_base.parametros[i].tipo);
        if (ambito->ambito_base.parametros[i].esVector || ambito->ambito_base.parametros[i].esMatriz) {
            tamParam = 8;
        }
        frameSize += (tamParam + 7) & ~7;
    }
    
    frameSize += 64; 
    // Alinear a 16 bytes
    frameSize = (frameSize + 15) & ~15;
    
    // Guardar el tamaño calculado
    ambito->ambito_base.tamanioStackFrame = frameSize;
    
    // Guardar frame pointer y link register
    snprintf(codigo, sizeof(codigo),
        "    // Prólogo de función '%s' (retorna %s)\n"
        "    stp x29, x30, [sp, #-16]!\n"
        "    mov x29, sp\n",
        ambito->ambito_base.nombreFuncion, obtenerNombreTipo(ambito->tipoRetorno));
    
    insertarFuncionAntesDeStart(gen, codigo);
    
    // Reservar espacio en el stack solo si hay parámetros
    if (frameSize > 0) {
        snprintf(codigo, sizeof(codigo),
            "    sub sp, sp, #%d\n",
            frameSize);
        insertarFuncionAntesDeStart(gen, codigo);
    }
}

void generarEpilogoFuncionRetorno(GeneradorDeclaraciones* gen, AmbitoFuncionRetorno* ambito) {
    char codigo[512];
    
    // Etiqueta de salida
    snprintf(codigo, sizeof(codigo),
        "%s:\n"
        "    // Epílogo de función '%s'\n",
        ambito->etiquetaRetorno, ambito->ambito_base.nombreFuncion);
    
    insertarFuncionAntesDeStart(gen, codigo);
    
    // Restaurar stack pointer
    if (ambito->ambito_base.tamanioStackFrame > 0) {
        snprintf(codigo, sizeof(codigo),
            "    add sp, sp, #%d\n",
            ambito->ambito_base.tamanioStackFrame);
        insertarFuncionAntesDeStart(gen, codigo);
    }
    
    // Restaurar frame pointer y retornar
    snprintf(codigo, sizeof(codigo),
        "    ldp x29, x30, [sp], #16\n"
        "    ret\n\n");
    
    insertarFuncionAntesDeStart(gen, codigo);
}

// NUEVA FUNCIÓN: Buscar variable local o parámetro
const char* generarCodigoAccesoVariable(AmbitoFuncionRetorno* ambito, const char* nombreVar, 
                                       char* codigo, size_t codigoSize, int* esLocal) {
    *esLocal = 0;
    
    printf("[DEPURACIÓN acceso de variables] Buscando variable '%s' en función '%s'\n", 
           nombreVar, ambito->ambito_base.nombreFuncion);
    // 1. Buscar en parámetros
    for (int i = 0; i < ambito->ambito_base.numParametros; i++) {
        if (strcmp(ambito->ambito_base.parametros[i].nombre, nombreVar) == 0) {
            *esLocal = 1;
            ParametroInfo* param = &ambito->ambito_base.parametros[i];
            int offset = i * 8; // Simplificado

            printf("[DEPURACIÓN acceso de variables] Variable encontrada en parámetros: '%s'\n", nombreVar);

            if (param->tipo == TIPO_INT || param->tipo == TIPO_BOOLEAN) {
                snprintf(codigo, codigoSize, "    ldr w1, [sp, #%d]  // param '%s'\n", offset, nombreVar);
                printf("[DEPURACIÓN acceso de variables] Cargando parámetro entero/booleano: '%s'\n", nombreVar);
                return "w1";
            } else if (param->tipo == TIPO_DOUBLE) {
                snprintf(codigo, codigoSize, "    ldr d0, [sp, #%d]  // param '%s'\n", offset, nombreVar);
                return "d0";
            } else if (param->tipo == TIPO_FLOAT) {
                snprintf(codigo, codigoSize, "    ldr s0, [sp, #%d]  // param '%s'\n", offset, nombreVar);
                return "s0";
            }

            printf("[DEPURACIÓN acceso de variables] Parámetro es String o referencia: '%s'\n", nombreVar);
            // Strings y referencias
            snprintf(codigo, codigoSize, "    ldr x1, [sp, #%d]  // param '%s'\n", offset, nombreVar);
            return "x1";
        }
    }

    printf("[DEPURACIÓN acceso de variables] No es parámetro, buscando en variables locales\n");
    
    // 2. Buscar en variables locales
    for (int i = 0; i < ambito->ambito_base.numVariables; i++) {
        if (strcmp(ambito->ambito_base.variables[i].nombre, nombreVar) == 0) {
            *esLocal = 1;
            VariableLocal* var = &ambito->ambito_base.variables[i];
            
            if (var->tipo == TIPO_INT || var->tipo == TIPO_BOOLEAN) {
                snprintf(codigo, codigoSize, "    ldr w1, [sp, #%d]  // local '%s'\n", var->offset_stack, nombreVar);
                return "w1";
            } else if (var->tipo == TIPO_DOUBLE) {
                snprintf(codigo, codigoSize, "    ldr d0, [sp, #%d]  // local '%s'\n", var->offset_stack, nombreVar);
                return "d0";
            } else if (var->tipo == TIPO_FLOAT) {
                snprintf(codigo, codigoSize, "    ldr s0, [sp, #%d]  // local '%s'\n", var->offset_stack, nombreVar);
                return "s0";
            }
            snprintf(codigo, codigoSize, "    ldr x1, [sp, #%d]  // local '%s'\n", var->offset_stack, nombreVar);
            return "x1";
        }
    }
    
    // 3. Si no es local, es variable global
    snprintf(codigo, codigoSize, "    ldr x1, =%s\n    ldr w1, [x1]\n", nombreVar);
    return "w1";
}

void generarCargaParametrosRetorno(GeneradorDeclaraciones* gen, AmbitoFuncionRetorno* ambito) {
    if (ambito->ambito_base.numParametros == 0) return;
    
    char codigo[512];
    
    insertarFuncionAntesDeStart(gen, "    // Cargar parámetros\n");
    
    // Parámetros en registros x0-x7 (o w0-w7, s0-s7, d0-d7 según tipo)
    int offset = 0;
    for (int i = 0; i < ambito->ambito_base.numParametros && i < 8; i++) {
        ParametroInfo* param = &ambito->ambito_base.parametros[i];

        printf("[DEPURACIÓN PARAM] Guardando parámetro '%s' en offset %d, tipo=%d\n",
               param->nombre, offset, param->tipo);
        
        if (param->esPorReferencia) {
            // Por referencia: guardar el puntero
            snprintf(codigo, sizeof(codigo),
                "    str x%d, [sp, #%d]  // param '%s' (por referencia)\n",
                i, offset, param->nombre);
        } else {
            // Por valor: guardar según el tipo
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

void generarReturnStatement(GeneradorDeclaraciones* gen, NodoAST* returnNodo,
                           AmbitoFuncionRetorno* ambito) {
    if (!returnNodo || returnNodo->tipo != AST_RETURN) {
        printf("[ERROR] Nodo return inválido\n");
        return;
    }
    
    char codigo[2048];
    ambito->tieneReturn = true;
    
    if (returnNodo->num_hijos == 0) {
        snprintf(codigo, sizeof(codigo),
            "    // Return vacío\n"
            "    b %s\n",
            ambito->etiquetaRetorno);
        insertarEnSeccionText(gen, codigo); // ✅ CAMBIO AQUÍ
        return;
    }
    
    NodoAST* exprRetorno = returnNodo->hijos[0];
    
    printf("[DEBUG RETURN] Evaluando expresión de retorno en función '%s'\n",
           ambito->ambito_base.nombreFuncion);
    printf("[DEBUG RETURN] Tipo de expresión: %d, valor: %s\n",
           exprRetorno->tipo, exprRetorno->valor ? exprRetorno->valor : "(null)");
    
    // ✅ CASO: OPERACIÓN (como a + b)
    if (exprRetorno->tipo == AST_OPERACION) {
        printf("[DEBUG RETURN] Expresión es una operación: %s\n", exprRetorno->valor);
        
        if (strcmp(exprRetorno->valor, "+") == 0 && exprRetorno->num_hijos == 2) {
            NodoAST* izq = exprRetorno->hijos[0];
            NodoAST* der = exprRetorno->hijos[1];
            
            if (izq->tipo == AST_IDENTIFICADOR && der->tipo == AST_IDENTIFICADOR) {
                int offsetIzq = -1, offsetDer = -1;
                
                for (int i = 0; i < ambito->ambito_base.numParametros; i++) {
                    if (strcmp(ambito->ambito_base.parametros[i].nombre, izq->valor) == 0) {
                        offsetIzq = i * 8;
                    }
                    if (strcmp(ambito->ambito_base.parametros[i].nombre, der->valor) == 0) {
                        offsetDer = i * 8;
                    }
                }
                
                if (offsetIzq >= 0 && offsetDer >= 0) {
                    snprintf(codigo, sizeof(codigo),
                        "    // Retornar a + b\n"
                        "    ldr w1, [sp, #%d]  // cargar '%s'\n"
                        "    ldr w2, [sp, #%d]  // cargar '%s'\n"
                        "    add w0, w1, w2\n"
                        "    b %s\n",
                        offsetIzq, izq->valor, offsetDer, der->valor, ambito->etiquetaRetorno);
                    insertarEnSeccionText(gen, codigo); // ✅ CAMBIO AQUÍ
                    return;
                }
            }
        }
        
        int regIndex = 1;
        ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, exprRetorno, &regIndex);
        insertarEnSeccionText(gen, resultado.codigo); // ✅ CAMBIO AQUÍ
        
        const char* regRetorno = obtenerRegistroRetorno(ambito->tipoRetorno);
        if (strcmp(resultado.resultado, regRetorno) != 0) {
            snprintf(codigo, sizeof(codigo),
                "    mov %s, %s\n"
                "    b %s\n", regRetorno, resultado.resultado, ambito->etiquetaRetorno);
        } else {
            snprintf(codigo, sizeof(codigo),
                "    b %s\n", ambito->etiquetaRetorno);
        }
        insertarEnSeccionText(gen, codigo); // ✅ CAMBIO AQUÍ
        return;
    }
    
    if (exprRetorno->tipo == AST_IDENTIFICADOR) {
        const char* nombreVar = exprRetorno->valor;
        
        // Verificar si es parámetro
        for (int i = 0; i < ambito->ambito_base.numParametros; i++) {
            if (strcmp(ambito->ambito_base.parametros[i].nombre, nombreVar) == 0) {
                int offset = i * 8;
                TipoDato tipoParam = ambito->ambito_base.parametros[i].tipo;
                
                if (tipoParam == TIPO_INT || tipoParam == TIPO_BOOLEAN) {
                    snprintf(codigo, sizeof(codigo),
                        "    // Retornar parámetro '%s'\n"
                        "    ldr w0, [sp, #%d]\n"
                        "    b %s\n",
                        nombreVar, offset, ambito->etiquetaRetorno);
                }
                insertarEnSeccionText(gen, codigo); // ✅ CAMBIO AQUÍ
                return;
            }
        }
        
        // Variable local o global
        bool esLocal = false;
        for (int i = 0; i < ambito->ambito_base.numVariables; i++) {
            if (strcmp(ambito->ambito_base.variables[i].nombre, nombreVar) == 0) {
                esLocal = true;
                break;
            }
        }
        
        if (esLocal) {
            printf("[DEBUG RETURN] Retornando variable local '%s' (sin prefijo)\n", nombreVar);
        } else {
            printf("[DEBUG RETURN] Retornando variable global '%s'\n", nombreVar);
        }
        
        snprintf(codigo, sizeof(codigo),
            "    // Retornar variable '%s'\n"
            "    ldr x1, =%s\n"
            "    ldr w0, [x1]\n"
            "    b %s\n",
            nombreVar, nombreVar, ambito->etiquetaRetorno);
        insertarEnSeccionText(gen, codigo); // ✅ CAMBIO AQUÍ
        return;
    }
    
    // Caso general
    int regIndex = 1;
    ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, exprRetorno, &regIndex);
    insertarEnSeccionText(gen, resultado.codigo); // ✅ CAMBIO AQUÍ
    
    const char* regRetorno = obtenerRegistroRetorno(ambito->tipoRetorno);
    if (strcmp(resultado.resultado, regRetorno) != 0) {
        snprintf(codigo, sizeof(codigo),
            "    mov %s, %s\n", regRetorno, resultado.resultado);
        insertarEnSeccionText(gen, codigo); // ✅ CAMBIO AQUÍ
    }
    
    snprintf(codigo, sizeof(codigo), "    b %s\n", ambito->etiquetaRetorno);
    insertarEnSeccionText(gen, codigo); // ✅ CAMBIO AQUÍ
}

void procesarCuerpoFuncionRetorno(GeneradorDeclaraciones* gen, NodoAST* cuerpoNodo,
                                  PilaAmbitosRetorno* pilaRetorno, NodoAST* raizAST) {
    AmbitoFuncionRetorno* ambito = getAmbitoRetornoActual(pilaRetorno);
    if (!ambito) return;

    // ✅ MARCAR QUE ESTAMOS EN UNA FUNCIÓN (para que insertarEnSeccionText use buffer_funcion)
    gen->dentro_de_funcion = true;
    strncpy(gen->nombre_funcion_actual, ambito->ambito_base.nombreFuncion, 
            sizeof(gen->nombre_funcion_actual) - 1);

    // ✅ SI EL NODO ES DIRECTAMENTE UN RETURN, GUARDARLO PARA PROCESARLO AL FINAL
    if (cuerpoNodo->tipo == AST_RETURN) {
        printf("[DEBUG RETURN] Return directo detectado, se procesará al final\n");
        // Procesar al final, después de las declaraciones
        generarReturnStatement(gen, cuerpoNodo, ambito);
        gen->dentro_de_funcion = false;
        gen->nombre_funcion_actual[0] = '\0';
        return;
    }

    // ✅ ALMACENAR EL NODO RETURN PARA PROCESARLO AL FINAL
    NodoAST* nodoReturn = NULL;

    for (int i = 0; i < cuerpoNodo->num_hijos; i++) {
        NodoAST* nodo = cuerpoNodo->hijos[i];
        if (!nodo) continue;

        printf("[DEBUG] Procesando nodo tipo %d en cuerpo de función '%s'\n", 
               nodo->tipo, ambito->ambito_base.nombreFuncion);

        switch (nodo->tipo) {
            case AST_BLOQUE:
            case AST_LISTA:
                procesarCuerpoFuncionRetorno(gen, nodo, pilaRetorno, raizAST);
                break;

            case AST_DECLARACION: {
                printf("[DEBUG FUNCION] Declaración de variable local en función '%s'\n", 
                    ambito->ambito_base.nombreFuncion);
                
                if (nodo->num_hijos >= 2) {
                    NodoAST* tipoNodo = nodo->hijos[0];
                    NodoAST* idNodo = nodo->hijos[1];
                    NodoAST* valorNodo = nodo->num_hijos > 2 ? nodo->hijos[2] : NULL;
                    
                    printf("[DEBUG FUNCION] Variable: '%s', tipo: '%s', tiene valor: %s\n",
                        idNodo->valor, tipoNodo->valor, valorNodo ? "SÍ" : "NO");
                    
                    // ✅ SI TIENE VALOR Y ES OPERACIÓN, PROCESARLA PRIMERO
                    if (valorNodo && valorNodo->tipo == AST_OPERACION) {
                        printf("[DEBUG FUNCION] Valor es una operación: '%s'\n", valorNodo->valor);
                        
                        // ✅ GENERAR DECLARACIÓN (esto irá a buffer_funcion automáticamente)
                        bool dummy_data = true, dummy_bss = true;
                        generarDeclaracionVariable(gen, nodo, &dummy_data, &dummy_bss);
                    } else {
                        // ✅ GENERAR DECLARACIÓN NORMAL
                        bool dummy_data = true, dummy_bss = true;
                        generarDeclaracionVariable(gen, nodo, &dummy_data, &dummy_bss);
                    }
                    
                    TipoDato tipo = obtenerTipoDesdeString(tipoNodo->valor);
                    if (ambito->ambito_base.numVariables < 50) {
                        VariableLocal* var = &ambito->ambito_base.variables[ambito->ambito_base.numVariables];
                        strncpy(var->nombre, idNodo->valor, MAX_NOMBRE - 1);
                        var->nombre[MAX_NOMBRE - 1] = '\0';
                        var->tipo = tipo;
                        var->esVector = false;
                        var->esMatriz = false;
                        var->offset_stack = -1;
                        ambito->ambito_base.numVariables++;
                        
                        printf("[DEBUG FUNCION] Variable local registrada: '%s' (total: %d)\n",
                            var->nombre, ambito->ambito_base.numVariables);
                    }
                }
                break;

            }

            case AST_RETURN:
                // ✅ GUARDAR EL NODO RETURN PARA PROCESARLO AL FINAL
                printf("[DEBUG RETURN] Return encontrado, se procesará al final del cuerpo\n");
                nodoReturn = nodo;
                break;

            case AST_ASIGNACION_COMPUESTA:
                generarAsignacionCompuestaEnFuncionRetorno(gen, nodo, pilaRetorno);
                break;

            case AST_PRINT:
                generarImpresionARM64(gen, nodo);
                break;

            case AST_ASIGNACION:
                generarAsignacionARM64(gen, nodo);
                break;

            case AST_IF:
                if (nodo->num_hijos == 2) generarCodigoIF(gen, nodo);
                else if (nodo->num_hijos == 3) generarCodigoIFElse(gen, nodo);
                else generarCodigoIFElseIF(gen, nodo);
                break;

            case AST_FOR:
                generarCodigoFor(gen, nodo);
                break;

            case AST_WHILE:
                generarCodigoWhile(gen, nodo);
                break;

            case AST_SWITCH:
                generarCodigoSwitch(gen, nodo);
                break;

            case AST_ACCESO_ESCRITURA:
                generarEscrituraVectorARM64(gen, nodo);
                break;

            case AST_INCREMENTO:
            case AST_DECREMENTO:
                break;

            default:
                printf("[DEBUG] Tipo no manejado en cuerpo de función: %d\n", nodo->tipo);
                break;
        }
    }

    // ✅ PROCESAR EL RETURN AL FINAL (después de todas las declaraciones)
    if (nodoReturn) {
        printf("[DEBUG RETURN] Procesando return al final del cuerpo de función '%s'\n", 
               ambito->ambito_base.nombreFuncion);
        generarReturnStatement(gen, nodoReturn, ambito);
    }

    // ✅ DESMARCAR que estamos en función
    gen->dentro_de_funcion = false;
    gen->nombre_funcion_actual[0] = '\0';
}

void generarDeclaracionFuncionRetorno(GeneradorDeclaraciones* gen, NodoAST* nodo,
                                      PilaAmbitosRetorno* pilaRetorno, NodoAST* raizAST) {
    if (!gen || !nodo || !pilaRetorno) {
        printf("[ERROR] Parámetros inválidos en generarDeclaracionFuncionRetorno\n");
        return;
    }
    
    if (nodo->tipo != AST_FUNCION || nodo->num_hijos < 4) {
        printf("[ERROR] Nodo no es función con retorno o estructura inválida\n");
        return;
    }
    
    NodoAST* tipoRetornoNodo = nodo->hijos[0];
    NodoAST* idNodo = nodo->hijos[1];
    NodoAST* parametrosNodo = nodo->hijos[2];
    NodoAST* cuerpoNodo = nodo->hijos[3];
    
    const char* nombreFuncion = idNodo->valor;
    TipoDato tipoRetorno = obtenerTipoDesdeString(tipoRetornoNodo->valor);
    
    printf("[DEBUG RETORNO] Generando función: '%s' -> %s\n", 
           nombreFuncion, obtenerNombreTipo(tipoRetorno));
    
    // Crear ámbito de la función
    pushAmbitoRetorno(pilaRetorno, nombreFuncion, tipoRetorno);
    
    // Extraer y agregar parámetros al ámbito
    ParametroInfo params[MAX_PARAMS];
    int numParams = 0;
    extraerParametrosDesdeAST(parametrosNodo, params, &numParams);
    
    AmbitoFuncionRetorno* ambito = getAmbitoRetornoActual(pilaRetorno);
    for (int i = 0; i < numParams; i++) {
        ambito->ambito_base.parametros[i] = params[i];
    }
    ambito->ambito_base.numParametros = numParams;
    
    // ✅ Recalcular tamaño del frame (SOLO parámetros, NO variables locales)
    ambito->ambito_base.tamanioStackFrame = 0;
    for (int i = 0; i < numParams; i++) {
        int tamParam = obtenerTamanioTipo(params[i].tipo);
        if (params[i].esVector || params[i].esMatriz) tamParam = 8;
        ambito->ambito_base.tamanioStackFrame += (tamParam + 7) & ~7;
    }
    
    // Limpiar buffer de función
    gen->buffer_funcion_len = 0;
    gen->buffer_funcion[0] = '\0';
    
    // Marcar que estamos en una función
    gen->dentro_de_funcion = true;
    strncpy(gen->nombre_funcion_actual, nombreFuncion, sizeof(gen->nombre_funcion_actual) - 1);
    gen->nombre_funcion_actual[sizeof(gen->nombre_funcion_actual) - 1] = '\0';
    
    // Generar etiqueta de la función
    char codigo[512];
    char nombreFunc[128];
    generarNombreFuncion(nombreFuncion, nombreFunc, sizeof(nombreFunc));
    
    snprintf(codigo, sizeof(codigo),
        "// ==================== FUNCIÓN: %s -> %s ====================\n"
        ".global %s\n"
        "%s:\n",
        nombreFuncion, obtenerNombreTipo(tipoRetorno), nombreFunc, nombreFunc);
    
    insertarFuncionAntesDeStart(gen, codigo);
    
    // Generar prólogo
    generarPrologoFuncionRetorno(gen, ambito);
    
    // Cargar parámetros
    generarCargaParametrosRetorno(gen, ambito);

    // ✅ LIMPIAR BUFFER DE FUNCIÓN ANTES DE PROCESAR EL CUERPO
    gen->buffer_funcion_len = 0;
    gen->buffer_funcion[0] = '\0';
    
    // Procesar el cuerpo
    procesarCuerpoFuncionRetorno(gen, cuerpoNodo, pilaRetorno, raizAST);

    // ✅ INSERTAR EL BUFFER DE FUNCIÓN **ANTES** DEL EPÍLOGO
    if (gen->buffer_funcion_len > 0) {
        // El buffer_funcion contiene:
        // 1. Código de declaraciones (como L = a + b)
        // 2. Código de return (salto a .L_return_suma)
        
        // Insertamos TODO antes de generar el epílogo
        insertarFuncionAntesDeStart(gen, gen->buffer_funcion);
        
        gen->buffer_funcion_len = 0;
        gen->buffer_funcion[0] = '\0';
    }
    
    // Validar que tenga al menos un return
    if (!ambito->tieneReturn) {
        printf("[WARNING] Función '%s' sin return explícito\n", nombreFuncion);
        // Generar return por defecto
        char codigoDefault[256];
        const char* regRetorno = obtenerRegistroRetorno(tipoRetorno);
        
        if (tipoRetorno == TIPO_INT || tipoRetorno == TIPO_BOOLEAN || tipoRetorno == TIPO_CHAR) {
            snprintf(codigoDefault, sizeof(codigoDefault),
                "    // Return por defecto\n"
                "    mov %s, #0\n", regRetorno);
        } else if (tipoRetorno == TIPO_FLOAT) {
            snprintf(codigoDefault, sizeof(codigoDefault),
                "    // Return por defecto\n"
                "    fmov %s, wzr\n", regRetorno);
        } else if (tipoRetorno == TIPO_DOUBLE) {
            snprintf(codigoDefault, sizeof(codigoDefault),
                "    // Return por defecto\n"
                "    fmov %s, xzr\n", regRetorno);
        } else if (tipoRetorno == TIPO_STRING) {
            snprintf(codigoDefault, sizeof(codigoDefault),
                "    // Return por defecto\n"
                "    mov %s, #0\n", regRetorno);
        }
        insertarFuncionAntesDeStart(gen, codigoDefault);
    }
    
    // Generar epílogo
    generarEpilogoFuncionRetorno(gen, ambito);
    
    // Insertar buffer de función antes del epílogo
    if (gen->buffer_funcion_len > 0) {
        char epilogo_marker[256];
        snprintf(epilogo_marker, sizeof(epilogo_marker), "%s:\n", ambito->etiquetaRetorno);
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
    
    // Desmarcar
    gen->dentro_de_funcion = false;
    gen->nombre_funcion_actual[0] = '\0';
    
    // Salir del ámbito
    popAmbitoRetorno(pilaRetorno);
    
    printf("[DEBUG RETORNO] Función '%s' generada exitosamente\n", nombreFuncion);
}