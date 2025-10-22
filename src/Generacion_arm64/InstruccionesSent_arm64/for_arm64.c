#include "for_arm64.h"
#include "../generador_arm64.h"
#include "If_arm64.h"
#include "Break_continue_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include <stdio.h>
#include <string.h>

static int contador_for = 0;

void generarCodigoFor(GeneradorDeclaraciones* gen, NodoAST* nodoFor) {
    if (!nodoFor || nodoFor->num_hijos < 4) return;

    int id_for = contador_for++;
    char etiqueta_inicio[32], etiqueta_fin[32], etiqueta_incremento[32];
    snprintf(etiqueta_inicio, sizeof(etiqueta_inicio), "for_loop_%d", id_for);
    snprintf(etiqueta_fin, sizeof(etiqueta_fin), "for_end_%d", id_for);
    snprintf(etiqueta_incremento, sizeof(etiqueta_incremento), "for_inc_%d", id_for);

    NodoAST* inicializacion = nodoFor->hijos[0]; // Declaración o asignación inicial
    NodoAST* condicion = nodoFor->hijos[1];      // Condición
    NodoAST* incremento = nodoFor->hijos[2];     // Incremento
    NodoAST* cuerpo = nodoFor->hijos[3];         // Bloque

     pushContextoBucle(etiqueta_fin, etiqueta_incremento, "for");

    GeneradorCodigoARM64(inicializacion);

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_inicio);
    insertarEnSeccionText(gen, buffer);

    // CONDICIÓN
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

    // CUERPO DEL BUCLE
    if (cuerpo) {
        generarBloqueMejorado(gen, cuerpo);
    }

    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_incremento);
    insertarEnSeccionText(gen, buffer);

    // INCREMENTO
    if (incremento) {
        GeneradorCodigoARM64(incremento);
    }

    // VOLVER AL INICIO DEL BUCLE
    snprintf(buffer, sizeof(buffer),
        "    // Volver al inicio del bucle\n"
        "    b %s\n", etiqueta_inicio);
    insertarEnSeccionText(gen, buffer);

    // ETIQUETA DE FIN DEL BUCLE
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_fin);
    insertarEnSeccionText(gen, buffer);

    popContextoBucle();
}