#include "Llamadas_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include "../../semantico/tipos.h"
#include "../generador_arm64.h"
#include "FuncionesPredefinidas_arm64.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern NodoAST* astGlobal;

// Busca una función con retorno en el AST
NodoAST* buscarFuncionRetornoEnAST(NodoAST* raiz, const char* nombreFuncion) {
    if (!raiz || !nombreFuncion) return NULL;
    
    // Si es la función que buscamos
    if (raiz->tipo == AST_FUNCION && raiz->num_hijos >= 2) {
        NodoAST* idNodo = raiz->hijos[1]; // El nombre está en hijos[1]
        if (idNodo && idNodo->tipo == AST_IDENTIFICADOR && 
            strcmp(idNodo->valor, nombreFuncion) == 0) {
            printf("[DEBUG] Función con retorno encontrada: %s\n", nombreFuncion);
            return raiz;
        }
    }
    
    // Buscar recursivamente en los hijos
    for (int i = 0; i < raiz->num_hijos; i++) {
        NodoAST* resultado = buscarFuncionRetornoEnAST(raiz->hijos[i], nombreFuncion);
        if (resultado) return resultado;
    }
    
    return NULL;
}

// Obtiene el tipo de retorno de una función
TipoDato obtenerTipoRetornoFuncion(NodoAST* funcionNodo) {
    if (!funcionNodo || funcionNodo->tipo != AST_FUNCION || funcionNodo->num_hijos < 1) {
        return TIPO_DESCONOCIDO;
    }
    
    NodoAST* tipoRetornoNodo = funcionNodo->hijos[0];
    if (!tipoRetornoNodo || !tipoRetornoNodo->valor) {
        return TIPO_DESCONOCIDO;
    }
    
    return obtenerTipoDesdeString(tipoRetornoNodo->valor);
}

