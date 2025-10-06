#include "../../AST/err.h"
#include "../tabla_simbolo.h"
#include "../../AST/ejecutar_ast.h"
#include "../tipos.h"
#include "../semantico.h"
#include "../../simbolos/Evaluador.h"
#include "Funcion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int verificarReturnEnBloque(NodoAST* nodo) {
    if (!nodo) return 0;
    
    // Si es un nodo return, encontramos uno
    if (nodo->tipo == AST_RETURN) {
        return 1;
    }
    
    // Buscar en todos los hijos
    for (int i = 0; i < nodo->num_hijos; i++) {
        if (verificarReturnEnBloque(nodo->hijos[i])) {
            return 1;
        }
    }
    
    return 0;
}

NodoAST* encontrarNodoReturn(NodoAST* nodo) {
    if (!nodo) return NULL;
    
    if (nodo->tipo == AST_RETURN) {
        return nodo;
    }
    
    for (int i = 0; i < nodo->num_hijos; i++) {
        NodoAST* encontrado = encontrarNodoReturn(nodo->hijos[i]);
        if (encontrado) return encontrado;
    }
    
    return NULL;
}

// Función auxiliar para verificar compatibilidad del tipo de retorno
int verificarCompatibilidadRetorno(TipoDato tipoExpresion, TipoDato tipoRetorno, NodoAST* nodoReturn) {
    if (tipoExpresion == tipoRetorno || esWidening(tipoExpresion, tipoRetorno)) {
        return 1;  // Tipos compatibles
    }
    
    char msg[200];
    snprintf(msg, sizeof(msg), "Tipo de retorno incompatible: esperado %s, encontrado %s",
            nombreTipo(tipoRetorno), nombreTipo(tipoExpresion));
    addError(msg, ambitoActual(), nodoReturn->linea, nodoReturn->columna);
    printf("[ERROR FUNCION] %s\n", msg);
    return 0;
}

// Función auxiliar para verificar si un tipo es vector
int esTipoVector(const char* tipo) {
    return tipo && strstr(tipo, "[]") != NULL;
}

