
#include "evaluarString.h"
#include "../../simbolos/Operador.h"
#include <ctype.h>
#include "../../AST/ast.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include "../../AST/err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char* concat3(const char* s1, const char* s2, const char* s3) {
    size_t len1 = s1 ? strlen(s1) : 0;
    size_t len2 = s2 ? strlen(s2) : 0;
    size_t len3 = s3 ? strlen(s3) : 0;
    
    char* result = (char*)malloc(len1 + len2 + len3 + 1);
    if (!result) return NULL;
    
    if (s1) strcpy(result, s1);
    else result[0] = '\0';
    
    if (s2) strcat(result, s2);
    if (s3) strcat(result, s3);
    
    return result;
}

// Función para evaluar AST_PARSEJOIN
ValorSimbolo evaluarParseJoin(NodoAST* e) {
    //printf("[DEBUG evaluarParseJoin] === INICIO ===\n");
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    r.tipo = TIPO_STRING;
    r.inicializado = 0;

    if (e->num_hijos < 2) {
        return r;
    }

    // Evaluar el delimitador
    ValorSimbolo delimitador = evaluarExpr(e->hijos[0]);
    
    if (!delimitador.inicializado || delimitador.tipo != TIPO_STRING) {
        char msg[200];
        snprintf(msg, sizeof(msg), "El delimitador de String.join debe ser una cadena, se recibió %s", 
                nombreTipo(delimitador.tipo));
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }

    // Obtener el nodo de la lista (segundo argumento)
    NodoAST* lista = e->hijos[1];
    
    // **CASO ESPECIAL: Si la lista contiene un identificador de vector**
    if (lista->tipo == AST_LISTA && lista->num_hijos == 1 && lista->hijos[0]->tipo == AST_IDENTIFICADOR) {
        
        NodoAST* idNodo = lista->hijos[0];
        
        Simbolo* vectorSimbolo = buscarSimbolo(idNodo->valor);
        
        
        if (vectorSimbolo && vectorSimbolo->clase == SIMBOLO_VECTOR) {
            
            // Procesar el string del vector para extraer elementos individuales
            if (vectorSimbolo->valor.inicializado && vectorSimbolo->valor.v.s) {
                char* vectorStr = strdup(vectorSimbolo->valor.v.s);
                
                // Remover corchetes externos [...]
                if (vectorStr[0] == '[') {
                    memmove(vectorStr, vectorStr + 1, strlen(vectorStr));
                    int len = strlen(vectorStr);
                    if (len > 0 && vectorStr[len - 1] == ']') {
                        vectorStr[len - 1] = '\0';
                    }
                }
                
                // Inicializar resultado
                char* resultado_final = NULL;
                
                // Dividir por comas y procesar cada elemento
                char* saveptr;
                char* token = strtok_r(vectorStr, ",", &saveptr);
                int esPrimerElemento = 1;
                int contadorElementos = 0;
                
                
                while (token != NULL) {
                    contadorElementos++;
                    
                    // Limpiar espacios al inicio
                    while (*token == ' ' || *token == '\t') token++;
                    
                    // Limpiar espacios al final
                    char* fin = token + strlen(token) - 1;
                    while (fin > token && (*fin == ' ' || *fin == '\t')) {
                        *fin = '\0';
                        fin--;
                    }
                    
                    
                    // Remover comillas si las hay
                    if (token[0] == '"' && strlen(token) > 1 && token[strlen(token)-1] == '"') {
                        token[strlen(token)-1] = '\0';
                        token++;
                    }
                    
                    
                    if (esPrimerElemento) {
                        // Primer elemento: crear el string inicial
                        resultado_final = strdup(token);
                        esPrimerElemento = 0;
                    } else {
                        char* temp = resultado_final;
                        resultado_final = concat3(temp, delimitador.v.s, token);
                        //printf("[DEBUG evaluarParseJoin] Resultado parcial: '%s'\n", resultado_final);
                        free(temp);
                    }
                    
                    token = strtok_r(NULL, ",", &saveptr);
                }
                
                
                // Asignar resultado
                r.inicializado = 1;
                r.v.s = resultado_final ? resultado_final : strdup("");
                
                free(vectorStr);
                return r;
            } else {
            }
        } else {
        }
    }
    
    // **CASO NORMAL: Lista de expresiones múltiples (AST_LISTA)**
    if (lista->tipo == AST_LISTA) {
        // Inicializar resultado como cadena vacía
        r.inicializado = 1;
        r.v.s = strdup("");

        // Unir las expresiones con el delimitador
        for (int i = 0; i < lista->num_hijos; i++) {
            // Evaluar la expresión actual
            ValorSimbolo elem = evaluarExpr(lista->hijos[i]);
            if (!elem.inicializado) {
                char msg[200];
                snprintf(msg, sizeof(msg), "Elemento no inicializado en String.join");
                addError(msg, ambitoActual(), e->linea, e->columna);
                printf("[ERROR] %s\n", msg);
                if (r.v.s) free(r.v.s);
                r.inicializado = 0;
                return r;
            }

            // Convertir el elemento a string si no lo es
            char* elemStr;
            if (elem.tipo == TIPO_STRING && elem.v.s) {
                elemStr = strdup(elem.v.s);
            } else {
                // Convertir otros tipos a string
                char buffer[256];
                switch (elem.tipo) {
                    case TIPO_INT:
                        snprintf(buffer, sizeof(buffer), "%d", elem.v.i);
                        break;
                    case TIPO_FLOAT:
                        snprintf(buffer, sizeof(buffer), "%f", elem.v.f);
                        break;
                    case TIPO_DOUBLE:
                        snprintf(buffer, sizeof(buffer), "%f", elem.v.d);
                        break;
                    case TIPO_CHAR:
                        snprintf(buffer, sizeof(buffer), "%c", elem.v.c);
                        break;
                    case TIPO_BOOLEAN:
                        snprintf(buffer, sizeof(buffer), "%s", elem.v.b ? "true" : "false");
                        break;
                    default:
                        buffer[0] = '\0';
                        break;
                }
                elemStr = strdup(buffer);
            }

            if (i > 0 && delimitador.v.s) {
                // Usar concat3 para unir: resultado_actual + delimitador + nuevo_elemento
                char* temp = r.v.s;
                r.v.s = concat3(temp, delimitador.v.s, elemStr);
                free(temp);
            } else {
                // Para el primer elemento, simplemente asignar
                if (r.v.s) free(r.v.s);
                r.v.s = strdup(elemStr);
            }
            
            free(elemStr);
        }

        //printf("[DEBUG evaluarParseJoin] Resultado lista normal: '%s'\n", r.v.s ? r.v.s : "(null)");
        return r;
    }
    
    return r;
}

