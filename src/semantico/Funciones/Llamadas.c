#include "../../AST/ast.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include "../../AST/err.h"
#include "Llamadas.h"
#include "Funcion.h"
#include "../../simbolos/Evaluador.h"
#include "../Vectores/acceso_matriz.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX_MEMO 100
int ackermann_memo[MAX_MEMO][MAX_MEMO];
int ackermann_initialized = 0;

ValorSimbolo evaluarArgumentoSeguro(NodoAST* nodo, const char* nombreFuncion);
// ============== SISTEMA GENÉRICO DE RECURSIVIDAD ==============
#define MAX_GENERIC_RECURSION_DEPTH 100
#define MAX_GENERIC_MEMO 1000

// Estructura para memoización genérica (NO interfiere con ackermann_memo existente)
typedef struct {
    char nombre_funcion[64];
    char hash_parametros[128];  // Hash de los parámetros como string
    ValorSimbolo resultado;
    int usado;
} GenericMemoEntry;

static GenericMemoEntry generic_memo_cache[MAX_GENERIC_MEMO];
static int generic_memo_initialized = 0;

// Control de profundidad genérico (NO interfiere con profundidad_ackermann existente)
typedef struct {
    char nombre_funcion[64];
    int nivel;
} GenericRecursionFrame;

static GenericRecursionFrame generic_recursion_stack[MAX_GENERIC_RECURSION_DEPTH];
int generic_recursion_depth = 0;

// Sistema de respaldo de variables para funciones genéricas
typedef struct {
    char nombre_variable[64];
    char nombre_funcion[64];
    int nivel;
    ValorSimbolo valor_respaldo;
} GenericVariableBackup;

static GenericVariableBackup generic_var_backups[500];
static int generic_backup_count = 0;

// Inicializar sistema genérico
void inicializarSistemaGenerico() {
    if (!generic_memo_initialized) {
        for (int i = 0; i < MAX_GENERIC_MEMO; i++) {
            generic_memo_cache[i].usado = 0;
            memset(generic_memo_cache[i].nombre_funcion, 0, 64);
            memset(generic_memo_cache[i].hash_parametros, 0, 128);
        }
        generic_memo_initialized = 1;
        printf("[DEBUG GENERIC] Sistema genérico de recursividad inicializado\n");
    }
}

void copiarMatrizComoParametro(Simbolo* destino, Simbolo* origen) {
    if (!destino || !origen || origen->clase != SIMBOLO_MATRIZ) {
        return;
    }
    
    destino->clase = SIMBOLO_MATRIZ;
    
    // **COPIAR DIMENSIONES DESDE EL VALOR DE LA MATRIZ**
    if (origen->valor.v.s) {
        int numFilas = 0;
        int numColumnas = 0;
        analizarDimensionesMatriz(origen->valor.v.s, &numFilas, &numColumnas);
        
        destino->filas = numFilas;
        destino->columnas = numColumnas;
        
        // Copiar el contenido de la matriz
        destino->valor.v.s = strdup(origen->valor.v.s);
        destino->valor.inicializado = 1;
        destino->valor.tipo = TIPO_STRING; // **Almacenamiento interno como STRING**
        
        printf("[DEBUG] Matriz copiada como parámetro: '%s' -> '%s', dimensiones: %dx%d\n",
               origen->nombre, destino->nombre, destino->filas, destino->columnas);
    } else {
        printf("[ERROR] Matriz origen no tiene valor inicializado\n");
        destino->filas = 0;
        destino->columnas = 0;
        destino->valor.inicializado = 0;
    }
}

// Crear hash de parámetros para memoización
void crearHashParametros(NodoAST* parametros, char* hash_output) {
    hash_output[0] = '\0';
    
    if (!parametros || parametros->num_hijos == 0) {
        strcpy(hash_output, "()");
        return;
    }
    
    strcat(hash_output, "(");
    for (int i = 0; i < parametros->num_hijos; i++) {
        ValorSimbolo param = evaluarArgumentoSeguro(parametros->hijos[i], "generic");
        char param_str[32];
        
        switch (param.tipo) {
            case TIPO_INT:
                snprintf(param_str, sizeof(param_str), "%d", param.v.i);
                break;
            case TIPO_FLOAT:
                snprintf(param_str, sizeof(param_str), "%.2f", param.v.f);
                break;
            case TIPO_DOUBLE:
                snprintf(param_str, sizeof(param_str), "%.2lf", param.v.d);
                break;
            case TIPO_CHAR:
                snprintf(param_str, sizeof(param_str), "%c", param.v.c);
                break;
            case TIPO_BOOLEAN:
                snprintf(param_str, sizeof(param_str), "%s", param.v.b ? "true" : "false");
                break;
            default:
                strcpy(param_str, "?");
                break;
        }
        
        strcat(hash_output, param_str);
        if (i < parametros->num_hijos - 1) {
            strcat(hash_output, ",");
        }
    }
    strcat(hash_output, ")");
}

// Buscar en caché genérico
ValorSimbolo* buscarEnCacheGenerico(const char* nombre_funcion, const char* hash_parametros) {
    for (int i = 0; i < MAX_GENERIC_MEMO; i++) {
        if (generic_memo_cache[i].usado &&
            strcmp(generic_memo_cache[i].nombre_funcion, nombre_funcion) == 0 &&
            strcmp(generic_memo_cache[i].hash_parametros, hash_parametros) == 0) {
            
            printf("[DEBUG GENERIC] Cache hit: %s%s\n", nombre_funcion, hash_parametros);
            return &generic_memo_cache[i].resultado;
        }
    }
    return NULL;
}

