#include "asignacion.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../AST/err.h"  
#include "../../AST/ast.h"
#include "../../AST/errorTIpos.h"
#include "../../simbolos/Evaluador.h"

const char* nombreTipo(TipoDato tipo);

// --- AUXILIAR: Para depuración de nodos ---
void debugNodoAST(NodoAST* nodo, int nivel) {
    if (!nodo) return;
    for (int i = 0; i < nivel; ++i) printf("  ");
    printf("[DEBUG AST] Tipo=%d, Valor='%s', Hijos=%d\n", nodo->tipo, nodo->valor ? nodo->valor : "NULL", nodo->num_hijos);
    for (int i = 0; i < nodo->num_hijos; ++i)
        debugNodoAST(nodo->hijos[i], nivel + 1);
}

// --- AUXILIAR: Evalúa operación compuesta entre valor actual y expr ---
ValorSimbolo evalOpCompuesta(const char* operador, ValorSimbolo valorActual, NodoAST* expr) {
    printf("[DEBUG evalOpCompuesta] Operador: %s, Valor actual tipo: %d\n", operador, valorActual.tipo);
    
    NodoAST* nodoTemp = malloc(sizeof(NodoAST));
    memset(nodoTemp, 0, sizeof(NodoAST));
    nodoTemp->tipo = AST_OPERACION;
    nodoTemp->valor = strdup(operador);
    nodoTemp->num_hijos = 2;
    nodoTemp->hijos = malloc(2 * sizeof(NodoAST*));

    NodoAST* nodoValorActual = malloc(sizeof(NodoAST));
    memset(nodoValorActual, 0, sizeof(NodoAST));
    if (valorActual.tipo == TIPO_STRING) {
        nodoValorActual->tipo = AST_CADENA;
        nodoValorActual->valor = valorActual.v.s ? strdup(valorActual.v.s) : strdup("");
        printf("[DEBUG evalOpCompuesta] Nodo valor actual (string): '%s'\n", nodoValorActual->valor);
    } else {
        nodoValorActual->tipo = AST_NUMERO;
        char valorStr[64];
        switch (valorActual.tipo) {
            case TIPO_INT: snprintf(valorStr, sizeof(valorStr), "%d", valorActual.v.i); break;
            case TIPO_FLOAT: snprintf(valorStr, sizeof(valorStr), "%f", valorActual.v.f); break;
            case TIPO_DOUBLE: snprintf(valorStr, sizeof(valorStr), "%lf", valorActual.v.d); break;
            default: strcpy(valorStr, "0"); break;
        }
        nodoValorActual->valor = strdup(valorStr);
        printf("[DEBUG evalOpCompuesta] Nodo valor actual (número): '%s'\n", nodoValorActual->valor);
    }
    
    nodoTemp->hijos[0] = nodoValorActual;
    nodoTemp->hijos[1] = expr;

    printf("[DEBUG evalOpCompuesta] Evaluando expresión compuesta...\n");
    ValorSimbolo resultado = evaluarExpr(nodoTemp);
    printf("[DEBUG evalOpCompuesta] Resultado tipo: %d, inicializado: %d\n", resultado.tipo, resultado.inicializado);
    
    if (resultado.tipo == TIPO_STRING && resultado.v.s) {
        printf("[DEBUG evalOpCompuesta] Resultado string: '%s'\n", resultado.v.s);
    }

    // Limpiar memoria temporal
    free(nodoTemp->valor);
    free(nodoValorActual->valor);
    free(nodoValorActual);
    free(nodoTemp->hijos);
    free(nodoTemp);

    return resultado;
}

// --- AUXILIAR: Devuelve el operador base de un compuesto (+= -> +, etc) ---
const char* operadorBase(const char* opComp) {
    if (!opComp) return NULL;
    if (strcmp(opComp, "+=") == 0) return "+";
    if (strcmp(opComp, "-=") == 0) return "-";
    if (strcmp(opComp, "*=") == 0) return "*";
    if (strcmp(opComp, "/=") == 0) return "/";
    if (strcmp(opComp, "%=") == 0) return "%";
    if (strcmp(opComp, "&=") == 0) return "&";
    if (strcmp(opComp, "|=") == 0) return "|";
    if (strcmp(opComp, "^=") == 0) return "^";
    if (strcmp(opComp, ">>=") == 0) return ">>";
    if (strcmp(opComp, "<<=") == 0) return "<<";
    return NULL;
}

