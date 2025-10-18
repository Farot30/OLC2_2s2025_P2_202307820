#include "Switch_arm64.h"
#include "../generador_arm64.h"
#include "../InstruccionesNorm_arm64/Operadores_arm64.h"
#include "../../semantico/semantico.h"
#include "Break_continue_arm64.h"
#include <stdio.h>
#include <string.h>

static int contador_switch = 0;

void generarCodigoSwitch(GeneradorDeclaraciones* gen, NodoAST* nodoSwitch) {
    if (!nodoSwitch || nodoSwitch->num_hijos < 2) return;

    int id_switch = contador_switch++;
    char etiqueta_fin[32], etiqueta_default[32];
    snprintf(etiqueta_fin, sizeof(etiqueta_fin), "switch_end_%d", id_switch);
    snprintf(etiqueta_default, sizeof(etiqueta_default), "switch_default_%d", id_switch);

    pushContextoBucle(etiqueta_fin, "", "switch");

    NodoAST* exprNodo = nodoSwitch->hijos[0];      // Variable a comparar
    NodoAST* casosNodo = nodoSwitch->hijos[1];     // Nodo de casos

    // Cargar valor de la variable a comparar en w0
    char buffer[256];
    snprintf(buffer, sizeof(buffer),
        "    // Evaluación de expresión switch\n"
        "    ldr x1, =%s\n"
        "    ldr w0, [x1]\n", exprNodo->valor);
    insertarEnSeccionText(gen, buffer);

    // Generar saltos para cada CASE
    for (int i = 0; i < casosNodo->num_hijos; i++) {
        NodoAST* caso = casosNodo->hijos[i];
        if (!caso) continue;

        if (caso->tipo == AST_CASE && caso->num_hijos >= 2) {
            char etiqueta_case[32];
            snprintf(etiqueta_case, sizeof(etiqueta_case), "switch_case_%d_%d", id_switch, i);

            // Valor del case
            NodoAST* valorCase = caso->hijos[0];
            snprintf(buffer, sizeof(buffer),
                "    mov w1, #%d\n"
                "    cmp w0, w1\n"
                "    beq %s\n", atoi(valorCase->valor), etiqueta_case);
            insertarEnSeccionText(gen, buffer);
        } else if (caso->tipo == AST_DEFAULT) {
            // Saltar a default si ningún case coincide
            snprintf(buffer, sizeof(buffer),
                "    b %s\n", etiqueta_default);
            insertarEnSeccionText(gen, buffer);
        }
    }

    // Generar bloques de cada CASE
    for (int i = 0; i < casosNodo->num_hijos; i++) {
        NodoAST* caso = casosNodo->hijos[i];
        if (!caso) continue;

        if (caso->tipo == AST_CASE && caso->num_hijos >= 2) {
            char etiqueta_case[32];
            snprintf(etiqueta_case, sizeof(etiqueta_case), "switch_case_%d_%d", id_switch, i);

            snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_case);
            insertarEnSeccionText(gen, buffer);

            // Procesar instrucciones del case (excepto valor y break)
            for (int j = 1; j < caso->num_hijos; j++) {
                NodoAST* hijo = caso->hijos[j];
                if (hijo->tipo == AST_BREAK) {
                    snprintf(buffer, sizeof(buffer), "    b %s\n", etiqueta_fin);
                    insertarEnSeccionText(gen, buffer);
                } else {
                    GeneradorCodigoARM64(hijo);
                }
            }
        } else if (caso->tipo == AST_DEFAULT) {
            snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_default);
            insertarEnSeccionText(gen, buffer);

            // Procesar instrucciones del default
            for (int j = 0; j < caso->num_hijos; j++) {
                GeneradorCodigoARM64(caso->hijos[j]);
            }
            // No hace falta saltar al fin, ya que es el último
        }
    }

    // Etiqueta de fin del switch
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_fin);
    insertarEnSeccionText(gen, buffer);

    popContextoBucle();
}