ValorSimbolo evaluarValueOf(NodoAST* e) {
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    r.tipo = TIPO_STRING;
    r.inicializado = 0;

    // Verificar que haya al menos un hijo
    if (e->num_hijos < 1) {
        printf("[ERROR] String.valueOf requiere un argumento\n");
        return r;
    }

    // Evaluar el argumento
    ValorSimbolo argVal = evaluarExpr(e->hijos[0]);

    // Verificar que el argumento esté inicializado
    if (!argVal.inicializado) {
        char msg[200];
        snprintf(msg, sizeof(msg), "Argumento de String.valueOf no inicializado");
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }

    // Convertir el argumento a cadena según su tipo
    r.tipo = TIPO_STRING;
    r.inicializado = 1;
    char buffer[256]; // Tamaño del buffer para la conversión

    switch (argVal.tipo) {
        case TIPO_INT:
            snprintf(buffer, sizeof(buffer), "%d", argVal.v.i);
            break;
        case TIPO_FLOAT:
            snprintf(buffer, sizeof(buffer), "%f", argVal.v.f);
            break;
        case TIPO_DOUBLE:
            snprintf(buffer, sizeof(buffer), "%f", argVal.v.d);
            break;
        case TIPO_CHAR:
            if (isprint((unsigned char)argVal.v.c)) {
                snprintf(buffer, sizeof(buffer), "%c", argVal.v.c);
            } else {
                snprintf(buffer, sizeof(buffer), "\\u%04x", (unsigned char)argVal.v.c);
            }
            break;
        case TIPO_BOOLEAN:
            snprintf(buffer, sizeof(buffer), "%s", argVal.v.b ? "true" : "false");
            break;
        case TIPO_STRING:
            // Si ya es una cadena, simplemente duplicarla
            r.v.s = strdup(argVal.v.s ? argVal.v.s : "");
            return r;
        default:
            // Tipo no soportado
            snprintf(buffer, sizeof(buffer), "[ERROR: Tipo no soportado]");
            printf("[ERROR] String.valueOf no soporta el tipo '%d'\n", argVal.tipo);
            addError(buffer, ambitoActual(), e->linea, e->columna);
            break;
    }

    // Asignar la cadena resultante
    r.v.s = strdup(buffer);
    return r;
}

