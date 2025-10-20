#include "declaracionesVec_arm64.h"
#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../../semantico/tipos.h"

void generarDeclaracionVectorARM64(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_DECLARACION_VECTOR) return;

    NodoAST* tipoNodo = nodo->hijos[0];
    NodoAST* idNodo = nodo->hijos[1];
    const char* nombreVector = idNodo->valor;
    const char* tipoVector = tipoNodo->valor;
    TipoDato tipo = obtenerTipoDesdeString(tipoVector);

    // Caso: int[] nums = new int[5];
    if (nodo->num_hijos >= 4 && nodo->hijos[2]->tipo == AST_TIPO && nodo->hijos[3]->tipo == AST_1DIMENSION) {
        NodoAST* dimensionNodo = nodo->hijos[3]->hijos[0]; // AST_NUMERO
        int tam = atoi(dimensionNodo->valor);

        printf("[DEBUG VECTOR] Declarando '%s' como vector de %d elementos en .bss\n", nombreVector, tam);

        char buffer[128];
        snprintf(buffer, sizeof(buffer), "%s: .space %d\n", nombreVector, tam * 4);
        insertarEnSeccionBss(gen, buffer);

    // Caso: int[] primes = { 2, 3, 5, 7, 11 };
    } else if (nodo->num_hijos >= 3 && nodo->hijos[2]->tipo == AST_LISTA_ELEMENTOS) {
        NodoAST* lista = nodo->hijos[2];
        printf("[DEBUG VECTOR] Nodo lista principal tipo: %d, valor: %s, hijos: %d\n", lista->tipo, lista->valor ? lista->valor : "(null)", lista->num_hijos);

        // Descender si el primer hijo es otra lista (tipo 37)
        if (lista->num_hijos == 1 && (lista->hijos[0]->tipo == AST_LISTA_ELEMENTOS || lista->hijos[0]->tipo == AST_INTERNO)) {
            lista = lista->hijos[0];
            printf("[DEBUG VECTOR] Descendiendo a sublista tipo: %d, valor: %s, hijos: %d\n", lista->tipo, lista->valor ? lista->valor : "(null)", lista->num_hijos);
        }

        int cantidad = lista->num_hijos;
        // Calcula el tamaño necesario para los valores
        size_t valores_len = 0;
        for (int i = 0; i < cantidad; i++) {
            NodoAST* elem = lista->hijos[i];
            valores_len += strlen(elem->valor) + 3; // +3 por posible comillas y coma
        }
        valores_len += 1; // Null terminator

        char* valores = malloc(valores_len);
        if (!valores) return;
        valores[0] = '\0';

        for (int i = 0; i < cantidad; i++) {
            NodoAST* elem = lista->hijos[i];
            if (tipo == TIPO_STRING) {
                if (elem->valor[0] != '"') {
                    strcat(valores, "\"");
                    strcat(valores, elem->valor);
                    strcat(valores, "\"");
                } else {
                    strcat(valores, elem->valor);
                }
            } else if (tipo == TIPO_BOOLEAN) {
                if (strcmp(elem->valor, "true") == 0) {
                    strcat(valores, "1");
                } else {
                    strcat(valores, "0");
                }
            } else {
                strcat(valores, elem->valor);
            }
            if (i < cantidad - 1) strcat(valores, ", ");
        }

        // Calcula el tamaño total para el string final
        size_t total_len = strlen(nombreVector) + strlen(valores) + 32;
        char* buffer = malloc(total_len);
        if (!buffer) {
            free(valores);
            return;
        }

        if (tipo == TIPO_STRING) {
            snprintf(buffer, total_len, "%s: .asciz %s\n", nombreVector, valores);
        } else if (tipo == TIPO_INT) {
            snprintf(buffer, total_len, "%s: .word %s\n", nombreVector, valores);
        } else if (tipo == TIPO_DOUBLE) {
            snprintf(buffer, total_len, "%s: .double %s\n", nombreVector, valores);
        } else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) {
            snprintf(buffer, total_len, "%s: .byte %s\n", nombreVector, valores);
        } else {
            snprintf(buffer, total_len, "%s: .word %s\n", nombreVector, valores);
        }

        printf("[DEBUG VECTOR] Generando declaración: %s", buffer);
        insertarEnSeccionData(gen, buffer);

        free(valores);
        free(buffer);
    }
}