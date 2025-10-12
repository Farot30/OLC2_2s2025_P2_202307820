#include "../../AST/ast.h"
#include "../tabla_simbolo.h"
#include "../../AST/err.h"
#include "../../simbolos/Evaluador.h"
#include "../semantico.h"
#include "../tipos.h"
#include "LlamadasVoid.h"
#include "Llamadas.h"
#include <stdio.h>
#include <string.h>

ResultadoEjecucion EjecutarLlamadaVoid(NodoAST* nodo) {
    // Aceptar tanto AST_LLAMADA_VOID como AST_LLAMADA para funciones void
    if (!nodo || (nodo->tipo != AST_LLAMADA_VOID && nodo->tipo != AST_LLAMADA)) {
        printf("[ERROR] Nodo inválido para llamada a función void (tipo: %d)\n", nodo ? nodo->tipo : 0);
        return EJEC_OK; 
    }

    if (nodo->num_hijos < 1 || nodo->hijos[0]->tipo != AST_IDENTIFICADOR) {
        printf("[ERROR] Llamada a función void requiere un identificador\n");
        addError("Llamada a función void inválida", ambitoActual(), nodo->linea, nodo->columna);
        return EJEC_OK; 
    }

    NodoAST* idNodo = nodo->hijos[0];
    printf("[DEBUG] Llamada a función void: %s\n", idNodo->valor);

    // Buscar la función en la tabla de símbolos
    Simbolo* simboloFuncion = buscarSimbolo(idNodo->valor);
    if (!simboloFuncion) {
        char msg[100];
        snprintf(msg, sizeof(msg), "Función void '%s' no declarada", idNodo->valor);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return EJEC_OK; 
    }

    // Verificar que sea una función void
    if (simboloFuncion->clase != SIMBOLO_FUNCION_VOID) {
        char msg[100];
        snprintf(msg, sizeof(msg), "'%s' no es una función void (clase: %d)", idNodo->valor, simboloFuncion->clase);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return EJEC_OK; 
    }

    // Obtener parámetros de la llamada y de la definición
    NodoAST* parametrosLlamada = (nodo->num_hijos > 1) ? nodo->hijos[1] : NULL;
    NodoAST* parametrosDef = simboloFuncion->nodo_params;
    int numParametros = (parametrosDef && parametrosDef->tipo == AST_PARAMETROS) ? parametrosDef->num_hijos : 0;

    // Verificar número de parámetros
    int numArgs = 0;
    if (parametrosLlamada && parametrosLlamada->tipo == AST_LISTA_PARAMETROS) {
        numArgs = parametrosLlamada->num_hijos;
    }

    if (numArgs != numParametros) {
        char msg[200];
        snprintf(msg, sizeof(msg), 
                "Número incorrecto de parámetros en función void '%s': esperados %d, encontrados %d",
                idNodo->valor, numParametros, numArgs);
        addError(msg, ambitoActual(), idNodo->linea, idNodo->columna);
        printf("[ERROR] %s\n", msg);
        return EJEC_OK;
    }

    // Crear un nuevo ámbito para la ejecución de la función
    printf("[DEBUG] Entrando al ámbito de la función void '%s'\n", idNodo->valor);
    entrarAmbito(simboloFuncion->nombre);

    // Evaluar y asignar parámetros
    for (int i = 0; i < numParametros; i++) {
        NodoAST* param = parametrosDef->hijos[i];
        NodoAST* tipoParam = param->hijos[0];
        NodoAST* idParam = param->hijos[1];
        const char* nombreParam = idParam->valor;
        
        // Determinar el tipo del parámetro
        TipoDato tipoParametro;
        TipoSimbolo claseParametro = SIMBOLO_VARIABLE;
        int esVector = 0;
        int esMatriz = 0;
        
        if (strstr(tipoParam->valor, "[][]") != NULL) {
            // **NUEVO: Es una matriz**
            char tipoBase[50] = {0};
            char* pos = strstr(tipoParam->valor, "[][]");
            size_t len = pos - tipoParam->valor;
            strncpy(tipoBase, tipoParam->valor, len);
            tipoParametro = obtenerTipoDesdeString(tipoBase);
            claseParametro = SIMBOLO_MATRIZ;
            esMatriz = 1; // **NUEVO**
            printf("[DEBUG] Parámetro matriz detectado: %s\n", nombreParam);
        } else if (strstr(tipoParam->valor, "[]") != NULL) {
            // **CÓDIGO EXISTENTE: Es un vector**
            char tipoBase[50] = {0};
            strncpy(tipoBase, tipoParam->valor, strlen(tipoParam->valor) - 2);
            tipoParametro = obtenerTipoDesdeString(tipoBase);
            claseParametro = SIMBOLO_VECTOR;
            esVector = 1;
        } else {
            // **CÓDIGO EXISTENTE: Tipos primitivos**
            tipoParametro = obtenerTipoDesdeString(tipoParam->valor);
        }

        
        // Determinar si se pasa por referencia o por valor
        int esPorReferencia = esTipoPorReferencia(tipoParametro, claseParametro);
        
        if (!esVector && !esPorReferencia) {
            // **PASO POR VALOR** - Tipos primitivos (int, float, double, char, boolean)
            ValorSimbolo valorArg = evaluarExpr(parametrosLlamada->hijos[i]);
            if (!valorArg.inicializado) {
                char msg[200];
                snprintf(msg, sizeof(msg), "Valor no inicializado para parámetro '%s'", nombreParam);
                addError(msg, ambitoActual(), 
                        parametrosLlamada->hijos[i]->linea, 
                        parametrosLlamada->hijos[i]->columna);
                printf("[ERROR] %s\n", msg);
                continue;
            }
            
            // Verificar compatibilidad de tipos
            if (valorArg.tipo != tipoParametro && !esWidening(valorArg.tipo, tipoParametro)) {
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
            
            // Declarar el parámetro en el ámbito local
            if (!buscarSimboloEnAmbito(nombreParam, ambitoActual())) {
                agregarSimboloDetallado(nombreParam, SIMBOLO_VARIABLE, tipoParametro, 
                                      ambitoActual(), idParam->linea, idParam->columna);
            }
            
            // Asignar el valor (COPIA)
            Simbolo* paramSim = buscarSimboloEnAmbito(nombreParam, ambitoActual());
            if (paramSim) {
                if (valorArg.tipo != paramSim->tipo && esWidening(valorArg.tipo, paramSim->tipo)) {
                    valorArg = convertirValor(valorArg, paramSim->tipo);
                }
                
                paramSim->valor = valorArg;
                paramSim->valor.inicializado = 1;
                
                printf("[DEBUG] Parámetro void '%s' inicializado POR VALOR en ámbito '%s'\n", 
                      nombreParam, ambitoActual());
            }
        } 
        else if (esPorReferencia && !esVector && !esMatriz) {
            // **PASO POR REFERENCIA** - String
            NodoAST* argNodo = parametrosLlamada->hijos[i];
            if (argNodo->tipo == AST_IDENTIFICADOR) {
                Simbolo* simboloArg = buscarSimbolo(argNodo->valor);
                if (!simboloArg || simboloArg->tipo != TIPO_STRING) {
                    char msg[200];
                    snprintf(msg, sizeof(msg), "El argumento para String '%s' no es válido", nombreParam);
                    addError(msg, ambitoActual(), 
                            parametrosLlamada->hijos[i]->linea, 
                            parametrosLlamada->hijos[i]->columna);
                    printf("[ERROR] %s\n", msg);
                    continue;
                }
                
                // Declarar el parámetro en el ámbito local
                if (!buscarSimboloEnAmbito(nombreParam, ambitoActual())) {
                    agregarSimboloDetallado(nombreParam, SIMBOLO_VARIABLE, tipoParametro,
                                            ambitoActual(), idParam->linea, idParam->columna);
                }
                
                Simbolo* paramSim = buscarSimboloEnAmbito(nombreParam, ambitoActual());
                if (paramSim) {
                    // Copiar valor inicial
                    paramSim->valor = simboloArg->valor;
                    if (simboloArg->valor.v.s) {
                        paramSim->valor.v.s = strdup(simboloArg->valor.v.s);
                    }
                    paramSim->valor.inicializado = 1;
                    
                    // Crear referencia para sincronización posterior
                    crearReferencia(nombreParam, simboloArg);
                    
                    printf("[DEBUG] Parámetro void '%s' inicializado POR REFERENCIA en ámbito '%s'\n", 
                          nombreParam, ambitoActual());
                }
            } else {
                char msg[200];
                snprintf(msg, sizeof(msg), "El argumento para String '%s' debe ser una variable", nombreParam);
                addError(msg, ambitoActual(), 
                        parametrosLlamada->hijos[i]->linea, 
                        parametrosLlamada->hijos[i]->columna);
                printf("[ERROR] %s\n", msg);
                continue;
            }
        } else if (esMatriz) {
            // **NUEVO: PASO POR REFERENCIA - Matrices**
            NodoAST* argNodo = parametrosLlamada->hijos[i];
            if (argNodo->tipo == AST_IDENTIFICADOR) {
                Simbolo* simboloArg = buscarSimbolo(argNodo->valor);
                if (!simboloArg || simboloArg->clase != SIMBOLO_MATRIZ) {
                    char msg[200];
                    snprintf(msg, sizeof(msg), "El argumento para matriz '%s' no es válido", nombreParam);
                    addError(msg, ambitoActual(), 
                            parametrosLlamada->hijos[i]->linea, 
                            parametrosLlamada->hijos[i]->columna);
                    printf("[ERROR] %s\n", msg);
                    continue;
                }
                
                // Declarar el parámetro matriz
                if (!buscarSimboloEnAmbito(nombreParam, ambitoActual())) {
                    agregarSimboloDetallado(nombreParam, SIMBOLO_MATRIZ, tipoParametro,
                                            ambitoActual(), idParam->linea, idParam->columna);
                }
                
                Simbolo* paramSim = buscarSimboloEnAmbito(nombreParam, ambitoActual());
                if (paramSim) {
                    copiarMatrizComoParametro(paramSim, simboloArg); // **NUEVO: función para matrices**
                    
                    // Crear referencia para sincronización
                    crearReferencia(nombreParam, simboloArg);
                    
                    printf("[DEBUG] Parámetro matriz void '%s' inicializado POR REFERENCIA\n", nombreParam);
                }
            }
        }
        else {
            // **PASO POR REFERENCIA** - Vectores/Arrays
            NodoAST* argNodo = parametrosLlamada->hijos[i];
            if (argNodo->tipo == AST_IDENTIFICADOR) {
                Simbolo* simboloArg = buscarSimbolo(argNodo->valor);
                if (!simboloArg || simboloArg->clase != SIMBOLO_VECTOR) {
                    char msg[200];
                    snprintf(msg, sizeof(msg), "El argumento para vector '%s' no es válido", nombreParam);
                    addError(msg, ambitoActual(), 
                            parametrosLlamada->hijos[i]->linea, 
                            parametrosLlamada->hijos[i]->columna);
                    printf("[ERROR] %s\n", msg);
                    continue;
                }
                
                // Declarar el parámetro vector
                if (!buscarSimboloEnAmbito(nombreParam, ambitoActual())) {
                    agregarSimboloDetallado(nombreParam, SIMBOLO_VECTOR, tipoParametro,
                                            ambitoActual(), idParam->linea, idParam->columna);
                }
                
                Simbolo* paramSim = buscarSimboloEnAmbito(nombreParam, ambitoActual());
                if (paramSim) {
                    copiarVectorComoParametro(paramSim, simboloArg);
                    
                    // Crear referencia para sincronización
                    crearReferencia(nombreParam, simboloArg);
                    
                    printf("[DEBUG] Parámetro vector void '%s' inicializado POR REFERENCIA\n", nombreParam);
                }
            }
        }
    }

    // Obtener el bloque de la función
    NodoAST* bloqueNodo = simboloFuncion->nodo_bloque;
    if (!bloqueNodo) {
        printf("[ERROR] La función '%s' no tiene un bloque asociado\n", idNodo->valor);
        addError("Función sin bloque de código", ambitoActual(), idNodo->linea, idNodo->columna);
        salirAmbito();
        return EJEC_OK; 
    }

    // Ejecutar el bloque de código de la función
    printf("[DEBUG] Ejecutando bloque de la función void '%s'\n", idNodo->valor);
    ResultadoEjecucion resultado = ejecutarNodoAST(bloqueNodo);

    // Verificar si hubo un break o continue que necesita ser propagado
    if (resultado == EJEC_BREAK || resultado == EJEC_CONTINUE) {
        printf("[ADVERTENCIA] Detectado %s en función void '%s', esto no debería ocurrir\n", 
               resultado == EJEC_BREAK ? "break" : "continue", idNodo->valor);
        // No propagamos BREAK/CONTINUE fuera de funciones
        resultado = EJEC_OK;
    }

    // Salir del ámbito de la función
    printf("[DEBUG] Saliendo del ámbito de la función void '%s'\n", idNodo->valor);
    salirAmbitoConLimpieza();

    return EJEC_OK;
}