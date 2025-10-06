#include "../../AST/err.h"
#include "../tabla_simbolo.h"
#include "../../AST/ejecutar_ast.h"
#include "FuncionVoid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ResultadoEjecucion DeclararFuncionVoid(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_FUNCION_VOID) {
        printf("[ERROR] Nodo inválido para declaración de función void\n");
        return EJEC_OK; 
    }

    if (nodo->num_hijos < 2 || nodo->hijos[0]->tipo != AST_IDENTIFICADOR || nodo->hijos[1]->tipo != AST_BLOQUE) {
        printf("[ERROR] Estructura inválida para declaración de función void\n");
        addError("Estructura inválida para función void", ambitoActual(), nodo->linea, nodo->columna);  
        return EJEC_OK; 
    }

    NodoAST* idNodo = nodo->hijos[0];
    NodoAST* bloqueNodo = nodo->hijos[1];

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
    
    // Obtener el símbolo recién creado para guardar el nodo del bloque
    Simbolo* simboloFuncion = buscarSimbolo(idNodo->valor);
    if (!simboloFuncion) {
        printf("[ERROR] No se pudo registrar la función '%s'\n", idNodo->valor);
        addError("Error al registrar función void", ambitoActual(), nodo->linea, nodo->columna);
        return EJEC_OK; 
    }
    
    // Guardar el nodo del bloque
    simboloFuncion->nodo_bloque = bloqueNodo;
    
    printf("[DEBUG] Función void '%s' declarada exitosamente\n", idNodo->valor);
    return EJEC_OK;
}