ResultadoEjecucion DeclararFuncion(NodoAST* nodo) {
    if(!nodo || nodo->tipo != AST_FUNCION) {
        printf("[ERROR FUNCION] Nodo inválido o no es tipo FUNCION\n");
        return EJEC_OK;
    }

    if(nodo->num_hijos < 4) {  // Tipo, ID, Parámetros, Bloque
        printf("[ERROR FUNCION] Función incompleta - esperados al menos 4 hijos, encontrados %d\n", nodo->num_hijos);
        return EJEC_OK;
    }

    NodoAST* tipoNodo = nodo->hijos[0];
    NodoAST* idNodo = nodo->hijos[1];
    NodoAST* parametros = nodo->hijos[2];
    NodoAST* bloqueNodo = nodo->hijos[3];

    // Verificar si la función ya está declarada
    Simbolo* simboloExistente = buscarSimbolo(idNodo->valor);
    if(simboloExistente) {
        char msg[100];
        snprintf(msg, sizeof(msg), "Función '%s' ya declarada", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR FUNCION] %s\n", msg);
        return EJEC_OK;
    }

    // Determinar el tipo de retorno
    TipoDato tipoRetorno = obtenerTipoDesdeString(tipoNodo->valor);
    if (tipoRetorno == TIPO_DESCONOCIDO || tipoRetorno == TIPO_FUNCION || tipoRetorno == TIPO_VOID) {
        char msg[100];
        snprintf(msg, sizeof(msg), "Tipo de retorno desconocido para función '%s'", idNodo->valor);
        addError(msg, ambitoActual(), tipoNodo->linea, tipoNodo->columna);
        printf("[ERROR FUNCION] %s\n", msg);
        return EJEC_OK;
    }

    printf("[DEBUG FUNCION] Declarando función: %s con tipo de retorno %s\n", 
           idNodo->valor, nombreTipo(tipoRetorno));

    // Agregar la función a la tabla de símbolos
    agregarSimboloDetallado(idNodo->valor, SIMBOLO_FUNCION, tipoRetorno, ambitoActual(), idNodo->linea, idNodo->columna);

    // Procesar parámetros si existen
    if(parametros && parametros->tipo == AST_PARAMETROS) {
        // Conjunto para detectar parámetros duplicados
        char* nombresParametros[100] = {0};
        int numParametros = 0;

        for(int i = 0; i < parametros->num_hijos; i++) {
            NodoAST* param = parametros->hijos[i];
            if(param->num_hijos != 2) {
                printf("[ERROR FUNCION] Parámetro inválido en función '%s'\n", idNodo->valor);
                addError("Parámetro inválido", ambitoActual(), param->linea, param->columna);
                continue;
            }
            
            NodoAST* tipoParam = param->hijos[0];
            NodoAST* idParam = param->hijos[1];

            // Verificar parámetro duplicado
            for(int j = 0; j < numParametros; j++) {
                if(strcmp(nombresParametros[j], idParam->valor) == 0) {
                    char msg[100];
                    snprintf(msg, sizeof(msg), "Parámetro duplicado '%s' en función '%s'", 
                            idParam->valor, idNodo->valor);
                    addError(msg, ambitoActual(), idParam->linea, idParam->columna);
                    printf("[ERROR FUNCION] %s\n", msg);
                    goto siguiente_parametro;
                }
            }
            
            // Agregar al conjunto de nombres de parámetros
            nombresParametros[numParametros++] = idParam->valor;

            // Determinar el tipo del parámetro y verificar si es un vector
            TipoDato tipoParametro;
            TipoSimbolo claseParametro = SIMBOLO_VARIABLE;
            
            if (esTipoVector(tipoParam->valor)) {
                // Es un tipo vector (ej: "int[]")
                char tipoBase[50] = {0};
                strncpy(tipoBase, tipoParam->valor, strlen(tipoParam->valor) - 2); // Quitar los corchetes
                tipoParametro = obtenerTipoDesdeString(tipoBase);
                claseParametro = SIMBOLO_VECTOR; // Marcar como vector
                printf("[DEBUG FUNCION] Parámetro '%s' es vector de tipo %s\n", 
                       idParam->valor, tipoBase);
            } else {
                // Es un tipo normal
                tipoParametro = obtenerTipoDesdeString(tipoParam->valor);
            }
            
            // Verificar tipo válido
            if (tipoParametro == TIPO_DESCONOCIDO || tipoParametro == TIPO_FUNCION || tipoParametro == TIPO_VOID) {
                char msg[100];
                snprintf(msg, sizeof(msg), "Tipo de parámetro desconocido en función '%s'", idNodo->valor);
                addError(msg, ambitoActual(), tipoParam->linea, tipoParam->columna);
                printf("[ERROR FUNCION] %s\n", msg);
                continue;
            }

            // Agregar el parámetro a la tabla de símbolos en el ámbito de la función
            agregarSimboloDetallado(idParam->valor, claseParametro, tipoParametro, 
                                   idNodo->valor, idParam->linea, idParam->columna);
            
            siguiente_parametro:;
        }
    }

    // Obtener el símbolo de la función para guardar el bloque
    Simbolo* simboloFuncion = buscarSimbolo(idNodo->valor);
    if(!simboloFuncion) {
        printf("[ERROR FUNCION] Error al agregar la función '%s' a la tabla de símbolos\n", idNodo->valor);
        addError("Error al agregar función a tabla de símbolos", ambitoActual(), idNodo->linea, idNodo->columna);
        return EJEC_OK;
    }

    // Guardar el bloque de código
    simboloFuncion->nodo_bloque = bloqueNodo;
    simboloFuncion->nodo_params = parametros;
    
    // Verificar el nodo de return
    if (nodo->num_hijos > 4 && nodo->hijos[4]->tipo == AST_RETURN) {
        NodoAST* returnNodo = nodo->hijos[4];
        
        // Verificar que el return tenga una expresión para funciones con tipo de retorno
        if (returnNodo->num_hijos > 0) {
            // Verificar que el tipo de la expresión sea compatible con el tipo de retorno
            TipoDato tipoExpresion = deducirTipoExpr(returnNodo->hijos[0]);
            verificarCompatibilidadRetorno(tipoExpresion, tipoRetorno, returnNodo);
            
            // Guardar también el nodo de return en el símbolo de la función
            simboloFuncion->nodo_return = returnNodo;
        } else {
            printf("[ERROR FUNCION] La función '%s' requiere un valor de retorno\n", idNodo->valor);
            addError("Return sin valor en función con tipo de retorno", ambitoActual(), returnNodo->linea, returnNodo->columna);
        }
    } else {
        // Si no hay nodo de return explícito, buscarlo en el bloque
        int tieneReturn = verificarReturnEnBloque(bloqueNodo);
        
        if (tieneReturn) {
            // Buscar el nodo return para guardarlo
            NodoAST* returnEncontrado = encontrarNodoReturn(bloqueNodo);
            
            if (returnEncontrado) {
                if (returnEncontrado->num_hijos > 0) {
                    TipoDato tipoExpresion = deducirTipoExpr(returnEncontrado->hijos[0]);
                    verificarCompatibilidadRetorno(tipoExpresion, tipoRetorno, returnEncontrado);
                    
                    // Guardar el nodo de return en el símbolo de la función
                    simboloFuncion->nodo_return = returnEncontrado;
                } else {
                    printf("[ERROR FUNCION] La función '%s' requiere un valor de retorno\n", idNodo->valor);
                    addError("Return sin valor en función con tipo de retorno", ambitoActual(), returnEncontrado->linea, returnEncontrado->columna);
                }
            }
        } else {
            printf("[ERROR FUNCION] No se encontró return en la función '%s'\n", idNodo->valor);
            addError("Función sin return", ambitoActual(), idNodo->linea, idNodo->columna);
        }
    }

    printf("[DEBUG FUNCION] Función '%s' declarada exitosamente\n", idNodo->valor);
    return EJEC_OK;
}
ResultadoEjecucion EjecutarFuncion(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_LLAMADA) {
        printf("[ERROR] Nodo inválido para ejecución de función\n");
        return EJEC_OK;
    }

    // Evaluar la llamada a función (que procesa parámetros y ejecuta el cuerpo)
    // pero ignoramos el valor de retorno porque estamos ejecutando la función
    // como una instrucción, no como parte de una expresión
    evaluarExpr(nodo);
    
    return EJEC_OK;
}

