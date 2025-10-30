#include "Embebidas_arm64.h"
#include "../../semantico/tipos.h"
#include "../Vectores_arm64/AccesoVector_arm64.h"
#include "../InstruccionesSent_arm64/ForEach_arm64.h"
#include "../Vectores_arm64/AccesoMatriz_arm64.h"

extern NodoAST* astGlobal;

NodoAST** obtenerElementosVector(NodoAST* raiz, const char* nombreVector, int* numElementos) {
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


static NodoAST* buscarDeclaracionVector(NodoAST* raiz, const char* nombreVector) {
    if (!raiz || !nombreVector) return NULL;
    
    if (raiz->tipo == AST_DECLARACION_VECTOR && raiz->num_hijos >= 2) {
        NodoAST* idNodo = raiz->hijos[1];
        if (idNodo && strcmp(idNodo->valor, nombreVector) == 0) {
            return raiz;
        }
    }
    
    for (int i = 0; i < raiz->num_hijos; i++) {
        NodoAST* resultado = buscarDeclaracionVector(raiz->hijos[i], nombreVector);
        if (resultado) return resultado;
    }
    
    return NULL;
}

int obtenerTamanoVectorDesdeAST(NodoAST* vectorNodo) {
    if (!vectorNodo) return 0;
    
    // **PRIORIDAD 1: Si existe un nodo de dimensión (caso: new int[5] o vector ampliado)**
    if (vectorNodo->num_hijos >= 4 && vectorNodo->hijos[3]->tipo == AST_1DIMENSION) {
        NodoAST* dimensionNodo = vectorNodo->hijos[3]->hijos[0];
        int tam = atoi(dimensionNodo->valor);
        printf("[DEBUG TAM] Tamaño desde dimensión explícita: %d\n", tam);
        return tam;
    }
    
    // **PRIORIDAD 2: Si es una lista de elementos con AST_ADD, buscar el vector original**
    if (vectorNodo->num_hijos >= 3 && vectorNodo->hijos[2]->tipo == AST_LISTA_ELEMENTOS) {
        NodoAST* lista = vectorNodo->hijos[2];
        
        if (lista->num_hijos == 1 && lista->hijos[0]->tipo == AST_ADD) {
            NodoAST* addNodo = lista->hijos[0];
            if (addNodo->num_hijos >= 1 && addNodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                const char* vectorOriginal = addNodo->hijos[0]->valor;
                int tamOriginal = obtenerTamanoVectorDesdeAST(buscarDeclaracionVector(astGlobal, vectorOriginal));
                printf("[DEBUG TAM] Tamaño desde Arrays.add: %d + 1 = %d\n", tamOriginal, tamOriginal + 1);
                return tamOriginal + 1;
            }
        }
        
        // **PRIORIDAD 3: Lista normal de elementos**
        if (lista->num_hijos == 1 && (lista->hijos[0]->tipo == AST_LISTA_ELEMENTOS || lista->hijos[0]->tipo == AST_INTERNO)) {
            lista = lista->hijos[0];
        }
        printf("[DEBUG TAM] Tamaño desde lista de elementos: %d\n", lista->num_hijos);
        return lista->num_hijos;
    }
    
    return 0;
}
static int contador_string_join = 0;

void generarStringJoinARM64(GeneradorDeclaraciones* gen, const char* nombreDestino, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_PARSEJOIN) {
        printf("[ERROR] Nodo inválido para String.join\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] String.join requiere al menos 2 argumentos (separador y lista)\n");
        return;
    }

    // ✅ GENERAR ID ÚNICO PARA ESTA LLAMADA
    int joinID = contador_string_join++;
    
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
        // ✅ USAR joinID EN VEZ DE nombreDestino PARA ETIQUETAS
        snprintf(buffer, sizeof(buffer),
            "    ldr x21, =%s  // Base del vector\n"
            "    mov w22, #0   // Índice\n"
            "    mov w23, #%d  // Tamaño\n",
            nombreVector, numElementos);
        strcat(codigo, buffer);

        // ✅ Loop con etiquetas únicas
        snprintf(buffer, sizeof(buffer),
            "str_join_loop_%d:\n"
            "    cmp w22, w23\n"
            "    bge str_join_end_%d\n",
            joinID, joinID);
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "    // Cargar elemento del vector\n"
            "    lsl x24, x22, #3  // índice * 8 (punteros de 64 bits)\n"
            "    ldr x25, [x21, x24]  // x25 = vector[i]\n");
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "    // Copiar string al buffer\n"
            "str_join_copy_char_%d:\n"
            "    ldrb w26, [x25], #1\n"
            "    cbz w26, str_join_copy_done_%d\n"
            "    strb w26, [x20], #1\n"
            "    b str_join_copy_char_%d\n",
            joinID, joinID, joinID);
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "str_join_copy_done_%d:\n"
            "    // Agregar separador si no es el último\n"
            "    add w22, w22, #1\n"
            "    cmp w22, w23\n"
            "    bge str_join_end_%d\n"
            "    ldr x27, =%s  // Separador\n",
            joinID, joinID, nombreSep);
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "str_join_copy_sep_%d:\n"
            "    ldrb w28, [x27], #1\n"
            "    cbz w28, str_join_loop_%d\n"
            "    strb w28, [x20], #1\n"
            "    b str_join_copy_sep_%d\n",
            joinID, joinID, joinID);
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "str_join_end_%d:\n"
            "    strb wzr, [x20]  // Null terminator\n",
            joinID);
        strcat(codigo, buffer);
    } else {
        // Caso: String.join(" | ", "10", "20", "30")
        printf("[DEBUG] Generando código para literales\n");
        
        for (int i = 0; i < numElementos; i++) {
            NodoAST* elem = elementos[i];
            if (!elem || elem->tipo != AST_CADENA) {
                printf("[ERROR] Elemento %d no es string literal (tipo: %d)\n", i, elem ? (int)elem->tipo : -1);
            }

            printf("[DEBUG] Procesando literal %d: '%s'\n", i, elem->valor);

            // Crear literal para cada elemento
            char nombreElem[64];
            snprintf(nombreElem, sizeof(nombreElem), "str_join_elem_%d_%d", 
                     gen->admin_registros->contador_constantes++, i);
            buffer[0] = '\0';
            snprintf(buffer, sizeof(buffer), "%s: .asciz \"%s\"\n", nombreElem, elem->valor);
            insertarEnSeccionData(gen, buffer);

            // ✅ Usar joinID en etiquetas
            snprintf(buffer, sizeof(buffer),
                "    // Copiar elemento %d: \"%s\"\n"
                "    ldr x25, =%s\n"
                "str_join_copy_%d_%d:\n"
                "    ldrb w26, [x25], #1\n"
                "    cbz w26, str_join_done_%d_%d\n"
                "    strb w26, [x20], #1\n"
                "    b str_join_copy_%d_%d\n"
                "str_join_done_%d_%d:\n",
                i, elem->valor, nombreElem,
                joinID, i, joinID, i, joinID, i, joinID, i);
            strcat(codigo, buffer);

            // Agregar separador si no es el último
            if (i < numElementos - 1) {
                snprintf(buffer, sizeof(buffer),
                    "    // Agregar separador\n"
                    "    ldr x27, =%s\n"
                    "str_join_sep_%d_%d:\n"
                    "    ldrb w28, [x27], #1\n"
                    "    cbz w28, str_join_next_%d_%d\n"
                    "    strb w28, [x20], #1\n"
                    "    b str_join_sep_%d_%d\n"
                    "str_join_next_%d_%d:\n",
                    nombreSep, joinID, i, joinID, i, joinID, i, joinID, i);
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
        "    ldr x1, =%s\n"              // ✅ Cargar dirección de la variable destino
        "    ldr x2, =%s\n"              // ✅ Cargar dirección del buffer resultado
        "    str x2, [x1]\n",            // ✅ Almacenar puntero al buffer en la variable
        nombreDestino, nombreDestino, nombreBuffer);
    strcat(codigo, buffer);

    insertarEnSeccionText(gen, codigo);
    printf("[DEBUG] String.join generado para '%s' con ID %d\n", nombreDestino, joinID);
}


void generarArraysIndexOfARM64(GeneradorDeclaraciones* gen, const char* nombreDestino, NodoAST* nodo) {
    if (!gen || !nodo || nodo->tipo != AST_INDEXOF) {
        printf("[ERROR] Nodo inválido para Arrays.indexOf\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] Arrays.indexOf requiere 2 argumentos (vector y elemento)\n");
        return;
    }

    // Extraer argumentos
    NodoAST* vectorNodo = nodo->hijos[0];
    NodoAST* elementoNodo = nodo->hijos[1];

    if (!vectorNodo || vectorNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] El primer argumento debe ser un identificador de vector\n");
        return;
    }

    const char* nombreVector = vectorNodo->valor;
    printf("[DEBUG] Arrays.indexOf para vector '%s'\n", nombreVector);

    // Buscar el vector en el AST
    NodoAST* vectorDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_VECTOR, nombreVector);
    if (!vectorDecl) {
        printf("[ERROR] Vector '%s' no encontrado en el AST\n", nombreVector);
        return;
    }

    // Obtener el tipo y tamaño del vector
    TipoDato tipoVector = obtenerTipoVectorDesdeAST(vectorDecl);
    int numElementos = obtenerTamanoVectorDesdeAST(vectorDecl);

    if (numElementos <= 0) {
        printf("[ERROR] El vector '%s' no tiene tamaño válido\n", nombreVector);
        return;
    }

    printf("[DEBUG] Vector '%s' tiene %d elementos, tipo: %d\n", nombreVector, numElementos, tipoVector);

    char buffer[2048];
    char codigo[2048] = "";

    // Comentario inicial
    snprintf(buffer, sizeof(buffer),
        "    // Arrays.indexOf(%s, elemento)\n",
        nombreVector);
    strcat(codigo, buffer);

    // Determinar si el elemento es literal o variable
    bool esLiteral = (elementoNodo->tipo == AST_CADENA || elementoNodo->tipo == AST_NUMERO);

    if (tipoVector == TIPO_STRING) {
        // **CASO: Strings - necesita strcmp**
        if (esLiteral) {
            char nombreElemBuscado[64];
            snprintf(nombreElemBuscado, sizeof(nombreElemBuscado), 
                     "indexOf_search_%d", gen->admin_registros->contador_constantes++);
            snprintf(buffer, sizeof(buffer), 
                     "%s: .asciz \"%s\"\n", nombreElemBuscado, elementoNodo->valor);
            insertarEnSeccionData(gen, buffer);

            snprintf(buffer, sizeof(buffer),
                "    ldr x10, =%s           // Elemento buscado\n",
                nombreElemBuscado);
        } else {
            snprintf(buffer, sizeof(buffer),
                "    ldr x10, =%s           // Dirección de la variable\n"
                "    ldr x10, [x10]         // Cargar puntero al string\n",
                elementoNodo->valor);
        }
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "    ldr x11, =%s           // Base del vector\n"
            "    mov w12, #0            // Índice actual\n"
            "    mov w13, #%d           // Tamaño del vector\n"
            "    mov w0, #-1            // Resultado (inicialmente -1)\n"
            "indexOf_loop_%s:\n"
            "    cmp w12, w13\n"
            "    bge indexOf_end_%s     // Si índice >= tamaño, terminar\n",
            nombreVector, numElementos, nombreDestino, nombreDestino);
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "    lsl x14, x12, #3       // índice * 8 (punteros)\n"
            "    ldr x15, [x11, x14]    // x15 = vector[i]\n"
            "    mov x0, x15            // Primer string\n"
            "    mov x1, x10            // Segundo string (buscado)\n"
            "indexOf_strcmp_%s:\n"
            "    ldrb w2, [x0], #1\n"
            "    ldrb w3, [x1], #1\n"
            "    cmp w2, w3\n"
            "    bne indexOf_next_%s\n"
            "    cbz w2, indexOf_found_%s\n"
            "    b indexOf_strcmp_%s\n"
            "indexOf_found_%s:\n"
            "    mov w0, w12\n"
            "    b indexOf_end_%s\n"
            "indexOf_next_%s:\n"
            "    add w12, w12, #1\n"
            "    b indexOf_loop_%s\n"
            "indexOf_end_%s:\n",
            nombreDestino, nombreDestino, nombreDestino, nombreDestino, 
            nombreDestino, nombreDestino, nombreDestino, nombreDestino, nombreDestino);
        strcat(codigo, buffer);

    } else if (tipoVector == TIPO_INT || tipoVector == TIPO_CHAR || tipoVector == TIPO_BOOLEAN) {
        int tamElemento = (tipoVector == TIPO_CHAR || tipoVector == TIPO_BOOLEAN) ? 1 : 4;
        const char* instrLoad = (tipoVector == TIPO_CHAR || tipoVector == TIPO_BOOLEAN) ? "ldrb" : "ldr";
        int shift = (tipoVector == TIPO_CHAR || tipoVector == TIPO_BOOLEAN) ? 0 : 2;

        if (esLiteral) {
            int valor = atoi(elementoNodo->valor);
            snprintf(buffer, sizeof(buffer),
                "    mov w10, #%d           // Elemento buscado\n", valor);
        } else {
            snprintf(buffer, sizeof(buffer),
                "    ldr x10, =%s           // Dirección de la variable\n"
                "    %s w10, [x10]          // Cargar valor\n",
                elementoNodo->valor, instrLoad);
        }
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "    ldr x11, =%s           // Base del vector\n"
            "    mov w12, #0            // Índice actual\n"
            "    mov w13, #%d           // Tamaño del vector\n"
            "    mov w0, #-1            // Resultado (inicialmente -1)\n"
            "indexOf_loop_%s:\n"
            "    cmp w12, w13\n"
            "    bge indexOf_end_%s\n",
            nombreVector, numElementos, nombreDestino, nombreDestino);
        strcat(codigo, buffer);

        if (shift > 0) {
            snprintf(buffer, sizeof(buffer),
                "    lsl x14, x12, #%d      // índice * %d\n"
                "    %s w15, [x11, x14]     // Cargar vector[i]\n",
                shift, tamElemento, instrLoad);
        } else {
            snprintf(buffer, sizeof(buffer),
                "    %s w15, [x11, w12, SXTW] // Cargar vector[i]\n",
                instrLoad);
        }
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "    cmp w15, w10\n"
            "    beq indexOf_found_%s\n"
            "    add w12, w12, #1\n"
            "    b indexOf_loop_%s\n"
            "indexOf_found_%s:\n"
            "    mov w0, w12\n"
            "indexOf_end_%s:\n",
            nombreDestino, nombreDestino, nombreDestino, nombreDestino);
        strcat(codigo, buffer);

    } else if (tipoVector == TIPO_FLOAT || tipoVector == TIPO_DOUBLE) {
        const char* instrLoad = (tipoVector == TIPO_FLOAT) ? "ldr s" : "ldr d";
        const char* regCmp = (tipoVector == TIPO_FLOAT) ? "s" : "d";
        int shift = (tipoVector == TIPO_FLOAT) ? 2 : 3;

        if (esLiteral) {
            char nombreConst[64];
            snprintf(nombreConst, sizeof(nombreConst), 
                     "indexOf_const_%d", gen->admin_registros->contador_constantes++);
            if (tipoVector == TIPO_FLOAT) {
                snprintf(buffer, sizeof(buffer), 
                         "%s: .float %s\n", nombreConst, elementoNodo->valor);
            } else {
                snprintf(buffer, sizeof(buffer), 
                         "%s: .double %s\n", nombreConst, elementoNodo->valor);
            }
            insertarEnSeccionData(gen, buffer);

            snprintf(buffer, sizeof(buffer),
                "    ldr x10, =%s\n"
                "    %s10, [x10]\n",
                nombreConst, instrLoad);
        } else {
            snprintf(buffer, sizeof(buffer),
                "    ldr x10, =%s\n"
                "    %s10, [x10]\n",
                elementoNodo->valor, instrLoad);
        }
        strcat(codigo, buffer);

        snprintf(buffer, sizeof(buffer),
            "    ldr x11, =%s\n"
            "    mov w12, #0\n"
            "    mov w13, #%d\n"
            "    mov w0, #-1\n"
            "indexOf_loop_%s:\n"
            "    cmp w12, w13\n"
            "    bge indexOf_end_%s\n"
            "    lsl x14, x12, #%d\n"
            "    %s15, [x11, x14]\n"
            "    fcmp %s15, %s10\n"
            "    beq indexOf_found_%s\n"
            "    add w12, w12, #1\n"
            "    b indexOf_loop_%s\n"
            "indexOf_found_%s:\n"
            "    mov w0, w12\n"
            "indexOf_end_%s:\n",
            nombreVector, numElementos, nombreDestino, nombreDestino,
            shift, instrLoad, regCmp, regCmp, nombreDestino, nombreDestino, nombreDestino, nombreDestino);
        strcat(codigo, buffer);
    }

    snprintf(buffer, sizeof(buffer),
        "    ldr x1, =%s\n"
        "    str w0, [x1]\n",
        nombreDestino);
    strcat(codigo, buffer);

    insertarEnSeccionText(gen, codigo);
    printf("[DEBUG] Arrays.indexOf generado para '%s'\n", nombreDestino);
}