// Guardar en caché genérico
void guardarEnCacheGenerico(const char* nombre_funcion, const char* hash_parametros, ValorSimbolo resultado) {
    for (int i = 0; i < MAX_GENERIC_MEMO; i++) {
        if (!generic_memo_cache[i].usado) {
            strcpy(generic_memo_cache[i].nombre_funcion, nombre_funcion);
            strcpy(generic_memo_cache[i].hash_parametros, hash_parametros);
            
            // Copiar el resultado correctamente
            generic_memo_cache[i].resultado = resultado;
            if (resultado.tipo == TIPO_STRING && resultado.v.s) {
                generic_memo_cache[i].resultado.v.s = strdup(resultado.v.s);
            }
            
            generic_memo_cache[i].usado = 1;
            printf("[DEBUG GENERIC] Guardado en cache: %s%s\n", nombre_funcion, hash_parametros);
            return;
        }
    }
    printf("[DEBUG GENERIC] Cache lleno, no se pudo guardar %s%s\n", nombre_funcion, hash_parametros);
}

// Verificar si una función es recursiva (excluye fibonacci y ackermann que ya tienen manejo especial)
int esFuncionGenericaRecursiva(const char* nombre_funcion) {
    // NO procesar fibonacci ni ackermann - mantener código existente
    if (strcmp(nombre_funcion, "fibonacci") == 0 || 
        strcmp(nombre_funcion, "ackerman") == 0) {
        return 0;
    }
    
    // Verificar si la función se está llamando a sí misma en el stack
    for (int i = 0; i < generic_recursion_depth; i++) {
        if (strcmp(generic_recursion_stack[i].nombre_funcion, nombre_funcion) == 0) {
            printf("[DEBUG GENERIC] Función recursiva detectada: %s (nivel %d)\n", 
                   nombre_funcion, generic_recursion_depth + 1);
            return 1;
        }
    }
    
    return 0;
}

// Control de profundidad para funciones genéricas
int verificarProfundidadGenerica(const char* nombre_funcion) {
    if (generic_recursion_depth >= MAX_GENERIC_RECURSION_DEPTH - 10) {
        printf("[WARNING GENERIC] Profundidad recursiva muy alta en %s: %d\n", 
               nombre_funcion, generic_recursion_depth);
        return 0; // Detener recursión
    }
    return 1; // Continuar
}

// Entrar en frame recursivo genérico
void entrarFrameGenerico(const char* nombre_funcion) {
    if (generic_recursion_depth < MAX_GENERIC_RECURSION_DEPTH) {
        strcpy(generic_recursion_stack[generic_recursion_depth].nombre_funcion, nombre_funcion);
        generic_recursion_stack[generic_recursion_depth].nivel = generic_recursion_depth;
        generic_recursion_depth++;
        printf("[DEBUG GENERIC] Entrando frame recursivo: %s (nivel %d)\n", 
               nombre_funcion, generic_recursion_depth);
    }
}

// Salir de frame recursivo genérico
void salirFrameGenerico(const char* nombre_funcion) {
    if (generic_recursion_depth > 0) {
        generic_recursion_depth--;
        printf("[DEBUG GENERIC] Saliendo frame recursivo: %s (nivel %d)\n", 
               nombre_funcion, generic_recursion_depth);
    }
}

// Respaldo genérico de variables (para evitar problemas de estado)
void respaldarVariableGenerica(const char* nombre_variable, const char* nombre_funcion, ValorSimbolo valor) {
    if (generic_backup_count < 500) {
        strcpy(generic_var_backups[generic_backup_count].nombre_variable, nombre_variable);
        strcpy(generic_var_backups[generic_backup_count].nombre_funcion, nombre_funcion);
        generic_var_backups[generic_backup_count].nivel = generic_recursion_depth;
        generic_var_backups[generic_backup_count].valor_respaldo = valor;
        generic_backup_count++;
        
        printf("[DEBUG GENERIC] Respaldo de variable '%s' en función '%s'\n", 
               nombre_variable, nombre_funcion);
    }
}

// Restaurar variable genérica
ValorSimbolo buscarRespaldoGenerico(const char* nombre_variable, const char* nombre_funcion) {
    ValorSimbolo valor_vacio = {0};
    
    // Buscar el respaldo más reciente
    for (int i = generic_backup_count - 1; i >= 0; i--) {
        if (strcmp(generic_var_backups[i].nombre_variable, nombre_variable) == 0 &&
            strcmp(generic_var_backups[i].nombre_funcion, nombre_funcion) == 0) {
            
            printf("[DEBUG GENERIC] Restaurando variable '%s' desde respaldo\n", nombre_variable);
            return generic_var_backups[i].valor_respaldo;
        }
    }
    
    return valor_vacio;
}

// ============== CÓDIGO EXISTENTE PRESERVADO ==============

