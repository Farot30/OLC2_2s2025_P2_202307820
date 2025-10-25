#include "Embebidas_arm64.h"
#include "../../semantico/tipos.h"
#include "../Vectores_arm64/AccesoVector_arm64.h"
#include "../InstruccionesSent_arm64/ForEach_arm64.h"

extern NodoAST* astGlobal;

NodoAST* obtenerElementosVector(NodoAST* raiz, const char* nombreVector, int* numElementos) {
    if (!raiz || !nombreVector || !numElementos) return NULL;

    // Buscar la declaración del vector
    NodoAST* vectorNodo = buscarDeclaracionPorIdentificador(raiz, AST_DECLARACION_VECTOR, nombreVector);
    if (!vectorNodo || vectorNodo->num_hijos < 3) {
        printf("[ERROR] No se encontró el vector '%s' o no tiene elementos\n", nombreVector);
        return NULL;
    }

    // Obtener la lista de elementos
    NodoAST* listaElementos = vectorNodo->hijos[2]; // Nodo AST_LISTA_ELEMENTOS
    if (!listaElementos || listaElementos->tipo != AST_LISTA_ELEMENTOS) {
        printf("[ERROR] El vector '%s' no tiene una lista de elementos válida\n", nombreVector);
        return NULL;
    }

    if(listaElementos->num_hijos == 1 && listaElementos->hijos[0]->tipo == AST_INTERNO) {
        NodoAST* interno = listaElementos->hijos[0];
        *numElementos = interno->num_hijos;
        return interno->hijos;
    }

    // Retornar los elementos
    *numElementos = listaElementos->num_hijos;
    return listaElementos->hijos;
}