static void actualizarVectorEnAST(NodoAST* vectorDecl, int nuevoTamano) {
    if (!vectorDecl || vectorDecl->tipo != AST_DECLARACION_VECTOR) return;
    
    // Si el hijo 2 es AST_LISTA_ELEMENTOS con AST_ADD, reemplazarlo
    if (vectorDecl->num_hijos >= 3 && vectorDecl->hijos[2]->tipo == AST_LISTA_ELEMENTOS) {
        NodoAST* lista = vectorDecl->hijos[2];
        
        // Si la lista tiene un AST_ADD, crear un nuevo nodo de dimensión
        if (lista->num_hijos == 1 && lista->hijos[0]->tipo == AST_ADD) {
            // Crear un nuevo nodo de dimensión que refleje el tamaño ampliado
            NodoAST* nuevaDimension = crearNodo(AST_1DIMENSION, "Dimensión");
            char tamStr[16];
            snprintf(tamStr, sizeof(tamStr), "%d", nuevoTamano);
            NodoAST* valorTam = crearNodo(AST_NUMERO, tamStr);
            agregarHijo(nuevaDimension, valorTam);
            
            // Reemplazar el hijo 3 (o agregarlo si no existe)
            if (vectorDecl->num_hijos >= 4) {
                vectorDecl->hijos[3] = nuevaDimension;
            } else {
                agregarHijo(vectorDecl, nuevaDimension);
            }
            
            printf("[DEBUG AST UPDATE] Vector actualizado en AST con tamaño: %d\n", nuevoTamano);
        }
    }
}

