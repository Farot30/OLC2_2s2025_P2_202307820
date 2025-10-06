#include "break.h"
#include <stdlib.h>
#include "../../AST/ejecutar_ast.h"
#include "../../AST/err.h"
#include "../tabla_simbolo.h"
#include "../../simbolos/Evaluador.h"

SentenciaBreak* crearSentenciaBreak(NodoAST* nodo) {
    SentenciaBreak* sentencia = (SentenciaBreak*)malloc(sizeof(SentenciaBreak));
    sentencia->nodo = nodo;
    return sentencia;
}

void liberarSentenciaBreak(SentenciaBreak* sentencia) {
    if (!sentencia) return;
    free(sentencia);
}

ResultadoEjecucion ejecutarSentenciaBreak(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_BREAK) return EJEC_OK;

    if (contexto_break == 0) {
        addError("Sentencia BREAK debe estar dentro de un ciclo o switch", ambitoActual(), nodo->linea, nodo->columna);
        return EJEC_OK;
    }

    printf("[DEBUG BREAK] Break detectado dentro de un contexto válido\n");
    return EJEC_BREAK;
}