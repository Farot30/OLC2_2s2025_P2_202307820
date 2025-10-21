#include "If_arm64.h"
#include "../InstruccionesNorm_arm64/Operadores_arm64.h"
#include "../../semantico/semantico.h"
#include "../generador_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include <string.h>
#include <stdlib.h>

// Contador global para etiquetas únicas
int contador_etiquetas_if = 0;

EtiquetasIF crearEtiquetasIF() {
    EtiquetasIF etiquetas;
    etiquetas.numero_if = contador_etiquetas_if++;
    
    snprintf(etiquetas.etiqueta_if, sizeof(etiquetas.etiqueta_if), "if_%d", etiquetas.numero_if);
    snprintf(etiquetas.etiqueta_endif, sizeof(etiquetas.etiqueta_endif), "if_end_%d", etiquetas.numero_if);
    snprintf(etiquetas.etiqueta_else, sizeof(etiquetas.etiqueta_else), "if_else_%d", etiquetas.numero_if);
    snprintf(etiquetas.etiqueta_elseif, sizeof(etiquetas.etiqueta_elseif), "if_elseif_%d", etiquetas.numero_if);
    
    return etiquetas;
}

void evaluarCondicionBooleanaMejorada(GeneradorDeclaraciones* gen, NodoAST* condicion, char* codigoResultado) {
    if (!condicion || !codigoResultado) return;
    
    char buffer[512] = "";
    
    if (condicion->tipo == AST_OPERACION) {
        int regIndex = 1;
        generarCodigoRecursivoBoolean(condicion, buffer, &regIndex, gen);
        strcpy(codigoResultado, buffer);
        return;
    }
    
    switch (condicion->tipo) {
        case AST_BOOLEAN:
            if (strcmp(condicion->valor, "true") == 0) {
                strcat(buffer, "    mov w0, #1\n");
            } else {
                strcat(buffer, "    mov w0, #0\n");
            }
            break;
            
        case AST_IDENTIFICADOR: {
            snprintf(buffer, sizeof(buffer),
                "    adrp x1, %s\n"
                "    add x1, x1, :lo12:%s\n"
                "    ldr w0, [x1]\n", condicion->valor, condicion->valor);
            break;
        }
        
        case AST_NUMERO: {
            int valor = atoi(condicion->valor);
            snprintf(buffer, sizeof(buffer), "    mov w0, #%d\n", valor != 0 ? 1 : 0);
            break;
        }
        
        default:
            strcat(buffer, "    mov w0, #0\n");
            break;
    }
    
    strcpy(codigoResultado, buffer);
}

void generarBloqueMejorado(GeneradorDeclaraciones* gen, NodoAST* bloque) {
    if (!bloque) return;
    (void)gen;

    for (int i = 0; i < bloque->num_hijos; i++) {
        NodoAST* hijo = bloque->hijos[i];
        if (hijo) {
            printf("[DEBUG BLOQUE] Procesando hijo tipo: %d en bloque\n", hijo->tipo);
            // **Usa el generador principal para cada hijo**
            GeneradorCodigoARM64(hijo);
        }
    }
}

void generarCodigoIF(GeneradorDeclaraciones* gen, NodoAST* nodoIF) {
    if (!nodoIF || nodoIF->num_hijos < 2) return;
    
    EtiquetasIF etiquetas = crearEtiquetasIF();
    NodoAST* condicion = nodoIF->hijos[0];
    NodoAST* bloqueIF = nodoIF->hijos[1];
    
    printf("[DEBUG] Generando código IF simple con etiquetas únicas\n");
    
    char codigoCompleto[2048] = "";
    char buffer[512];
    
    // **AGREGAR ETIQUETA IF AL INICIO (OPCIONAL)**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiquetas.etiqueta_if);
    strcat(codigoCompleto, buffer);
    
    // **PASO 1: EVALUAR CONDICIÓN**
    snprintf(buffer, sizeof(buffer), "    // Evaluación de condición IF\n");
    strcat(codigoCompleto, buffer);
    
    char codigoCondicion[512];
    int regIndex = 1;
    ResultadoExprEmbebida resultadoCond = generarExprEmbebidaARM64(gen, condicion, &regIndex);
    strcat(codigoCondicion, resultadoCond.codigo);
    
    // **PASO 2: SALTAR SI CONDICIÓN ES FALSA**
    snprintf(buffer, sizeof(buffer),
        "    // Saltar si condición es falsa\n"
        "    cmp w0, #0\n"
        "    beq %s\n\n", etiquetas.etiqueta_endif);
    strcat(codigoCompleto, buffer);
    
    // **INSERTAR TODO LO ANTERIOR**
    insertarEnSeccionText(gen, codigoCompleto);
    
    // **PASO 3: GENERAR BLOQUE IF**
    generarBloqueMejorado(gen, bloqueIF);
    
    // **PASO 4: ETIQUETA DE FINAL**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiquetas.etiqueta_endif);
    insertarEnSeccionText(gen, buffer);
}