int contador_vectores_ampliados = 0;

void generarArraysAddOfArm64(GeneradorDeclaraciones* gen, const char* nombreDestino, NodoAST* nodo) {
    if (!gen || !nombreDestino || !nodo || nodo->tipo != AST_ADD) {
        printf("[ERROR] Nodo inválido para Arrays.add\n");
        return;
    }

    if (nodo->num_hijos < 2) {
        printf("[ERROR] Arrays.add requiere 2 argumentos (vector y nuevo valor)\n");
        return;
    }

    // Extraer argumentos
    NodoAST* vectorNodo = nodo->hijos[0];
    NodoAST* nuevoValorNodo = nodo->hijos[1];

    if (!vectorNodo || vectorNodo->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] El primer argumento debe ser un identificador de vector\n");
        return;
    }

    const char* nombreVector = vectorNodo->valor;
    printf("[DEBUG ARRAYS.ADD] Procesando vector '%s'\n", nombreVector);

    // Buscar el vector en el AST
    NodoAST* vectorDecl = buscarDeclaracionVector(astGlobal, nombreVector);
    if (!vectorDecl) {
        printf("[ERROR] Vector '%s' no encontrado en el AST\n", nombreVector);
        return;
    }

    // Obtener el tipo y tamaño del vector
    TipoDato tipoVector = obtenerTipoVectorDesdeAST(vectorDecl);
    int numElementos = obtenerTamanoVectorDesdeAST(vectorDecl);

    if (numElementos <= 0) {
        printf("[ERROR] El vector '%s' no tiene tamaño válido\n", nombreVector);
        return;
    }

    printf("[DEBUG ARRAYS.ADD] Vector '%s': %d elementos, tipo: %d\n", 
           nombreVector, numElementos, tipoVector);

    // Determinar características del tipo
    int tamElemento;
    const char* directiva;
    const char* instruccionCarga;
    const char* instruccionStore;
    const char* sufijo;
    int shift;

    switch (tipoVector) {
        case TIPO_INT:
            tamElemento = 4;
            directiva = ".word";
            instruccionCarga = "ldr";
            instruccionStore = "str";
            sufijo = "w";
            shift = 2;
            break;
        case TIPO_DOUBLE:
            tamElemento = 8;
            directiva = ".double";
            instruccionCarga = "ldr";
            instruccionStore = "str";
            sufijo = "d";
            shift = 3;
            break;
        case TIPO_FLOAT:
            tamElemento = 4;
            directiva = ".float";
            instruccionCarga = "ldr";
            instruccionStore = "str";
            sufijo = "s";
            shift = 2;
            break;
        case TIPO_CHAR:
        case TIPO_BOOLEAN:
            tamElemento = 1;
            directiva = ".byte";
            instruccionCarga = "ldrb";
            instruccionStore = "strb";
            sufijo = "w";
            shift = 0;
            break;
        case TIPO_STRING:
            tamElemento = 8;
            directiva = ".quad";
            instruccionCarga = "ldr";
            instruccionStore = "str";
            sufijo = "x";
            shift = 3;
            break;
        default:
            printf("[ERROR] Tipo de vector no soportado: %d\n", tipoVector);
            return;
    }

    char buffer[2048];
    char codigo[4096] = "";

    int nuevoTamano = numElementos + 1;

    // **GENERAR NOMBRE ÚNICO PARA EL VECTOR FÍSICO**
    char nombreVectorFisico[64];
    snprintf(nombreVectorFisico, sizeof(nombreVectorFisico), 
             "%s_data_%d", nombreDestino, gen->admin_registros->contador_constantes++);

    // **1. Reservar espacio en .bss para el vector físico**
    snprintf(buffer, sizeof(buffer),
        "%s: .skip %d    // Vector ampliado (%d elementos)\n", 
        nombreVectorFisico, nuevoTamano * tamElemento, nuevoTamano);
    insertarEnSeccionBss(gen, buffer);

    // **2. DECLARAR EL ALIAS EN .data (solo la primera vez)**
     char patronAlias[128];
    snprintf(patronAlias, sizeof(patronAlias), "%s:", nombreDestino);

    // ** BUSCAR CON EL PATRÓN CORRECTO (sin \n al inicio)**
    if (!strstr(gen->buffer, patronAlias)) {
        snprintf(buffer, sizeof(buffer),
            "%s: .quad %s    // Puntero al vector actual\n",
            nombreDestino, nombreVectorFisico);
        insertarEnSeccionData(gen, buffer);
        printf("[DEBUG] Alias '%s' creado apuntando a '%s'\n", nombreDestino, nombreVectorFisico);
    } else {
        printf("[DEBUG] Alias '%s' ya existe, se actualizará el puntero en runtime\n", nombreDestino);
    }

    // **3. Generar código de copia**
    snprintf(buffer, sizeof(buffer),
        "    // Arrays.add: %s = %s.add(nuevo_valor)\n"
        "    ldr x10, =%s           // Dirección del puntero\n",
        nombreDestino, nombreVector, nombreVector);

    // ** SIEMPRE DESREFERENCIAR SI ES UN PUNTERO**
    char patronPuntero[128];
    snprintf(patronPuntero, sizeof(patronPuntero), "%s: .quad", nombreVector);
    
    if (strstr(gen->buffer, patronPuntero)) {
        // Es un puntero, desreferenciarlo
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
            "    ldr x10, [x10]             // Desreferenciar puntero\n");
        printf("[DEBUG] Desreferenciando puntero '%s'\n", nombreVector);
    } else {
        printf("[DEBUG] '%s' es vector directo, no necesita desreferenciar\n", nombreVector);
    }

    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer),
        "    ldr x11, =%s           // Base del nuevo vector físico\n"
        "    mov w12, #0            // Índice = 0\n"
        "\n"
        "array_add_copy_%d:\n"
        "    cmp w12, #%d           // ¿Índice < tamaño?\n"
        "    bge array_add_done_%d  // Si no, terminar copia\n",
        nombreVectorFisico,
        gen->admin_registros->contador_constantes,
        numElementos,
        gen->admin_registros->contador_constantes);
    strcat(codigo, buffer);

    // **PASO 2: Copiar elementos**
    if (shift > 0) {
        snprintf(buffer, sizeof(buffer),
            "    %s %s14, [x10, w12, SXTW #%d]  // Cargar elemento[i]\n"
            "    %s %s14, [x11, w12, SXTW #%d]  // Almacenar en nuevo[i]\n",
            instruccionCarga, sufijo, shift,
            instruccionStore, sufijo, shift);
    } else {
        snprintf(buffer, sizeof(buffer),
            "    %s %s14, [x10, w12, SXTW]      // Cargar byte[i]\n"
            "    %s %s14, [x11, w12, SXTW]      // Almacenar en nuevo[i]\n",
            instruccionCarga, sufijo,
            instruccionStore, sufijo);
    }
    strcat(codigo, buffer);

    snprintf(buffer, sizeof(buffer),
        "    add w12, w12, #1       // i++\n"
        "    b array_add_copy_%d\n"
        "\n"
        "array_add_done_%d:\n",
        gen->admin_registros->contador_constantes,
        gen->admin_registros->contador_constantes);
    strcat(codigo, buffer);

    gen->admin_registros->contador_constantes++;
    insertarEnSeccionText(gen, codigo);
    codigo[0] = '\0';

    // **PASO 3: Evaluar el nuevo valor**
    int regIndex = 1;
    ResultadoExprEmbebida nuevoValor = generarExprEmbebidaARM64(gen, nuevoValorNodo, &regIndex);
    insertarEnSeccionText(gen, nuevoValor.codigo);

    // **PASO 4: Agregar el nuevo valor al final**
    snprintf(buffer, sizeof(buffer),
        "    // Agregar nuevo valor al final\n"
        "    ldr x11, =%s           // Base del nuevo vector\n"
        "    mov w12, #%d           // Índice = tamaño_original\n",
        nombreVectorFisico, numElementos);
    strcat(codigo, buffer);

    if (shift > 0) {
        snprintf(buffer, sizeof(buffer),
            "    %s %s, [x11, w12, SXTW #%d]    // nuevo[%d] = valor\n",
            instruccionStore, nuevoValor.resultado, shift, numElementos);
    } else {
        snprintf(buffer, sizeof(buffer),
            "    %s %s, [x11, w12, SXTW]        // nuevo[%d] = valor\n",
            instruccionStore, nuevoValor.resultado, numElementos);
    }
    strcat(codigo, buffer);

    // **PASO 5: ACTUALIZAR EL PUNTERO EN .data**
    snprintf(buffer, sizeof(buffer),
        "    // Actualizar puntero '%s' al nuevo vector\n"
        "    ldr x1, =%s            // Dirección del puntero\n"
        "    ldr x2, =%s            // Dirección del nuevo vector físico\n"
        "    str x2, [x1]           // Actualizar puntero\n",
        nombreDestino, nombreDestino, nombreVectorFisico);
    strcat(codigo, buffer);

    insertarEnSeccionText(gen, codigo);

    // Actualizar AST
    NodoAST* vectorDestinoDecl = buscarDeclaracionVector(astGlobal, nombreDestino);
    if (vectorDestinoDecl) {
        actualizarVectorEnAST(vectorDestinoDecl, nuevoTamano);
    }

    printf("[DEBUG ARRAYS.ADD] Código generado exitosamente para '%s' -> '%s'\n", 
           nombreDestino, nombreVectorFisico);
}

