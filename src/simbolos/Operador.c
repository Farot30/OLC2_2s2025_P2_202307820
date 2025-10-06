#include "Evaluador.h"
#include <string.h>
#include <stdio.h>
#include "../semantico/tipos.h"
#include "../semantico/semantico.h"

char* valorToString(ValorSimbolo val) {
    char* result = malloc(256); // Aumentar tamaño del buffer
    if (!result) return NULL;

    if (!val.inicializado) {
        strcpy(result, "");
        return result;
    }

    switch (val.tipo) {
        case TIPO_INT:
            snprintf(result, 256, "%d", val.v.i);
            break;
        case TIPO_FLOAT:
            snprintf(result, 256, "%.6f", val.v.f);
            break;
        case TIPO_DOUBLE:
            snprintf(result, 256, "%.6lf", val.v.d);
            break;
        case TIPO_CHAR:
            snprintf(result, 256, "%c", val.v.c);
            break;
        case TIPO_BOOLEAN:
            snprintf(result, 256, "%s", val.v.b ? "true" : "false");
            break;
        case TIPO_STRING:
            free(result); // Liberar el buffer inicial
            if (val.v.s) {
                return strdup(val.v.s); // Retornar copia del string
            } else {
                return strdup(""); // String vacío si es NULL
            }
            break;
        default:
            strcpy(result, "");
            break;
    }
    return result;
}
double convertirANumero(ValorSimbolo v) {
    if (v.tipo == TIPO_INT) return v.v.i;
    if (v.tipo == TIPO_FLOAT) return v.v.f;
    if (v.tipo == TIPO_DOUBLE) return v.v.d;
    if (v.tipo == TIPO_CHAR) return (unsigned char)v.v.c;
    return 0;  // Si no es numérico, devolver 0
}

// En Operador.c, reemplaza la función evaluarOperacion con esta versión corregida:

