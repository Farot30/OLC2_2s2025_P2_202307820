#include "Evaluador.h"
#include "Operador.h"
#include "EvaluadorArray.h"
#include "../semantico/semantico.h"
#include "../semantico/Vectores/acceso_vector.h"
#include "../semantico/Vectores/acceso_matriz.h"
#include "../semantico/Vectores/acceso_arrays.h"
#include "../AST/err.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Función principal para evaluar acceso a arrays
ValorSimbolo evaluarAccesoArrays(NodoAST* nodo) {
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    
    if (!nodo || nodo->tipo != AST_ACCESO) {
        printf("[EVALUADOR] Nodo de acceso inválido\n");
        return r;
    }

    // Obtener identificador
    if (nodo->num_hijos < 1 || nodo->hijos[0]->tipo != AST_IDENTIFICADOR) {
        printf("[EVALUADOR] No se encontró identificador en acceso\n");
        return r;
    }
    
    NodoAST* idNodo = nodo->hijos[0];
    Simbolo* simbolo = buscarSimbolo(idNodo->valor);
    
    if (!simbolo) {
        printf("[EVALUADOR] Variable '%s' no declarada\n", idNodo->valor);
        return r;
    }
    
    // Identificar el tipo de estructura de datos y dirigir a la función correcta
    if (simbolo->clase == SIMBOLO_VECTOR) {
        // Verificar acceso a vector (validación semántica)
        if (verificarAccesoVector(nodo)) {
            // Evaluar acceso a vector
            return evaluarAccesoVector(nodo);
        }
    } 
    else if (simbolo->clase == SIMBOLO_MATRIZ) {
        // Verificar acceso a matriz (validación semántica)
        if (verificarAccesoMatriz(nodo)) {
            // Evaluar acceso a matriz
            return evaluarAccesoMatriz(nodo);
        }
    }
    else if (simbolo->clase == SIMBOLO_ARRAY_MULTIDIMENSIONAL) {
    // Verificar que el acceso a array sea válido
    if (!verificarAccesoArray(nodo)) {
        printf("[ERROR] Acceso inválido a array '%s'\n", idNodo->valor);
        return r;
    }
    
    // Evaluar acceso a array
    ValorSimbolo valorArray = evaluarAccesoArray(nodo);
    if (valorArray.inicializado) {
        printf("[DEBUG EVALUADOR] Acceso a array evaluado exitosamente, valor = ");
        // Imprimir el valor según su tipo (código similar al existente)
        return valorArray;
    }
    } else {
        printf("[EVALUADOR] Variable '%s' no es un vector ni matriz ni array\n", idNodo->valor);
    }
    
    return r;
}