#include "DeclaracionesMat_arm64.h"
#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../../semantico/tipos.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void generarDeclaracionMatrizARM64(GeneradorDeclaraciones* gen, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_DECLARACION_MATRIZ) {
        printf("[ERROR] Nodo inválido en generarDeclaracionMatrizARM64\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] Nodo con menos de 2 hijos en generarDeclaracionMatrizARM64\n");
        return;
    }

    NodoAST* tipoNodo = nodo->hijos[0];
    NodoAST* idNodo = nodo->hijos[1];
    const char* nombreMatriz = idNodo->valor;
    const char* tipoMatriz = tipoNodo->valor;
    TipoDato tipo = obtenerTipoDesdeString(tipoMatriz);

    printf("[DEBUG MATRIZ] Procesando declaración de matriz '%s' tipo '%s'\n", nombreMatriz, tipoMatriz);

    // ===========================================================================
    // CASO 1: float[][] mat = new float[3][3];
    // Estructura: AST_TIPO, AST_IDENTIFICADOR, AST_TIPO, AST_DIMENSIONES
    // ===========================================================================
    if (nodo->num_hijos >= 4 && 
        nodo->hijos[2]->tipo == AST_TIPO && 
        nodo->hijos[3]->tipo == AST_DIMENSIONES) {
        
        NodoAST* dimensionesNodo = nodo->hijos[3];
        
        if (dimensionesNodo->num_hijos < 2) {
            printf("[ERROR] Se esperaban al menos 2 dimensiones para la matriz\n");
            return;
        }

        // Extraer las dimensiones (filas y columnas)
        int filas = atoi(dimensionesNodo->hijos[0]->valor);
        int columnas = atoi(dimensionesNodo->hijos[1]->valor);
        int totalElementos = filas * columnas;

        printf("[DEBUG MATRIZ] Declarando '%s' como matriz [%d][%d] con %d elementos\n", 
               nombreMatriz, filas, columnas, totalElementos);

        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));

        // Generar declaración según el tipo
        if (tipo == TIPO_INT) {
            snprintf(buffer, sizeof(buffer), "%s: .word ", nombreMatriz);
            for (int i = 0; i < totalElementos; i++) {
                strcat(buffer, "0");
                if (i < totalElementos - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        } 
        else if (tipo == TIPO_FLOAT) {
            snprintf(buffer, sizeof(buffer), "%s: .float ", nombreMatriz);
            for (int i = 0; i < totalElementos; i++) {
                strcat(buffer, "0.0");
                if (i < totalElementos - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        } 
        else if (tipo == TIPO_DOUBLE) {
            snprintf(buffer, sizeof(buffer), "%s: .double ", nombreMatriz);
            for (int i = 0; i < totalElementos; i++) {
                strcat(buffer, "0.0");
                if (i < totalElementos - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        } 
        else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) {
            snprintf(buffer, sizeof(buffer), "%s: .byte ", nombreMatriz);
            for (int i = 0; i < totalElementos; i++) {
                strcat(buffer, "0");
                if (i < totalElementos - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        } 
        else if (tipo == TIPO_STRING) {
            // Para strings, crear un array de punteros
            for (int i = 0; i < totalElementos; i++) {
                char strbuf[64];
                snprintf(strbuf, sizeof(strbuf), "%s_str_%d: .asciz \"\"\n", nombreMatriz, i);
                strcat(buffer, strbuf);
            }
            strcat(buffer, nombreMatriz);
            strcat(buffer, ": .quad ");
            for (int i = 0; i < totalElementos; i++) {
                char refbuf[32];
                snprintf(refbuf, sizeof(refbuf), "%s_str_%d", nombreMatriz, i);
                strcat(buffer, refbuf);
                if (i < totalElementos - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        }
        else {
            // Tipo por defecto: int
            snprintf(buffer, sizeof(buffer), "%s: .word ", nombreMatriz);
            for (int i = 0; i < totalElementos; i++) {
                strcat(buffer, "0");
                if (i < totalElementos - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        }

        printf("[DEBUG MATRIZ] Generando en .data:\n%s", buffer);
        insertarEnSeccionData(gen, buffer);
        return;
    }

    // ===========================================================================
    // CASO 2: int[][] board = { {0,1,0}, {1,0,1}, {0,1,0} };
    // Estructura: AST_TIPO, AST_IDENTIFICADOR, AST_LISTA_ELEMENTOS
    // ===========================================================================
    if (nodo->num_hijos >= 3 && nodo->hijos[2]->tipo == AST_LISTA_ELEMENTOS) {
        NodoAST* listaElementos = nodo->hijos[2];
        
        printf("[DEBUG MATRIZ] Procesando matriz con inicialización literal\n");
        printf("[DEBUG MATRIZ] Lista elementos - tipo: %d, hijos: %d\n", 
               listaElementos->tipo, listaElementos->num_hijos);

        // Descender a la lista de filas
        NodoAST* listaFilas = listaElementos;
        if (listaElementos->num_hijos == 1 && 
            (listaElementos->hijos[0]->tipo == AST_LISTA_ELEMENTOS || 
             listaElementos->hijos[0]->tipo == 5)) {
            listaFilas = listaElementos->hijos[0];
            printf("[DEBUG MATRIZ] Descendiendo a lista de filas - tipo: %d, hijos: %d\n", 
                   listaFilas->tipo, listaFilas->num_hijos);
        }

        int numFilas = listaFilas->num_hijos;
        if (numFilas == 0) {
            printf("[ERROR] No se encontraron filas en la matriz\n");
            return;
        }

        // Contar columnas desde la primera fila
        NodoAST* primeraFila = listaFilas->hijos[0];
        int numColumnas = primeraFila->num_hijos;

        printf("[DEBUG MATRIZ] Dimensiones detectadas: [%d][%d]\n", numFilas, numColumnas);

        // Validar que todas las filas tengan el mismo número de columnas
        for (int i = 1; i < numFilas; i++) {
            if (listaFilas->hijos[i]->num_hijos != numColumnas) {
                printf("[ERROR] La fila %d tiene %d columnas, pero se esperaban %d\n",
                       i, listaFilas->hijos[i]->num_hijos, numColumnas);
                return;
            }
        }

        // Calcular tamaño necesario para el buffer
        int totalElementos = numFilas * numColumnas;
        size_t bufferSize = strlen(nombreMatriz) + (totalElementos * 20) + 1024;
        char* buffer = malloc(bufferSize);
        if (!buffer) {
            printf("[ERROR] No se pudo asignar memoria para el buffer\n");
            return;
        }
        buffer[0] = '\0';

        // Construir la declaración según el tipo
        if (tipo == TIPO_STRING) {
            // Para strings: crear cada string individual
            int idx = 0;
            for (int i = 0; i < numFilas; i++) {
                NodoAST* fila = listaFilas->hijos[i];
                for (int j = 0; j < fila->num_hijos; j++) {
                    NodoAST* elem = fila->hijos[j];
                    char strbuf[128];
                    const char* valorStr = elem->valor;
                    // Quitar comillas si existen
                    if (valorStr[0] == '"') valorStr++;
                    size_t len = strlen(valorStr);
                    char* valorSinComillas = malloc(len + 1);
                    strncpy(valorSinComillas, valorStr, len);
                    if (valorSinComillas[len-1] == '"') valorSinComillas[len-1] = '\0';
                    else valorSinComillas[len] = '\0';
                    
                    snprintf(strbuf, sizeof(strbuf), "%s_str_%d: .asciz \"%s\"\n", 
                             nombreMatriz, idx, valorSinComillas);
                    strcat(buffer, strbuf);
                    free(valorSinComillas);
                    idx++;
                }
            }
            // Declarar el array de punteros
            char quadbuf[256];
            snprintf(quadbuf, sizeof(quadbuf), "%s: .quad ", nombreMatriz);
            strcat(buffer, quadbuf);
            for (int i = 0; i < totalElementos; i++) {
                char refbuf[32];
                snprintf(refbuf, sizeof(refbuf), "%s_str_%d", nombreMatriz, i);
                strcat(buffer, refbuf);
                if (i < totalElementos - 1) strcat(buffer, ", ");
            }
            strcat(buffer, "\n");
        } 
        else {
            // Para tipos numéricos: construir la lista de valores
            const char* directiva;
            if (tipo == TIPO_INT) directiva = ".word";
            else if (tipo == TIPO_FLOAT) directiva = ".float";
            else if (tipo == TIPO_DOUBLE) directiva = ".double";
            else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) directiva = ".byte";
            else directiva = ".word"; // Por defecto

            snprintf(buffer, bufferSize, "%s: %s ", nombreMatriz, directiva);

            // Recorrer todas las filas y columnas en orden
            for (int i = 0; i < numFilas; i++) {
                NodoAST* fila = listaFilas->hijos[i];
                for (int j = 0; j < fila->num_hijos; j++) {
                    NodoAST* elem = fila->hijos[j];
                    
                    // Convertir booleanos a 0/1
                    if (tipo == TIPO_BOOLEAN) {
                        if (strcmp(elem->valor, "true") == 0) {
                            strcat(buffer, "1");
                        } else {
                            strcat(buffer, "0");
                        }
                    } else {
                        strcat(buffer, elem->valor);
                    }

                    // Agregar coma si no es el último elemento
                    if (!(i == numFilas - 1 && j == fila->num_hijos - 1)) {
                        strcat(buffer, ", ");
                    }
                }
            }
            strcat(buffer, "\n");
        }

        printf("[DEBUG MATRIZ] Generando en .data:\n%s", buffer);
        insertarEnSeccionData(gen, buffer);
        free(buffer);
        return;
    }

    printf("[ERROR] Formato de declaración de matriz no reconocido\n");
}