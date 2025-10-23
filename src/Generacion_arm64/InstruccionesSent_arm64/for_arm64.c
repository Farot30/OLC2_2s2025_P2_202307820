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

    NodoAST* inicializacion = nodoFor->hijos[0];
    NodoAST* condicion = nodoFor->hijos[1];
    NodoAST* incremento = nodoFor->hijos[2];
    NodoAST* cuerpo = nodoFor->hijos[3];

    pushContextoBucle(etiqueta_fin, etiqueta_incremento, "for");

    char buffer[512];

    // **PASO 1: INICIALIZACIÓN**
    if (inicializacion) {
        // ✅ VERIFICAR SI ES UNA DECLARACIÓN
        if (inicializacion->tipo == AST_DECLARACION && inicializacion->num_hijos >= 2) {
            NodoAST* idNodo = inicializacion->hijos[1];
            NodoAST* valorNodo = inicializacion->num_hijos > 2 ? inicializacion->hijos[2] : NULL;
            
            const char* nombreVariable = idNodo->valor;
            
            // **DECLARAR LA VARIABLE (solo si no existe)**
            GeneradorCodigoARM64(inicializacion);
            
            // **REINICIALIZAR EN .TEXT PARA FOR ANIDADOS**
            if (valorNodo) {
                printf("[DEBUG FOR] Reinicializando variable '%s' en .text\n", nombreVariable);
                
                if (valorNodo->tipo == AST_NUMERO) {
                    snprintf(buffer, sizeof(buffer),
                        "    // Reinicialización de %s (for anidado)\n"
                        "    ldr x1, =%s\n"
                        "    mov w0, #%s\n"
                        "    str w0, [x1]\n",
                        nombreVariable, nombreVariable, valorNodo->valor);
                    insertarEnSeccionText(gen, buffer);
                } else if (valorNodo->tipo == AST_IDENTIFICADOR) {
                    snprintf(buffer, sizeof(buffer),
                        "    // Reinicialización de %s desde %s\n"
                        "    ldr x1, =%s\n"
                        "    ldr w0, [x1]\n"
                        "    ldr x1, =%s\n"
                        "    str w0, [x1]\n",
                        nombreVariable, valorNodo->valor,
                        valorNodo->valor, nombreVariable);
                    insertarEnSeccionText(gen, buffer);
                } else {
                    // Expresión compleja
                    int regIndex = 1;
                    ResultadoExprEmbebida resultadoInit = generarExprEmbebidaARM64(gen, valorNodo, &regIndex);
                    insertarEnSeccionText(gen, resultadoInit.codigo);
                    
                    snprintf(buffer, sizeof(buffer),
                        "    ldr x1, =%s\n"
                        "    str %s, [x1]\n",
                        nombreVariable, resultadoInit.resultado);
                    insertarEnSeccionText(gen, buffer);
                }
            }
        } else {
            // Si no es declaración, solo ejecutar
            GeneradorCodigoARM64(inicializacion);
        }
    }
    
    // **PASO 2: ETIQUETA DE INICIO DEL BUCLE**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_inicio);
    insertarEnSeccionText(gen, buffer);

    // **PASO 3: CONDICIÓN**
    if (condicion) {
        char codigoCondicion[512] = "";
        int regIndex = 1;

        ResultadoExprEmbebida resultadoCond = generarExprEmbebidaARM64(gen, condicion, &regIndex);
        strcat(codigoCondicion, resultadoCond.codigo);

        snprintf(buffer, sizeof(buffer),
            "    // Saltar al final si la condición es falsa\n"
            "    cbz w0, %s\n\n", etiqueta_fin);
        strcat(codigoCondicion, buffer);

        insertarEnSeccionText(gen, codigoCondicion);
    }

    // **PASO 4: CUERPO DEL BUCLE**
    if (cuerpo) {
        if (cuerpo->tipo == AST_BLOQUE) {
            generarBloqueMejorado(gen, cuerpo);
        } else {
            GeneradorCodigoARM64(cuerpo);
        }
    }

    // **PASO 5: ETIQUETA DE INCREMENTO**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_incremento);
    insertarEnSeccionText(gen, buffer);

    // **PASO 6: INCREMENTO**
    if (incremento) {
        GeneradorCodigoARM64(incremento);
    }

    // **PASO 7: VOLVER AL INICIO DEL BUCLE**
    snprintf(buffer, sizeof(buffer),
        "    // Volver al inicio del bucle\n"
        "    b %s\n", etiqueta_inicio);
    insertarEnSeccionText(gen, buffer);

    // **PASO 8: ETIQUETA DE FIN DEL BUCLE**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_fin);
    insertarEnSeccionText(gen, buffer);

    popContextoBucle();
}