// En evaluarString.c - Reforzar la función evaluarEqual
ValorSimbolo evaluarEqual(NodoAST* e) {
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    r.tipo = TIPO_BOOLEAN;
    r.inicializado = 0;

    // Validaciones más rigurosas
    if (!e || e->tipo != AST_EQUAL || strcmp(e->valor, ".equals") != 0 || e->num_hijos != 2) {
        printf("[ERROR] Nodo inválido para .equals (tipo=%d, valor=%s, hijos=%d)\n", 
               e ? e->tipo : -0, e ? (e->valor ? e->valor : "NULL") : "NULL", e ? e->num_hijos : 0);
        return r;
    }

    // Evaluar los operandos
    ValorSimbolo base = evaluarExpr(e->hijos[0]);
    ValorSimbolo arg = evaluarExpr(e->hijos[1]);

    // Verificar inicialización
    if (!base.inicializado || !arg.inicializado) {
        printf("[ERROR] Operandos de .equals no inicializados (base=%d, arg=%d)\n", 
               base.inicializado, arg.inicializado);
        return r;
    }

    // Hacer más flexibles las comparaciones
    // Si ambos son strings, comparamos normalmente
    if (base.tipo == TIPO_STRING && arg.tipo == TIPO_STRING) {
        r.inicializado = 1;
        r.v.b = (base.v.s && arg.v.s) ? strcmp(base.v.s, arg.v.s) == 0 : (base.v.s == arg.v.s);
        printf("[DEBUG] .equals: '%s' == '%s' = %s\n", 
               base.v.s ? base.v.s : "null", arg.v.s ? arg.v.s : "null", r.v.b ? "true" : "false");
    }
    // Si alguno no es string, intentamos la conversión
    else {
        // Convertir a strings para comparar (similar a Java)
        char baseStr[256] = {0};
        char argStr[256] = {0};
        
        // Convertir base a string
        if (base.tipo == TIPO_STRING && base.v.s) {
            strncpy(baseStr, base.v.s, sizeof(baseStr)-1);
        } else {
            // Usar el mismo código de la función valueOf para convertir
            switch (base.tipo) {
                case TIPO_INT:      snprintf(baseStr, sizeof(baseStr), "%d", base.v.i); break;
                case TIPO_FLOAT:    snprintf(baseStr, sizeof(baseStr), "%f", base.v.f); break;
                case TIPO_DOUBLE:   snprintf(baseStr, sizeof(baseStr), "%f", base.v.d); break;
                case TIPO_CHAR:     snprintf(baseStr, sizeof(baseStr), "%c", base.v.c); break;
                case TIPO_BOOLEAN:  snprintf(baseStr, sizeof(baseStr), "%s", base.v.b ? "true" : "false"); break;
                default:            baseStr[0] = '\0'; break;
            }
        }
        
        // Convertir arg a string
        if (arg.tipo == TIPO_STRING && arg.v.s) {
            strncpy(argStr, arg.v.s, sizeof(argStr)-1);
        } else {
            switch (arg.tipo) {
                case TIPO_INT:      snprintf(argStr, sizeof(argStr), "%d", arg.v.i); break;
                case TIPO_FLOAT:    snprintf(argStr, sizeof(argStr), "%f", arg.v.f); break;
                case TIPO_DOUBLE:   snprintf(argStr, sizeof(argStr), "%f", arg.v.d); break;
                case TIPO_CHAR:     snprintf(argStr, sizeof(argStr), "%c", arg.v.c); break;
                case TIPO_BOOLEAN:  snprintf(argStr, sizeof(argStr), "%s", arg.v.b ? "true" : "false"); break;
                default:            argStr[0] = '\0'; break;
            }
        }
        
        r.inicializado = 1;
        r.v.b = (strcmp(baseStr, argStr) == 0);
        printf("[DEBUG] .equals (convertido): '%s' == '%s' = %s\n", 
               baseStr, argStr, r.v.b ? "true" : "false");
    }

    return r;
}

