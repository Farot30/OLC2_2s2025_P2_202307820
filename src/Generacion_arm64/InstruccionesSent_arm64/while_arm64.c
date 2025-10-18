#include "while_arm64.h"
#include "../generador_arm64.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "If_arm64.h"
#include "Break_continue_arm64.h"  // **AGREGAR ESTA LÍNEA**

static int contador_while_arm64 = 0;

void generarCodigoWhile(GeneradorDeclaraciones* gen, NodoAST* nodoWhile) {
    if(!nodoWhile || nodoWhile->num_hijos < 2) return;
    
    int id_while = contador_while_arm64++;
    char etiqueta_inicio[30], etiqueta_fin[30];
    sprintf(etiqueta_inicio, "while_inicio_%d", id_while);
    sprintf(etiqueta_fin, "while_fin_%d", id_while);

    NodoAST* condicion = nodoWhile->hijos[0];
    NodoAST* cuerpo = nodoWhile->hijos[1];

    // **PUSH CONTEXTO PARA BREAK/CONTINUE**
    pushContextoBucle(etiqueta_fin, etiqueta_inicio, "while");

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s:\n", etiqueta_inicio);
    insertarEnSeccionText(gen, buffer);

    if(condicion){
        char codigoCondicion[512];
        evaluarCondicionBooleanaMejorada(gen, condicion, codigoCondicion);
        insertarEnSeccionText(gen, codigoCondicion);
        snprintf(buffer, sizeof(buffer),
                 "    cmp w0, #0\n"
                 "    beq %s\n", etiqueta_fin);
        insertarEnSeccionText(gen, buffer);
    }

    if (cuerpo){
        generarBloqueMejorado(gen, cuerpo);
    }

    snprintf(buffer, sizeof(buffer), 
                "    b %s\n"
                "%s:\n", etiqueta_inicio, etiqueta_fin);
    insertarEnSeccionText(gen, buffer);

    // **POP CONTEXTO**
    popContextoBucle();
}