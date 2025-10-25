#include "Do_While_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include "../generador_arm64.h"
#include "Break_continue_arm64.h"
#include "If_arm64.h"
#include <stdio.h>
#include <string.h>

static int contador_do_while = 0;

void generarCodigoDoWhile(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_DO_WHILE) {
        printf("[ERROR] Nodo inválido para do-while\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] Do-while requiere bloque y condición\n");
        return;
    }

    NodoAST* bloqueNodo = nodo->hijos[0];
    NodoAST* condicionNodo = nodo->hijos[1];

    int id_do_while = contador_do_while++;
    char etiqueta_inicio[64];
    char etiqueta_condicion[64];
    char etiqueta_fin[64];

    snprintf(etiqueta_inicio, sizeof(etiqueta_inicio), "do_while_start_%d", id_do_while);
    snprintf(etiqueta_condicion, sizeof(etiqueta_condicion), "do_while_cond_%d", id_do_while);
    snprintf(etiqueta_fin, sizeof(etiqueta_fin), "do_while_end_%d", id_do_while);

    char codigo[512];

    pushContextoBucle(etiqueta_fin, etiqueta_condicion, "do-while");

    // Comentario inicial
    snprintf(codigo, sizeof(codigo),
        "    // Do-While loop %d\n"
        "%s:\n",
        id_do_while, etiqueta_inicio);
    insertarEnSeccionText(gen, codigo);

    if (bloqueNodo && bloqueNodo->tipo == AST_BLOQUE) {
        generarBloqueMejorado(gen, bloqueNodo);
    } else {
        GeneradorCodigoARM64(bloqueNodo);
    }

    snprintf(codigo, sizeof(codigo), "%s:\n", etiqueta_condicion);
    insertarEnSeccionText(gen, codigo);

    int regIndex = 1;
    ResultadoExprEmbebida condicion = generarExprEmbebidaARM64(gen, condicionNodo, &regIndex);
    insertarEnSeccionText(gen, condicion.codigo);

    snprintf(codigo, sizeof(codigo),
        "    // Saltar al inicio si condición es verdadera\n"
        "    cmp %s, #1\n"
        "    beq %s\n",
        condicion.resultado, etiqueta_inicio);
    insertarEnSeccionText(gen, codigo);

    snprintf(codigo, sizeof(codigo), "%s:\n", etiqueta_fin);
    insertarEnSeccionText(gen, codigo);

    popContextoBucle();

    printf("[DEBUG] Código do-while generado con ID %d\n", id_do_while);
}