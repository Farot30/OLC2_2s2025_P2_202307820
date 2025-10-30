#include "AccesoMatriz_arm64.h"
#include "../../AST/ast.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../../semantico/tipos.h"
#include "../../semantico/semantico.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern NodoAST* astGlobal;

/**
 * Función auxiliar para detectar si un acceso es a vector o matriz
 * Retorna el número de dimensiones del acceso
 */
int contarDimensionesAcceso(NodoAST* accesoNodo) {
    if (!accesoNodo || accesoNodo->tipo != AST_ACCESO) return 0;
    if (accesoNodo->num_hijos < 2) return 0;
    
    NodoAST* dimensionesNodo = accesoNodo->hijos[1];
    if (!dimensionesNodo || dimensionesNodo->tipo != AST_DIMENSIONES) return 0;
    
    // Contar cuántos hijos de tipo AST_1DIMENSION hay
    int count = 0;
    for (int i = 0; i < dimensionesNodo->num_hijos; i++) {
        if (dimensionesNodo->hijos[i]->tipo == AST_1DIMENSION) {
            count++;
        }
    }
    
    return count;
}

/**
 * Función auxiliar para obtener las dimensiones de una matriz desde su declaración
 */
void obtenerDimensionesMatriz(NodoAST* matrizDecl, int* filas, int* columnas) {
    *filas = 0;
    *columnas = 0;
    
    if (!matrizDecl || matrizDecl->tipo != AST_DECLARACION_MATRIZ) return;
    
    // Caso 1: new float[3][3]
    if (matrizDecl->num_hijos >= 4 && matrizDecl->hijos[3]->tipo == AST_DIMENSIONES) {
        NodoAST* dimNodo = matrizDecl->hijos[3];
        if (dimNodo->num_hijos >= 2) {
            *filas = atoi(dimNodo->hijos[0]->valor);
            *columnas = atoi(dimNodo->hijos[1]->valor);
            printf("[DEBUG MATRIZ] Dimensiones desde 'new': [%d][%d]\n", *filas, *columnas);
            return;
        }
    }
    
    // Caso 2: {{0,1,2}, {3,4,5}, {6,7,8}}
    if (matrizDecl->num_hijos >= 3 && matrizDecl->hijos[2]->tipo == AST_LISTA_ELEMENTOS) {
        NodoAST* listaElementos = matrizDecl->hijos[2];
        NodoAST* listaFilas = listaElementos;
        
        if (listaElementos->num_hijos == 1 && listaElementos->hijos[0]->tipo == AST_LISTA_ELEMENTOS) {
            listaFilas = listaElementos->hijos[0];
        }
        
        *filas = listaFilas->num_hijos;
        if (*filas > 0 && listaFilas->hijos[0]->tipo == AST_INTERNO) {
            *columnas = listaFilas->hijos[0]->num_hijos;
        }
        
        printf("[DEBUG MATRIZ] Dimensiones desde inicialización literal: [%d][%d]\n", *filas, *columnas);
    }
}

/**
 * Genera código ARM64 para acceso a matriz
 * Fórmula: offset = (fila * numColumnas + columna) * tamaño_elemento
 */
