#include "LlamadasVoid_arm64.h"
#include "../../semantico/tipos.h"
#include "../generador_arm64.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern NodoAST* astGlobal;

// ==================== BÚSQUEDA DE FUNCIONES EN EL AST ====================

NodoAST* buscarFuncionVoidEnAST(NodoAST* raiz, const char* nombreFuncion) {
    if (!raiz || !nombreFuncion) return NULL;
    
    // Si es una función void y el nombre coincide
    if (raiz->tipo == AST_FUNCION_VOID && raiz->num_hijos >= 1) {
        NodoAST* idNodo = raiz->hijos[0];
        if (idNodo && idNodo->tipo == AST_IDENTIFICADOR && 
            strcmp(idNodo->valor, nombreFuncion) == 0) {
            printf("[DEBUG BUSQUEDA] Función void '%s' encontrada\n", nombreFuncion);
            return raiz;
        }
    }
    
    // Buscar recursivamente en los hijos
    for (int i = 0; i < raiz->num_hijos; i++) {
        NodoAST* resultado = buscarFuncionVoidEnAST(raiz->hijos[i], nombreFuncion);
        if (resultado) return resultado;
    }
    
    return NULL;
}

bool verificarArgumentosConParametros(ArgumentoLlamada args[], int numArgs, 
                                     ParametroInfo params[], int numParams) {
    if (numArgs != numParams) {
        printf("[ERROR] Número de argumentos (%d) no coincide con parámetros (%d)\n", 
               numArgs, numParams);
        return false;
    }
    
    for (int i = 0; i < numArgs; i++) {
        // Verificar que los tipos sean compatibles
        if (args[i].tipo != params[i].tipo) {
            printf("[ERROR] Tipo del argumento %d (%d) no coincide con parámetro (%d)\n",
                   i, args[i].tipo, params[i].tipo);
            return false;
        }
        
        // Verificar que vectores/matrices coincidan
        if (args[i].esVector != params[i].esVector || 
            args[i].esMatriz != params[i].esMatriz) {
            printf("[ERROR] Dimensionalidad del argumento %d no coincide\n", i);
            return false;
        }
    }
    
    return true;
}

// ==================== EXTRACCIÓN DE ARGUMENTOS ====================

TipoDato determinarTipoArgumento(NodoAST* argNodo, NodoAST* raizAST,
                                bool* esVector, bool* esMatriz) {
    *esVector = false;
    *esMatriz = false;
    
    if (!argNodo) return TIPO_DESCONOCIDO;
    
    switch (argNodo->tipo) {
        case AST_NUMERO:
            if (strchr(argNodo->valor, '.')) {
                return TIPO_DOUBLE;
            }
            return TIPO_INT;
            
        case AST_CADENA:
        case AST_STRING:
            return TIPO_STRING;
            
        case AST_CARACTER:
            return TIPO_CHAR;
            
        case AST_BOOLEAN:
            return TIPO_BOOLEAN;
            
        case AST_IDENTIFICADOR: {
            // Buscar en el AST para determinar el tipo
            TipoDato tipo = deducirTipoExprARM64(argNodo, raizAST);
            
            // Verificar si es un vector o matriz
            NodoAST* declVector = buscarDeclaracionPorIdentificador(raizAST, AST_DECLARACION_VECTOR, argNodo->valor);
            if (declVector) {
                *esVector = true;
                return tipo;
            }
            
            NodoAST* declMatriz = buscarDeclaracionPorIdentificador(raizAST, AST_DECLARACION_MATRIZ, argNodo->valor);
            if (declMatriz) {
                *esMatriz = true;
                return tipo;
            }
            
            return tipo;
        }
            
        default:
            return deducirTipoExprARM64(argNodo, raizAST);
    }
}

