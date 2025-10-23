#include "declaracion.h"
#include "../tipos.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include "../../AST/err.h"
#include "../../AST/errorTIpos.h"
#include "../../simbolos/Evaluador.h" 
#include "../Vectores/acceso_vector.h"
#include "../Vectores/acceso_matriz.h"
#include "../Vectores/acceso_arrays.h"
#include <stdio.h>
const char* ambitoActual(void);
const char* nombreTipo(TipoDato tipo);


int verificarDeclaracionConstante(NodoAST* nodo) {
    if (!nodo || nodo->num_hijos < 3) {
        printf("[ERROR] Estructura de nodo de declaración de constante inválida\n");
        return 0;
    }

    NodoAST* tipoNodo = nodo->hijos[0];
    NodoAST* identificador = nodo->hijos[1];
    NodoAST* valor = nodo->hijos[2];
    TipoDato tipoDeclarado = obtenerTipoDesdeString(tipoNodo->valor);

    if (buscarSimbolo(identificador->valor)) {
        addError("Constante redeclarada", ambitoActual(), identificador->linea, identificador->columna);
        nodo->valido = 0;
        return 0;
    }
    if (!valor) {
        addError("Constante debe inicializarse al declararse", ambitoActual(), identificador->linea, identificador->columna);
        nodo->valido = 0;
        return 0;
    }

    agregarSimboloDetallado(
        identificador->valor,
        SIMBOLO_CONST, // O SIMBOLO_CONST si tienes uno
        tipoDeclarado,
        ambitoActual(),
        identificador->linea,
        identificador->columna
    );
    Simbolo* sim = buscarSimbolo(identificador->valor);
    sim->es_constante = 1; // Marca como constante

    ValorSimbolo v = evaluarExpr(valor);
    if (!v.inicializado) {
        addError("No se pudo inicializar la constante", ambitoActual(), identificador->linea, identificador->columna);
        nodo->valido = 0;
        return 0;
    }
    setValorSimbolo(sim, v.tipo, v);
    return 1;
}

