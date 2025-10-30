#include "tipos.h"
#include "tabla_simbolo.h"
#include "semantico.h"
#include <string.h>
#include <stdlib.h>

const char* obtenerNombreTipo(TipoDato tipo) {
    switch (tipo) {
        case TIPO_INT: return "int";
        case TIPO_DOUBLE: return "double";
        case TIPO_FLOAT: return "float";
        case TIPO_CHAR: return "char";
        case TIPO_BOOLEAN: return "boolean";
        case TIPO_STRING: return "String";
        default: return "desconocido";
    }
}

TipoDato obtenerTipoDesdeString(const char* str) {
    if (!str) return TIPO_DESCONOCIDO;
    
    // **PRIMERO: tipos primitivos**
    if (strcmp(str, "int") == 0) return TIPO_INT;
    if (strcmp(str, "float") == 0) return TIPO_FLOAT;
    if (strcmp(str, "double") == 0) return TIPO_DOUBLE;
    if (strcmp(str, "char") == 0) return TIPO_CHAR;
    if (strcmp(str, "String") == 0) return TIPO_STRING;
    if (strcmp(str, "string") == 0) return TIPO_STRING; // por si acaso
    if (strcmp(str, "boolean") == 0) return TIPO_BOOLEAN;
    if (strcmp(str, "void") == 0) return TIPO_VOID;
    
    // **SEGUNDO: matrices - MANTENER EL TIPO ORIGINAL**
    if (strstr(str, "[][]") != NULL) {
        char tipoBase[50] = {0};
        char* pos = strstr(str, "[][]");
        size_t len = pos - str;
        if (len > 0 && len < sizeof(tipoBase)) {
            strncpy(tipoBase, str, len);
            tipoBase[len] = '\0';
            
            TipoDato tipoElemento = obtenerTipoDesdeString(tipoBase);
            printf("[DEBUG TIPOS] Función matriz '%s' -> tipo base '%s' (%d)\n", 
                   str, tipoBase, tipoElemento);
            
            // **RETORNAR EL TIPO BASE, NO STRING**
            // La matriz se identifica por la clase SIMBOLO_MATRIZ, no por el tipo
            return tipoElemento;
        }
    } else if (strstr(str, "[]") != NULL) {
        char tipoBase[50] = {0};
        size_t len = strlen(str) - 2;
        if (len > 0 && len < sizeof(tipoBase)) {
            strncpy(tipoBase, str, len);
            tipoBase[len] = '\0';
            
            TipoDato tipoElemento = obtenerTipoDesdeString(tipoBase);
            printf("[DEBUG TIPOS] Función vector '%s' -> tipo base '%s' (%d)\n", 
                   str, tipoBase, tipoElemento);
            return tipoElemento;
        }
    }
    
    printf("[DEBUG TIPOS] Tipo desconocido: '%s'\n", str);
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


// Busca el tipo de un identificador en el AST, recursivamente
TipoDato buscarTipoIdentificadorAST(const char* nombre, NodoAST* nodo) {
    if (!nodo) return TIPO_DESCONOCIDO;

    // Si es una declaración y el identificador coincide
    if (nodo->tipo == AST_DECLARACION && nodo->num_hijos >= 2) {
        NodoAST* tipoNodo = nodo->hijos[0];
        NodoAST* idNodo = nodo->hijos[1];
        if (idNodo && idNodo->tipo == AST_IDENTIFICADOR && strcmp(idNodo->valor, nombre) == 0) {
            return obtenerTipoDesdeString(tipoNodo->valor);
        }
    }
    // Buscar recursivamente en los hijos
    for (int i = 0; i < nodo->num_hijos; i++) {
        TipoDato t = buscarTipoIdentificadorAST(nombre, nodo->hijos[i]);
        if (t != TIPO_DESCONOCIDO) return t;
    }

    if (nodo->tipo == AST_DECLARACION_VECTOR && nodo->num_hijos >= 2) {
        NodoAST* idNodo = nodo->hijos[1];
        if (idNodo && idNodo->tipo == AST_IDENTIFICADOR && strcmp(idNodo->valor, nombre) == 0) {
            return TIPO_VECTOR;
        }
    }
    return TIPO_DESCONOCIDO;
}

TipoDato deducirTipoExprARM64(NodoAST* expr, NodoAST* raizAST) {
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
        case AST_STRING:
            return TIPO_STRING;
        case AST_CARACTER:
            return TIPO_CHAR;
        case AST_BOOLEAN:
            return TIPO_BOOLEAN;
        case AST_IDENTIFICADOR: {
            TipoDato tipoDeducido = buscarTipoIdentificadorAST(expr->valor, raizAST);
            if (tipoDeducido != TIPO_DESCONOCIDO) {
                //printf("[DEBUG TIPO] Identificador: %s, tipo deducido (AST): %d\n", expr->valor, tipoDeducido);
                return tipoDeducido;
            }
            
            // Buscar en la tabla de símbolos (variables y constantes)
            Simbolo* sim = buscarSimbolo(expr->valor);
            if (sim) {
                //printf("[DEBUG TIPO] Identificador: %s, tipo deducido (tabla): %d\n", expr->valor, sim->tipo);
                return sim->tipo;
            }
            //printf("[DEBUG TIPO] Identificador: %s, tipo deducido: %d\n", expr->valor, TIPO_DESCONOCIDO);
            return TIPO_DESCONOCIDO;
        }
        case AST_OPERACION: {
            if (strcmp(expr->valor, "!") == 0 && expr->num_hijos == 1) {
                TipoDato hijoTipo = deducirTipoExprARM64(expr->hijos[0], raizAST);
                if (hijoTipo == TIPO_BOOLEAN) return TIPO_BOOLEAN;
                return TIPO_DESCONOCIDO;
            }
            // --- CORRECCIÓN: Si es concatenación y uno es string, retorna string ---
            if (strcmp(expr->valor, "+") == 0) {
                TipoDato tipoIzq = deducirTipoExprARM64(expr->hijos[0], raizAST);
                TipoDato tipoDer = deducirTipoExprARM64(expr->hijos[1], raizAST);
                if (tipoIzq == TIPO_STRING || tipoDer == TIPO_STRING) return TIPO_STRING;
                // Si ambos son char, también puede considerarse string (opcional)
            }

            if (strcmp(expr->valor, "&&") == 0 ||
                strcmp(expr->valor, "||") == 0) {
                return TIPO_BOOLEAN;
            }
            // Resto de operaciones aritméticas
            TipoDato tipoIzq = deducirTipoExprARM64(expr->hijos[0], raizAST);
            TipoDato tipoDer = deducirTipoExprARM64(expr->hijos[1], raizAST);
            if (tipoIzq == TIPO_DOUBLE || tipoDer == TIPO_DOUBLE) return TIPO_DOUBLE;
            if (tipoIzq == TIPO_FLOAT || tipoDer == TIPO_FLOAT) return TIPO_FLOAT;
            if (tipoIzq == TIPO_INT || tipoDer == TIPO_INT) return TIPO_INT;
            if (tipoIzq == TIPO_CHAR || tipoDer == TIPO_CHAR) return TIPO_INT;
            return TIPO_DESCONOCIDO;
        }
        default:
            return TIPO_DESCONOCIDO;
    }
}

TipoDato obtenerTipoBaseVector(NodoAST* raiz, const char* nombre) {
    if (!raiz) return TIPO_DESCONOCIDO;

    // Busca la declaración del vector por nombre
    if (raiz->tipo == AST_DECLARACION_VECTOR && raiz->num_hijos >= 2) {
        NodoAST* tipoNodo = raiz->hijos[0];
        NodoAST* idNodo = raiz->hijos[1];
        if (idNodo && idNodo->tipo == AST_IDENTIFICADOR && strcmp(idNodo->valor, nombre) == 0) {
            // El tipo base está en tipoNodo->valor
            return obtenerTipoDesdeString(tipoNodo->valor);
        }
    }
    // Buscar recursivamente en los hijos
    for (int i = 0; i < raiz->num_hijos; i++) {
        TipoDato t = obtenerTipoBaseVector(raiz->hijos[i], nombre);
        if (t != TIPO_DESCONOCIDO) return t;
    }
    return TIPO_DESCONOCIDO;
}