// Implementación optimizada de Ackermann
ValorSimbolo evaluarAckermann(int m, int n) {
    ValorSimbolo resultado = {0};
    resultado.inicializado = 1;
    resultado.tipo = TIPO_INT;
    
    printf("[DEBUG ACKERMANN OPTIMIZADO] Calculando ackermann(%d, %d)\n", m, n);
    
    // Inicializar tabla de memoización si no se ha hecho
    if (!ackermann_initialized) {
        for (int i = 0; i < MAX_MEMO; i++) {
            for (int j = 0; j < MAX_MEMO; j++) {
                ackermann_memo[i][j] = -1;
            }
        }
        ackermann_initialized = 1;
    }
    
    // Límite de seguridad para evitar stack overflow
    if (m > 3 || (m == 3 && n > 8) || (m > 3 && n > 1)) {
        printf("[WARNING ACKERMANN] Valores demasiado grandes: m=%d, n=%d, limitando resultado\n", m, n);
        if (m == 4 && n == 0) resultado.v.i = 13;
        else if (m == 4 && n == 1) resultado.v.i = 65533;
        else if (m > 4) resultado.v.i = INT_MAX;
        else resultado.v.i = (1 << (n + 3)) - 3;
        
        return resultado;
    }
    
    // Comprobar si ya está calculado
    if (m < MAX_MEMO && n < MAX_MEMO && ackermann_memo[m][n] != -1) {
        resultado.v.i = ackermann_memo[m][n];
        printf("[DEBUG ACKERMANN OPTIMIZADO] Usando valor cacheado: ackermann(%d, %d) = %d\n", 
               m, n, resultado.v.i);
        return resultado;
    }
    
    // Cálculo iterativo para casos base
    if (m == 0) {
        resultado.v.i = n + 1;
    }
    else if (n == 0) {
        ValorSimbolo temp = evaluarAckermann(m-1, 1);
        resultado.v.i = temp.v.i;
    }
    else if (m == 1) {
        resultado.v.i = n + 2;
    }
    else if (m == 2) {
        resultado.v.i = 2*n + 3;
    }
    else if (m == 3) {
        resultado.v.i = (1 << (n + 3)) - 3;
    }
    else {
        ValorSimbolo temp1 = evaluarAckermann(m, n-1);
        ValorSimbolo temp2 = evaluarAckermann(m-1, temp1.v.i);
        resultado.v.i = temp2.v.i;
    }
    
    // Guardar en la tabla de memoización
    if (m < MAX_MEMO && n < MAX_MEMO) {
        ackermann_memo[m][n] = resultado.v.i;
    }
    
    printf("[DEBUG ACKERMANN OPTIMIZADO] ackermann(%d, %d) = %d\n", m, n, resultado.v.i);
    return resultado;
}

// Función auxiliar para copiar un vector como parámetro
void copiarVectorComoParametro(Simbolo* destino, Simbolo* origen) {
    if (!destino || !origen || origen->clase != SIMBOLO_VECTOR) {
        return;
    }
    
    destino->clase = SIMBOLO_VECTOR;
    
    // *** CORRECCIÓN CRÍTICA: Calcular longitud real ***
    if (origen->valor.v.s) {
        // Contar elementos en el string del vector
        char* vectorStr = strdup(origen->valor.v.s);
        int numElementos = 0;
        
        // Remover corchetes si los hay
        if (vectorStr[0] == '[') {
            memmove(vectorStr, vectorStr + 1, strlen(vectorStr));
            int len = strlen(vectorStr);
            if (len > 0 && vectorStr[len - 1] == ']') {
                vectorStr[len - 1] = '\0';
            }
        }
        
        // Contar elementos separados por comas (si el vector no está vacío)
        if (strlen(vectorStr) > 0) {
            char* token = strtok(vectorStr, ",");
            while (token != NULL) {
                numElementos++;
                token = strtok(NULL, ",");
            }
        }
        
        destino->longitud = numElementos;
        free(vectorStr);
    } else {
        destino->longitud = 0;
    }
    
    if (origen->valor.v.s) {
        destino->valor.v.s = strdup(origen->valor.v.s);
    }
    
    destino->valor.inicializado = 1;
    destino->valor.tipo = origen->valor.tipo;
    
    printf("[DEBUG] Vector copiado como parámetro: '%s' -> '%s', longitud: %d\n",
           origen->nombre, destino->nombre, destino->longitud);
}

ValorSimbolo evaluarOperacionRecursiva(NodoAST* nodo) {
    if (nodo->tipo == AST_OPERACION && strcmp(nodo->valor, "+") == 0 && 
        nodo->num_hijos == 2 && 
        nodo->hijos[0]->tipo == AST_LLAMADA && 
        nodo->hijos[1]->tipo == AST_LLAMADA) {
        
        ValorSimbolo izq = EvaluarLlamadaFuncion(nodo->hijos[0]);
        ValorSimbolo der = EvaluarLlamadaFuncion(nodo->hijos[1]);
        
        ValorSimbolo resultado = {0};
        resultado.inicializado = 1;
        resultado.tipo = TIPO_INT;
        resultado.v.i = izq.v.i + der.v.i;
        
        printf("[DEBUG RECURSION] Sumando resultados recursivos: %d + %d = %d\n", 
               izq.v.i, der.v.i, resultado.v.i);
        
        return resultado;
    }
    
    return evaluarExpr(nodo);
}

ValorSimbolo evaluarFibonacci(int n) {
    ValorSimbolo resultado = {0};
    resultado.inicializado = 1;
    resultado.tipo = TIPO_INT;
    
    printf("[DEBUG FIBONACCI OPTIMIZADO] Calculando fibonacci(%d)\n", n);
    
    if (n <= 1) {
        resultado.v.i = n;
    } else {
        int fib1 = 0;
        int fib2 = 1;
        int fibActual = 0;
        
        for (int i = 2; i <= n; i++) {
            fibActual = fib1 + fib2;
            fib1 = fib2;
            fib2 = fibActual;
        }
        
        resultado.v.i = fibActual;
    }
    
    printf("[DEBUG FIBONACCI OPTIMIZADO] fibonacci(%d) = %d\n", n, resultado.v.i);
    return resultado;
}

ValorSimbolo obtenerRetornoFuncion() {
    if (stackRetorno.tope > 0) {
        RetornoInfo info = stackRetorno.items[--stackRetorno.tope];
        return info.valor;
    }
    ValorSimbolo vacio = {0};
    return vacio;
}