ResultadoExprEmbebida generarAccesoMatrizARM64(GeneradorDeclaraciones* gen, NodoAST* accesoNodo, int* regIndex) {
    ResultadoExprEmbebida resultado = {0};
    memset(&resultado, 0, sizeof(ResultadoExprEmbebida));
    
    if (!accesoNodo || accesoNodo->tipo != AST_ACCESO) {
        printf("[ERROR] Nodo inválido para acceso a matriz\n");
        return resultado;
    }
    
    if (accesoNodo->num_hijos < 2) {
        printf("[ERROR] Acceso a matriz sin suficientes hijos\n");
        return resultado;
    }
    
    // Obtener identificador de la matriz
    NodoAST* idNodo = accesoNodo->hijos[0];
    if (!idNodo || idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Identificador inválido en acceso a matriz\n");
        return resultado;
    }
    
    const char* nombreMatriz = idNodo->valor;
    printf("[DEBUG MATRIZ] Generando acceso a matriz '%s'\n", nombreMatriz);
    
    // Buscar la declaración de la matriz
    NodoAST* matrizDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_MATRIZ, nombreMatriz);
    if (!matrizDecl) {
        printf("[ERROR] Matriz '%s' no encontrada en el AST\n", nombreMatriz);
        return resultado;
    }
    
    // Obtener tipo base y dimensiones
    TipoDato tipoBase = obtenerTipoMatrizDesdeAST(matrizDecl);
    int numFilas = 0, numColumnas = 0;
    obtenerDimensionesMatriz(matrizDecl, &numFilas, &numColumnas);
    
    if (numColumnas == 0) {
        printf("[ERROR] No se pudieron determinar las dimensiones de la matriz '%s'\n", nombreMatriz);
        return resultado;
    }
    
    printf("[DEBUG MATRIZ] Tipo base: %d, Dimensiones: [%d][%d]\n", tipoBase, numFilas, numColumnas);
    
    // Obtener los índices
    NodoAST* dimensionesNodo = accesoNodo->hijos[1];
    if (dimensionesNodo->num_hijos < 2) {
        printf("[ERROR] Se esperaban 2 dimensiones para acceso a matriz\n");
        return resultado;
    }
    
    NodoAST* dimFila = dimensionesNodo->hijos[0];
    NodoAST* dimColumna = dimensionesNodo->hijos[1];
    
    if (dimFila->tipo != AST_1DIMENSION || dimColumna->tipo != AST_1DIMENSION) {
        printf("[ERROR] Formato de dimensiones inválido\n");
        return resultado;
    }
    
    NodoAST* indiceFila = dimFila->hijos[0];
    NodoAST* indiceColumna = dimColumna->hijos[0];
    
    char codigo[512] = "";
    char buffer[256];
    
    // Determinar tamaño del elemento
    int tamElemento = 4; // Por defecto: int
    const char* instrLoad = "ldr w";
    
    switch (tipoBase) {
        case TIPO_INT:
            tamElemento = 4;
            instrLoad = "ldr w";
            break;
        case TIPO_FLOAT:
            tamElemento = 4;
            instrLoad = "ldr s";
            break;
        case TIPO_DOUBLE:
            tamElemento = 8;
            instrLoad = "ldr d";
            break;
        case TIPO_CHAR:
        case TIPO_BOOLEAN:
            tamElemento = 1;
            instrLoad = "ldrb w";
            break;
        case TIPO_STRING:
            tamElemento = 8; // Puntero
            instrLoad = "ldr x";
            break;
        default:
            tamElemento = 4;
            instrLoad = "ldr w";
    }
    
    snprintf(buffer, sizeof(buffer), 
        "    // Acceso a matriz %s[fila][columna]\n", nombreMatriz);
    strcat(codigo, buffer);
    
    // PASO 1: Evaluar índice de fila
    int regFila = *regIndex;
    if (indiceFila->tipo == AST_NUMERO) {
        snprintf(buffer, sizeof(buffer), 
            "    mov w%d, #%s\n", regFila, indiceFila->valor);
    } else if (indiceFila->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s\n"
            "    ldr w%d, [x%d]\n",
            regFila, indiceFila->valor, regFila, regFila);
    } else {
        // Expresión compleja
        ResultadoExprEmbebida resFila = generarExprEmbebidaARM64(gen, indiceFila, regIndex);
        strcat(codigo, resFila.codigo);
        snprintf(buffer, sizeof(buffer), 
            "    mov w%d, %s\n", regFila, resFila.resultado);
    }
    strcat(codigo, buffer);
    (*regIndex)++;
    
    // PASO 2: Evaluar índice de columna
    int regColumna = *regIndex;
    if (indiceColumna->tipo == AST_NUMERO) {
        snprintf(buffer, sizeof(buffer), 
            "    mov w%d, #%s\n", regColumna, indiceColumna->valor);
    } else if (indiceColumna->tipo == AST_IDENTIFICADOR) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x%d, =%s\n"
            "    ldr w%d, [x%d]\n",
            regColumna, indiceColumna->valor, regColumna, regColumna);
    } else {
        // Expresión compleja
        ResultadoExprEmbebida resCol = generarExprEmbebidaARM64(gen, indiceColumna, regIndex);
        strcat(codigo, resCol.codigo);
        snprintf(buffer, sizeof(buffer), 
            "    mov w%d, %s\n", regColumna, resCol.resultado);
    }
    strcat(codigo, buffer);
    (*regIndex)++;
    
    // PASO 3: Calcular offset = (fila * numColumnas + columna) * tamElemento
    int regOffset = *regIndex;
    snprintf(buffer, sizeof(buffer),
        "    // Calcular offset: (fila * %d + columna) * %d\n"
        "    mov w%d, #%d\n"           // w_temp = numColumnas
        "    mul w%d, w%d, w%d\n"      // fila * numColumnas
        "    add w%d, w%d, w%d\n"      // + columna
        "    mov w%d, #%d\n"           // tamaño elemento
        "    mul w%d, w%d, w%d\n",     // * tamElemento
        numColumnas, tamElemento,
        regOffset + 1, numColumnas,
        regOffset, regFila, regOffset + 1,
        regOffset, regOffset, regColumna,
        regOffset + 1, tamElemento,
        regOffset, regOffset, regOffset + 1);
    strcat(codigo, buffer);
    (*regIndex) += 2;
    
    // PASO 4: Cargar el valor
    int regResultado = *regIndex;
    snprintf(buffer, sizeof(buffer),
        "    ldr x%d, =%s\n"
        "    add x%d, x%d, w%d, sxtw\n"
        "    %s%d, [x%d]\n",
        regResultado, nombreMatriz,
        regResultado, regResultado, regOffset,
        instrLoad, regResultado, regResultado);
    strcat(codigo, buffer);
    
    // Configurar resultado
    strcpy(resultado.codigo, codigo);
    
    if (tipoBase == TIPO_FLOAT) {
        snprintf(resultado.resultado, sizeof(resultado.resultado), "s%d", regResultado);
    } else if (tipoBase == TIPO_DOUBLE) {
        snprintf(resultado.resultado, sizeof(resultado.resultado), "d%d", regResultado);
    } else if (tipoBase == TIPO_STRING) {
        snprintf(resultado.resultado, sizeof(resultado.resultado), "x%d", regResultado);
    } else {
        snprintf(resultado.resultado, sizeof(resultado.resultado), "w%d", regResultado);
    }
    
    resultado.tipo = tipoBase;
    (*regIndex)++;
    
    printf("[DEBUG MATRIZ] Acceso generado, resultado en: %s\n", resultado.resultado);
    return resultado;
}

