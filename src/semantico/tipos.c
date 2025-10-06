#include "tipos.h"
#include "tabla_simbolo.h"
#include "semantico.h"
#include <string.h>
#include <stdlib.h>

TipoDato obtenerTipoDesdeString(const char* tipoStr) {
    if (strcmp(tipoStr, "int") == 0) return TIPO_INT;
    if (strcmp(tipoStr, "float") == 0) return TIPO_FLOAT;
    if (strcmp(tipoStr, "double") == 0) return TIPO_DOUBLE;
    if (strcmp(tipoStr, "char") == 0) return TIPO_CHAR;
    if (strcmp(tipoStr, "string") == 0) return TIPO_STRING;
    if (strcmp(tipoStr, "boolean") == 0) return TIPO_BOOLEAN;
    if (strcmp(tipoStr, "void") == 0 ) return TIPO_VOID;
    if (strcmp(tipoStr, "funcion") == 0 ) return TIPO_FUNCION;
    return TIPO_DESCONOCIDO;
}

static int rankTipo(TipoDato t){
    switch(t){
        case TIPO_CHAR: return 0;
        case TIPO_INT: return 1;
        case TIPO_FLOAT: return 2;
        case TIPO_DOUBLE: return 3;
        default: return -1;
    }
}

static int esNumerico(TipoDato t){
    return t==TIPO_CHAR || t==TIPO_INT || t==TIPO_FLOAT || t==TIPO_DOUBLE;
}

int esWidening(TipoDato origen, TipoDato destino) {
    //printf("[DEBUG esWidening] Verificando widening de %d a %d\n", origen, destino);
    if (!esNumerico(origen) || !esNumerico(destino)) return 0;
    int resultado = rankTipo(origen) <= rankTipo(destino);
    //printf("[DEBUG esWidening] Resultado: %d\n", resultado);
    return resultado;
}

int esNarrowing(TipoDato origen, TipoDato destino){
    if(!esNumerico(origen) || !esNumerico(destino)) return 0;
    return rankTipo(origen) > rankTipo(destino);
}

TipoDato deducirTipoExpr(NodoAST* expr) {
    if (!expr) return TIPO_DESCONOCIDO;

    switch (expr->tipo) {
        case AST_NUMERO:
            if (strchr(expr->valor, '.')) {
                size_t len = strlen(expr->valor);
                if (len > 0 && (expr->valor[len-1] == 'f' || expr->valor[len-1] == 'F'))
                    return TIPO_FLOAT;
                return TIPO_DOUBLE;
            }
            
            return TIPO_INT;
        case AST_CADENA:
            return TIPO_STRING;
        case AST_CARACTER:
            return TIPO_CHAR;
        case AST_BOOLEAN:
            return TIPO_BOOLEAN;
        case AST_IDENTIFICADOR: {
            Simbolo* sim = buscarSimbolo(expr->valor);
            if (sim) return sim->tipo;
            return TIPO_DESCONOCIDO;
        }
        case AST_OPERACION: {
            if (expr->num_hijos != 2) return TIPO_DESCONOCIDO;

            // Operadores bit a bit siempre devuelven int
            if (strcmp(expr->valor, "<<") == 0 || strcmp(expr->valor, ">>") == 0 ||
                strcmp(expr->valor, "&") == 0 || strcmp(expr->valor, "|") == 0 || 
                strcmp(expr->valor, "^") == 0) {
                return TIPO_INT;
            }

            TipoDato tipoIzq = deducirTipoExpr(expr->hijos[0]);
            TipoDato tipoDer = deducirTipoExpr(expr->hijos[1]);
            
            // Regla de promoción de tipos para operaciones aritméticas
            if (tipoIzq == TIPO_DOUBLE || tipoDer == TIPO_DOUBLE) return TIPO_DOUBLE;
            if (tipoIzq == TIPO_FLOAT || tipoDer == TIPO_FLOAT) return TIPO_FLOAT;
            if (tipoIzq == TIPO_INT || tipoDer == TIPO_INT) return TIPO_INT;
            if (tipoIzq == TIPO_CHAR || tipoDer == TIPO_CHAR) return TIPO_INT; // chars se promueven a int
            
            return TIPO_DESCONOCIDO;
        }

        case AST_CAST: {
            // valor guarda el string del tipo destino
            TipoDato destino = obtenerTipoDesdeString(expr->valor);
            TipoDato original = deducirTipoExpr(expr->hijos[0]);
            // Si es cast válido numérico, regresar destino, sino desconocido
            if(esNumerico(original) && esNumerico(destino)) return destino;
            if(destino == original) return destino;
            return TIPO_DESCONOCIDO;
        }
        case AST_EQUAL: {
            if (expr->num_hijos == 2) {
                TipoDato tipoIzq = deducirTipoExpr(expr->hijos[0]);
                TipoDato tipoDer = deducirTipoExpr(expr->hijos[1]);
                if ((esNumerico(tipoIzq) && esNumerico(tipoDer)) || tipoIzq == tipoDer)
                    return TIPO_BOOLEAN;
                return TIPO_DESCONOCIDO;
            }
            return TIPO_DESCONOCIDO;
        }
        case AST_FUNCION_VOID:
            return TIPO_VOID;
        case AST_FUNCION:
            return TIPO_FUNCION;
        default:
            return TIPO_DESCONOCIDO;
    }
}

const char* valorPorDefectoTipo(TipoDato t){
    switch(t){
        case TIPO_INT: return "0";
        case TIPO_FLOAT: return "0.0f";
        case TIPO_DOUBLE: return "0.0";
        case TIPO_CHAR: return "\\u0000";
        case TIPO_STRING: return "null";
        case TIPO_BOOLEAN: return "false";
        case TIPO_VOID: return "void";
        case TIPO_FUNCION: return "funcion";
        default: return "?";
    }
}