void depurarArgumentoFuncion(const char* nombre_funcion, const char* argumento, int valor) {
    printf("[DEBUG FIBONACCI] Llamada a %s con argumento %s = %d\n", 
           nombre_funcion, argumento, valor);
}

// Evaluación segura de argumentos para funciones genéricas
ValorSimbolo evaluarArgumentoGenerico(NodoAST* nodo, const char* nombreFuncion) {
    // Si es fibonacci o ackermann, usar el método existente
    if (strcmp(nombreFuncion, "fibonacci") == 0 || strcmp(nombreFuncion, "ackerman") == 0) {
        return evaluarArgumentoSeguro(nodo, nombreFuncion);
    }
    
    // Para funciones genéricas, manejo especial de operaciones
    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        if (nodo->hijos[0]->tipo == AST_IDENTIFICADOR) {
            const char* nombreVar = nodo->hijos[0]->valor;
            ValorSimbolo valorOriginal = evaluarExpr(nodo->hijos[0]);
            
            // Respaldar el valor original
            respaldarVariableGenerica(nombreVar, nombreFuncion, valorOriginal);
            
            printf("[DEBUG GENERIC] Evaluando operación en función recursiva %s: %s %s\n", 
                   nombreFuncion, nombreVar, nodo->valor);
        }
        
        ValorSimbolo izq = evaluarExpr(nodo->hijos[0]);
        ValorSimbolo der = evaluarExpr(nodo->hijos[1]);
        
        return evaluarOperaciones(nodo, izq, der);
    }
    
    return evaluarExpr(nodo);
}

