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

    // **PASO 1: INICIALIZACIÓN (SOLO UNA VEZ)**
    GeneradorCodigoARM64(inicializacion);

    char buffer[256];
    
    // **PASO 2: DETECTAR SI ES UN BUCLE ANIDADO Y OBTENER VARIABLE INTERNA**
    NodoAST* forInterno = NULL;
    NodoAST* inicializacionInterna = NULL;
    char* variableInterna = NULL;
    char* valorInicialInterno = NULL;
    
    if (cuerpo && cuerpo->tipo == AST_BLOQUE && cuerpo->num_hijos > 0) {
        for (int i = 0; i < cuerpo->num_hijos; i++) {
            if (cuerpo->hijos[i] && cuerpo->hijos[i]->tipo == AST_FOR) {
                forInterno = cuerpo->hijos[i];
                
                // **EXTRAER INFORMACIÓN DEL FOR INTERNO**
                if (forInterno->num_hijos >= 1) {
                    inicializacionInterna = forInterno->hijos[0]; // Declaración del for interno
                    
                    if (inicializacionInterna && 
                        inicializacionInterna->tipo == AST_DECLARACION && 
                        inicializacionInterna->num_hijos >= 3) {
                        
                        NodoAST* idNodoInterno = inicializacionInterna->hijos[1]; // Variable (j)
                        NodoAST* valorNodoInterno = inicializacionInterna->hijos[2]; // Valor inicial (1)
                        
                        if (idNodoInterno && valorNodoInterno && valorNodoInterno->tipo == AST_NUMERO) {
                            variableInterna = idNodoInterno->valor;
                            valorInicialInterno = valorNodoInterno->valor;
                            printf("[DEBUG FOR] Bucle anidado detectado: variable interna='%s', valor='%s'\n", 
                                   variableInterna, valorInicialInterno);
                        }
                    }
                }
                break;
            }
        }
    }

    // **PASO 3: ETIQUETA DE INICIO DEL BUCLE EXTERNO**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_inicio);
    insertarEnSeccionText(gen, buffer);

    // **PASO 4: SI HAY BUCLE ANIDADO, REINICIALIZAR VARIABLE INTERNA**
    if (variableInterna && valorInicialInterno) {
        snprintf(buffer, sizeof(buffer),
            "    // Reinicializar variable del bucle interno: %s = %s\n"
            "    mov w0, #%s\n"
            "    ldr x1, =%s\n"
            "    str w0, [x1]\n\n",
            variableInterna, valorInicialInterno, valorInicialInterno, variableInterna);
        insertarEnSeccionText(gen, buffer);
    }

    // **PASO 5: CONDICIÓN**
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

    // **PASO 6: CUERPO DEL BUCLE**
    if (cuerpo) {
        if (cuerpo->tipo == AST_BLOQUE) {
            generarBloqueMejorado(gen, cuerpo);
        } else {
            GeneradorCodigoARM64(cuerpo);
        }
    }

    // **PASO 7: ETIQUETA DE INCREMENTO**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_incremento);
    insertarEnSeccionText(gen, buffer);

    // **PASO 8: INCREMENTO**
    if (incremento) {
        GeneradorCodigoARM64(incremento);
    }

    // **PASO 9: VOLVER AL INICIO DEL BUCLE**
    snprintf(buffer, sizeof(buffer),
        "    // Volver al inicio del bucle\n"
        "    b %s\n", etiqueta_inicio);
    insertarEnSeccionText(gen, buffer);

    // **PASO 10: ETIQUETA DE FIN DEL BUCLE**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_fin);
    insertarEnSeccionText(gen, buffer);

    popContextoBucle();
}