void extraerArgumentosDesdeAST(NodoAST* argumentosNodo, ArgumentoLlamada args[],
                              int* numArgs, NodoAST* raizAST) {
    *numArgs = 0;
    
    if (!argumentosNodo) {
        printf("[DEBUG] No hay argumentos\n");
        return;
    }
    
    // El nodo puede ser AST_LISTA_PARAMETROS o contener directamente los argumentos
    int inicio = 0;
    int numHijos = argumentosNodo->num_hijos;
    
    if (argumentosNodo->tipo == AST_LISTA_PARAMETROS) {
        printf("[DEBUG] Extrayendo argumentos de LISTA_PARAMETROS, num_hijos=%d\n", numHijos);
    } else {
        printf("[DEBUG] Extrayendo argumentos directamente, num_hijos=%d\n", numHijos);
    }
    
    for (int i = inicio; i < numHijos && *numArgs < MAX_PARAMS; i++) {
        NodoAST* argNodo = argumentosNodo->hijos[i];
        
        if (!argNodo) continue;
        
        ArgumentoLlamada* arg = &args[*numArgs];
        memset(arg, 0, sizeof(ArgumentoLlamada));
        
        // Determinar si es literal o variable
        if (argNodo->tipo == AST_IDENTIFICADOR) {
            // Es una variable
            strncpy(arg->nombreVariable, argNodo->valor, MAX_NOMBRE - 1);
            arg->esLiteral = false;
            
            // Determinar tipo y si es vector/matriz
            arg->tipo = determinarTipoArgumento(argNodo, raizAST, &arg->esVector, &arg->esMatriz);
            
            // Determinar si pasa por referencia
            arg->esPorReferencia = (arg->tipo == TIPO_STRING || arg->esVector || arg->esMatriz);
            
            printf("[DEBUG] Argumento %d: variable '%s', tipo=%d, vector=%d, matriz=%d, porRef=%d\n",
                   *numArgs, arg->nombreVariable, arg->tipo, arg->esVector, arg->esMatriz, 
                   arg->esPorReferencia);
        } else {
            // Es un literal
            arg->esLiteral = true;
            arg->esPorReferencia = false;
            
            if (argNodo->tipo == AST_NUMERO) {
                strncpy(arg->valorLiteral, argNodo->valor, sizeof(arg->valorLiteral) - 1);
                arg->tipo = strchr(argNodo->valor, '.') ? TIPO_DOUBLE : TIPO_INT;
            } else if (argNodo->tipo == AST_CADENA || argNodo->tipo == AST_STRING) {
                strncpy(arg->valorLiteral, argNodo->valor, sizeof(arg->valorLiteral) - 1);
                arg->tipo = TIPO_STRING;
            } else if (argNodo->tipo == AST_CARACTER) {
                strncpy(arg->valorLiteral, argNodo->valor, sizeof(arg->valorLiteral) - 1);
                arg->tipo = TIPO_CHAR;
            } else if (argNodo->tipo == AST_BOOLEAN) {
                strncpy(arg->valorLiteral, argNodo->valor, sizeof(arg->valorLiteral) - 1);
                arg->tipo = TIPO_BOOLEAN;
            }
            
            printf("[DEBUG] Argumento %d: literal '%s', tipo=%d\n",
                   *numArgs, arg->valorLiteral, arg->tipo);
        }
        
        (*numArgs)++;
    }
}

// ==================== GENERACIÓN DE CÓDIGO ====================