void generarPasarArgumentosRetorno(GeneradorDeclaraciones* gen, 
                                   NodoAST* argumentosNodo,
                                   ParametroInfo* parametros,
                                   int numParametros) {
    if (!argumentosNodo || argumentosNodo->tipo != AST_LISTA_PARAMETROS) {
        printf("[DEBUG] No hay argumentos para pasar\n");
        return;
    }
    
    printf("[DEBUG] Pasando %d argumentos\n", argumentosNodo->num_hijos);
    
    char codigo[2048] = "";
    char buffer[256];
    
    // ✅ GUARDAR ESTADO DE dentro_de_funcion PARA RESTAURAR DESPUÉS
    bool dentroFuncionOriginal = gen->dentro_de_funcion;
    char nombreFuncionOriginal[128];
    strncpy(nombreFuncionOriginal, gen->nombre_funcion_actual, sizeof(nombreFuncionOriginal) - 1);
    
    // Los primeros 8 argumentos van en registros w0-w7
    for (int i = 0; i < argumentosNodo->num_hijos && i < 8; i++) {
        NodoAST* argNodo = argumentosNodo->hijos[i];
        
        if (!argNodo) {
            printf("[ERROR] Argumento %d es NULL\n", i);
            continue;
        }
        
        if (i >= numParametros) {
            printf("[WARNING] Más argumentos que parámetros\n");
            break;
        }
        
        ParametroInfo* param = &parametros[i];
        
        printf("[DEBUG] Argumento %d: tipo esperado=%s, nodo tipo=%d, valor=%s\n", 
               i, obtenerNombreTipo(param->tipo), argNodo->tipo, 
               argNodo->valor ? argNodo->valor : "(null)");
        
        // ✅ PARA ARGUMENTOS IDENTIFICADORES, USAR REGISTRO TEMPORAL
        if (argNodo->tipo == AST_IDENTIFICADOR) {
            if (param->tipo == TIPO_INT || param->tipo == TIPO_BOOLEAN || param->tipo == TIPO_CHAR) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x10, =%s\n"      // ✅ Usar x10 como temporal
                    "    ldr w%d, [x10]\n",   // ✅ Cargar valor en w{i}
                    argNodo->valor, i);
            } else if (param->tipo == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x10, =%s\n"
                    "    ldr s%d, [x10]\n",
                    argNodo->valor, i);
            } else if (param->tipo == TIPO_DOUBLE) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x10, =%s\n"
                    "    ldr d%d, [x10]\n",
                    argNodo->valor, i);
            } else if (param->tipo == TIPO_STRING) {
                snprintf(buffer, sizeof(buffer),
                    "    ldr x10, =%s\n"
                    "    ldr x%d, [x10]\n",
                    argNodo->valor, i);
            }
            strcat(codigo, buffer);
        } 
        else if (argNodo->tipo == AST_NUMERO) {
            // Literal numérico
            if (param->tipo == TIPO_INT || param->tipo == TIPO_BOOLEAN || param->tipo == TIPO_CHAR) {
                snprintf(buffer, sizeof(buffer),
                    "    mov w%d, #%s\n",
                    i, argNodo->valor);
            } else if (param->tipo == TIPO_FLOAT || param->tipo == TIPO_DOUBLE) {
                // Crear constante y cargar
                char nombreConst[64];
                snprintf(nombreConst, sizeof(nombreConst), "arg_const_%d_%d", i, gen->admin_registros->contador_constantes++);
                char bufferData[128];
                if (param->tipo == TIPO_FLOAT) {
                    snprintf(bufferData, sizeof(bufferData), "%s: .float %s\n", nombreConst, argNodo->valor);
                    insertarEnSeccionData(gen, bufferData);
                    snprintf(buffer, sizeof(buffer),
                        "    ldr x10, =%s\n"
                        "    ldr s%d, [x10]\n",
                        nombreConst, i);
                } else {
                    snprintf(bufferData, sizeof(bufferData), "%s: .double %s\n", nombreConst, argNodo->valor);
                    insertarEnSeccionData(gen, bufferData);
                    snprintf(buffer, sizeof(buffer),
                        "    ldr x10, =%s\n"
                        "    ldr d%d, [x10]\n",
                        nombreConst, i);
                }
            }
            strcat(codigo, buffer);
        }
        else if (argNodo->tipo == AST_CADENA) {
            // Literal de string
            char nombreStr[64];
            snprintf(nombreStr, sizeof(nombreStr), "arg_str_%d_%d", i, gen->admin_registros->contador_constantes++);
            char bufferData[256];
            snprintf(bufferData, sizeof(bufferData), "%s: .asciz %s\n", nombreStr, argNodo->valor);
            insertarEnSeccionData(gen, bufferData);
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n",
                i, nombreStr);
            strcat(codigo, buffer);
        }
        else {
            // ✅ EXPRESIÓN: TEMPORALMENTE DESACTIVAR dentro_de_funcion PARA EVALUAR
            gen->dentro_de_funcion = false;
            gen->nombre_funcion_actual[0] = '\0';
            
            int regIndex = 10;
            ResultadoExprEmbebida resultado = generarExprEmbebidaARM64(gen, argNodo, &regIndex);
            
            // ✅ RESTAURAR ESTADO
            gen->dentro_de_funcion = dentroFuncionOriginal;
            strncpy(gen->nombre_funcion_actual, nombreFuncionOriginal, sizeof(gen->nombre_funcion_actual) - 1);
            
            strcat(codigo, resultado.codigo);
            
            // Mover resultado al registro de parámetro
            if (param->tipo == TIPO_INT || param->tipo == TIPO_BOOLEAN || param->tipo == TIPO_CHAR) {
                snprintf(buffer, sizeof(buffer), "    mov w%d, %s\n", i, resultado.resultado);
            } else if (param->tipo == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer), "    fmov s%d, s0\n", i);
            } else if (param->tipo == TIPO_DOUBLE) {
                snprintf(buffer, sizeof(buffer), "    fmov d%d, d0\n", i);
            } else if (param->tipo == TIPO_STRING) {
                snprintf(buffer, sizeof(buffer), "    mov x%d, %s\n", i, resultado.resultado);
            }
            strcat(codigo, buffer);
        }
    }
    
    // ✅ RESTAURAR ESTADO FINAL
    gen->dentro_de_funcion = dentroFuncionOriginal;
    strncpy(gen->nombre_funcion_actual, nombreFuncionOriginal, sizeof(gen->nombre_funcion_actual) - 1);
    
    if (argumentosNodo->num_hijos > 8) {
        printf("[WARNING] Funciones con más de 8 parámetros no soportadas completamente\n");
    }
    
    insertarEnSeccionText(gen, codigo);
}