int obtenerTamanoVectorDesdeAST_Length(NodoAST* vectorDecl) {
    if (!vectorDecl || vectorDecl->tipo != AST_DECLARACION_VECTOR) {
        return 0;
    }
    
    // **CASO 1: Vector con dimensión explícita (new int[5])**
    if (vectorDecl->num_hijos >= 4 && vectorDecl->hijos[3]->tipo == AST_1DIMENSION) {
        NodoAST* dimensionNodo = vectorDecl->hijos[3]->hijos[0];
        int tam = atoi(dimensionNodo->valor);
        printf("[DEBUG ARRAY.LENGTH] Tamaño desde dimensión explícita: %d\n", tam);
        return tam;
    }
    
    // **CASO 2: Vector con inicialización literal { 1, 2, 3 }**
    if (vectorDecl->num_hijos >= 3 && vectorDecl->hijos[2]->tipo == AST_LISTA_ELEMENTOS) {
        NodoAST* lista = vectorDecl->hijos[2];
        
        // Si la lista tiene un AST_ADD, buscar el vector original
        if (lista->num_hijos == 1 && lista->hijos[0]->tipo == AST_ADD) {
            NodoAST* addNodo = lista->hijos[0];
            if (addNodo->num_hijos >= 1 && addNodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
                const char* vectorOriginal = addNodo->hijos[0]->valor;
                NodoAST* vectorOrigen = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_VECTOR, vectorOriginal);
                int tamOriginal = obtenerTamanoVectorDesdeAST_Length(vectorOrigen);
                printf("[DEBUG ARRAY.LENGTH] Tamaño desde Arrays.add: %d + 1 = %d\n", tamOriginal, tamOriginal + 1);
                return tamOriginal + 1;
            }
        }
        
        // Lista normal de elementos
        if (lista->num_hijos == 1 && (lista->hijos[0]->tipo == AST_LISTA_ELEMENTOS || lista->hijos[0]->tipo == AST_INTERNO)) {
            lista = lista->hijos[0];
        }
        
        printf("[DEBUG ARRAY.LENGTH] Tamaño desde lista de elementos: %d\n", lista->num_hijos);
        return lista->num_hijos;
    }
    
    return 0;
}

