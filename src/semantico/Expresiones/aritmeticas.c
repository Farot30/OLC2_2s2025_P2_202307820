#include "aritmeticas.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include <stdio.h>
#include "../../AST/err.h"  


const char* nombreTipo(TipoDato tipo);

int verificacionAritmetico(NodoAST* nodo){
    if (!nodo || nodo->num_hijos < 2 || !nodo->hijos[0] || !nodo->hijos[1]) {
        printf("[ERROR] Nodo aritmético mal formado\n");
        return 0;
    }
    NodoAST* hijoIzquierdo = nodo->hijos[0];
    NodoAST* hijoDerecho = nodo->hijos[1];
    TipoDato tipoIzquierdo = deducirTipoExpr(hijoIzquierdo);
    TipoDato tipoDerecho = deducirTipoExpr(hijoDerecho);

    // Permitir suma de cadenas
    if (nodo->valor && strcmp(nodo->valor, "+") == 0) {
        if (tipoIzquierdo == TIPO_STRING || tipoDerecho == TIPO_STRING) {
            return 1; // Permitir concatenación
        }
    }

    // Validación numérica para otras operaciones
    int numIzq = (tipoIzquierdo == TIPO_INT || tipoIzquierdo == TIPO_FLOAT || tipoIzquierdo == TIPO_DOUBLE || tipoIzquierdo == TIPO_CHAR);
    int numDer = (tipoDerecho == TIPO_INT || tipoDerecho == TIPO_FLOAT || tipoDerecho == TIPO_DOUBLE || tipoDerecho == TIPO_CHAR);

    if (!numIzq || !numDer) {
        char msg[160];
        snprintf(msg, sizeof(msg), "Operación inválida entre %s y %s",
                 nombreTipo(tipoIzquierdo), nombreTipo(tipoDerecho));
        addError(msg, "global", nodo->linea, nodo->columna);
        printf("%s\n", msg);
        return 0;
    }
    return 1;
}