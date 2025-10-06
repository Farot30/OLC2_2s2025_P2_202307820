#include "SentIF.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../AST/ejecutar_ast.h"
#include "../../AST/err.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../../simbolos/Evaluador.h"


SentenciaIf* crearSentenciaIf(NodoAST* condicion, NodoAST* bloqueVerdadero, NodoAST* bloqueFalso) {
    SentenciaIf* sentencia = (SentenciaIf*)malloc(sizeof(SentenciaIf));
    sentencia->condicion = condicion;
    sentencia->bloqueVerdadero = bloqueVerdadero;
    sentencia->bloqueFalso = bloqueFalso;
    return sentencia;
}

void liberarSentenciaIf(SentenciaIf* sentencia) {
    if (!sentencia) return;
    free(sentencia);
}

ResultadoEjecucion ejecutarNodoIf(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_IF) return EJEC_OK;

    NodoAST* condicion = nodo->hijos[0];
    NodoAST* bloqueVerdadero = nodo->hijos[1];
    NodoAST* bloqueFalso = (nodo->num_hijos > 2) ? nodo->hijos[2] : NULL;

    ValorSimbolo vCond = evaluarExpr(condicion);

    if (!vCond.inicializado) {
        addError("Condición del IF no está inicializada", ambitoActual(), nodo->linea, nodo->columna);
        return EJEC_OK;
    }

    if (vCond.tipo != TIPO_BOOLEAN) {
        addError("Condición del IF no es booleana", ambitoActual(), nodo->linea, nodo->columna);
        return EJEC_OK;
    }

    if (vCond.v.b) {
        entrarAmbito("if");
        ResultadoEjecucion res = ejecutarNodoAST(bloqueVerdadero);
        salirAmbito();
        if (res == EJEC_RETURN) return EJEC_RETURN; // <-- Propaga el return
        return res;
    } else if (bloqueFalso) {
        entrarAmbito("else");
        ResultadoEjecucion res = ejecutarNodoAST(bloqueFalso);
        salirAmbito();
        if (res == EJEC_RETURN) return EJEC_RETURN; // <-- Propaga el return
        return res;
    }
    return EJEC_OK;
}

