#include "semantico.h"
#include "tabla_simbolo.h"
#include "tipos.h"
#include <stdio.h>
#include <string.h>
#include "Instrucciones/declaracion.h"
#include "Instrucciones/asignacion.h"
#include "Expresiones/aritmeticas.h"
#include "Instrucciones/Print.h"
#include "../AST/err.h"

int error_semantico = 0;

const char* nombreTipo(TipoDato tipo) {
    switch(tipo) {
        case TIPO_INT: return "int";
        case TIPO_FLOAT: return "float";
        case TIPO_DOUBLE: return "double";
        case TIPO_CHAR: return "char";
        case TIPO_STRING: return "string";
        case TIPO_BOOLEAN: return "boolean";
        default: return "NULL";
    }
}

int verificarSemantica(NodoAST* nodo) {
    if (!nodo) return 1;

    int resultado = 1;
    switch (nodo->tipo) {
        case AST_LISTA:
            // Verificar cada instrucción independientemente
            for (int i = 0; i < nodo->num_hijos; ++i) {
                // Si una instrucción falla, no la marcamos como inválida
                // pero tampoco propagamos su invalidez al nodo lista
                if (!verificarSemantica(nodo->hijos[i])) {
                    resultado = 0;
                }
            }
            return resultado;
            
        case AST_DECLARACION: {
            if (!verificarDeclaracion(nodo)) { 
                error_semantico = 1; 
                nodo->valido = 0;  // Marcar declaración completa como inválida
                return 0; 
            }
            return 1;
        }
        
        case AST_ASIGNACION: {
            if (!verificarAsignacionSimple(nodo)) { 
                error_semantico = 1; 
                nodo->valido = 0;  // Marcar asignación completa como inválida
                return 0; 
            }
            return 1;
        }
        case AST_ASIGNACION_COMPUESTA: {
            if (!verificarAsignacionCompuesta(nodo)) { 
                error_semantico = 1; 
                nodo->valido = 0;  // Marcar asignación compuesta como inválida
                return 0; 
            }
            return 1;
        }
        
        case AST_OPERACION: {
            if (!verificacionAritmetico(nodo)) { 
                error_semantico = 1; 
                nodo->valido = 0;  // Marcar operación como inválida
                return 0; 
            }
            return 1;
        }
        
        case AST_IDENTIFICADOR: {
            if (!buscarSimbolo(nodo->valor)) {
                char msg[128];
                snprintf(msg,sizeof(msg),"Variable '%s' no declarada", nodo->valor);
                addError(msg,ambitoActual(),nodo->linea,nodo->columna);
                error_semantico = 1;
                nodo->valido = 0;  // Marcar nodo como inválido
                return 0;
            }
            return 1;
        }
        case AST_PRINT: {
            if (nodo->num_hijos != 1) {
                char msg[128];
                snprintf(msg, sizeof(msg), "Print debe tener exactamente un argumento");
                addError(msg, ambitoActual(), nodo->linea, nodo->columna);
                error_semantico = 1;
                return 0;
            }
            // Solo verificar la expresión, no ejecutarla
            return verificarSemantica(nodo->hijos[0]);
        }
        case AST_CAST: {
            TipoDato dest = obtenerTipoDesdeString(nodo->valor);
            TipoDato orig = deducirTipoExpr(nodo->hijos[0]);
            int ok = (dest!=TIPO_DESCONOCIDO)&&(orig!=TIPO_DESCONOCIDO);
            if(!ok){
                char msg[128];
                snprintf(msg,sizeof(msg),"Cast inválido (%s)", nodo->valor?nodo->valor:"?");
                addError(msg,"global",nodo->linea,nodo->columna);
                error_semantico=1;
                return 0;
            }
            return verificarSemantica(nodo->hijos[0]);
        }
        default:
            break;
    }

    for (int i = 0; i < nodo->num_hijos; ++i) {
        if (!verificarSemantica(nodo->hijos[i])) resultado = 0;
    }
    return resultado;

}