int verificarDeclaracion(NodoAST* nodo) {
    if (!nodo || nodo->num_hijos < 2) {
        printf("[ERROR] Estructura de nodo de declaración inválida\n");
        return 0;
    }

    NodoAST* tipoNodo = nodo->hijos[0];
    NodoAST* identificador = nodo->hijos[1];
    NodoAST* valor = (nodo->num_hijos > 2) ? nodo->hijos[2] : NULL;
    TipoDato tipoDeclarado = obtenerTipoDesdeString(tipoNodo->valor);

    printf("[DEBUG] Declarando '%s': tipo declarado=%s\n",
           identificador->valor, nombreTipo(tipoDeclarado));

    Simbolo* sims = buscarSimbolo(identificador->valor);
    if (sims) {
        // Si ya existe, actualiza tipo y valor (como una redeclaración)
        printf("[DEBUG] Redeclarando variable '%s', actualizando tipo a %s\n", identificador->valor, nombreTipo(tipoDeclarado));
        sims->tipo = tipoDeclarado;
        sims->clase = SIMBOLO_VARIABLE;
        if (sims->ambito) free(sims->ambito);
            sims->ambito = strdup(ambitoActual());
        sims->linea = identificador->linea;
        sims->columna = identificador->columna;
    } else {
        agregarSimboloDetallado(
            identificador->valor,
            SIMBOLO_VARIABLE,
            tipoDeclarado,
            ambitoActual(),
            identificador->linea,
            identificador->columna
        );
        sims = buscarSimbolo(identificador->valor);
    }

    Simbolo* sim = buscarSimbolo(identificador->valor); 
    
    if (valor && valor->valor) {
        // Usar valor inicial
        ValorSimbolo v = evaluarExpr(valor);
        setValorSimbolo(sim, v.tipo, v);
    } else {
        // Usar valor por defecto solo en la tabla de símbolos
        const char* def = valorPorDefectoTipo(tipoDeclarado);
        ValorSimbolo v;
        v.inicializado = 1;
        v.tipo = tipoDeclarado;
        
        // Asignar según tipo...
        if (tipoDeclarado == TIPO_INT) v.v.i = atoi(def);
        else if (tipoDeclarado == TIPO_FLOAT) v.v.f = atof(def);
        else if (tipoDeclarado == TIPO_DOUBLE) v.v.d = atof(def);
        else if (tipoDeclarado == TIPO_BOOLEAN) v.v.b = (strcmp(def, "true") == 0) ? 1 : 0;
        else if (tipoDeclarado == TIPO_CHAR) v.v.c = '\0';
        else if (tipoDeclarado == TIPO_STRING) v.v.s = NULL;
        
        setValorSimbolo(sim, tipoDeclarado, v);
        printf("[DEBUG] Variable '%s' inicializada con valor por defecto\n", identificador->valor);
        return 1; // Terminar aquí para declaraciones sin inicializador
    }
    
    if (sim && nodo->num_hijos > 1) {
        // Verificación para métodos de parseo
        if (valor->tipo == AST_PARSEINT || valor->tipo == AST_PARSEFLOAT || valor->tipo == AST_PARSEDOUBLE) {
            TipoDato tipoParse;
            const char* nombreFuncion;
            
            if (valor->tipo == AST_PARSEINT) {
                tipoParse = TIPO_INT;
                nombreFuncion = "Integer.parseInt";
            } else if (valor->tipo == AST_PARSEFLOAT) {
                tipoParse = TIPO_FLOAT;
                nombreFuncion = "Float.parseFloat";
            } else {
                tipoParse = TIPO_DOUBLE;
                nombreFuncion = "Double.parseDouble";
            }
            
            // Verificar que el tipo de la declaración coincida con el tipo del parse
            if (tipoDeclarado != tipoParse) {
                char msg[200];
                snprintf(msg, sizeof(msg), 
                        "Tipo incompatible: '%s' devuelve %s pero se está asignando a una variable de tipo %s", 
                        nombreFuncion, nombreTipo(tipoParse), nombreTipo(tipoDeclarado));
                addError(msg, ambitoActual(), identificador->linea, identificador->columna);
                printf("[ERROR] %s\n", msg);
                identificador->valido = 0;
                nodo->valido = 0;
                //eliminarSimbolo(identificador->valor);
                return 0;
            }
            
            printf("[DEBUG] Tipo correcto para %s: %s\n", nombreFuncion, nombreTipo(tipoParse));
            
            // Continuar con la evaluación normal después de verificar el tipo
            ValorSimbolo v = evaluarExpr(valor);
            if(v.inicializado){

                setValorSimbolo(sim, v.tipo, v);
                
                // Mostrar valor convertido según el tipo
                if (tipoParse == TIPO_INT) {
                    printf("[DEBUG] Variable '%s' inicializada con valor %d\n", identificador->valor, v.v.i);
                } else if (tipoParse == TIPO_FLOAT) {
                    printf("[DEBUG] Variable '%s' inicializada con valor %f\n", identificador->valor, v.v.f);
                } else {
                    printf("[DEBUG] Variable '%s' inicializada con valor %f\n", identificador->valor, v.v.d);
                }
            } else {
                printf("[ERROR] Error al evaluar %s para '%s'\n", nombreFuncion, identificador->valor);
                nodo->valido = 0;
                return 0;
            }
        }
        // Si el valor es un acceso a vector/matriz
        else if (valor->tipo == AST_ACCESO) {
            // Obtener el identificador para determinar si es vector o matriz
            if (valor->num_hijos > 0 && valor->hijos[0]->tipo == AST_IDENTIFICADOR) {
                Simbolo* simboloAcceso = buscarSimbolo(valor->hijos[0]->valor);

                if (simboloAcceso) {
                    printf("[DEBUG] Verificando acceso a %s '%s' para inicialización de '%s'\n", 
                        simboloAcceso->clase == SIMBOLO_MATRIZ ? "matriz" :
                        simboloAcceso->clase == SIMBOLO_VECTOR ? "vector" :
                        simboloAcceso->clase == SIMBOLO_ARRAY_MULTIDIMENSIONAL ? "array multidimensional" :
                        "variable",
                        valor->hijos[0]->valor, identificador->valor);

                    ValorSimbolo valorAcceso; // Para almacenar el resultado del acceso
                    memset(&valorAcceso, 0, sizeof(valorAcceso));
                    
                    // Dirigir a la función específica según el tipo de estructura
                    if (simboloAcceso->clase == SIMBOLO_MATRIZ) {
                        if (!verificarAccesoMatriz(valor)) {
                            char msg[128];
                            snprintf(msg, sizeof(msg), "Acceso inválido para inicialización de '%s'", identificador->valor);
                            addError(msg, ambitoActual(), identificador->linea, identificador->columna);
                            printf("[ERROR] %s\n", msg);
                            nodo->valido = 0;
                            return 0;
                        }
                        
                        // Evaluar el acceso a matriz y obtener el valor
                        valorAcceso = evaluarAccesoMatriz(valor);
                    } 
                    else if (simboloAcceso->clase == SIMBOLO_VECTOR) {
                        if (!verificarAccesoVector(valor)) {
                            char msg[128];
                            snprintf(msg, sizeof(msg), "Acceso inválido para inicialización de '%s'", identificador->valor);
                            addError(msg, ambitoActual(), identificador->linea, identificador->columna);
                            printf("[ERROR] %s\n", msg);
                            nodo->valido = 0;
                            return 0;
                        }
                        
                        // Evaluar el acceso a vector y obtener el valor
                        valorAcceso = evaluarAccesoVector(valor);
                    }else if (simboloAcceso->clase == SIMBOLO_ARRAY_MULTIDIMENSIONAL) {
                        if (!verificarAccesoArray(valor)) {
                            char msg[128];
                            snprintf(msg, sizeof(msg), "Acceso inválido para inicialización de '%s'", identificador->valor);
                            addError(msg, ambitoActual(), identificador->linea, identificador->columna);
                            printf("[ERROR] %s\n", msg);
                            nodo->valido = 0;
                            return 0;
                        }

                        // Evaluar el acceso a array y obtener el valor
                        valorAcceso = evaluarAccesoArray(valor);
                    } else {
                        char msg[128];
                        snprintf(msg, sizeof(msg), "Variable '%s' no es un vector ni matriz", valor->hijos[0]->valor);
                        addError(msg, ambitoActual(), valor->hijos[0]->linea, valor->hijos[0]->columna);
                        printf("[ERROR] %s\n", msg);
                        nodo->valido = 0;
                        return 0;
                    }
                    
                    // Verificar que el acceso haya producido un valor inicializado
                    if (!valorAcceso.inicializado) {
                        char msg[128];
                        snprintf(msg, sizeof(msg), "No se pudo obtener valor del acceso para inicializar '%s'", 
                                identificador->valor);
                        addError(msg, ambitoActual(), identificador->linea, identificador->columna);
                        printf("[ERROR] %s\n", msg);
                        nodo->valido = 0;
                        return 0;
                    }
                    
                    // Verificar compatibilidad de tipos
                    if (sim->tipo != valorAcceso.tipo && !esWidening(valorAcceso.tipo, sim->tipo)) {
                        char msg[160];
                        snprintf(msg, sizeof(msg), "Tipo incompatible en declaración '%s': %s vs %s",
                                identificador->valor, nombreTipo(sim->tipo), nombreTipo(valorAcceso.tipo));
                        addError(msg, ambitoActual(), identificador->linea, identificador->columna);
                        printf("[ERROR] %s\n", msg);
                        nodo->valido = 0;
                        return 0;
                    }
                    
                    // Asignar el valor obtenido al símbolo
                    setValorSimbolo(sim, valorAcceso.tipo, valorAcceso); 
                    
                    // Mostrar el valor asignado para depuración
                    printf("[DEBUG] Variable '%s' inicializada con valor de acceso a %s: ", 
                        identificador->valor,
                        simboloAcceso->clase == SIMBOLO_MATRIZ ? "matriz" : "vector");
                    
                    switch (sim->tipo) {
                        case TIPO_INT:
                            printf("%d\n", sim->valor.v.i);
                            break;
                        case TIPO_FLOAT:
                            printf("%f\n", sim->valor.v.f);
                            break;
                        case TIPO_DOUBLE:
                            printf("%lf\n", sim->valor.v.d);
                            break;
                        case TIPO_CHAR:
                            printf("'%c'\n", sim->valor.v.c);
                            break;
                        case TIPO_BOOLEAN:
                            printf("%s\n", sim->valor.v.b ? "true" : "false");
                            break;
                        case TIPO_STRING:
                            printf("\"%s\"\n", sim->valor.v.s ? sim->valor.v.s : "(null)");
                            break;
                        default:
                            printf("(tipo desconocido)\n");
                            break;
                    }
                } else {
                    char msg[128];
                    snprintf(msg, sizeof(msg), "Variable '%s' no declarada", valor->hijos[0]->valor);
                    addError(msg, ambitoActual(), valor->hijos[0]->linea, valor->hijos[0]->columna);
                    printf("[ERROR] %s\n", msg);
                    nodo->valido = 0;
                    return 0;
                }
            }
        } else if (valor->tipo == AST_ADD) {
            // Verifica que haya al menos un hijo (el identificador del array)
            if (valor->num_hijos < 1 || valor->hijos[0]->tipo != AST_IDENTIFICADOR) {
                printf("[ERROR] Estructura inválida para Array.add\n");
                nodo->valido = 0;
                return 0;
            }
            
            // Obtén el identificador del array original
            NodoAST* idArrayNodo = valor->hijos[0];
            Simbolo* simboloArray = buscarSimbolo(idArrayNodo->valor);
            
            if (!simboloArray) {
                printf("[ERROR] Array '%s' no declarado\n", idArrayNodo->valor);
                nodo->valido = 0;
                return 0;
            }
            
            // Verifica que sea realmente un vector
            if (simboloArray->clase != SIMBOLO_VECTOR) {
                printf("[ERROR] '%s' no es un vector\n", idArrayNodo->valor);
                nodo->valido = 0;
                return 0;
            }
            
            // Verifica compatibilidad de tipos
            if (tipoDeclarado != simboloArray->tipo) {
                char msg[200];
                snprintf(msg, sizeof(msg), 
                    "Tipo incompatible en declaración '%s': el método add() de '%s' devuelve %s[] pero se está asignando a %s[]",
                    identificador->valor, idArrayNodo->valor, 
                    nombreTipo(simboloArray->tipo), nombreTipo(tipoDeclarado));
                addError(msg, ambitoActual(), identificador->linea, identificador->columna);
                printf("[ERROR] %s\n", msg);
                nodo->valido = 0;
                return 0;
            }
            
            printf("[DEBUG] Tipos compatibles: nuevo vector '%s' (%s[]) y vector original '%s' (%s[])\n",
                identificador->valor, nombreTipo(tipoDeclarado),
                idArrayNodo->valor, nombreTipo(simboloArray->tipo));
            
            // Evalúa la expresión para obtener el nuevo array
            ValorSimbolo nuevoArray = evaluarExpr(valor);
            if (nuevoArray.inicializado) {
                // Actualiza el tipo de símbolo a VECTOR para que se maneje correctamente
                modificarClaseSimbolo(identificador->valor, SIMBOLO_VECTOR);
                
                // Asigna el valor (formato del array como string)
                setValorSimbolo(sim, nuevoArray.tipo, nuevoArray);
                
            } else {
                printf("[ERROR] Error al evaluar Array.add para '%s'\n", identificador->valor);
                nodo->valido = 0;
                return 0;
            }
        }else {
           // printf("[DEBUG DECLARACION] === EVALUANDO EXPRESIÓN PARA '%s' ===\n", identificador->valor);
            // Caso normal (expresión)
             ValorSimbolo v = evaluarExpr(valor);

            // printf("[DEBUG DECLARACION] Expresión evaluada: inicializado=%d, tipo=%d\n", 
             //      v.inicializado, v.tipo);
            if(v.inicializado){

                if (valor->tipo == AST_LLAMADA) {

                }
                // Compara el tipo declarado con el tipo evaluado
                if (sim->tipo != v.tipo && v.tipo != TIPO_DESCONOCIDO) {
                    if (!esWidening(v.tipo, sim->tipo)) {
                        // Permitir narrowing casting de int a char
                        if (sim->tipo == TIPO_CHAR && v.tipo == TIPO_INT) {
                            // printf("[DEBUG DECLARACION] Narrowing casting de int a char para '%s': %d -> '%c'\n",
                            //        identificador->valor, v.v.i, (char)v.v.i);
                            // Convertir el valor de int a char
                            v.tipo = TIPO_CHAR;
                            v.v.c = (char)v.v.i;
                        } else {
                            char msg[160];
                            snprintf(msg, sizeof(msg), "Tipo incompatible en declaración '%s': %s vs %s",
                                    identificador->valor, nombreTipo(sim->tipo), nombreTipo(v.tipo));
                            addError(msg, ambitoActual(), identificador->linea, identificador->columna);
                            printf("[ERROR] %s\n", msg);
                            valor->valido = 0;
                            nodo->valido = 0;
                            //eliminarSimbolo(identificador->valor);
                            return 0;
                        }
                    } else {
                        printf("[DEBUG] Conversión implícita en asignación: de %s to %s\n", 
                            nombreTipo(v.tipo), nombreTipo(sim->tipo));
                    }
                }

                setValorSimbolo(sim, v.tipo, v);
                
                // Mostrar valor según el tipo
                if (sim->tipo == TIPO_INT) {
                    printf("[DEBUG] Variable '%s' inicializada con valor %d\n", identificador->valor, v.v.i);
                } else if (sim->tipo == TIPO_FLOAT) {
                    printf("[DEBUG] Variable '%s' inicializada con valor %f\n", identificador->valor, v.v.f);
                } else if (sim->tipo == TIPO_DOUBLE) {
                    printf("[DEBUG] Variable '%s' inicializada con valor %f\n", identificador->valor, v.v.d);
                } else if (sim->tipo == TIPO_CHAR) {
                    printf("[DEBUG] Variable '%s' inicializada con valor '%c'\n", identificador->valor, v.v.c);
                } else if (sim->tipo == TIPO_BOOLEAN) {
                    printf("[DEBUG] Variable '%s' inicializada con valor %s\n", 
                           identificador->valor, v.v.b ? "true" : "false");
                } else if (sim->tipo == TIPO_STRING) {
                    printf("[DEBUG] Variable '%s' inicializada con valor \"%s\"\n", 
                           identificador->valor, v.v.s ? v.v.s : "(null)");
                } else {
                    printf("[DEBUG] Variable '%s' inicializada con valor (tipo desconocido)\n", identificador->valor);
                }
            } else {
                printf("[ERROR] Expresión no inicializada para '%s'\n", identificador->valor);
                nodo->valido = 0;
                return 0;
            }
        }
    }
    
    return 1;
}