void generarCargaArgumento(GeneradorDeclaraciones* gen, ArgumentoLlamada* arg,
                          int registroDestino, PilaAmbitos* pilaAmbitos) {
    char codigo[512];
    
    if (arg->esLiteral) {
        // Cargar literal
        if (arg->tipo == TIPO_INT || arg->tipo == TIPO_BOOLEAN) {
            int valor = (arg->tipo == TIPO_BOOLEAN && strcmp(arg->valorLiteral, "true") == 0) ? 1 : atoi(arg->valorLiteral);
            snprintf(codigo, sizeof(codigo),
                "    mov w%d, #%d  // literal %s\n",
                registroDestino, valor, arg->valorLiteral);
        } else if (arg->tipo == TIPO_STRING) {
            // Crear literal de string temporal
            static int contador_str_temp = 0;
            char nombre_literal[64];
            snprintf(nombre_literal, sizeof(nombre_literal), "str_temp_%d", contador_str_temp++);
            
            // Agregar literal a .data
            char decl[256];
            snprintf(decl, sizeof(decl), "%s: .asciz %s\n", nombre_literal, arg->valorLiteral);
            insertarEnSeccionData(gen, decl);
            
            snprintf(codigo, sizeof(codigo),
                "    ldr x%d, =%s  // literal string\n",
                registroDestino, nombre_literal);
        }
        insertarEnSeccionText(gen, codigo);
        return;
    }
    
    // Es una variable
    if (arg->esPorReferencia) {
        // Por referencia: pasar la dirección
        if (gen->dentro_de_funcion) {
            // Variable local: buscar en el ámbito
            VariableLocal* var = buscarVariableEnAmbito(pilaAmbitos, arg->nombreVariable);
            ParametroInfo* param = buscarParametroEnAmbito(pilaAmbitos, arg->nombreVariable);
            
            if (var) {
                snprintf(codigo, sizeof(codigo),
                    "    add x%d, sp, #%d  // dirección de variable local '%s'\n",
                    registroDestino, var->offset_stack, arg->nombreVariable);
            } else if (param) {
                snprintf(codigo, sizeof(codigo),
                    "    ldr x%d, [sp, #%d]  // dirección de parámetro '%s'\n",
                    registroDestino, param->tipo * 8, arg->nombreVariable);
            } else {
                // Variable global
                snprintf(codigo, sizeof(codigo),
                    "    ldr x%d, =%s  // dirección de variable global '%s'\n",
                    registroDestino, arg->nombreVariable, arg->nombreVariable);
            }
        } else {
            // Variable global
            snprintf(codigo, sizeof(codigo),
                "    ldr x%d, =%s  // dirección de variable global '%s'\n",
                registroDestino, arg->nombreVariable, arg->nombreVariable);
        }
    } else {
        // Por valor: pasar el valor
        if (gen->dentro_de_funcion) {
            // Variable local
            VariableLocal* var = buscarVariableEnAmbito(pilaAmbitos, arg->nombreVariable);
            if (var) {
                if (arg->tipo == TIPO_INT || arg->tipo == TIPO_BOOLEAN) {
                    snprintf(codigo, sizeof(codigo),
                        "    ldr w%d, [sp, #%d]  // valor de '%s'\n",
                        registroDestino, var->offset_stack, arg->nombreVariable);
                } else if (arg->tipo == TIPO_FLOAT) {
                    snprintf(codigo, sizeof(codigo),
                        "    ldr s%d, [sp, #%d]  // valor de '%s'\n",
                        registroDestino, var->offset_stack, arg->nombreVariable);
                } else if (arg->tipo == TIPO_DOUBLE) {
                    snprintf(codigo, sizeof(codigo),
                        "    ldr d%d, [sp, #%d]  // valor de '%s'\n",
                        registroDestino, var->offset_stack, arg->nombreVariable);
                } else if (arg->tipo == TIPO_CHAR) {
                    snprintf(codigo, sizeof(codigo),
                        "    ldrb w%d, [sp, #%d]  // valor de '%s'\n",
                        registroDestino, var->offset_stack, arg->nombreVariable);
                }
            } else {
                // Variable global
                if (arg->tipo == TIPO_INT || arg->tipo == TIPO_BOOLEAN) {
                    snprintf(codigo, sizeof(codigo),
                        "    ldr x1, =%s\n"
                        "    ldr w%d, [x1]  // valor de '%s'\n",
                        arg->nombreVariable, registroDestino, arg->nombreVariable);
                } else if (arg->tipo == TIPO_FLOAT) {
                    snprintf(codigo, sizeof(codigo),
                        "    ldr x1, =%s\n"
                        "    ldr s%d, [x1]  // valor de '%s'\n",
                        arg->nombreVariable, registroDestino, arg->nombreVariable);
                } else if (arg->tipo == TIPO_DOUBLE) {
                    snprintf(codigo, sizeof(codigo),
                        "    ldr x1, =%s\n"
                        "    ldr d%d, [x1]  // valor de '%s'\n",
                        arg->nombreVariable, registroDestino, arg->nombreVariable);
                } else if (arg->tipo == TIPO_CHAR) {
                    snprintf(codigo, sizeof(codigo),
                        "    ldr x1, =%s\n"
                        "    ldrb w%d, [x1]  // valor de '%s'\n",
                        arg->nombreVariable, registroDestino, arg->nombreVariable);
                }
            }
        } else {
            // Variable global
            if (arg->tipo == TIPO_INT || arg->tipo == TIPO_BOOLEAN) {
                snprintf(codigo, sizeof(codigo),
                    "    ldr x1, =%s\n"
                    "    ldr w%d, [x1]  // valor de '%s'\n",
                    arg->nombreVariable, registroDestino, arg->nombreVariable);
            } else if (arg->tipo == TIPO_FLOAT) {
                snprintf(codigo, sizeof(codigo),
                    "    ldr x1, =%s\n"
                    "    ldr s%d, [x1]  // valor de '%s'\n",
                    arg->nombreVariable, registroDestino, arg->nombreVariable);
            } else if (arg->tipo == TIPO_DOUBLE) {
                snprintf(codigo, sizeof(codigo),
                    "    ldr x1, =%s\n"
                    "    ldr d%d, [x1]  // valor de '%s'\n",
                    arg->nombreVariable, registroDestino, arg->nombreVariable);
            } else if (arg->tipo == TIPO_CHAR) {
                snprintf(codigo, sizeof(codigo),
                    "    ldr x1, =%s\n"
                    "    ldrb w%d, [x1]  // valor de '%s'\n",
                    arg->nombreVariable, registroDestino, arg->nombreVariable);
            }
        }
    }
    
    insertarEnSeccionText(gen, codigo);
}