void generarCodigoIFElse(GeneradorDeclaraciones* gen, NodoAST* nodoIF) {
    if (!nodoIF || nodoIF->num_hijos < 3) return;
    
    EtiquetasIF etiquetas = crearEtiquetasIF();
    NodoAST* condicion = nodoIF->hijos[0];
    NodoAST* bloqueIF = nodoIF->hijos[1];
    NodoAST* bloqueElse = nodoIF->hijos[2];
    
    printf("[DEBUG] Generando código IF-ELSE con etiquetas únicas\n");
    
    char codigoCompleto[2048] = "";
    char buffer[512];
    
    // **PASO 1: EVALUAR CONDICIÓN**
    snprintf(buffer, sizeof(buffer), "    // Evaluación de condición IF\n");
    strcat(codigoCompleto, buffer);
    
    char codigoCondicion[512];
    evaluarCondicionBooleanaMejorada(gen, condicion, codigoCondicion);
    strcat(codigoCompleto, codigoCondicion);
    
    // **PASO 2: SALTAR A ELSE SI CONDICIÓN ES FALSA**
    snprintf(buffer, sizeof(buffer),
        "    // Saltar a else si condición es falsa\n"
        "    cmp w0, #0\n"
        "    beq %s\n\n", etiquetas.etiqueta_else);
    strcat(codigoCompleto, buffer);
    
    insertarEnSeccionText(gen, codigoCompleto);
    
    // **PASO 3: GENERAR BLOQUE IF**
    generarBloqueMejorado(gen, bloqueIF);
    
    // **PASO 4: SALTAR AL FINAL DESPUÉS DEL BLOQUE IF**
    snprintf(buffer, sizeof(buffer), "    b %s\n\n", etiquetas.etiqueta_endif);
    insertarEnSeccionText(gen, buffer);
    
    // **PASO 5: ETIQUETA ELSE**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiquetas.etiqueta_else);
    insertarEnSeccionText(gen, buffer);
    
    // **PASO 6: GENERAR BLOQUE ELSE**
    generarBloqueMejorado(gen, bloqueElse);
    
    // **PASO 7: ETIQUETA DE FINAL**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiquetas.etiqueta_endif);
    insertarEnSeccionText(gen, buffer);
}

void generarCodigoIFElseIF(GeneradorDeclaraciones* gen, NodoAST* nodoIF) {
    if (!nodoIF || nodoIF->num_hijos < 3) return;
    
    EtiquetasIF etiquetas = crearEtiquetasIF();
    NodoAST* condicion1 = nodoIF->hijos[0];
    NodoAST* bloqueIF = nodoIF->hijos[1];
    NodoAST* nodoElseIF = nodoIF->hijos[2]; // Nodo ELSEIF completo
    
    printf("[DEBUG] Generando código IF-ELSE IF con etiquetas únicas\n");
    
    char codigoCompleto[2048] = "";
    char buffer[512];
    
    // **PASO 1: EVALUAR PRIMERA CONDICIÓN**
    snprintf(buffer, sizeof(buffer), "    // Evaluación de condición IF\n");
    strcat(codigoCompleto, buffer);
    
    char codigoCondicion[512];
    evaluarCondicionBooleanaMejorada(gen, condicion1, codigoCondicion);
    strcat(codigoCompleto, codigoCondicion);
    
    // **PASO 2: SALTAR A ELSEIF SI CONDICIÓN ES FALSA**
    snprintf(buffer, sizeof(buffer),
        "    // Saltar a elseif si condición es falsa\n"
        "    cmp w0, #0\n"
        "    beq %s\n\n", etiquetas.etiqueta_elseif);
    strcat(codigoCompleto, buffer);
    
    insertarEnSeccionText(gen, codigoCompleto);
    
    // **PASO 3: GENERAR BLOQUE IF**
    generarBloqueMejorado(gen, bloqueIF);
    
    // **PASO 4: SALTAR AL FINAL DESPUÉS DEL BLOQUE IF**
    snprintf(buffer, sizeof(buffer), "    b %s\n\n", etiquetas.etiqueta_endif);
    insertarEnSeccionText(gen, buffer);
    
    // **PASO 5: ETIQUETA ELSEIF**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiquetas.etiqueta_elseif);
    insertarEnSeccionText(gen, buffer);
    
    // **PASO 6: PROCESAR ELSEIF RECURSIVAMENTE**
    if (nodoElseIF) {
        GeneradorCodigoARM64(nodoElseIF);
    }
    
    // **PASO 7: ETIQUETA DE FINAL**
    snprintf(buffer, sizeof(buffer), "%s:\n", etiquetas.etiqueta_endif);
    insertarEnSeccionText(gen, buffer);
}