/**
 * Obtiene las dimensiones de una matriz desde su declaración
 */
void obtenerDimensionesMatrizDesdeAST_Length(NodoAST* matrizDecl, int* filas, int* columnas) {
    *filas = 0;
    *columnas = 0;
    
    if (!matrizDecl || matrizDecl->tipo != AST_DECLARACION_MATRIZ) {
        return;
    }
    
    // **CASO 1: new int[3][3]**
    if (matrizDecl->num_hijos >= 4 && matrizDecl->hijos[3]->tipo == AST_DIMENSIONES) {
        NodoAST* dimNodo = matrizDecl->hijos[3];
        if (dimNodo->num_hijos >= 2) {
            *filas = atoi(dimNodo->hijos[0]->valor);
            *columnas = atoi(dimNodo->hijos[1]->valor);
            printf("[DEBUG ARRAY.LENGTH] Dimensiones desde 'new': [%d][%d]\n", *filas, *columnas);
            return;
        }
    }
    
    // **CASO 2: {{1,2,3}, {4,5,6}, {7,8,9}}**
    if (matrizDecl->num_hijos >= 3 && matrizDecl->hijos[2]->tipo == AST_LISTA_ELEMENTOS) {
        NodoAST* listaElementos = matrizDecl->hijos[2];
        NodoAST* listaFilas = listaElementos;
        
        // Descender si hay un nivel adicional
        if (listaElementos->num_hijos == 1 && listaElementos->hijos[0]->tipo == AST_LISTA_ELEMENTOS) {
            listaFilas = listaElementos->hijos[0];
        }
        
        *filas = listaFilas->num_hijos;
        if (*filas > 0 && listaFilas->hijos[0]->tipo == AST_INTERNO) {
            *columnas = listaFilas->hijos[0]->num_hijos;
        }
        
        printf("[DEBUG ARRAY.LENGTH] Dimensiones desde inicialización literal: [%d][%d]\n", *filas, *columnas);
    }
}

