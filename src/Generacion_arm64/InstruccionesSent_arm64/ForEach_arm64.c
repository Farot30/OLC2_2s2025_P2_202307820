#include "ForEach_arm64.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../InstruccionesNorm_arm64/Operadores_arm64.h"
#include "../generador_arm64.h"
#include "../../semantico/semantico.h"
#include "../../semantico/tipos.h"
#include "../../semantico/tabla_simbolo.h"
#include "../../AST/ast.h"
#include "../Vectores_arm64/AccesoVector_arm64.h"
#include <string.h>
#include <stdlib.h>

extern NodoAST* astGlobal;

// Contador global para etiquetas únicas
static int contador_foreach = 0;

// Función para obtener el tamaño de un vector desde el AST
int obtenerTamanioVector(NodoAST* raiz, const char* nombreVector) {
    if (!raiz || !nombreVector) return 0;
    
    if (raiz->tipo == AST_DECLARACION_VECTOR && raiz->num_hijos >= 2) {
        NodoAST* idNodo = raiz->hijos[1];
        if (idNodo && strcmp(idNodo->valor, nombreVector) == 0) {
            // Caso: new int[5]
            if (raiz->num_hijos >= 4 && raiz->hijos[3]->tipo == AST_1DIMENSION) {
                NodoAST* dimNodo = raiz->hijos[3]->hijos[0];
                return atoi(dimNodo->valor);
            }
            // Caso: { 1, 2, 3, 4, 5 }
            if (raiz->num_hijos >= 3 && raiz->hijos[2]->tipo == AST_LISTA_ELEMENTOS) {
                NodoAST* lista = raiz->hijos[2];
                // Descender si hay sublista
                if (lista->num_hijos == 1 && 
                    (lista->hijos[0]->tipo == AST_LISTA_ELEMENTOS || 
                     lista->hijos[0]->tipo == AST_INTERNO)) {
                    lista = lista->hijos[0];
                }
                return lista->num_hijos;
            }
        }
    }
    
    // Buscar recursivamente
    for (int i = 0; i < raiz->num_hijos; i++) {
        int tam = obtenerTamanioVector(raiz->hijos[i], nombreVector);
        if (tam > 0) return tam;
    }
    
    return 0;
}

// Función para generar el bloque del for-each
static void generarBloqueForEach(GeneradorDeclaraciones* gen, NodoAST* bloque) {
    if (!bloque) return;
    
    for (int i = 0; i < bloque->num_hijos; i++) {
        NodoAST* hijo = bloque->hijos[i];
        if (hijo) {
            printf("[DEBUG FOREACH] Procesando instrucción tipo: %d\n", hijo->tipo);
            GeneradorCodigoARM64(hijo);
        }
    }
}

