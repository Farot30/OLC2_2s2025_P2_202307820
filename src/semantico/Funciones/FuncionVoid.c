#include "../../AST/err.h"
#include "../tabla_simbolo.h"
#include "../../AST/ejecutar_ast.h"
#include "../tipos.h"
#include "FuncionVoid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ResultadoEjecucion DeclararFuncionVoid(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_FUNCION_VOID) {
        printf("[ERROR] Nodo inválido para declaración de función void\n");
        return EJEC_OK; 
    }

    // Puede tener 2 o 3 hijos: ID, [PARAMETROS], BLOQUE
    if (nodo->num_hijos < 2 || nodo->hijos[0]->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Estructura inválida para declaración de función void\n");
        addError("Estructura inválida para función void", ambitoActual(), nodo->linea, nodo->columna);  
        return EJEC_OK; 
    }

    NodoAST* idNodo = nodo->hijos[0];
    NodoAST* parametros = NULL;
    NodoAST* bloqueNodo = NULL;

    // Determinar estructura: con o sin parámetros
    if (nodo->num_hijos == 2) {
        // void func() { ... }
        bloqueNodo = nodo->hijos[1];
    } else if (nodo->num_hijos == 3) {
        // void func(params) { ... }
        parametros = nodo->hijos[1];
        bloqueNodo = nodo->hijos[2];
    } else {
        printf("[ERROR] Número incorrecto de hijos para función void: %d\n", nodo->num_hijos);
        addError("Estructura inválida para función void", ambitoActual(), nodo->linea, nodo->columna);
        return EJEC_OK;
    }

    printf("[DEBUG] Declarando función void: %s\n", idNodo->valor);

    // Verificar si la función ya está declarada
    Simbolo* simboloExistente = buscarSimbolo(idNodo->valor);
    if (simboloExistente) {
        printf("[ERROR] La función '%s' ya está declarada\n", idNodo->valor);
        addError("Función ya declarada", ambitoActual(), idNodo->linea, idNodo->columna);
        return EJEC_OK; 
    }

    // Crear el símbolo para la función void
    agregarSimboloDetallado(idNodo->valor, SIMBOLO_FUNCION_VOID, TIPO_VOID, 
                           ambitoActual(), idNodo->linea, idNodo->columna);
    
    // Obtener el símbolo recién creado para guardar los nodos
    Simbolo* simboloFuncion = buscarSimbolo(idNodo->valor);
    if (!simboloFuncion) {
        printf("[ERROR] No se pudo registrar la función '%s'\n", idNodo->valor);
        addError("Error al registrar función void", ambitoActual(), nodo->linea, nodo->columna);
        return EJEC_OK; 
    }
    
    // Procesar parámetros si existen
    if (parametros && parametros->tipo == AST_PARAMETROS) {
        // Conjunto para detectar parámetros duplicados
        char* nombresParametros[100] = {0};
        int numParametros = 0;

        for (int i = 0; i < parametros->num_hijos; i++) {
            NodoAST* param = parametros->hijos[i];
            if (param->num_hijos != 2) {
                printf("[ERROR] Parámetro inválido en función void '%s'\n", idNodo->valor);
                addError("Parámetro inválido", ambitoActual(), param->linea, param->columna);
                continue;
            }
            
            NodoAST* tipoParam = param->hijos[0];
            NodoAST* idParam = param->hijos[1];

            // Verificar parámetro duplicado
            for (int j = 0; j < numParametros; j++) {
                if (strcmp(nombresParametros[j], idParam->valor) == 0) {
                    char msg[100];
                    snprintf(msg, sizeof(msg), "Parámetro duplicado '%s' en función void '%s'", 
                            idParam->valor, idNodo->valor);
                    addError(msg, ambitoActual(), idParam->linea, idParam->columna);
                    printf("[ERROR] %s\n", msg);
                    goto siguiente_parametro;
                }
            }
            
            // Agregar al conjunto de nombres de parámetros
            nombresParametros[numParametros++] = idParam->valor;

            // Determinar el tipo del parámetro
            TipoDato tipoParametro;
            TipoSimbolo claseParametro = SIMBOLO_VARIABLE;
            
            // Verificar si es un tipo vector (ej: "String[]")
            if (strstr(tipoParam->valor, "[][]") != NULL) {
                char tipoBase[50] = {0};
                strncpy(tipoBase, tipoParam->valor, strlen(tipoParam->valor) - 4); // Quitar "[][]"
                tipoParametro = obtenerTipoDesdeString(tipoBase);
                claseParametro = SIMBOLO_MATRIZ; // **NUEVO: Marcar como matriz**
                printf("[DEBUG] Parámetro void '%s' es matriz de tipo %s\n", 
                    idParam->valor, tipoBase);
            }
            // Verificar si es un tipo vector (ej: "String[]") - CÓDIGO EXISTENTE
            else if (strstr(tipoParam->valor, "[]") != NULL) {
                char tipoBase[50] = {0};
                strncpy(tipoBase, tipoParam->valor, strlen(tipoParam->valor) - 2); // Quitar "[]"
                tipoParametro = obtenerTipoDesdeString(tipoBase);
                claseParametro = SIMBOLO_VECTOR; // Marcar como vector
                printf("[DEBUG] Parámetro void '%s' es vector de tipo %s\n", 
                    idParam->valor, tipoBase);
            } else {
                // CÓDIGO EXISTENTE para tipos primitivos
                tipoParametro = obtenerTipoDesdeString(tipoParam->valor);
            }
            
            // Verificar tipo válido
            if (tipoParametro == TIPO_DESCONOCIDO || tipoParametro == TIPO_FUNCION) {
                char msg[100];
                snprintf(msg, sizeof(msg), "Tipo de parámetro desconocido en función void '%s'", 
                        idNodo->valor);
                addError(msg, ambitoActual(), tipoParam->linea, tipoParam->columna);
                printf("[ERROR] %s\n", msg);
                continue;
            }

            // Agregar el parámetro a la tabla de símbolos en el ámbito de la función
            agregarSimboloDetallado(idParam->valor, claseParametro, tipoParametro, 
                                   idNodo->valor, idParam->linea, idParam->columna);
            
            siguiente_parametro:;
        }
    }
    
    // Guardar los nodos en el símbolo
    simboloFuncion->nodo_bloque = bloqueNodo;
    simboloFuncion->nodo_params = parametros;
    
    printf("[DEBUG] Función void '%s' declarada exitosamente\n", idNodo->valor);
    return EJEC_OK;
}