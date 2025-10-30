#include "AccesoVector_arm64.h"
#include "../InstruccionesNorm_arm64/declaraciones_arm64.h"
#include "../Embebidas/EvaluadorExpr_arm64.h"
#include "../../semantico/tipos.h"
#include "../../simbolos/Evaluador.h"
#include "../../semantico/semantico.h"
#include "../../AST/ast.h"
#include "AccesoMatriz_arm64.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>  
#include <stdlib.h>

extern NodoAST* astGlobal;

// Función auxiliar para buscar un vector en el AST
NodoAST* buscarVectorEnAST(const char* nombre) {
    if (!astGlobal || !nombre) return NULL;
    return buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_VECTOR, nombre);
}


// Función auxiliar para obtener el tipo base del vector desde el AST
TipoDato obtenerTipoVectorDesdeAST(NodoAST* vectorNodo) {
    if (!vectorNodo) {
        printf("[ERROR obtenerTipoVectorDesdeAST] vectorNodo es NULL\n");
        return TIPO_DESCONOCIDO;
    }
    if (vectorNodo->tipo != AST_DECLARACION_VECTOR) {
        printf("[ERROR obtenerTipoVectorDesdeAST] Nodo no es declaración de vector (tipo=%d)\n", vectorNodo->tipo);
        return TIPO_DESCONOCIDO;
    }
    if (vectorNodo->num_hijos < 1) {
        printf("[ERROR obtenerTipoVectorDesdeAST] Vector sin hijos\n");
        return TIPO_DESCONOCIDO;
    }

    // Buscar el primer hijo de tipo AST_TIPO que tenga valor
    for (int i = 0; i < vectorNodo->num_hijos; i++) {
        NodoAST* hijo = vectorNodo->hijos[i];
        if (!hijo) continue;
        // AST_TIPO o el valor numérico que uses para tipo (ej: 36)
        if ((hijo->tipo == AST_TIPO || hijo->tipo == 36) && hijo->valor) {
            TipoDato tipoDeducido = obtenerTipoDesdeString(hijo->valor);
            printf("[DEBUG obtenerTipoVectorDesdeAST] Tipo encontrado: %s -> %d\n", hijo->valor, tipoDeducido);
            return tipoDeducido;
        }
    }
    printf("[ERROR obtenerTipoVectorDesdeAST] No se encontró el nodo de tipo en el vector\n");
    return TIPO_DESCONOCIDO;
}

