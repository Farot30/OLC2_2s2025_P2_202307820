#include "Switch.h"
#include "../../AST/ejecutar_ast.h"
#include "../../AST/err.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../../simbolos/Evaluador.h"

SentenciaSwitch* CrearSetenciaSwitch(NodoAST* expresion, NodoAST* casos[], int num_casos, NodoAST* casoDefault) {
    SentenciaSwitch* setencia = (SentenciaSwitch*)malloc(sizeof(SentenciaSwitch));
    setencia->expresion = expresion;
    setencia->casos = casos;
    setencia->num_casos = num_casos;
    setencia->casoDefault = casoDefault;
    return setencia;
}

void LiberarSetenciaSwitch(SentenciaSwitch* setencia) {
    if (!setencia) return;
    free(setencia);
}

ResultadoEjecucion EjecturarNodoSwitch(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_SWITCH) return EJEC_OK;

    NodoAST* expresion = nodo->hijos[0];
    NodoAST* casos = nodo->hijos[1];
    NodoAST* casoDefault = NULL;

    // Buscar el nodo DEFAULT dentro de los casos
    for (int i = 0; i < casos->num_hijos; i++) {
        if (casos->hijos[i]->tipo == AST_DEFAULT) {
            casoDefault = casos->hijos[i];
            break;
        }
    }

    // Evaluar la expresión del switch
    ValorSimbolo vexp = evaluarExpr(expresion);

    if (vexp.tipo != TIPO_INT && vexp.tipo != TIPO_CHAR && vexp.tipo != TIPO_STRING) {
        addError("La condición del SWITCH debe ser de tipo int, char o string", ambitoActual(), nodo->linea, nodo->columna);
        return EJEC_OK;
    }

    int casoEncontrado = 0;

    // Iterar sobre los casos
    for (int i = 0; i < casos->num_hijos; i++) {
        NodoAST* caso = casos->hijos[i];
        if (caso->tipo == AST_DEFAULT) continue; // Saltar el caso DEFAULT aquí

        NodoAST* valorCaso = caso->hijos[0];
        NodoAST* bloqueCaso = caso->hijos[1];

        // Evaluar el valor del caso
        ValorSimbolo vCaso = evaluarExpr(valorCaso);

        // Verificar si los tipos coinciden y están inicializados
        if (vexp.tipo == vCaso.tipo && vexp.inicializado && vCaso.inicializado) {
            int esIgual = 0;

            // Comparar los valores según el tipo
            switch (vexp.tipo) {
                case TIPO_INT:
                    esIgual = (vexp.v.i == vCaso.v.i);
                    break;
                case TIPO_CHAR:
                    esIgual = (vexp.v.c == vCaso.v.c);
                    break;
                case TIPO_FLOAT:
                    esIgual = (vexp.v.f == vCaso.v.f);
                    break;
                case TIPO_DOUBLE:
                    esIgual = (vexp.v.d == vCaso.v.d);
                    break;
                case TIPO_STRING:
                    esIgual = (strcmp(vexp.v.s, vCaso.v.s) == 0);
                    break;
                default:
                    break;
            }

            // Si hay coincidencia, ejecutar el bloque del caso
            if (esIgual) {
                entrarAmbito("switch");
                ResultadoEjecucion res = ejecutarNodoAST(bloqueCaso);
                salirAmbito();
                casoEncontrado = 1;

                if (res == EJEC_BREAK) return EJEC_BREAK;
            }
        }

        // Liberar memoria del valor del caso si es una cadena
        if (vCaso.tipo == TIPO_STRING) {
            free(vCaso.v.s);
        }
    }

    // Si no se encontró ningún caso, ejecutar el caso default (si existe)
    if (!casoEncontrado && casoDefault) {
        NodoAST* bloqueDefault = casoDefault->hijos[0];
        entrarAmbito("default");
        ResultadoEjecucion res = ejecutarNodoAST(bloqueDefault);
        salirAmbito();
        return res;
    }

    // Liberar memoria del valor de la expresión si es una cadena
    if (vexp.tipo == TIPO_STRING) {
        free(vexp.v.s);
    }
    return EJEC_OK;
}
