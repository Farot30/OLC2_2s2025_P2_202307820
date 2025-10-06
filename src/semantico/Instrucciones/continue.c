#include "continue.h"
#include <stdlib.h>
#include "../../AST/ejecutar_ast.h"
#include "../../AST/err.h"
#include "../tabla_simbolo.h"
#include "../../simbolos/Evaluador.h"

SentenciaContinue* crearSentenciaContinue(NodoAST* nodo) {
    SentenciaContinue* sentencia = (SentenciaContinue*)malloc(sizeof(SentenciaContinue));
    sentencia->nodo = nodo;
    return sentencia;
}

void liberarSentenciaContinue(SentenciaContinue* sentencia) {
    if (!sentencia) return;
    free(sentencia);
}

ResultadoEjecucion ejecutarSentenciaContinue(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_CONTINUE) return EJEC_OK;

    if(contexto_continue == 0) {
        addError("Sentencia CONTINUE debe estar dentro de un ciclo", ambitoActual(), nodo->linea, nodo->columna);
        return EJEC_OK;
    }

    return EJEC_CONTINUE;
}