ValorSimbolo evaluarOperacion(NodoAST* e, ValorSimbolo a, ValorSimbolo b) {
    printf("[DEBUG OPERADOR] === INICIANDO evaluarOperacion ===\n");
    printf("[DEBUG OPERADOR] Operador: '%s'\n", e->valor);
    printf("[DEBUG OPERADOR] Tipo a: %d, Tipo b: %d\n", a.tipo, b.tipo);

    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    r.tipo = TIPO_DESCONOCIDO;

    // Verificar que ambos operandos estén inicializados
    if (!a.inicializado || !b.inicializado) {
        printf("[DEBUG OPERADOR] ERROR: Uno de los operandos no está inicializado\n");
        return r;
    }

    // Operador suma: manejar tanto aritmética como concatenación de strings
    if (strcmp(e->valor, "+") == 0) {
        // Si uno de los operandos es string, hacer concatenación
        if (a.tipo == TIPO_STRING || b.tipo == TIPO_STRING) {
            r.tipo = TIPO_STRING;
            r.inicializado = 1;
            
            // Convertir ambos operandos a string
            char* str_a = valorToString(a);
            char* str_b = valorToString(b);
            
            if (str_a && str_b) {
                // Calcular tamaño necesario y concatenar
                size_t len = strlen(str_a) + strlen(str_b) + 1;
                r.v.s = malloc(len);
                if (r.v.s) {
                    strcpy(r.v.s, str_a);
                    strcat(r.v.s, str_b);
                } else {
                    r.v.s = strdup(""); // En caso de error de memoria
                }
            } else {
                r.v.s = strdup(""); // En caso de error
            }
            
            // Liberar strings temporales
            if (str_a) free(str_a);
            if (str_b) free(str_b);
            
            printf("[DEBUG OPERADOR] Concatenación realizada: '%s'\n", r.v.s);
            return r;
        }
        // Si ambos son numéricos, hacer suma aritmética
        else if (esNum(a.tipo) && esNum(b.tipo)) {
            double va = convertirANumero(a);
            double vb = convertirANumero(b);
            double resultado = va + vb;

            r.tipo = (a.tipo == TIPO_DOUBLE || b.tipo == TIPO_DOUBLE) ? TIPO_DOUBLE :
                     (a.tipo == TIPO_FLOAT || b.tipo == TIPO_FLOAT) ? TIPO_FLOAT : TIPO_INT;
            r.inicializado = 1;

            if (r.tipo == TIPO_DOUBLE) r.v.d = resultado;
            else if (r.tipo == TIPO_FLOAT) r.v.f = (float)resultado;
            else r.v.i = (int)resultado;

            printf("[DEBUG OPERADOR] Suma aritmética realizada: %f\n", resultado);
            return r;
        }
        else {
            printf("[EVALUADOR] Operación '+' con tipos incompatibles: %d + %d\n", a.tipo, b.tipo);
            return r;
        }
    }

    // Comparaciones numéricas
    if (strcmp(e->valor, ">") == 0 || strcmp(e->valor, "<") == 0 ||
        strcmp(e->valor, ">=") == 0 || strcmp(e->valor, "<=") == 0) {
        if (esNum(a.tipo) && esNum(b.tipo)) {
            double va = convertirANumero(a);
            double vb = convertirANumero(b);
            r.tipo = TIPO_BOOLEAN;
            r.inicializado = 1;

            if (strcmp(e->valor, ">") == 0) r.v.b = (va > vb);
            else if (strcmp(e->valor, "<") == 0) r.v.b = (va < vb);
            else if (strcmp(e->valor, ">=") == 0) r.v.b = (va >= vb);
            else if (strcmp(e->valor, "<=") == 0) r.v.b = (va <= vb);

            return r;
        } else {
            printf("[EVALUADOR] Comparación '%s' solo válida para tipos numéricos\n", e->valor);
            return r;
        }
    }

    // Comparaciones de igualdad
    if (strcmp(e->valor, "==") == 0 || strcmp(e->valor, "!=") == 0) {
        r.tipo = TIPO_BOOLEAN;
        r.inicializado = 1;

        if (a.tipo == b.tipo) {
            switch (a.tipo) {
                case TIPO_INT:
                    r.v.b = (strcmp(e->valor, "==") == 0) ? (a.v.i == b.v.i) : (a.v.i != b.v.i);
                    break;
                case TIPO_FLOAT:
                    r.v.b = (strcmp(e->valor, "==") == 0) ? (a.v.f == b.v.f) : (a.v.f != b.v.f);
                    break;
                case TIPO_DOUBLE:
                    r.v.b = (strcmp(e->valor, "==") == 0) ? (a.v.d == b.v.d) : (a.v.d != b.v.d);
                    break;
                case TIPO_CHAR:
                    r.v.b = (strcmp(e->valor, "==") == 0) ? (a.v.c == b.v.c) : (a.v.c != b.v.c);
                    break;
                case TIPO_BOOLEAN:
                    r.v.b = (strcmp(e->valor, "==") == 0) ? (a.v.b == b.v.b) : (a.v.b != b.v.b);
                    break;
                case TIPO_STRING:
                    if (a.v.s && b.v.s) {
                        r.v.b = (strcmp(e->valor, "==") == 0) ? (strcmp(a.v.s, b.v.s) == 0) : (strcmp(a.v.s, b.v.s) != 0);
                    } else {
                        r.v.b = (a.v.s == b.v.s); // Ambos NULL o no
                    }
                    break;
                default:
                    r.v.b = 0;
                    break;
            }
        } else if (esNum(a.tipo) && esNum(b.tipo)) {
            double va = convertirANumero(a);
            double vb = convertirANumero(b);
            r.v.b = (strcmp(e->valor, "==") == 0) ? (va == vb) : (va != vb);
        } else {
            printf("[EVALUADOR] Comparación '%s' con tipos incompatibles: %d vs %d\n", e->valor, a.tipo, b.tipo);
            r.v.b = 0;
        }
        return r;
    }

    // Operaciones lógicas
    if (strcmp(e->valor, "&&") == 0 || strcmp(e->valor, "||") == 0) {
        if (a.tipo == TIPO_BOOLEAN && b.tipo == TIPO_BOOLEAN) {
            r.tipo = TIPO_BOOLEAN;
            r.inicializado = 1;
            r.v.b = (strcmp(e->valor, "&&") == 0) ? (a.v.b && b.v.b) : (a.v.b || b.v.b);
            return r;
        } else {
            printf("[EVALUADOR] Operador '%s' solo válido para boolean\n", e->valor);
            return r;
        }
    }

    // Otras operaciones aritméticas (-, *, /, %)
    if (strcmp(e->valor, "-") == 0 || strcmp(e->valor, "*") == 0 ||
        strcmp(e->valor, "/") == 0 || strcmp(e->valor, "%") == 0) {
        if (esNum(a.tipo) && esNum(b.tipo)) {
            double va = convertirANumero(a);
            double vb = convertirANumero(b);
            double res = 0;

            if (strcmp(e->valor, "-") == 0) res = va - vb;
            else if (strcmp(e->valor, "*") == 0) res = va * vb;
            else if (strcmp(e->valor, "/") == 0) res = (vb != 0) ? va / vb : 0;
            else if (strcmp(e->valor, "%") == 0) res = (long)va % (long)vb;

            r.tipo = (a.tipo == TIPO_DOUBLE || b.tipo == TIPO_DOUBLE) ? TIPO_DOUBLE :
                     (a.tipo == TIPO_FLOAT || b.tipo == TIPO_FLOAT) ? TIPO_FLOAT : TIPO_INT;
            r.inicializado = 1;

            if (r.tipo == TIPO_DOUBLE) r.v.d = res;
            else if (r.tipo == TIPO_FLOAT) r.v.f = res;
            else r.v.i = (int)res;

            return r;
        } else {
            printf("[EVALUADOR] Operación '%s' solo válida para tipos numéricos\n", e->valor);
            return r;
        }
    }
    if(strcmp(e->valor, ">>") == 0) {
        if ((a.tipo == TIPO_INT || a.tipo == TIPO_CHAR) && 
            (b.tipo == TIPO_INT || b.tipo == TIPO_CHAR)) {
            int valorA = (a.tipo == TIPO_CHAR) ? (int)a.v.c : a.v.i;
            int valorB = (b.tipo == TIPO_CHAR) ? (int)b.v.c : b.v.i;
            r.tipo = TIPO_INT;
            r.inicializado = 1;
            r.v.i = valorA >> valorB;
            return r;
        } else {
            printf("[EVALUADOR] Operador '>>' solo válido para enteros y char\n");
            return r;
        }
    }



    if (strcmp(e->valor, "<<") == 0) {
        if ((a.tipo == TIPO_INT || a.tipo == TIPO_CHAR) && 
            (b.tipo == TIPO_INT || b.tipo == TIPO_CHAR)) {
            int valorA = (a.tipo == TIPO_CHAR) ? (int)a.v.c : a.v.i;
            int valorB = (b.tipo == TIPO_CHAR) ? (int)b.v.c : b.v.i;
            r.tipo = TIPO_INT;
            r.inicializado = 1;
            r.v.i = valorA << valorB;
            return r;
        } else {
            printf("[EVALUADOR] Operador '<<' solo válido para enteros y char\n");
            return r;
        }
    }

    // Operador XOR (^)
    if (strcmp(e->valor, "^") == 0) {
        if ((a.tipo == TIPO_INT || a.tipo == TIPO_CHAR) && 
            (b.tipo == TIPO_INT || b.tipo == TIPO_CHAR)) {
            int valorA = (a.tipo == TIPO_CHAR) ? (int)a.v.c : a.v.i;
            int valorB = (b.tipo == TIPO_CHAR) ? (int)b.v.c : b.v.i;
            r.tipo = TIPO_INT;
            r.inicializado = 1;
            r.v.i = valorA ^ valorB;
            return r;
        } else {
            printf("[EVALUADOR] Operador '^' solo válido para enteros y char\n");
            return r;
        }
    }

    // Operador OR (|)
    if (strcmp(e->valor, "|") == 0) {
        if ((a.tipo == TIPO_INT || a.tipo == TIPO_CHAR) && 
            (b.tipo == TIPO_INT || b.tipo == TIPO_CHAR)) {
            int valorA = (a.tipo == TIPO_CHAR) ? (int)a.v.c : a.v.i;
            int valorB = (b.tipo == TIPO_CHAR) ? (int)b.v.c : b.v.i;
            r.tipo = TIPO_INT;
            r.inicializado = 1;
            r.v.i = valorA | valorB;
            return r;
        } else {
            printf("[EVALUADOR] Operador '|' solo válido para enteros y char\n");
            return r;
        }
    }

    // Operador AND (&)
    if (strcmp(e->valor, "&") == 0) {
        if ((a.tipo == TIPO_INT || a.tipo == TIPO_CHAR) && 
            (b.tipo == TIPO_INT || b.tipo == TIPO_CHAR)) {
            int valorA = (a.tipo == TIPO_CHAR) ? (int)a.v.c : a.v.i;
            int valorB = (b.tipo == TIPO_CHAR) ? (int)b.v.c : b.v.i;
            r.tipo = TIPO_INT;
            r.inicializado = 1;
            r.v.i = valorA & valorB;
            return r;
        } else {
            printf("[EVALUADOR] Operador '&' solo válido para enteros y char\n");
            return r;
        }
    }

    printf("[EVALUADOR] Operador '%s' no reconocido\n", e->valor);
    return r;
}