// Genera código para llamar a una función con retorno
void generarLlamadaFuncionRetorno(GeneradorDeclaraciones* gen, NodoAST* llamadaNodo,
                                  PilaAmbitosRetorno* pilaRetorno, NodoAST* raizAST) {
    if (!gen || !llamadaNodo || llamadaNodo->tipo != AST_LLAMADA) {
        printf("[ERROR] Nodo de llamada inválido\n");
        return;
    }
    
    if (llamadaNodo->num_hijos < 1) {
        printf("[ERROR] Llamada sin nombre de función\n");
        return;
    }
    
    NodoAST* idNodo = llamadaNodo->hijos[0];
    NodoAST* argumentosNodo = llamadaNodo->num_hijos > 1 ? llamadaNodo->hijos[1] : NULL;
    
    if (!idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Identificador de función inválido\n");
        return;
    }
    
    const char* nombreFuncion = idNodo->valor;
    
    printf("[DEBUG] Generando llamada a función con retorno: %s\n", nombreFuncion);
    

    if (esFuncionPredefinida(nombreFuncion)) {
        printf("[DEBUG] '%s' es una función predefinida\n", nombreFuncion);
        generarFuncionPredefinidaSiNecesario(gen, nombreFuncion);
        
        // Generar llamada
        char codigo[512];
        char nombreFunc[128];
        generarNombreFuncion(nombreFuncion, nombreFunc, sizeof(nombreFunc));
        
        // Pasar argumentos (si los tiene)
        if (argumentosNodo) {
            // ✅ NECESITAS EXTRAER PARÁMETROS DE LA FIRMA PREDEFINIDA
            // Por ahora, asumimos que no tienen parámetros (void functions)
        }
        
        snprintf(codigo, sizeof(codigo),
            "    // Llamada a función predefinida: %s\n"
            "    bl %s\n",
            nombreFuncion, nombreFunc);
        insertarEnSeccionText(gen, codigo);
        
        printf("[DEBUG] Llamada a función predefinida '%s' generada\n", nombreFuncion);
        return;
    }
    // Buscar la declaración de la función en el AST
    NodoAST* funcionNodo = buscarFuncionRetornoEnAST(raizAST, nombreFuncion);
    if (!funcionNodo) {
        printf("[ERROR] Función '%s' no encontrada en el AST\n", nombreFuncion);
        return;
    }
    
    // Obtener tipo de retorno
    TipoDato tipoRetorno = obtenerTipoRetornoFuncion(funcionNodo);
    printf("[DEBUG] Tipo de retorno: %s\n", obtenerNombreTipo(tipoRetorno));
    
    // Extraer parámetros de la función
    NodoAST* parametrosNodo = funcionNodo->hijos[2]; // Los parámetros están en hijos[2]
    ParametroInfo params[MAX_PARAMS];
    int numParams = 0;
    extraerParametrosDesdeAST(parametrosNodo, params, &numParams);
    
    printf("[DEBUG] Función tiene %d parámetros\n", numParams);
    
    // Validar número de argumentos
    int numArgs = argumentosNodo ? argumentosNodo->num_hijos : 0;
    if (numArgs != numParams) {
        printf("[WARNING] Número de argumentos (%d) no coincide con parámetros (%d)\n",
               numArgs, numParams);
    }
    
    char codigo[512];
    
    // Comentario de inicio
    snprintf(codigo, sizeof(codigo),
        "    // Llamada a función: %s\n",
        nombreFuncion);
    insertarEnSeccionText(gen, codigo);
    
    // Pasar argumentos a registros
    if (argumentosNodo) {
        generarPasarArgumentosRetorno(gen, argumentosNodo, params, numParams);
    }
    
    // Generar nombre de función (func_nombreFuncion)
    char nombreFunc[128];
    generarNombreFuncion(nombreFuncion, nombreFunc, sizeof(nombreFunc));
    
    // Llamar a la función
    snprintf(codigo, sizeof(codigo),
        "    bl %s\n",
        nombreFunc);
    insertarEnSeccionText(gen, codigo);
    
    // El resultado queda en w0/x0/s0/d0 según el tipo de retorno
    // No necesitamos hacer nada más, el valor está listo para ser usado
    
    printf("[DEBUG] Llamada a función '%s' generada exitosamente\n", nombreFuncion);
}