/**
 * Función auxiliar para obtener el tipo base de una matriz desde su declaración
 */
TipoDato obtenerTipoMatrizDesdeAST(NodoAST* matrizDecl) {
    if (!matrizDecl || matrizDecl->tipo != AST_DECLARACION_MATRIZ) {
        return TIPO_DESCONOCIDO;
    }
    
    if (matrizDecl->num_hijos < 1) {
        return TIPO_DESCONOCIDO;
    }
    
    NodoAST* tipoNodo = matrizDecl->hijos[0];
    if (!tipoNodo || tipoNodo->tipo != AST_TIPO) {
        return TIPO_DESCONOCIDO;
    }
    
    return obtenerTipoDesdeString(tipoNodo->valor);
}

void generarAccesoMatrizParaAsignacion(GeneradorDeclaraciones* gen, NodoAST* nodo, const char* nombreDestino) {
    int regIndex = 1;
    ResultadoExprEmbebida resultado = generarAccesoMatrizARM64(gen, nodo, &regIndex);
    
    if (!nombreDestino) {
        // Solo insertar el código de acceso
        insertarEnSeccionText(gen, resultado.codigo);
        return;
    }
    
    // Insertar código de acceso
    insertarEnSeccionText(gen, resultado.codigo);
    
    // Buscar el tipo del destino en el AST
    char buffer[512];
    TipoDato tipoDest = TIPO_INT;
    
    // Intentar buscar como variable normal
    NodoAST* destNodo = buscarNodoEnAST(astGlobal, AST_DECLARACION, nombreDestino);
    if (destNodo && destNodo->num_hijos > 0) {
        NodoAST* tipoNodo = destNodo->hijos[0];
        if (tipoNodo->tipo == AST_TIPO && tipoNodo->valor) {
            tipoDest = obtenerTipoDesdeString(tipoNodo->valor);
        }
    }
    
    printf("[DEBUG ACCESO MATRIZ] Asignando resultado a '%s' (tipo: %d)\n", nombreDestino, tipoDest);
    
    // ✅ GENERAR CÓDIGO DE ASIGNACIÓN SEGÚN EL TIPO
    if (tipoDest == TIPO_DOUBLE) {
        snprintf(buffer, sizeof(buffer),
            "    // Asignar resultado del acceso a matriz a %s\n"
            "    ldr x1, =%s\n"
            "    str %s, [x1]\n",
            nombreDestino, nombreDestino, resultado.resultado);
    } else if (tipoDest == TIPO_FLOAT) {
        snprintf(buffer, sizeof(buffer),
            "    // Asignar resultado del acceso a matriz a %s\n"
            "    ldr x1, =%s\n"
            "    str %s, [x1]\n",
            nombreDestino, nombreDestino, resultado.resultado);
    } else if (tipoDest == TIPO_CHAR || tipoDest == TIPO_BOOLEAN) {
        snprintf(buffer, sizeof(buffer),
            "    // Asignar resultado del acceso a matriz a %s\n"
            "    ldr x1, =%s\n"
            "    strb %s, [x1]\n",
            nombreDestino, nombreDestino, resultado.resultado);
    } else if (tipoDest == TIPO_STRING) {
        snprintf(buffer, sizeof(buffer),
            "    // Asignar resultado del acceso a matriz a %s\n"
            "    ldr x1, =%s\n"
            "    str %s, [x1]\n",
            nombreDestino, nombreDestino, resultado.resultado);
    } else { // TIPO_INT por defecto
        snprintf(buffer, sizeof(buffer),
            "    // Asignar resultado del acceso a matriz a %s\n"
            "    ldr x1, =%s\n"
            "    str %s, [x1]\n",
            nombreDestino, nombreDestino, resultado.resultado);
    }
    insertarEnSeccionText(gen, buffer);
    
    printf("[DEBUG ACCESO MATRIZ] Código de asignación generado para '%s'\n", nombreDestino);
}