void generarCodigoForEach(GeneradorDeclaraciones* gen, NodoAST* nodoForEach) {
    if (!gen || !nodoForEach || nodoForEach->tipo != AST_FOR_VECTOR) {
        printf("[ERROR] Nodo inválido para for-each\n");
        return;
    }
    
    if (nodoForEach->num_hijos < 3) {
        printf("[ERROR] For-each debe tener 3 hijos (declaración, vector, bloque)\n");
        return;
    }
    
    // Extraer componentes del for-each
    NodoAST* declaracion = nodoForEach->hijos[0];  // int nota
    NodoAST* vectorNodo = nodoForEach->hijos[1];   // notasParaForeach
    NodoAST* bloque = nodoForEach->hijos[2];       // { ... }
    
    if (declaracion->tipo != AST_DECLARACION || vectorNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Estructura de for-each inválida\n");
        return;
    }
    
    // Obtener información de la variable iteradora
    NodoAST* tipoNodo = declaracion->hijos[0];
    NodoAST* varNodo = declaracion->hijos[1];
    const char* nombreVar = varNodo->valor;
    const char* tipoVar = tipoNodo->valor;
    TipoDato tipo = obtenerTipoDesdeString(tipoVar);
    
    // Obtener información del vector
    const char* nombreVector = vectorNodo->valor;
    NodoAST* vectorDeclNodo = buscarVectorEnAST(nombreVector);

    if (!vectorDeclNodo) {
        printf("[ERROR] Vector '%s' no encontrado en el AST\n", nombreVector);
        return;
    }

    if (vectorDeclNodo->num_hijos > 0) {
        NodoAST* tipoNodoVec = vectorDeclNodo->hijos[0];
        if (tipoNodoVec->tipo == AST_TIPO && tipoNodoVec->valor)
            tipo = obtenerTipoDesdeString(tipoNodoVec->valor);
    }

    // Obtener tamaño del vector desde el AST
    int tamanioVector = obtenerTamanioVector(astGlobal, nombreVector);
    if (tamanioVector <= 0) {
        printf("[ERROR] No se pudo determinar el tamaño del vector '%s'\n", nombreVector);
        return;
    }
    
    printf("[DEBUG FOREACH] Generando for-each: %s %s : %s (tamaño: %d)\n", 
           tipoVar, nombreVar, nombreVector, tamanioVector);
    
    // Crear etiquetas únicas
    EtiquetasForEach etiquetas;
    etiquetas.numero_foreach = contador_foreach++;
    snprintf(etiquetas.etiqueta_inicio, sizeof(etiquetas.etiqueta_inicio), 
             "foreach_init_%d", etiquetas.numero_foreach);
    snprintf(etiquetas.etiqueta_loop, sizeof(etiquetas.etiqueta_loop), 
             "foreach_loop_%d", etiquetas.numero_foreach);
    snprintf(etiquetas.etiqueta_fin, sizeof(etiquetas.etiqueta_fin), 
             "foreach_end_%d", etiquetas.numero_foreach);
    
    // Determinar tamaño del elemento y tipo de carga
    int tamElemento = 4;
    const char* instruccionCarga = "ldr";
    const char* sufijoCarga = "w";
    const char* instruccionStore = "str";
    int shift = 2; // log2(4) para int
    
    if (tipo == TIPO_DOUBLE) {
        tamElemento = 8;
        sufijoCarga = "d";
        instruccionStore = "str";
        shift = 3;
    } else if (tipo == TIPO_FLOAT) {
        tamElemento = 4;
        sufijoCarga = "s";
        instruccionStore = "str";
        shift = 2;
    } else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) {
        tamElemento = 1;
        instruccionCarga = "ldrb";
        sufijoCarga = "w";
        instruccionStore = "strb";
        shift = 0;
    } else if (tipo == TIPO_STRING) {
        tamElemento = 8;
        sufijoCarga = "x";
        instruccionStore = "str";
        shift = 3;
    }
    
    char buffer[512];
    char codigo[2048] = "";
    
    // **PASO 1: Declarar la variable iteradora en .data (scope del for-each)**
    snprintf(buffer, sizeof(buffer), "%s: ", nombreVar);
    if (tipo == TIPO_INT) {
        strcat(buffer, ".word 0\n");
    } else if (tipo == TIPO_DOUBLE) {
        strcat(buffer, ".double 0.0\n");
    } else if (tipo == TIPO_FLOAT) {
        strcat(buffer, ".float 0.0\n");
    } else if (tipo == TIPO_CHAR || tipo == TIPO_BOOLEAN) {
        strcat(buffer, ".byte 0\n");
    } else if (tipo == TIPO_STRING) {
        strcat(buffer, ".quad 0\n");
    }
    insertarEnSeccionData(gen, buffer);
    
    // **PASO 2: Generar código de inicialización**
    snprintf(buffer, sizeof(buffer),
        "    // For-each: %s %s : %s\n"
        "%s:\n"
        "    mov w19, #0                // Índice inicial = 0\n"
        "    mov w20, #%d               // Tamaño del vector\n"
        "    ldr x21, =%s               // Dirección base del vector\n\n",
        tipoVar, nombreVar, nombreVector,
        etiquetas.etiqueta_inicio,
        tamanioVector,
        nombreVector);
    strcat(codigo, buffer);
    
    // **PASO 3: Etiqueta de inicio del loop**
    snprintf(buffer, sizeof(buffer),
        "%s:\n"
        "    // Verificar si hemos terminado\n"
        "    cmp w19, w20\n"
        "    bge %s                     // Si índice >= tamaño, salir\n\n",
        etiquetas.etiqueta_loop,
        etiquetas.etiqueta_fin);
    strcat(codigo, buffer);
    
    // **PASO 4: Cargar elemento actual en la variable iteradora**
    snprintf(buffer, sizeof(buffer),
        "    // Cargar elemento actual: %s = %s[índice]\n",
        nombreVar, nombreVector);
    strcat(codigo, buffer);
    
    if (shift > 0) {
        // Usar indexación escalada
        snprintf(buffer, sizeof(buffer),
            "    %s %s22, [x21, w19, SXTW #%d]  // Cargar elemento\n"
            "    ldr x23, =%s\n"
            "    %s %s22, [x23]              // Almacenar en variable iteradora\n\n",
            instruccionCarga, sufijoCarga, shift,
            nombreVar,
            instruccionStore, sufijoCarga);
    } else {
        // Sin escala (para bytes)
        snprintf(buffer, sizeof(buffer),
            "    %s %s22, [x21, w19, SXTW]      // Cargar elemento\n"
            "    ldr x23, =%s\n"
            "    %s %s22, [x23]              // Almacenar en variable iteradora\n\n",
            instruccionCarga, sufijoCarga,
            nombreVar,
            instruccionStore, sufijoCarga);
    }
    strcat(codigo, buffer);
    
    // Insertar todo el código de inicialización y loop
    insertarEnSeccionText(gen, codigo);
    
    // **PASO 5: Generar el bloque del for-each**
    generarBloqueForEach(gen, bloque);
    
    // **PASO 6: Incrementar índice y saltar al inicio del loop**
    snprintf(buffer, sizeof(buffer),
        "    // Incrementar índice y continuar\n"
        "    add w19, w19, #1\n"
        "    b %s\n\n",
        etiquetas.etiqueta_loop);
    insertarEnSeccionText(gen, buffer);
    
    // **PASO 7: Etiqueta de fin**
    snprintf(buffer, sizeof(buffer),
        "%s:\n"
        "    // Fin del for-each\n\n",
        etiquetas.etiqueta_fin);
    insertarEnSeccionText(gen, buffer);
    
    printf("[DEBUG FOREACH] Código for-each generado exitosamente\n");
}