void generarStringJoinARM64(GeneradorDeclaraciones* gen, const char* nombreDestino, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_PARSEJOIN) {
        printf("[ERROR] Nodo inválido para String.join\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] String.join requiere al menos 2 argumentos (separador y lista)\n");
        return;
    }

    // Extraer separador y lista
    NodoAST* separadorNodo = nodo->hijos[0];
    NodoAST* listaNodo = nodo->hijos[1];

    if (!separadorNodo || separadorNodo->tipo != AST_CADENA) {
        printf("[ERROR] El separador debe ser una cadena literal\n");
        return;
    }

    const char* separador = separadorNodo->valor;
    printf("[DEBUG] Separador detectado: '%s'\n", separador);

    // Crear literal para el separador
    char nombreSep[64];
    snprintf(nombreSep, sizeof(nombreSep), "str_join_sep_%d", gen->admin_registros->contador_constantes++);
    char buffer[1024];
    buffer[0] = '\0';
    snprintf(buffer, sizeof(buffer), "%s: .asciz \"%s\"\n", nombreSep, separador);
    insertarEnSeccionData(gen, buffer);

    // ✅ VERIFICAR SI listaNodo ES AST_PROGRAMA Y DESCENDER
    printf("[DEBUG] Tipo de listaNodo: %d, valor: %s\n", listaNodo->tipo, listaNodo->valor ? listaNodo->valor : "(null)");
    
    if (listaNodo->tipo == AST_PROGRAMA && listaNodo->num_hijos == 1) {
        printf("[DEBUG] Descendiendo dentro de AST_PROGRAMA\n");
        listaNodo = listaNodo->hijos[0];
        printf("[DEBUG] Nuevo tipo de listaNodo: %d, valor: %s\n", listaNodo->tipo, listaNodo->valor ? listaNodo->valor : "(null)");
    }

    // Verificar si es un vector o una lista de strings literales
    bool esVector = false;
    const char* nombreVector = NULL;
    int numElementos = 0;
    NodoAST** elementos = NULL;

   if (listaNodo->tipo == AST_LISTA) {
        // Si la lista tiene un solo hijo y ese hijo es un identificador, es un vector
        if (listaNodo->num_hijos == 1 && listaNodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
            esVector = true;
            nombreVector = listaNodo->hijos[0]->valor;
            printf("[DEBUG] Vector detectado dentro de AST_LISTA: '%s'\n", nombreVector);

            elementos = obtenerElementosVector(astGlobal, nombreVector, &numElementos);
            if (!elementos || numElementos <= 0) {
                printf("[ERROR] No se pudieron obtener los elementos del vector '%s'\n", nombreVector);
                return;
            }
            printf("[DEBUG] Vector '%s' tiene %d elementos\n", nombreVector, numElementos);
        } else {
            // Lista de literales
            numElementos = listaNodo->num_hijos;
            elementos = listaNodo->hijos;
            printf("[DEBUG] Lista de literales detectada con %d elementos\n", numElementos);
        }
    }

    if (numElementos == 0) {
        printf("[ERROR] La lista no puede estar vacía\n");
        return;
    }

    // Crear buffer temporal para el resultado
    char nombreBuffer[64];
    snprintf(nombreBuffer, sizeof(nombreBuffer), "str_join_buffer_%d", gen->admin_registros->contador_constantes++);
    buffer[0] = '\0';
    snprintf(buffer, sizeof(buffer), "%s: .skip 1024\n", nombreBuffer);
    insertarEnSeccionBss(gen, buffer);

    // Código ARM64
    char codigo[4096] = "";

    snprintf(buffer, sizeof(buffer),
        "    // String.join(\"%s\", %s)\n"
        "    ldr x19, =%s  // Buffer resultado\n"
        "    mov x20, x19  // Puntero actual\n",
        separador, esVector ? nombreVector : "literales", nombreBuffer);
    strcat(codigo, buffer);

    if (esVector) {
        // Caso: String.join(",", vector)
        printf("[DEBUG] Generando código para vector '%s'\n", nombreVector);
        
        snprintf(buffer, sizeof(buffer),
            "    ldr x21, =%s  // Base del vector\n"
            "    mov w22, #0   // Índice\n"
            "    mov w23, #%d  // Tamaño\n",
            nombreVector, numElementos);
        strcat(codigo, buffer);

        // Loop para concatenar elementos del vector
        snprintf(buffer, sizeof(buffer),
            "str_join_loop_%s:\n"
            "    cmp w22, w23\n"
            "    bge str_join_end_%s\n",
            nombreDestino, nombreDestino);
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "    // Cargar elemento del vector\n"
            "    lsl x24, x22, #3  // índice * 8 (punteros de 64 bits)\n"
            "    ldr x25, [x21, x24]  // x25 = vector[i]\n");
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "    // Copiar string al buffer\n"
            "str_join_copy_char_%s:\n"
            "    ldrb w26, [x25], #1\n"
            "    cbz w26, str_join_copy_done_%s\n"
            "    strb w26, [x20], #1\n"
            "    b str_join_copy_char_%s\n",
            nombreDestino, nombreDestino, nombreDestino);
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "str_join_copy_done_%s:\n"
            "    // Agregar separador si no es el último\n"
            "    add w22, w22, #1\n"
            "    cmp w22, w23\n"
            "    bge str_join_end_%s\n"
            "    ldr x27, =%s  // Separador\n",
            nombreDestino, nombreDestino, nombreSep);
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "str_join_copy_sep_%s:\n"
            "    ldrb w28, [x27], #1\n"
            "    cbz w28, str_join_loop_%s\n"
            "    strb w28, [x20], #1\n"
            "    b str_join_copy_sep_%s\n",
            nombreDestino, nombreDestino, nombreDestino);
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "str_join_end_%s:\n"
            "    strb wzr, [x20]  // Null terminator\n",
            nombreDestino);
        strcat(codigo, buffer);
    } else {
        // Caso: String.join(" | ", "10", "20", "30")
        printf("[DEBUG] Generando código para literales\n");
        
        for (int i = 0; i < numElementos; i++) {
            NodoAST* elem = elementos[i];
            if (!elem || elem->tipo != AST_CADENA) {
                printf("[ERROR] Elemento %d no es string literal (tipo: %d)\n", i, elem ? elem->tipo : -1);
                return;
            }

            printf("[DEBUG] Procesando literal %d: '%s'\n", i, elem->valor);

            // Crear literal para cada elemento
            char nombreElem[64];
            snprintf(nombreElem, sizeof(nombreElem), "str_join_elem_%d_%d", 
                     gen->admin_registros->contador_constantes++, i);
            buffer[0] = '\0';
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%s\"\n", nombreElem, elem->valor);
            insertarEnSeccionData(gen, buffer);

            // Copiar elemento al buffer
            snprintf(buffer, sizeof(buffer),
                "    // Copiar elemento %d: \"%s\"\n"
                "    ldr x25, =%s\n"
                "str_join_copy_%d_%s:\n"
                "    ldrb w26, [x25], #1\n"
                "    cbz w26, str_join_done_%d_%s\n"
                "    strb w26, [x20], #1\n"
                "    b str_join_copy_%d_%s\n"
                "str_join_done_%d_%s:\n",
                i, elem->valor, nombreElem,
                i, nombreDestino, i, nombreDestino, i, nombreDestino, i, nombreDestino);
            strcat(codigo, buffer);

            // Agregar separador si no es el último
            if (i < numElementos - 1) {
                snprintf(buffer, sizeof(buffer),
                    "    // Agregar separador\n"
                    "    ldr x27, =%s\n"
                    "str_join_sep_%d_%s:\n"
                    "    ldrb w28, [x27], #1\n"
                    "    cbz w28, str_join_next_%d_%s\n"
                    "    strb w28, [x20], #1\n"
                    "    b str_join_sep_%d_%s\n"
                    "str_join_next_%d_%s:\n",
                    nombreSep, i, nombreDestino, i, nombreDestino, i, nombreDestino, i, nombreDestino);
                strcat(codigo, buffer);
            }
        }

        // Null terminator
        snprintf(buffer, sizeof(buffer), "    strb wzr, [x20]\n");
        strcat(codigo, buffer);
    }

    // Asignar resultado a la variable destino
    snprintf(buffer, sizeof(buffer),
        "    // Asignar resultado a '%s'\n"
        "    ldr x1, =%s\n"
        "    ldr x2, =%s\n"
        "    str x2, [x1]\n",
        nombreDestino, nombreDestino, nombreBuffer);
    strcat(codigo, buffer);

    insertarEnSeccionText(gen, codigo);
    printf("[DEBUG] String.join generado para '%s'\n", nombreDestino);
}