// Genera código ARM64 para acceder a un vector y devuelve información del resultado
ResultadoExprEmbebida generarAccesoVectorARM64(GeneradorDeclaraciones* gen, NodoAST* nodo, int* regIndex) {
    ResultadoExprEmbebida resultado = {0};
    memset(resultado.codigo, 0, sizeof(resultado.codigo));
    memset(resultado.resultado, 0, sizeof(resultado.resultado));
    
    if (!gen || !nodo || nodo->tipo != AST_ACCESO) {
        printf("[ERROR] Nodo inválido para acceso de vector\n");
        return resultado;
    }

    // Extraer identificador del vector y dimensiones
    if (nodo->num_hijos < 2) {
        printf("[ERROR] Estructura de acceso incompleta\n");
        return resultado;
    }

    NodoAST* idNodo = nodo->hijos[0];
    NodoAST* dimensionesNodo = nodo->hijos[1];

    if (idNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Identificador inválido en acceso\n");
        return resultado;
    }

    const char* nombreVector = idNodo->valor;

    // ✅ BUSCAR EL VECTOR EN EL AST EN LUGAR DE LA TABLA DE SÍMBOLOS
    NodoAST* vectorNodo = buscarVectorEnAST(nombreVector);
    if (!vectorNodo) {
        printf("[ERROR] Vector '%s' no encontrado en el AST\n", nombreVector);
        return resultado;
    }
    printf("[DEBUG ACCESO VECTOR] Vector '%s' encontrado en AST\n", nombreVector);

    // ✅ OBTENER EL TIPO BASE DEL VECTOR DESDE EL AST
    TipoDato tipoVector = obtenerTipoVectorDesdeAST(vectorNodo);
    printf("[DEBUG ACCESO VECTOR] Vector '%s' encontrado en AST, tipo: %d\n", nombreVector, tipoVector);

    // Obtener la dimensión (índice)
    if (dimensionesNodo->tipo != AST_DIMENSIONES || dimensionesNodo->num_hijos < 1) {
        printf("[ERROR] Dimensiones inválidas\n");
        return resultado;
    }

    NodoAST* dimNodo = NULL;
    for (int i = 0; i < dimensionesNodo->num_hijos; i++) {
        if (dimensionesNodo->hijos[i]->tipo == AST_1DIMENSION) {
            dimNodo = dimensionesNodo->hijos[i];
            break;
        }
    }

    if (!dimNodo || dimNodo->num_hijos < 1) {
        printf("[ERROR] No se encontró dimensión válida\n");
        return resultado;
    }

    NodoAST* indiceNodo = dimNodo->hijos[0];
    char buffer[512];

    printf("[DEBUG ACCESO VECTOR] Generando acceso a '%s'\n", nombreVector);

    // Determinar el tamaño del elemento según el tipo
    int tamElemento = 4; // Por defecto: int
    const char* instruccionCarga = "ldr";
    const char* tipoRegistro = "w";
    const char* sufijoCarga = "w";
    
    resultado.tipo = tipoVector;
    
    if (tipoVector == TIPO_DOUBLE) {
        tamElemento = 8;
        tipoRegistro = "d";
        sufijoCarga = "d";
    } else if (tipoVector == TIPO_FLOAT) {
        tamElemento = 4;
        tipoRegistro = "s";
        sufijoCarga = "s";
    } else if (tipoVector == TIPO_CHAR || tipoVector == TIPO_BOOLEAN) {
        tamElemento = 1;
        instruccionCarga = "ldrb";
        sufijoCarga = "w";
    } else if (tipoVector == TIPO_STRING) {
        tamElemento = 8; // Punteros de 64 bits
        tipoRegistro = "x";
        sufijoCarga = "x";
    }

    // **EVALUACIÓN RECURSIVA DEL ÍNDICE**
    printf("[DEBUG ACCESO VECTOR] Evaluando índice recursivamente\n");
        
    // Evaluar la expresión del índice usando el evaluador recursivo
    ResultadoExprEmbebida indiceResultado = generarExprEmbebidaARM64(gen, indiceNodo, regIndex);
    
    // Agregar el código generado para evaluar el índice
    strcat(resultado.codigo, indiceResultado.codigo);
    
    // Generar comentario
    snprintf(buffer, sizeof(buffer),
        "    // Acceso a vector %s[índice]\n",
        nombreVector);
    strcat(resultado.codigo, buffer);
    
    // **OPTIMIZACIÓN: El índice ya está en el registro indicado**
    // Verificar si el resultado del índice es un registro 'w'
    char regIndiceStr[16];
    if (indiceResultado.resultado[0] == 'w') {
        // Ya es un registro w, usarlo directamente
        strncpy(regIndiceStr, indiceResultado.resultado, sizeof(regIndiceStr) - 1);
    } else {
        // No es w, necesita conversión (poco probable pero por seguridad)
        snprintf(regIndiceStr, sizeof(regIndiceStr), "w%d", (*regIndex)++);
        snprintf(buffer, sizeof(buffer),
            "    mov %s, %s\n", regIndiceStr, indiceResultado.resultado);
        strcat(resultado.codigo, buffer);
    }
    
    // **MÉTODO OPTIMIZADO: Usar indexación con escala directa**
    // ARM64 soporta: ldr wd, [xbase, windex, SXTW #shift]
    // donde shift = log2(tamElemento)
    
    int shift = 0;
    if (tamElemento == 2) shift = 1;
    else if (tamElemento == 4) shift = 2;
    else if (tamElemento == 8) shift = 3;
    
    // Registro para resultado
    int regResultado = (*regIndex)++;
    
    // **✅ VERIFICAR SI ES UN PUNTERO Y DESREFERENCIAR**
    char patronPuntero[128];
    snprintf(patronPuntero, sizeof(patronPuntero), "%s: .quad", nombreVector);
    
    if (strstr(gen->buffer, patronPuntero)) {
        // Es un puntero, necesita doble indirección
        if (shift > 0 && tamElemento != 1) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"           // Cargar dirección del puntero
                "    ldr x%d, [x%d]             // ✅ Desreferenciar puntero\n"
                "    %s %s%d, [x%d, %s, SXTW #%d]\n",  // Acceso escalado
                regResultado, nombreVector,
                regResultado, regResultado,
                instruccionCarga, sufijoCarga, regResultado, regResultado, regIndiceStr, shift);
        } else {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"           // Cargar dirección del puntero
                "    ldr x%d, [x%d]             // ✅ Desreferenciar puntero\n"
                "    %s %s%d, [x%d, %s, SXTW]\n",  // Acceso sin escala
                regResultado, nombreVector,
                regResultado, regResultado,
                instruccionCarga, sufijoCarga, regResultado, regResultado, regIndiceStr);
        }
    } else {
        // Vector directo (caso original)
        if (shift > 0 && tamElemento != 1) {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    %s %s%d, [x%d, %s, SXTW #%d]\n",
                regResultado, nombreVector,
                instruccionCarga, sufijoCarga, regResultado, regResultado, regIndiceStr, shift);
        } else {
            snprintf(buffer, sizeof(buffer),
                "    ldr x%d, =%s\n"
                "    %s %s%d, [x%d, %s, SXTW]\n",
                regResultado, nombreVector,
                instruccionCarga, sufijoCarga, regResultado, regResultado, regIndiceStr);
        }
    }
    strcat(resultado.codigo, buffer);
    
    // Establecer el resultado
    snprintf(resultado.resultado, sizeof(resultado.resultado), 
             "%s%d", tipoRegistro, regResultado);
    
    printf("[DEBUG ACCESO VECTOR] Código generado: resultado en %s\n", resultado.resultado);
    return resultado;
}

