#include "../../AST/ast.h"
#include "../tabla_simbolo.h"
#include "../../AST/err.h"
#include "LlamadasVoid.h"
#include <stdio.h>
#include <string.h>

ResultadoEjecucion EjecutarLlamadaVoid(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_LLAMADA_VOID) {
        printf("[ERROR] Nodo inválido para llamada a función void\n");
        return EJEC_OK; 
    }

    if (nodo->num_hijos < 1 || nodo->hijos[0]->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Llamada a función void requiere un identificador\n");
        addError("Llamada a función void inválida", ambitoActual(), nodo->linea, nodo->columna);
        return EJEC_OK; 
    }

    NodoAST* idNodo = nodo->hijos[0];
    printf("[DEBUG] Llamada a función void: %s\n", idNodo->valor);

    // Buscar la función en la tabla de símbolos
    Simbolo* simboloFuncion = buscarSimbolo(idNodo->valor);
    if (!simboloFuncion) {
        char msg[100];
        snprintf(msg, sizeof(msg), "Función void '%s' no declarada", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return EJEC_OK; 
    }

    // Verificar que sea una función void
    if (simboloFuncion->clase != SIMBOLO_FUNCION_VOID) {
        char msg[100];
        snprintf(msg, sizeof(msg), "'%s' no es una función void", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return EJEC_OK; 
    }

    // Obtener el bloque de la función
    NodoAST* bloqueNodo = simboloFuncion->nodo_bloque;
    if (!bloqueNodo) {
        printf("[ERROR] La función '%s' no tiene un bloque asociado\n", idNodo->valor);
        addError("Función sin bloque de código", ambitoActual(), idNodo->linea, idNodo->columna);
        return EJEC_OK; 
    }

    // Crear un nuevo ámbito para la ejecución de la función
    printf("[DEBUG] Entrando al ámbito de la función void '%s'\n", idNodo->valor);
    entrarAmbito(simboloFuncion->nombre);

    // Ejecutar el bloque de código de la función
    printf("[DEBUG] Ejecutando bloque de la función void '%s'\n", idNodo->valor);
    ResultadoEjecucion resultado = ejecutarNodoAST(bloqueNodo);

    // Verificar si hubo un break o continue que necesita ser propagado
    if (resultado == EJEC_BREAK || resultado == EJEC_CONTINUE) {
        printf("[ADVERTENCIA] Detectado %s en función void '%s', esto no debería ocurrir\n", 
               resultado == EJEC_BREAK ? "break" : "continue", idNodo->valor);
        // No propagamos BREAK/CONTINUE fuera de funciones
        resultado = EJEC_OK;
    }

    // Salir del ámbito de la función
    printf("[DEBUG] Saliendo del ámbito de la función void '%s'\n", idNodo->valor);
    salirAmbito();

    return EJEC_OK;
}