void generarPasoDeArgumentos(GeneradorDeclaraciones* gen, ArgumentoLlamada args[],
                            int numArgs, PilaAmbitos* pilaAmbitos) {
    char codigo[256];
    
    insertarEnSeccionText(gen, "    // Pasar argumentos a la función\n");
    
    // Los primeros 8 argumentos van en x0-x7
    for (int i = 0; i < numArgs && i < 8; i++) {
        generarCargaArgumento(gen, &args[i], i, pilaAmbitos);
    }
    
    // Si hay más de 8 argumentos, los demás van al stack (no implementado aún)
    if (numArgs > 8) {
        printf("[WARNING] Más de 8 argumentos no implementado completamente\n");
    }
}

void generarRestauracionPorReferencia(GeneradorDeclaraciones* gen,
                                     ArgumentoLlamada args[], int numArgs,
                                     ParametroInfo params[], int numParams) {
    // No es necesario hacer nada en ARM64 para parámetros por referencia
    // Los cambios ya se reflejan automáticamente porque pasamos la dirección
    printf("[DEBUG] Restauración por referencia (automática en ARM64)\n");
}

// ==================== FUNCIÓN PRINCIPAL ====================

void generarLlamadaFuncionVoid(GeneradorDeclaraciones* gen, NodoAST* nodo,
                              PilaAmbitos* pilaAmbitos, NodoAST* raizAST) {
    if (!gen || !nodo || !pilaAmbitos) {
        printf("[ERROR] Parámetros inválidos en generarLlamadaFuncionVoid\n");
        return;
    }
    
    // Validar estructura del nodo
    if ((nodo->tipo != AST_LLAMADA_VOID && nodo->tipo != AST_LLAMADA) || nodo->num_hijos < 1) {
        printf("[ERROR] Nodo no es llamada a función void o estructura inválida\n");
        return;
    }
    
    NodoAST* idNodo = nodo->hijos[0];
    NodoAST* argumentosNodo = nodo->num_hijos > 1 ? nodo->hijos[1] : NULL;
    
    const char* nombreFuncion = idNodo->valor;
    
    printf("[DEBUG LLAMADA] Generando llamada a función void: '%s'\n", nombreFuncion);
    
    // Buscar la declaración de la función en el AST
    NodoAST* funcionDecl = buscarFuncionVoidEnAST(raizAST, nombreFuncion);
    if (!funcionDecl) {
        printf("[ERROR] Función void '%s' no encontrada en el AST\n", nombreFuncion);
        return;
    }
    
    // Extraer parámetros de la declaración
    NodoAST* parametrosNodo = funcionDecl->hijos[1];
    ParametroInfo params[MAX_PARAMS];
    int numParams = 0;
    extraerParametrosDesdeAST(parametrosNodo, params, &numParams);
    
    // Extraer argumentos de la llamada
    ArgumentoLlamada args[MAX_PARAMS];
    int numArgs = 0;
    extraerArgumentosDesdeAST(argumentosNodo, args, &numArgs, raizAST);
    
    // Verificar que los argumentos coincidan con los parámetros
    if (!verificarArgumentosConParametros(args, numArgs, params, numParams)) {
        printf("[ERROR] Argumentos no coinciden con parámetros de la función '%s'\n", nombreFuncion);
        return;
    }
    
    // Generar código
    char codigo[256];
    char nombreFunc[128];
    generarNombreFuncion(nombreFuncion, nombreFunc, sizeof(nombreFunc));
    
    snprintf(codigo, sizeof(codigo),
        "    // Llamada a función void: %s\n",
        nombreFuncion);
    insertarEnSeccionText(gen, codigo);
    
    // Pasar argumentos
    generarPasoDeArgumentos(gen, args, numArgs, pilaAmbitos);
    
    // Llamar a la función
    snprintf(codigo, sizeof(codigo),
        "    bl %s\n",
        nombreFunc);
    insertarEnSeccionText(gen, codigo);
    
    // Restaurar parámetros por referencia (si es necesario)
    generarRestauracionPorReferencia(gen, args, numArgs, params, numParams);
    
    printf("[DEBUG LLAMADA] Llamada a función '%s' generada exitosamente\n", nombreFuncion);
}