// Versión simplificada para asignaciones que necesita el nombre de destino
void generarAccesoVectorParaAsignacion(GeneradorDeclaraciones* gen, NodoAST* nodo, const char* nombreDestino) {
    int regIndex = 1;
    ResultadoExprEmbebida resultado = generarAccesoVectorARM64(gen, nodo, &regIndex);
    
    if (!nombreDestino) {
        // Solo insertar el código de acceso
        insertarEnSeccionText(gen, resultado.codigo);
        return;
    }
    
    // Insertar código de acceso
    insertarEnSeccionText(gen, resultado.codigo);
    
    // Buscar el tipo del destino en el AST
    char buffer[256];
    TipoDato tipoDest = TIPO_INT;
    
    // Intentar buscar como variable normal
    NodoAST* destNodo = buscarNodoEnAST(astGlobal, AST_DECLARACION, nombreDestino);
    if (destNodo && destNodo->num_hijos > 0) {
        NodoAST* tipoNodo = destNodo->hijos[0];
        if (tipoNodo->tipo == AST_TIPO && tipoNodo->valor) {
            tipoDest = obtenerTipoDesdeString(tipoNodo->valor);
        }
    }
    
    if (tipoDest == TIPO_DOUBLE) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    str %s, [x1]\n",
            nombreDestino, resultado.resultado);
    } else if (tipoDest == TIPO_FLOAT) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    str %s, [x1]\n",
            nombreDestino, resultado.resultado);
    } else if (tipoDest == TIPO_CHAR || tipoDest == TIPO_BOOLEAN) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    strb %s, [x1]\n",
            nombreDestino, resultado.resultado);
    } else if (tipoDest == TIPO_STRING) {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    str %s, [x1]\n",
            nombreDestino, resultado.resultado);
    } else {
        snprintf(buffer, sizeof(buffer),
            "    ldr x1, =%s\n"
            "    str %s, [x1]\n",
            nombreDestino, resultado.resultado);
    }
    insertarEnSeccionText(gen, buffer);
}