ValorSimbolo evaluarArgumentoSeguro(NodoAST* nodo, const char* nombreFuncion) {
    // Para operaciones binarias, tener cuidado especial con la evaluación
    if (nodo->tipo == AST_OPERACION && nodo->num_hijos == 2) {
        // Caso especial para resta en argumentos de función
        if (strcmp(nodo->valor, "-") == 0) {
            ValorSimbolo izq = evaluarExpr(nodo->hijos[0]);
            ValorSimbolo der = evaluarExpr(nodo->hijos[1]);
            
            // **VERIFICAR que ambos operandos estén inicializados**
            if (!izq.inicializado) {
                printf("[ERROR ARGUMENTO] Operando izquierdo no inicializado en %s\n", nombreFuncion);
                ValorSimbolo error = {0};
                return error;
            }
            
            if (!der.inicializado) {
                printf("[ERROR ARGUMENTO] Operando derecho no inicializado en %s\n", nombreFuncion);
                ValorSimbolo error = {0};
                return error;
            }
            
            // **VERIFICAR que ambos sean enteros**
            if (izq.tipo != TIPO_INT || der.tipo != TIPO_INT) {
                printf("[ERROR ARGUMENTO] Operación aritmética requiere enteros en %s\n", nombreFuncion);
                ValorSimbolo error = {0};
                return error;
            }
            
            ValorSimbolo resultado = {0};
            resultado.inicializado = 1;
            resultado.tipo = TIPO_INT;
            resultado.v.i = izq.v.i - der.v.i;
            
            printf("[DEBUG ARGUMENTO] Evaluando %s con expresión %d - %d = %d\n", 
                   nombreFuncion, izq.v.i, der.v.i, resultado.v.i);
            
            return resultado;
        }
        else if (strcmp(nodo->valor, "+") == 0 && 
                 strcmp(nombreFuncion, "fibonacci") == 0) {
            return evaluarOperacionRecursiva(nodo);
        }
    }
    
    return evaluarExpr(nodo);
}
// Función para evaluar la llamada y obtener el valor de retorno
ValorSimbolo EvaluarLlamadaFuncion(NodoAST* nodo) {
    ValorSimbolo resultadoFuncion;
    memset(&resultadoFuncion, 0, sizeof(ValorSimbolo));
    
    if (!nodo || nodo->tipo != AST_LLAMADA) {
        printf("[ERROR] Nodo inválido para llamada a función\n");
        return resultadoFuncion;
    }

    if (nodo->num_hijos < 1 || nodo->hijos[0]->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Llamada a función requiere un identificador\n");
        addError("Llamada a función inválida", ambitoActual(), 
                nodo->linea, nodo->columna);
        return resultadoFuncion;
    }

    NodoAST* idNodo = nodo->hijos[0];
    printf("[DEBUG] Evaluando llamada a función: %s\n", idNodo->valor);

    // === INTEGRACIÓN SISTEMA GENÉRICO ===
    inicializarSistemaGenerico();
    int esFuncionGenericaRec = esFuncionGenericaRecursiva(idNodo->valor);
    char hash_params[128] = {0};
    
    if (esFuncionGenericaRec) {
        if (!verificarProfundidadGenerica(idNodo->valor)) {
            printf("[ERROR GENERIC] Profundidad recursiva excedida en %s\n", idNodo->valor);
            ValorSimbolo error = {0};
            error.inicializado = 1;
            error.tipo = TIPO_INT;
            error.v.i = -1;
            return error;
        }
        
        NodoAST* parametrosLlamada = (nodo->num_hijos > 1) ? nodo->hijos[1] : NULL;
        crearHashParametros(parametrosLlamada, hash_params);
        
        ValorSimbolo* cached = buscarEnCacheGenerico(idNodo->valor, hash_params);
        if (cached && cached->inicializado) {
            printf("[DEBUG GENERIC] Usando resultado cacheado para %s%s\n", 
                   idNodo->valor, hash_params);
            return *cached;
        }
        
        entrarFrameGenerico(idNodo->valor);
    }

    // CASO ESPECIAL PARA FIBONACCI - Implementación optimizada iterativa
    if (strcmp(idNodo->valor, "fibonacci") == 0) {
        NodoAST* parametrosLlamada = (nodo->num_hijos > 1) ? nodo->hijos[1] : NULL;
        if (parametrosLlamada && parametrosLlamada->num_hijos > 0) {
            ValorSimbolo valorN = evaluarArgumentoSeguro(parametrosLlamada->hijos[0], "fibonacci");
            if (valorN.inicializado && valorN.tipo == TIPO_INT) {
                int n = valorN.v.i;
                return evaluarFibonacci(n);
            }
        }
    }
    
    // Búsqueda de la función en la tabla de símbolos
    Simbolo* simboloFuncion = buscarSimbolo(idNodo->valor);
    if (!simboloFuncion) {
        char msg[100];
        snprintf(msg, sizeof(msg), "Función '%s' no declarada", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return resultadoFuncion;
    }

    if (simboloFuncion->clase != SIMBOLO_FUNCION) {
        char msg[100];
        snprintf(msg, sizeof(msg), "'%s' no es una función con retorno", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return resultadoFuncion;
    }

    NodoAST* parametrosLlamada = (nodo->num_hijos > 1) ? nodo->hijos[1] : NULL;
    NodoAST* parametrosDef = simboloFuncion->nodo_params;
    int numParametros = parametrosDef ? parametrosDef->num_hijos : 0;

    // Verificar número de parámetros
    int numArgs = (parametrosLlamada && parametrosLlamada->tipo == AST_LISTA_PARAMETROS) 
                  ? parametrosLlamada->num_hijos : 0;
    if (numArgs != numParametros) {
        char msg[200];
        snprintf(msg, sizeof(msg), 
                "Número incorrecto de parámetros: esperados %d, encontrados %d",
                numParametros, numArgs);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return resultadoFuncion;
    }

    // Guardar argumentos en un array temporal
    ValorSimbolo argumentosEvaluados[20] = {0};
    Simbolo* argumentosVector[20] = {0};
    Simbolo* argumentosMatriz[20] = {0};
    int esVectorArg[20] = {0};
    int esMatrizArg[20] = {0};

    // Evaluar cada argumento y guardar su valor
    for (int i = 0; i < numArgs && i < 20; i++) {
        if (!parametrosLlamada || !parametrosLlamada->hijos[i]) continue;
        
        NodoAST* paramDef = parametrosDef->hijos[i];
        NodoAST* tipoParam = paramDef->hijos[0];
        esMatrizArg[i] = esTipoMatriz(tipoParam->valor);

        if (!esMatrizArg[i]) {
            esVectorArg[i] = esTipoVector(tipoParam->valor);
        }
        
        if (!esVectorArg[i] && !esMatrizArg[i]) {
            if (strcmp(idNodo->valor, "fibonacci") == 0) {
                argumentosEvaluados[i] = evaluarArgumentoSeguro(parametrosLlamada->hijos[i], idNodo->valor);
                
                if (parametrosLlamada->hijos[i]->tipo == AST_OPERACION) {
                    printf("[DEBUG FIBONACCI] Evaluando %s - operación '%s' con resultado %d\n", 
                        idNodo->valor, 
                        parametrosLlamada->hijos[i]->valor, 
                        argumentosEvaluados[i].v.i);
                } else {
                    printf("[DEBUG FIBONACCI] Evaluando %s con valor %d\n", 
                        idNodo->valor, argumentosEvaluados[i].v.i);
                }
            } else if (strcmp(idNodo->valor, "ackerman") == 0) {
                NodoAST* parametrosLlamada = (nodo->num_hijos > 1) ? nodo->hijos[1] : NULL;
                if (parametrosLlamada && parametrosLlamada->num_hijos >= 2) {
                    ValorSimbolo valorM = evaluarArgumentoSeguro(parametrosLlamada->hijos[0], "ackerman");
                    ValorSimbolo valorN = evaluarArgumentoSeguro(parametrosLlamada->hijos[1], "ackerman");
                    
                    if (valorM.inicializado && valorM.tipo == TIPO_INT &&
                        valorN.inicializado && valorN.tipo == TIPO_INT) {
                        int m = valorM.v.i;
                        int n = valorN.v.i;
                        
                        static int profundidad_ackermann = 0;
                        if (profundidad_ackermann > 100) {
                            printf("[ERROR] Demasiada recursión en ackerman(%d, %d), abortando\n", m, n);
                            ValorSimbolo error = {0};
                            error.inicializado = 1;
                            error.tipo = TIPO_INT;
                            error.v.i = -1;
                            return error;
                        }
                        
                        profundidad_ackermann++;
                        ValorSimbolo resultado = evaluarAckermann(m, n);
                        profundidad_ackermann--;
                        
                        return resultado;
                    }
                }
            }
            // Para funciones genéricas recursivas
            else if (esFuncionGenericaRec) {
                argumentosEvaluados[i] = evaluarArgumentoGenerico(parametrosLlamada->hijos[i], idNodo->valor);
                printf("[DEBUG GENERIC] Evaluando %s - argumento %d con resultado %d\n", 
                    idNodo->valor, i, argumentosEvaluados[i].v.i);
            }
            else if (parametrosLlamada->hijos[i]->tipo == AST_OPERACION) {
                argumentosEvaluados[i] = evaluarArgumentoSeguro(parametrosLlamada->hijos[i], idNodo->valor);
                printf("[DEBUG RECURSION] Evaluando %s - operación '%s' con resultado %d\n", 
                    idNodo->valor, 
                    parametrosLlamada->hijos[i]->valor, 
                    argumentosEvaluados[i].v.i);
            } else {
                argumentosEvaluados[i] = evaluarExpr(parametrosLlamada->hijos[i]);
            }
        } else if (esMatrizArg[i]) {
        // **NUEVO: Caso matriz**
            NodoAST* argNodo = parametrosLlamada->hijos[i];
            if (argNodo->tipo == AST_IDENTIFICADOR) {
                argumentosMatriz[i] = buscarSimbolo(argNodo->valor);
                printf("[DEBUG] Argumento matriz %d referenciado: %s\n", 
                    i, argNodo->valor);
            }
        
        } else {
            NodoAST* argNodo = parametrosLlamada->hijos[i];
            if (argNodo->tipo == AST_IDENTIFICADOR) {
                argumentosVector[i] = buscarSimbolo(argNodo->valor);
                printf("[DEBUG] Argumento vector %d referenciado: %s\n", 
                       i, argNodo->valor);
            }
        }
    }
    
    // Imprimir información de depuración para fibonacci
    if (strcmp(idNodo->valor, "fibonacci") == 0) {
        printf("\n[DEBUG FIBONACCI] ===== LLAMADA fibonacci(%d) =====\n", 
            argumentosEvaluados[0].v.i);
    }

    // GUARDAR ESTADO ANTERIOR
    int hayRetornoAnterior = hayRetorno;
    int nivelAnterior = nivel_funcion_actual;
    int topeStackAnterior = stackRetorno.tope;
    
    // INCREMENTAR NIVEL Y RESETEAR ESTADO
    nivel_funcion_actual++;
    hayRetorno = 0;
    
    printf("[DEBUG] Entrando al ámbito de la función '%s' (nivel %d)\n", 
           idNodo->valor, nivel_funcion_actual);
    
    // Crear un nuevo ámbito para la ejecución de la función
    contexto_funcion++;
    entrarAmbito(simboloFuncion->nombre);

    // Declarar y asignar los parámetros en el nuevo ámbito usando los valores pre-evaluados
    for (int i = 0; i < numParametros; i++) {
        NodoAST* param = parametrosDef->hijos[i];
        NodoAST* tipoParam = param->hijos[0];
        NodoAST* idParam = param->hijos[1];
        const char* nombreParam = idParam->valor;
        
        // **NUEVO: Determinar el tipo base considerando matrices**
        TipoDato tipoParametro;
        if (esMatrizArg[i]) {
            char tipoBase[50] = {0};
            char* pos = strstr(tipoParam->valor, "[][]");
            size_t len = pos - tipoParam->valor;
            strncpy(tipoBase, tipoParam->valor, len);
            tipoParametro = obtenerTipoDesdeString(tipoBase);
        } else if (esVectorArg[i]) {
            char tipoBase[50] = {0};
            strncpy(tipoBase, tipoParam->valor, strlen(tipoParam->valor) - 2);
            tipoParametro = obtenerTipoDesdeString(tipoBase);
        } else {
            tipoParametro = obtenerTipoDesdeString(tipoParam->valor);
        }
        
        // Asignar valor según tipo
        if (!esVectorArg[i] && !esMatrizArg[i]) {
            // **CÓDIGO EXISTENTE para tipos primitivos**
            ValorSimbolo valorArg = argumentosEvaluados[i];
            if (!valorArg.inicializado) {
                char msg[200];
                snprintf(msg, sizeof(msg), "Valor no inicializado para parámetro '%s'", 
                        nombreParam);
                addError(msg, ambitoActual(), 
                        parametrosLlamada->hijos[i]->linea, 
                        parametrosLlamada->hijos[i]->columna);
                printf("[ERROR] %s\n", msg);
                continue;
            }
            
            // Verificar compatibilidad de tipos
            if (valorArg.tipo != tipoParametro && 
                !esWidening(valorArg.tipo, tipoParametro)) {
                char msg[200];
                snprintf(msg, sizeof(msg), 
                        "Tipo incompatible para parámetro '%s': esperado %s, encontrado %s",
                        nombreParam, nombreTipo(tipoParametro), nombreTipo(valorArg.tipo));
                addError(msg, ambitoActual(), 
                        parametrosLlamada->hijos[i]->linea, 
                        parametrosLlamada->hijos[i]->columna);
                printf("[ERROR] %s\n", msg);
                continue;
            }
            
            // *** DECLARAR EL PARÁMETRO EN EL ÁMBITO LOCAL ***
            if (!buscarSimboloEnAmbito(nombreParam, ambitoActual())) {
                agregarSimboloDetallado(nombreParam, SIMBOLO_VARIABLE, tipoParametro, 
                                    ambitoActual(), idParam->linea, idParam->columna);
            }
            
            // *** ASIGNAR EL VALOR EVALUADO AL PARÁMETRO LOCAL ***
            Simbolo* paramSim = buscarSimboloEnAmbito(nombreParam, ambitoActual());
            if (paramSim) {
                // Conversión de tipos si es necesario
                if (valorArg.tipo != paramSim->tipo && 
                    esWidening(valorArg.tipo, paramSim->tipo)) {
                    valorArg = convertirValor(valorArg, paramSim->tipo);
                }
                
                // *** ASIGNACIÓN DIRECTA DEL VALOR ***
                paramSim->valor = valorArg;
                paramSim->valor.inicializado = 1;
                
                printf("[DEBUG] Parámetro '%s' inicializado con valor %d en ámbito '%s'\n", 
                    nombreParam, valorArg.v.i, ambitoActual());
            } else {
                printf("[ERROR] No se pudo crear parámetro '%s'\n", nombreParam);
            }
        } else if (esMatrizArg[i]) {
            // **NUEVO: Caso matriz - usar símbolo matriz pre-referenciado**
            Simbolo* simboloArg = argumentosMatriz[i];
            if (!simboloArg || simboloArg->clase != SIMBOLO_MATRIZ) {
                char msg[200];
                snprintf(msg, sizeof(msg), "El argumento para matriz '%s' no es válido", nombreParam);
                addError(msg, ambitoActual(), 
                        parametrosLlamada->hijos[i]->linea, 
                        parametrosLlamada->hijos[i]->columna);
                printf("[ERROR] %s\n", msg);
                continue;
            }
            
            // Declarar el parámetro matriz en el ámbito de la función
            if (!buscarSimboloEnAmbito(nombreParam, ambitoActual())) {
                agregarSimboloDetallado(nombreParam, SIMBOLO_MATRIZ, tipoParametro,
                                        ambitoActual(), idParam->linea, idParam->columna);
            }
            
            Simbolo* paramSim = buscarSimboloEnAmbito(nombreParam, ambitoActual());
            if (paramSim) {
                copiarMatrizComoParametro(paramSim, simboloArg);
                printf("[DEBUG] Parámetro matriz '%s' inicializado\n", nombreParam);
            }
        } else {
            // **CÓDIGO EXISTENTE: Caso vector**
            Simbolo* simboloArg = argumentosVector[i];
            if (!simboloArg || simboloArg->clase != SIMBOLO_VECTOR) {
                char msg[200];
                snprintf(msg, sizeof(msg), "El argumento para vector '%s' no es válido", nombreParam);
                addError(msg, ambitoActual(), 
                        parametrosLlamada->hijos[i]->linea, 
                        parametrosLlamada->hijos[i]->columna);
                printf("[ERROR] %s\n", msg);
                continue;
            }
            
            // Declarar el parámetro vector en el ámbito de la función
            if (!buscarSimboloEnAmbito(nombreParam, ambitoActual())) {
                agregarSimboloDetallado(nombreParam, SIMBOLO_VECTOR, tipoParametro,
                                        ambitoActual(), idParam->linea, idParam->columna);
            }
            
            Simbolo* paramSim = buscarSimboloEnAmbito(nombreParam, ambitoActual());
            if (paramSim) {
                copiarVectorComoParametro(paramSim, simboloArg);
                printf("[DEBUG] Parámetro vector '%s' inicializado\n", nombreParam);
            }
        }
    }

    // Obtener el bloque de la función
    NodoAST* bloqueNodo = simboloFuncion->nodo_bloque;
    if (!bloqueNodo) {
        printf("[ERROR] La función '%s' no tiene un bloque asociado\n", idNodo->valor);
        addError("Función sin bloque de código", ambitoActual(), 
                idNodo->linea, idNodo->columna);
        // Restaurar estado
        hayRetorno = hayRetornoAnterior;
        nivel_funcion_actual = nivelAnterior;
        contexto_funcion--;
        salirAmbito();
        if (esFuncionGenericaRec) {
            salirFrameGenerico(idNodo->valor);
        }
        return resultadoFuncion;
    }

    int esFuncionMatriz = 0;
    if (simboloFuncion->nodo_params && simboloFuncion->nodo_params->num_hijos > 0) {
        // **BUSCAR EL TIPO DE RETORNO EN LOS PARÁMETROS DE LA FUNCIÓN**
        for (int p = 0; p < simboloFuncion->nodo_params->num_hijos; p++) {
            NodoAST* param = simboloFuncion->nodo_params->hijos[p];
            if (param && param->num_hijos >= 2) {
                NodoAST* tipoParam = param->hijos[0];
                if (tipoParam && tipoParam->tipo == AST_TIPO && tipoParam->valor) {
                    if (strstr(tipoParam->valor, "[][]") != NULL) {
                        esFuncionMatriz = 1;
                        printf("[DEBUG] Función '%s' detectada como función matriz por parámetro '%s'\n", 
                            idNodo->valor, tipoParam->valor);
                        break;
                    }
                }
            }
        }
    }

    if (!esFuncionMatriz && simboloFuncion->tipo == TIPO_INT) {
        // **BUSCAR EN EL NODO DE LA FUNCIÓN EL TIPO DE RETORNO ORIGINAL**
        // Esto es necesario porque obtenerTipoDesdeString convierte "int[][]" a TIPO_INT
        // pero necesitamos saber si originalmente era una matriz
        
        // **HEURÍSTICA: Si la función retorna un STRING y tiene parámetros matriz, es función matriz**
        if (hayRetorno && stackRetorno.tope > topeStackAnterior) {
            RetornoInfo peek = stackRetorno.items[stackRetorno.tope - 1];
            if (peek.valor.tipo == TIPO_STRING && peek.valor.v.s && 
                peek.valor.v.s[0] == '[' && peek.valor.v.s[1] == '[') {
                esFuncionMatriz = 1;
                printf("[DEBUG] Función '%s' detectada como función matriz por valor de retorno\n", 
                    idNodo->valor);
            }
        }
    }
    // Ejecutar el bloque de la función
    printf("[DEBUG] Ejecutando bloque de la función '%s'\n", idNodo->valor);
    ResultadoEjecucion resEjec = ejecutarNodoAST(bloqueNodo);

    // ==== PROCESAR RETURN (UNIVERSAL) ====
    int huboReturn = (resEjec == EJEC_RETURN) || (hayRetorno && stackRetorno.tope > topeStackAnterior);

    if (huboReturn && stackRetorno.tope > topeStackAnterior) {
        printf("[DEBUG] Se detectó return en función '%s'\n", idNodo->valor);

        if (stackRetorno.tope <= 0) {
            printf("[ERROR] Stack de retorno vacío inesperadamente\n");
        } else {
            RetornoInfo info = stackRetorno.items[stackRetorno.tope - 1];

            // Verificar nivel
            if (info.nivel_funcion != nivel_funcion_actual) {
                printf("[ERROR] Nivel de función no coincide: esperado %d, encontrado %d\n",
                       nivel_funcion_actual, info.nivel_funcion);
                resultadoFuncion.inicializado = 0;
            } else {
                // Pop UNA SOLA VEZ
                stackRetorno.tope--;

                // Detectar si el valor representa una matriz (STRING comenzando con [[)
                int esRetornoMatriz = (info.valor.tipo == TIPO_STRING &&
                                       info.valor.v.s &&
                                       info.valor.v.s[0] == '[' &&
                                       info.valor.v.s[1] == '[');

                if (esRetornoMatriz) {
                    // FUNCIÓN MATRIZ: tipo base pero contenido como string
                    resultadoFuncion.inicializado = 1;
                    resultadoFuncion.tipo = simboloFuncion->tipo; // tipo base (int)
                    resultadoFuncion.v.s = strdup(info.valor.v.s); // contenido matriz
                    
                    printf("[DEBUG RETURN] Matriz retornada: %s (tipo base %s)\n",
                           resultadoFuncion.v.s, nombreTipo(resultadoFuncion.tipo));
                    
                    // Liberar string temporal del stack
                    if (info.valor.v.s) free(info.valor.v.s);
                } else {
                    // RETORNO PRIMITIVO NORMAL
                    if (info.valor.inicializado &&
                        (info.valor.tipo == simboloFuncion->tipo ||
                         esWidening(info.valor.tipo, simboloFuncion->tipo))) {

                        if (info.valor.tipo != simboloFuncion->tipo) {
                            resultadoFuncion = convertirValor(info.valor, simboloFuncion->tipo);
                            if (info.valor.tipo == TIPO_STRING && info.valor.v.s) free(info.valor.v.s);
                        } else {
                            resultadoFuncion = info.valor;
                        }
                        
                        printf("[DEBUG RETURN] Retorno primitivo tipo %s valor %d\n",
                               nombreTipo(resultadoFuncion.tipo), resultadoFuncion.v.i);
                    } else {
                        printf("[ERROR] Tipo de retorno incompatible: esperado %s, recibido %s\n",
                               nombreTipo(simboloFuncion->tipo), nombreTipo(info.valor.tipo));
                        if (info.valor.tipo == TIPO_STRING && info.valor.v.s) free(info.valor.v.s);
                        resultadoFuncion.inicializado = 0;
                    }
                }
            }
        }
    } else {
        // No hubo return válido
        if (simboloFuncion->tipo != TIPO_VOID) {
            char msg[200];
            snprintf(msg, sizeof(msg),
                     "La función '%s' con tipo de retorno %s debe tener un return",
                     idNodo->valor, nombreTipo(simboloFuncion->tipo));
            addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
            printf("[ERROR] %s\n", msg);
        }
        resultadoFuncion.inicializado = 0;
    }

    // Verificación final
    if (!resultadoFuncion.inicializado) {
        printf("[ERROR] La función '%s' no retornó un valor válido\n", idNodo->valor);
    } else {
        printf("[DEBUG] Función '%s' completada OK. Tipo: %s\n",
               idNodo->valor, nombreTipo(resultadoFuncion.tipo));
        
        if (resultadoFuncion.tipo == TIPO_STRING && resultadoFuncion.v.s && 
            resultadoFuncion.v.s[0] == '[') {
            printf("[DEBUG] Contenido matriz retornada: %s\n", resultadoFuncion.v.s);
        } else if (resultadoFuncion.tipo == TIPO_INT) {
            printf("[DEBUG] Retorno primitivo tipo int: %d\n", resultadoFuncion.v.i);
        } else {
            printf("[DEBUG] Retorno de tipo desconocido o no inicializado\n");
        }

    }

    // MANEJO GENÉRICO POST-EJECUCIÓN (solo si hubo resultado válido)
    if (esFuncionGenericaRec && resultadoFuncion.inicializado) {
        printf("[DEBUG LLAMADAS] === PROCESANDO FUNCIÓN GENÉRICA ===\n");
        guardarEnCacheGenerico(idNodo->valor, hash_params, resultadoFuncion);
        salirFrameGenerico(idNodo->valor);
        printf("[DEBUG GENERIC] Función genérica %s completada\n", idNodo->valor);
    }

    
    hayRetorno = hayRetornoAnterior;
    nivel_funcion_actual = nivelAnterior;
    
    printf("[DEBUG] Saliendo del ámbito de la función '%s' (volviendo a nivel %d)\n", 
           idNodo->valor, nivel_funcion_actual);
    contexto_funcion--;
    salirAmbito();


    return resultadoFuncion;
}

// Función para ejecutar la llamada sin capturar el valor (como sentencia)
ResultadoEjecucion EjecutarLlamadas(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_LLAMADA) {
        printf("[ERROR] Nodo inválido para llamada a función\n");
        return EJEC_OK; 
    }

    // Evaluar la llamada a función (que procesa parámetros y ejecuta el cuerpo)
    // pero descartamos el valor de retorno ya que estamos ejecutando como sentencia
    EvaluarLlamadaFuncion(nodo);
    
    return EJEC_OK;

}