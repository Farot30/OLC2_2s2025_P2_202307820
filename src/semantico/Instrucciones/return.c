#include "return.h"
#include <stdlib.h>
#include "../../AST/ejecutar_ast.h"
#include "../../AST/err.h"
#include "../tabla_simbolo.h"
#include "../../simbolos/Evaluador.h"
#include "../Funciones/Funcion.h"
#include "../semantico.h"
#include <string.h>

SentenciaReturn* crearSentenciaReturn(NodoAST* nodo) {
    SentenciaReturn* sentencia = (SentenciaReturn*)malloc(sizeof(SentenciaReturn));
    sentencia->nodo = nodo;
    memset(&sentencia->valor, 0, sizeof(ValorSimbolo));
    sentencia->valor.inicializado = 0;
    return sentencia;
}

void liberarSentenciaReturn(SentenciaReturn* sentencia) {
    if (!sentencia) return;
    
    // **SOLO liberar string SI ES REALMENTE UN STRING**
    if (sentencia->valor.tipo == TIPO_STRING && sentencia->valor.v.s) {
        free(sentencia->valor.v.s);
        sentencia->valor.v.s = NULL;
    }
    
    free(sentencia);
}

ResultadoEjecucion ejecutarSentenciaReturn(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_RETURN) return EJEC_OK;

    // Verificar si estamos dentro de una función
    if (contexto_funcion == 0) {
        addError("Sentencia RETURN debe estar dentro de una función", 
                 ambitoActual(), nodo->linea, nodo->columna);
        printf("[ERROR] La instrucción 'return' solo puede aparecer dentro de una función\n");
        return EJEC_OK;
    }

    // Crear estructura para el retorno
    RetornoInfo info;
    memset(&info, 0, sizeof(RetornoInfo));  // **INICIALIZAR TODO A CERO**
    info.nivel_funcion = nivel_funcion_actual;

    // Evaluar la expresión de retorno si existe
    if (nodo->num_hijos > 0) {
        ValorSimbolo nuevoValorRetorno = evaluarExpr(nodo->hijos[0]);
        
        if (nuevoValorRetorno.inicializado) {
            printf("[DEBUG RETURN] Return con valor de tipo %s en nivel %d\n", 
                   nombreTipo(nuevoValorRetorno.tipo), nivel_funcion_actual);

            if (nodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                Simbolo* simboloReturn = buscarSimbolo(nodo->hijos[0]->valor);
                if (simboloReturn && simboloReturn->clase == SIMBOLO_MATRIZ) {
                    printf("[DEBUG RETURN] Retornando matriz con tipo base %s: %s\n", 
                        nombreTipo(nuevoValorRetorno.tipo),
                        nuevoValorRetorno.v.s ? nuevoValorRetorno.v.s : "NULL");
                }
            }
            
            // **COPIA SEGURA DEL VALOR**
            info.valor = nuevoValorRetorno;
            
            // **SOLO para TIPO_STRING hacer strdup**
            if (nuevoValorRetorno.tipo == TIPO_STRING && nuevoValorRetorno.v.s != NULL) {
                info.valor.v.s = strdup(nuevoValorRetorno.v.s);
                printf("[DEBUG RETURN] String duplicado para stack\n");
            }
            // **Para otros tipos, NO modificar v.s - ya está inicializado como NULL por memset**
            
            // Push al stack
            if (stackRetorno.tope < 100) {
                stackRetorno.items[stackRetorno.tope++] = info;
                printf("[DEBUG RETURN] Valor agregado al stack en posición %d\n", 
                       stackRetorno.tope - 1);
            } else {
                printf("[ERROR RETURN] Stack de retorno lleno\n");
            }
            
            hayRetorno = 1;
        } else {
            printf("[ERROR] Valor de retorno no inicializado\n");
            addError("Valor de retorno no inicializado", ambitoActual(), 
                    nodo->linea, nodo->columna);
        }
    } else {
        // Return sin valor (para funciones void)
        Simbolo* funcionActual = buscarSimbolo(ambitoActual());
        if (funcionActual && funcionActual->clase == SIMBOLO_FUNCION && 
            funcionActual->tipo != TIPO_VOID) {
            char msg[200];
            snprintf(msg, sizeof(msg), 
                    "Función con tipo de retorno %s no puede tener un return vacío", 
                    nombreTipo(funcionActual->tipo));
            addError(msg, ambitoActual(), nodo->linea, nodo->columna);
            printf("[ERROR RETURN] %s\n", msg);
        }
        
        // **INICIALIZAR VALOR VOID CORRECTAMENTE**
        info.valor.tipo = TIPO_VOID;
        info.valor.inicializado = 1;
        info.valor.v.s = NULL;  // **ASEGURAR QUE SEA NULL**
        
        // Push al stack
        if (stackRetorno.tope < 100) {
            stackRetorno.items[stackRetorno.tope++] = info;
        }
        
        printf("[DEBUG RETURN] Return sin valor\n");
        hayRetorno = 1;
    }
    
    return EJEC_RETURN;
}