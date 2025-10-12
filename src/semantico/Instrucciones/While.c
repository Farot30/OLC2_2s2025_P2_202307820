#include "While.h"
#include <stdlib.h>
#include "../../AST/ejecutar_ast.h"
#include "../tabla_simbolo.h"
#include "../../simbolos/Evaluador.h"
#include "../../AST/err.h"
extern int contadorWhile;
SentenciaWhile* crearSentenciaWhile(NodoAST* condicion, NodoAST* bloque) {
    SentenciaWhile* sentencia = (SentenciaWhile*)malloc(sizeof(SentenciaWhile));
    sentencia->condicion = condicion;
    sentencia->bloque = bloque;
    return sentencia;
}

void liberarSentenciaWhile(SentenciaWhile* sentencia) {
    if (!sentencia) return;
    free(sentencia);
}

// Función auxiliar para convertir cualquier tipo a booleano
int convertirABooleano(ValorSimbolo valor) {
    switch(valor.tipo) {
        case TIPO_BOOLEAN:
            return valor.v.b;
        case TIPO_INT:
            return valor.v.i != 0;
        case TIPO_FLOAT:
            return valor.v.f != 0.0f;
        case TIPO_DOUBLE:
            return valor.v.d != 0.0;
        case TIPO_CHAR:
            return valor.v.c != 0;
        case TIPO_STRING:
            return valor.v.s != NULL && strlen(valor.v.s) > 0;
        default:
            return 0;  // Por defecto, falso
    }
}
//int max = 10;

//int iteracion = 0;
ResultadoEjecucion ejecutarSetenciaWhile(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_WHILE) {
        printf("[DEBUG WHILE] Nodo inválido para while\n");
        return EJEC_OK;
    }

    if (nodo->num_hijos < 2) {
        printf("[DEBUG WHILE] While sin suficientes hijos (condición y bloque)\n");
        return EJEC_OK;
    }

    NodoAST* condicion = nodo->hijos[0];
    NodoAST* bloque = nodo->hijos[1];
    
    printf("[DEBUG WHILE] Iniciando bucle while\n");
    entrarAmbito("while");
    contexto_break++;
    contexto_continue++;
    
    while (1) {
        // Evaluar la condición
        /*iteracion++;
        printf("iteracion no:%d\n", iteracion);*/
        /*
        if (--max <= 0) {
            printf("[ERROR WHILE] Límite máximo de iteraciones alcanzado, posible bucle infinito\n");
            addError("Límite máximo de iteraciones alcanzado, posible bucle infinito", ambitoActual(), nodo->linea, nodo->columna);
            break;
        }*/
        ValorSimbolo vCond = evaluarExpr(condicion);

        printf("[DEBUG WHILE] Condición evaluada - inicializado: %d, tipo: %d\n", 
               vCond.inicializado, vCond.tipo);

        if (!vCond.inicializado) {
            printf("[ERROR WHILE] Condición del WHILE no está inicializada\n");
            addError("Condición del WHILE no está inicializada", ambitoActual(), nodo->linea, nodo->columna);
            break;
        }

        // Convertir la condición a booleano
        int condicionVerdadera = convertirABooleano(vCond);
        printf("[DEBUG WHILE] Condición convertida a boolean: %s\n", 
               condicionVerdadera ? "true" : "false");


        if (!condicionVerdadera) {
            printf("[DEBUG WHILE] Condición falsa, ejecutando una vez más\n");
            break;
        }

        // Ejecutar el bloque
        printf("[DEBUG WHILE] Ejecutando bloque del while\n");
        ResultadoEjecucion res = ejecutarNodoAST(bloque);

        if (res == EJEC_BREAK) {
            printf("[DEBUG WHILE] Break detectado, saliendo del bucle\n");
            break;
        }

        if (res == EJEC_CONTINUE) {
            printf("[DEBUG WHILE] Continue detectado, continuando al siguiente ciclo\n");
            continue;
        }
    }
    salirAmbito();
    contexto_break--;
    contexto_continue--;
    return EJEC_OK;
}