int verificarAsignacionSimple(NodoAST* nodo) {
    if (!nodo || nodo->num_hijos < 2) {
        printf("[DEBUG verificarAsignacionSimple] Nodo inválido\n");
        return 0;
    }
    
    NodoAST* identificador = nodo->hijos[0];
    NodoAST* valor = nodo->hijos[1];
    
    // Buscar el símbolo con ámbito
    Simbolo* sim = buscarSimboloConAmbito(identificador->valor);
    if (!sim) {
        printf("[DEBUG verificarAsignacionSimple] Símbolo '%s' no encontrado\n", identificador->valor);
        return 0;
    }
    
    if (sim->es_constante) {
        printf("[DEBUG verificarAsignacionSimple] No se puede asignar a constante '%s'\n", identificador->valor);
        return 0;
    }

    TipoDato tipoValor = deducirTipoExpr(valor);
    if (sim->tipo != tipoValor && tipoValor != TIPO_DESCONOCIDO) {
        if (!esWidening(tipoValor, sim->tipo)) {
            char msg[160];
            snprintf(msg, sizeof(msg), "Tipo incompatible en asignación '%s': se esperaba %s, se recibió %s",
                     identificador->valor, nombreTipo(sim->tipo), nombreTipo(tipoValor));
            addError(msg, ambitoActual(), identificador->linea, identificador->columna);
            nodo->valido = 0;
            return 0;
        }
    }
    
    ValorSimbolo valorEvaluado = evaluarExpr(valor);
    if (valorEvaluado.inicializado) {
        setValorSimbolo(sim, valorEvaluado.tipo, valorEvaluado);
        sincronizarReferencia(identificador->valor);
        return 1;
    } else {
        nodo->valido = 0;
        return 0;
    }
}

int verificarAsignacionCompuesta(NodoAST* nodo) {
    printf("[DEBUG verificarAsignacionCompuesta] === INICIANDO VERIFICACIÓN ===\n");
    
    if (!nodo || nodo->num_hijos < 2) {
        printf("[DEBUG verificarAsignacionCompuesta] Nodo inválido o sin hijos suficientes\n");
        return 0;
    }
    
    NodoAST* identificador = nodo->hijos[0];
    NodoAST* valor = nodo->hijos[1];
    
    printf("[DEBUG verificarAsignacionCompuesta] Variable: '%s', Operador: '%s'\n", 
           identificador->valor, nodo->valor);
    
    // Buscar el símbolo con ámbito correcto
    Simbolo* sim = buscarSimboloConDepuracion(identificador->valor);
    
    // Validar el símbolo antes de usarlo
    validarSimbolo(sim, "asignación compuesta");
    if (!sim) {
        printf("[DEBUG verificarAsignacionCompuesta] ERROR: Símbolo '%s' no encontrado\n", identificador->valor);
        return 0;
    }
    
    if (sim->es_constante) {
        printf("[DEBUG verificarAsignacionCompuesta] ERROR: No se puede asignar a constante '%s'\n", identificador->valor);
        return 0;
    }

    // Obtener valor actual
    ValorSimbolo valorActual = getValorSimbolo(sim);
    if (!valorActual.inicializado) {
        printf("[DEBUG verificarAsignacionCompuesta] ERROR: Variable '%s' no inicializada\n", identificador->valor);
        return 0;
    }
    
    printf("[DEBUG verificarAsignacionCompuesta] Valor actual obtenido correctamente\n");

    // Obtener operador base
    const char* op = operadorBase(nodo->valor);
    if (!op) {
        printf("[DEBUG verificarAsignacionCompuesta] ERROR: Operador '%s' no reconocido\n", nodo->valor);
        op = nodo->valor; // Usar el operador tal como está
    }
    
    printf("[DEBUG verificarAsignacionCompuesta] Operador base: '%s'\n", op);

    // Evaluar la operación compuesta
    ValorSimbolo resultado = evalOpCompuesta(op, valorActual, valor);
    
    if (resultado.inicializado) {
        printf("[DEBUG verificarAsignacionCompuesta] Operación evaluada exitosamente\n");
        
        // Asignar el resultado al símbolo
        setValorSimbolo(sim, resultado.tipo, resultado);
        sincronizarReferencia(identificador->valor);
        
        printf("[DEBUG verificarAsignacionCompuesta] Valor asignado exitosamente\n");
        return 1;
    } else {
        printf("[DEBUG verificarAsignacionCompuesta] ERROR: Falló la evaluación de la operación\n");
        nodo->valido = 0;
        return 0;
    }
}