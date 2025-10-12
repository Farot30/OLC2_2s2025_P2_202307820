#include <stdlib.h>
#include "for.h"
#include <string.h>
#include <stdbool.h>
#include "../../AST/ejecutar_ast.h"
#include "../../AST/err.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../../simbolos/Evaluador.h"

SentenciaFor* crearSentenciaFor(NodoAST* inicializacion, NodoAST* condicion, NodoAST* actualizacion, NodoAST* bloque) {
    SentenciaFor* sentencia = (SentenciaFor*)malloc(sizeof(SentenciaFor));
    sentencia->inicializacion = inicializacion;
    sentencia->condicion = condicion;
    sentencia->incremento = actualizacion;
    sentencia->bloque = bloque;
    return sentencia;
}

void liberarSentenciaFor(SentenciaFor* sentencia) {
    if(!sentencia) return;
    free(sentencia);
}

ResultadoEjecucion ejecutarSentenciaFor(NodoAST* nodo) {
    if (!nodo || nodo->tipo != AST_FOR) {
        printf("[ERROR FOR] Nodo inválido o no es tipo FOR\n");
        return EJEC_OK;
    }

    // Verificar que tenemos al menos 3 hijos (inicialización, condición, incremento)
    if (nodo->num_hijos < 3) {
        printf("[ERROR FOR] FOR incompleto - esperados al menos 3 hijos, encontrados %d\n", nodo->num_hijos);
        return EJEC_OK;
    }

    NodoAST* inicializacion = nodo->hijos[0];
    NodoAST* condicion = nodo->hijos[1];
    NodoAST* incremento = nodo->hijos[2];
    NodoAST* bloque = (nodo->num_hijos >= 4) ? nodo->hijos[3] : NULL;

    printf("[DEBUG FOR] === INICIANDO BUCLE FOR ===\n");
    printf("[DEBUG FOR] Número de hijos: %d\n", nodo->num_hijos);
    printf("[DEBUG FOR] Inicialización: %s\n", inicializacion ? "presente" : "NULL");
    printf("[DEBUG FOR] Condición: %s\n", condicion ? "presente" : "NULL");
    printf("[DEBUG FOR] Incremento: %s\n", incremento ? "presente" : "NULL");
    printf("[DEBUG FOR] Bloque: %s\n", bloque ? "presente" : "NULL (vacío)");

    // Ejecutar inicialización
    if(inicializacion) {
        printf("[DEBUG FOR] Ejecutando inicialización\n");
        ResultadoEjecucion res_init = ejecutarNodoAST(inicializacion);
        if (res_init != EJEC_OK) {
            printf("[ERROR FOR] Error en inicialización\n");
            return res_init;
        }
    }
    
    // Establecer un límite de seguridad para evitar bucles infinitos durante desarrollo
    int max_iteraciones = 1000;
    int contador_iteraciones = 0;
    entrarAmbito("for");
    
    while (contador_iteraciones < max_iteraciones) {
        contador_iteraciones++;
        
        // Evaluar condición
        if (condicion) {
            printf("[DEBUG FOR] Evaluando condición (iteración %d)\n", contador_iteraciones);
            ValorSimbolo vCond = evaluarExpr(condicion);
            
            // Verificar inicialización
            if (!vCond.inicializado) {
                printf("[ERROR FOR] Condición del FOR no está inicializada o tiene error de acceso a índice\n");
                addError("Condición del FOR tiene error o accede a índice fuera de rango", 
                        ambitoActual(), nodo->linea, nodo->columna);
                break;  // Salir del bucle si hay error
            }
            
            // Verificar tipo booleano
            if (vCond.tipo != TIPO_BOOLEAN) {
                printf("[ERROR FOR] Condición del FOR no es booleana (tipo: %d)\n", vCond.tipo);
                addError("Condición del FOR no es booleana", ambitoActual(), nodo->linea, nodo->columna);
                break;  // Salir del bucle si hay error de tipo
            }
            
            printf("[DEBUG FOR] Condición evaluada: %s\n", vCond.v.b ? "true" : "false");
            if (!vCond.v.b) {
                printf("[DEBUG FOR] Condición falsa, saliendo del bucle\n");
                break;
            }
        }

        // Ejecutar bloque (si existe)
        if (bloque) {
            printf("[DEBUG FOR] Ejecutando bloque de código\n");
            
            ResultadoEjecucion res = ejecutarNodoAST(bloque);
            
            if (res == EJEC_BREAK) {
                printf("[DEBUG FOR] Break detectado, saliendo del bucle\n");
                break; 
            } else if (res == EJEC_CONTINUE) {
                printf("[DEBUG FOR] Continue detectado, ejecutando incremento antes de continuar\n");
                
                // EJECUTAR EL INCREMENTO ANTES DEL CONTINUE
                if(incremento) {
                    printf("[DEBUG FOR] Ejecutando incremento por continue: tipo=%d, valor='%s'\n", 
                           incremento->tipo, incremento->valor ? incremento->valor : "NULL");
                    
                    // Manejar diferentes tipos de nodos de incremento
                    if (incremento->tipo == AST_OPERACION && 
                        incremento->valor && 
                        (strcmp(incremento->valor, "++") == 0 || strcmp(incremento->valor, "--") == 0)) {
                        
                        // Para operaciones de incremento/decremento
                        if (incremento->num_hijos > 0 && incremento->hijos[0] && 
                            incremento->hijos[0]->tipo == AST_IDENTIFICADOR) {
                            
                            const char* nombreVar = incremento->hijos[0]->valor;
                            Simbolo* simbolo = buscarSimbolo(nombreVar);
                            
                            if (simbolo && simbolo->tipo == TIPO_INT) {
                                if (strcmp(incremento->valor, "++") == 0) {
                                    simbolo->valor.v.i++;
                                    printf("[DEBUG FOR] Incrementado '%s' a: %d (por continue)\n", nombreVar, simbolo->valor.v.i);
                                } else {
                                    simbolo->valor.v.i--;
                                    printf("[DEBUG FOR] Decrementado '%s' a: %d (por continue)\n", nombreVar, simbolo->valor.v.i);
                                }
                            } else {
                                printf("[WARNING FOR] Variable '%s' no encontrada o no es int (continue)\n", nombreVar);
                            }
                        }
                    } else {
                        // Para cualquier otro tipo de expresión de incremento
                        ResultadoEjecucion res_inc = ejecutarNodoAST(incremento);
                        if (res_inc != EJEC_OK) {
                            printf("[WARNING FOR] Error en incremento durante continue\n");
                        }
                    }
                }
                
                // AHORA SÍ HACER EL CONTINUE
                continue;
                
            } else if (res != EJEC_OK) {
                printf("[ERROR FOR] Error al ejecutar bloque del FOR\n");
                return res; 
            } 
        }

        // Ejecutar incremento normal (cuando NO hay continue)
        if(incremento) {
            printf("[DEBUG FOR] Ejecutando incremento normal: tipo=%d, valor='%s'\n", 
                   incremento->tipo, incremento->valor ? incremento->valor : "NULL");
            
            // Manejar diferentes tipos de nodos de incremento
            if (incremento->tipo == AST_OPERACION && 
                incremento->valor && 
                (strcmp(incremento->valor, "++") == 0 || strcmp(incremento->valor, "--") == 0)) {
                
                // Para operaciones de incremento/decremento
                if (incremento->num_hijos > 0 && incremento->hijos[0] && 
                    incremento->hijos[0]->tipo == AST_IDENTIFICADOR) {
                    
                    const char* nombreVar = incremento->hijos[0]->valor;
                    Simbolo* simbolo = buscarSimbolo(nombreVar);
                    
                    if (simbolo && simbolo->tipo == TIPO_INT) {
                        if (strcmp(incremento->valor, "++") == 0) {
                            simbolo->valor.v.i++;
                            printf("[DEBUG FOR] Incrementado '%s' a: %d\n", nombreVar, simbolo->valor.v.i);
                        } else {
                            simbolo->valor.v.i--;
                            printf("[DEBUG FOR] Decrementado '%s' a: %d\n", nombreVar, simbolo->valor.v.i);
                        }
                    } else {
                        printf("[WARNING FOR] Variable '%s' no encontrada o no es int\n", nombreVar);
                    }
                }
            } else {
                // Para cualquier otro tipo de expresión de incremento
                ResultadoEjecucion res_inc = ejecutarNodoAST(incremento);
                if (res_inc != EJEC_OK) {
                    printf("[WARNING FOR] Error en incremento, continuando\n");
                }
            }
        }
        
        // Verificar límite de seguridad
        if (contador_iteraciones >= max_iteraciones) {
            printf("[ERROR FOR] Posible bucle infinito detectado después de %d iteraciones\n", contador_iteraciones);
            addError("Posible bucle infinito en FOR", ambitoActual(), nodo->linea, nodo->columna);
            break;
        }
    }
    
    salirAmbito();
    
    printf("[DEBUG FOR] === BUCLE FOR COMPLETADO después de %d iteraciones ===\n", contador_iteraciones);
    return EJEC_OK;
}