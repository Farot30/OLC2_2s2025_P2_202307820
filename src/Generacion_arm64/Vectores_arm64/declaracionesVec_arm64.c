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

        printf("[DEBUG VECTOR] Declarando '%s' como vector de %d elementos en .data con valores por defecto\n", nombreVector, tam);

        char buffer[1024];
        buffer[0] = '\0';

        // Llenar con valores por defecto según tipo
        if (tipo == TIPO_INT) {
            strcat(buffer, nombreVector);
            strcat(buffer, ": .word ");
            for (int i = 0; i < tam; i++) {
                strcat(buffer, "0");
                if (i < tam - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        } else if (tipo == TIPO_DOUBLE) {
            strcat(buffer, nombreVector);
            strcat(buffer, ": .double ");
            for (int i = 0; i < tam; i++) {
                strcat(buffer, "0.0");
                if (i < tam - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        } else if (tipo == TIPO_BOOLEAN || tipo == TIPO_CHAR) {
            strcat(buffer, nombreVector);
            strcat(buffer, ": .byte ");
            for (int i = 0; i < tam; i++) {
                strcat(buffer, "0");
                if (i < tam - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        } else if (tipo == TIPO_STRING) {
            // 1. Declarar cada string vacío
            for (int i = 0; i < tam; i++) {
                char strbuf[64];
                snprintf(strbuf, sizeof(strbuf), "%s_str_%d: .asciz \"\"\n", nombreVector, i);
                strcat(buffer, strbuf);
            }
            // 2. Declarar el vector como arreglo de punteros
            strcat(buffer, nombreVector);
            strcat(buffer, ": .quad ");
            for (int i = 0; i < tam; i++) {
                char refbuf[32];
                snprintf(refbuf, sizeof(refbuf), "%s_str_%d", nombreVector, i);
                strcat(buffer, refbuf);
                if (i < tam - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        }else {
            // Por defecto, int
            strcat(buffer, nombreVector);
            strcat(buffer, ": .word ");
            for (int i = 0; i < tam; i++) {
                strcat(buffer, "0");
                if (i < tam - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        }

        insertarEnSeccionData(gen, buffer);

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
            // 1. Declarar cada string
            char* strbufs = malloc(cantidad * 64);
            strbufs[0] = '\0';
            for (int i = 0; i < cantidad; i++) {
                NodoAST* elem = lista->hijos[i];
                char strbuf[64];
                // Quitar comillas si existen
                const char* valorStr = elem->valor;
                if (valorStr[0] == '"') valorStr++;
                snprintf(strbuf, sizeof(strbuf), "%s_str_%d: .asciz \"%s\"\n", nombreVector, i, valorStr);
                strcat(strbufs, strbuf);
            }
            // 2. Declarar el vector como arreglo de punteros
            char* quadbuf = malloc(cantidad * 32 + strlen(nombreVector) + 16);
            snprintf(quadbuf, cantidad * 32 + strlen(nombreVector) + 16, "%s: .quad ", nombreVector);
            for (int i = 0; i < cantidad; i++) {
                char refbuf[32];
                snprintf(refbuf, sizeof(refbuf), "%s_str_%d", nombreVector, i);
                strcat(quadbuf, refbuf);
                if (i < cantidad - 1) strcat(quadbuf, ", ");
            }
            strcat(quadbuf, "\n");

            // Unir todo y declarar
            char* buffer = malloc(strlen(strbufs) + strlen(quadbuf) + 1);
            buffer[0] = '\0';
            strcat(buffer, strbufs);
            strcat(buffer, quadbuf);

            insertarEnSeccionData(gen, buffer);

            free(strbufs);
            free(quadbuf);
            free(buffer);
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