ResultadoExprEmbebida generarArrayLengthARM64(GeneradorDeclaraciones* gen, NodoAST* nodo, int* regIndex) {
    ResultadoExprEmbebida resultado = {0};
    memset(&resultado, 0, sizeof(ResultadoExprEmbebida));
    
    if (!nodo || nodo->tipo != AST_LENGHT) {
        printf("[ERROR] Nodo inválido para Array.length\n");
        return resultado;
    }
    
    if (nodo->num_hijos < 1) {
        printf("[ERROR] Array.length sin argumentos\n");
        return resultado;
    }
    
    NodoAST* arrayNodo = nodo->hijos[0];
    char codigo[1024] = "";
    char buffer[256];
    
    // **CASO 1: vector.length**
    if (arrayNodo->tipo == AST_IDENTIFICADOR) {
        const char* nombreArray = arrayNodo->valor;
        
        // Buscar primero como vector
        NodoAST* vectorDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_VECTOR, nombreArray);
        
        if (vectorDecl) {
            // Es un vector
            int tamVector = obtenerTamanoVectorDesdeAST_Length(vectorDecl);
            
            if (tamVector <= 0) {
                printf("[ERROR] No se pudo determinar el tamaño del vector '%s'\n", nombreArray);
                return resultado;
            }
            
            printf("[DEBUG ARRAY.LENGTH] Vector '%s' tiene tamaño: %d\n", nombreArray, tamVector);

            // Agregar la constante de tamaño en .data si no existe
            int id_size = gen->admin_registros->contador_constantes++;
            char nombreSize[128];
            snprintf(nombreSize, sizeof(nombreSize), "%s_size_%d: .word %d\n", nombreArray, id_size, tamVector);
            if (!strstr(gen->buffer, nombreSize)) {
                insertarEnSeccionData(gen, nombreSize);
            }

            // Genera el código ARM64 usando la etiqueta única
            snprintf(buffer, sizeof(buffer),
                "    // Array.length: %s.length\n"
                "    ldr x3, =%s_size_%d\n"
                "    ldr w%d, [x3]\n",
                nombreArray, nombreArray, id_size, *regIndex);
            strcat(codigo, buffer);

            snprintf(resultado.resultado, sizeof(resultado.resultado), "w%d", *regIndex);
            resultado.tipo = TIPO_INT;
            (*regIndex)++;
            
        } else {
            // Buscar como matriz
            NodoAST* matrizDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_MATRIZ, nombreArray);
            
            if (matrizDecl) {
                // Es una matriz - retornar número de filas
                int filas = 0, columnas = 0;
                obtenerDimensionesMatrizDesdeAST_Length(matrizDecl, &filas, &columnas);
                
                if (filas <= 0) {
                    printf("[ERROR] No se pudo determinar el número de filas de la matriz '%s'\n", nombreArray);
                    return resultado;
                }
                
                printf("[DEBUG ARRAY.LENGTH] Matriz '%s' tiene %d filas\n", nombreArray, filas);
                
                // Generar código ARM64 que carga el número de filas
                snprintf(buffer, sizeof(buffer),
                    "    // Array.length: %s.length = %d (filas)\n"
                    "    mov w%d, #%d\n",
                    nombreArray, filas, *regIndex, filas);
                strcat(codigo, buffer);
                
                snprintf(resultado.resultado, sizeof(resultado.resultado), "w%d", *regIndex);
                resultado.tipo = TIPO_INT;
                (*regIndex)++;
                
            } else {
                printf("[ERROR] Array '%s' no encontrado en el AST\n", nombreArray);
                return resultado;
            }
        }
    }
    // **CASO 2: matriz[i].length (acceso a fila)**
    else if (arrayNodo->tipo == AST_ACCESO) {
        printf("[DEBUG ARRAY.LENGTH] Procesando matriz[i].length\n");
        
        // Verificar que sea un acceso unidimensional
        int numDimensiones = contarDimensionesAcceso(arrayNodo);
        
        if (numDimensiones != 1) {
            printf("[ERROR] Array.length solo soporta acceso unidimensional (matriz[i].length)\n");
            return resultado;
        }
        
        // Obtener el nombre de la matriz
        if (!arrayNodo->hijos[0] || arrayNodo->hijos[0]->tipo != AST_IDENTIFICADOR) {
            printf("[ERROR] Identificador inválido en acceso para Array.length\n");
            return resultado;
        }
        
        const char* nombreMatriz = arrayNodo->hijos[0]->valor;
        
        // Buscar la matriz
        NodoAST* matrizDecl = buscarDeclaracionPorIdentificador(astGlobal, AST_DECLARACION_MATRIZ, nombreMatriz);
        
        if (!matrizDecl) {
            printf("[ERROR] Matriz '%s' no encontrada para Array.length\n", nombreMatriz);
            return resultado;
        }
        
        // Obtener dimensiones
        int filas = 0, columnas = 0;
        obtenerDimensionesMatrizDesdeAST_Length(matrizDecl, &filas, &columnas);
        
        if (columnas <= 0) {
            printf("[ERROR] No se pudo determinar el número de columnas de la matriz '%s'\n", nombreMatriz);
            return resultado;
        }
        
        printf("[DEBUG ARRAY.LENGTH] Matriz '%s'[i] tiene %d columnas\n", nombreMatriz, columnas);
        
        // **NOTA: No necesitamos evaluar el índice ya que todas las filas tienen el mismo número de columnas**
        // Solo retornamos el número de columnas
        snprintf(buffer, sizeof(buffer),
            "    // Array.length: %s[i].length = %d (columnas)\n"
            "    mov w%d, #%d\n",
            nombreMatriz, columnas, *regIndex, columnas);
        strcat(codigo, buffer);
        
        snprintf(resultado.resultado, sizeof(resultado.resultado), "w%d", *regIndex);
        resultado.tipo = TIPO_INT;
        (*regIndex)++;
        
    } else {
        printf("[ERROR] Tipo de nodo no soportado para Array.length: %d\n", arrayNodo->tipo);
        return resultado;
    }
    
    // Copiar el código generado al resultado
    strcpy(resultado.codigo, codigo);
    
    printf("[DEBUG ARRAY.LENGTH] Código generado exitosamente\n");
    return resultado;
}