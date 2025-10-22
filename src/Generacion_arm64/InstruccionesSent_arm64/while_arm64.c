#include "while_arm64.h"
#include "../generador_arm64.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "If_arm64.h"
#include "Break_continue_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"

static int contador_while_arm64 = 0;

// ...existing code...
void generarCodigoWhile(GeneradorDeclaraciones* gen, NodoAST* nodoWhile) {
    if (!nodoWhile || nodoWhile->num_hijos < 2) return;

    int id_while = contador_while_arm64++;
    char etiqueta_inicio[30], etiqueta_fin[30];
    sprintf(etiqueta_inicio, "while_inicio_%d", id_while);
    sprintf(etiqueta_fin, "while_fin_%d", id_while);

    NodoAST* condicion = nodoWhile->hijos[0];
    NodoAST* cuerpo = nodoWhile->hijos[1];

    pushContextoBucle(etiqueta_fin, etiqueta_inicio, "while");

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_inicio);
    insertarEnSeccionText(gen, buffer);

    // **EVALUAR CONDICIÓN**
    if (condicion) {
        char codigoCondicion[512] = "";
        int regIndex = 1;

        ResultadoExprEmbebida resultadoCond = generarExprEmbebidaARM64(gen, condicion, &regIndex);
        strcat(codigoCondicion, resultadoCond.codigo);

        // **SALTAR SI w0 == 0 (condición falsa)**
        snprintf(buffer, sizeof(buffer),
            "    // Saltar a else si condición es falsa\n"
            "    cbz w0, %s\n\n", etiqueta_fin); // ✅ cbz en vez de beq
        strcat(codigoCondicion, buffer);

        insertarEnSeccionText(gen, codigoCondicion);
    }

    // **GENERAR CUERPO**
    if (cuerpo) {
        generarBloqueMejorado(gen, cuerpo);
    }

    // **SALTAR AL INICIO**
    snprintf(buffer, sizeof(buffer),
             "    b %s\n"
             "%s:\n", etiqueta_inicio, etiqueta_fin);
    insertarEnSeccionText(gen, buffer);

    popContextoBucle();
}