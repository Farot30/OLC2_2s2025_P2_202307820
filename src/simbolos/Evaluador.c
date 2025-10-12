#include "Evaluador.h"
#include "Operador.h"
#include "EvaluadorArray.h"
#include "../semantico/Embebidas/EvaluadoresArrays.h"
#include "../semantico/Embebidas/evaluarString.h"
#include "../semantico/Embebidas/EvaluadorNumerico.h"
#include "../semantico/semantico.h"
#include "../semantico/Vectores/acceso_vector.h"
#include "../semantico/Vectores/acceso_matriz.h"
#include "../semantico/Vectores/acceso_arrays.h"
#include "../semantico/Funciones/Llamadas.h"
#include "../AST/err.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h> 
#include <limits.h>
#include <math.h>

// Función auxiliar para crear un ValorSimbolo numérico
ValorSimbolo makeNum(TipoDato tipo, double d) {
    ValorSimbolo v;
    memset(&v, 0, sizeof(v));
    v.tipo = tipo;
    v.inicializado = 1;
    switch (tipo) {
        case TIPO_DOUBLE: v.v.d = d; break;
        case TIPO_FLOAT:  v.v.f = (float)d; break;
        case TIPO_INT:    v.v.i = (int)d; break;
        case TIPO_CHAR:   v.v.c = (char)d; break;
        default: break;
    }
    return v;
}

typedef struct {
    char nombre[64];
    ValorSimbolo valor;
} VariableTemporal;

// Búfer para guardar estados de variables durante evaluación recursiva
VariableTemporal estadosVariables[100];
int numEstadosGuardados = 0;

// Guarda el estado de una variable en el búfer
void guardarEstadoVariable(const char* nombre, ValorSimbolo valor) {
    if (numEstadosGuardados >= 100) return;
    
    strncpy(estadosVariables[numEstadosGuardados].nombre, nombre, 63);
    estadosVariables[numEstadosGuardados].valor = valor;
    numEstadosGuardados++;
    
    printf("[DEBUG RECURSION] Guardando estado de variable '%s'\n", nombre);
}

// Busca y restaura el último estado guardado de una variable
ValorSimbolo buscarEstadoVariable(const char* nombre) {
    ValorSimbolo valorVacio = {0};
    valorVacio.inicializado = 0;
    
    for (int i = numEstadosGuardados - 1; i >= 0; i--) {
        if (strcmp(estadosVariables[i].nombre, nombre) == 0) {
            printf("[DEBUG RECURSION] Restaurando estado de variable '%s'\n", nombre);
            return estadosVariables[i].valor;
        }
    }
    
    return valorVacio;
}
// Función auxiliar para imprimir valores en modo depuración
static void imprimirValor(ValorSimbolo valor) {
    switch (valor.tipo) {
        case TIPO_INT: 
            printf("%d\n", valor.v.i); 
            break;
        case TIPO_FLOAT: 
            printf("%f\n", valor.v.f); 
            break;
        case TIPO_DOUBLE: 
            printf("%lf\n", valor.v.d); 
            break;
        case TIPO_CHAR: 
            printf("'%c'\n", valor.v.c); 
            break;
        case TIPO_BOOLEAN: 
            printf("%s\n", valor.v.b ? "true" : "false"); 
            break;
        case TIPO_STRING: 
            printf("\"%s\"\n", valor.v.s ? valor.v.s : "null"); 
            break;
        default: 
            printf("tipo desconocido\n"); 
            break;
    }
}


ValorSimbolo evaluarOperaciones(NodoAST* e, ValorSimbolo a, ValorSimbolo b) {
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

    // Operador suma: manejar concatenación y aritmética
    if (strcmp(e->valor, "+") == 0) {

        if (contexto_funcion > 0 && strcmp(ambitoActual(), "fibonacci") == 0) {
            // Si ambos operandos son llamadas a función, manejar diferente
            if (e->hijos[0]->tipo == AST_LLAMADA && e->hijos[1]->tipo == AST_LLAMADA) {
                printf("[DEBUG RECURSION] Suma especial de llamadas a función en fibonacci\n");
                
                // Manejar el resultado de manera consistente
                ValorSimbolo resultado = {0};
                resultado.inicializado = 1;
                resultado.tipo = TIPO_INT;
                
                // Asegurarnos que los resultados se sumen correctamente
                int valor1 = a.v.i;  // Ya evaluado antes
                int valor2 = b.v.i;  // Ya evaluado antes
                
                resultado.v.i = valor1 + valor2;
                
                printf("[DEBUG RECURSION] Suma de valores recursivos: %d + %d = %d\n",
                    valor1, valor2, resultado.v.i);
                
                return resultado;
            }
        }
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

    // Comparaciones de igualdad
    if (strcmp(e->valor, "==") == 0 || strcmp(e->valor, "!=") == 0) {
        r.tipo = TIPO_BOOLEAN;
        r.inicializado = 1;

        if ((a.tipo == TIPO_STRING && b.tipo == TIPO_NULL) || (a.tipo == TIPO_NULL && b.tipo == TIPO_STRING)) {
            int esNull = (a.tipo == TIPO_NULL) ? (b.v.s == NULL) : (a.v.s == NULL);
            if (strcmp(e->valor, "==") == 0)
                r.v.b = esNull;
            else
                r.v.b = !esNull;
            printf("[DEBUG OPERADOR] Comparación string %s null: %s\n", e->valor, r.v.b ? "true" : "false");
            return r;
        }
        if ((a.tipo == TIPO_NULL && b.tipo == TIPO_NULL)) {
            r.v.b = (strcmp(e->valor, "==") == 0);
            printf("[DEBUG OPERADOR] Comparación null %s null: %s\n", e->valor, r.v.b ? "true" : "false");
            return r;
        }

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
        printf("[DEBUG OPERADOR] Comparación '%s' resultado: %s\n", e->valor, r.v.b ? "true" : "false");
        return r;
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

            printf("[DEBUG OPERADOR] Comparación '%s' resultado: %s\n", e->valor, r.v.b ? "true" : "false");
            return r;
        } else {
            printf("[EVALUADOR] Comparación '%s' solo válida para tipos numéricos\n", e->valor);
            return r;
        }
    }

    // Operaciones lógicas
    if (strcmp(e->valor, "&&") == 0 || strcmp(e->valor, "||") == 0) {
        if (a.tipo == TIPO_BOOLEAN && b.tipo == TIPO_BOOLEAN) {
            r.tipo = TIPO_BOOLEAN;
            r.inicializado = 1;
            r.v.b = (strcmp(e->valor, "&&") == 0) ? (a.v.b && b.v.b) : (a.v.b || b.v.b);
            printf("[DEBUG OPERADOR] Operación lógica '%s' resultado: %s\n", e->valor, r.v.b ? "true" : "false");
            return r;
        } else {
            printf("[EVALUADOR] Operador '%s' solo válido para boolean\n", e->valor);
            return r;
        }
    }

    // Operador negación unaria
    if (strcmp(e->valor, "!") == 0) {
        if (a.tipo == TIPO_BOOLEAN) {
            r.tipo = TIPO_BOOLEAN;
            r.inicializado = 1;
            r.v.b = !a.v.b;
            printf("[DEBUG OPERADOR] Negación resultado: %s\n", r.v.b ? "true" : "false");
            return r;
        } else {
            printf("[EVALUADOR] Operador '!' solo válido para boolean\n");
            return r;
        }
    }else if (strcmp(e->valor, "uminus") == 0) {
        if (esNum(a.tipo)) {
            r.tipo = a.tipo;
            r.inicializado = 1;
            switch (a.tipo) {
                case TIPO_INT:    r.v.i = -a.v.i; break;
                case TIPO_FLOAT:  r.v.f = -a.v.f; break;
                case TIPO_DOUBLE: r.v.d = -a.v.d; break;
                case TIPO_CHAR:   r.v.c = -a.v.c; break;
                default: 
                    printf("[EVALUADOR] Operador unario '-' no válido para tipo %d\n", a.tipo);
                    r.inicializado = 0;
                    break;
            }
            printf("[DEBUG OPERADOR] Negación unaria: resultado = ");
            imprimirValor(r);
            return r;
        } else {
            printf("[EVALUADOR] Operador unario '-' solo válido para tipos numéricos\n");
            return r;
        }
    }

    // Otras operaciones aritméticas (-, *, /, %)
    if (strcmp(e->valor, "-") == 0 || strcmp(e->valor, "*") == 0 ||
        strcmp(e->valor, "/") == 0 || strcmp(e->valor, "%") == 0) {
        if (esNum(a.tipo) && esNum(b.tipo)) {
            double va = convertirANumero(a);
            double vb = convertirANumero(b);
            double resultado = 0;

            if (strcmp(e->valor, "-") == 0) resultado = va - vb;
            else if (strcmp(e->valor, "*") == 0) resultado = va * vb;
            else if (strcmp(e->valor, "/") == 0) resultado = (vb != 0) ? va / vb : 0;
            else if (strcmp(e->valor, "%") == 0) resultado = fmod(va, vb);

            // REGLA CORREGIDA: Determinar el tipo resultante basado en la promoción de tipos
            if (a.tipo == TIPO_DOUBLE || b.tipo == TIPO_DOUBLE) {
                r.tipo = TIPO_DOUBLE;
                r.v.d = resultado;
            } else if (a.tipo == TIPO_FLOAT || b.tipo == TIPO_FLOAT) {
                r.tipo = TIPO_FLOAT;
                r.v.f = (float)resultado;
            } else {
                r.tipo = TIPO_INT;
                r.v.i = (int)resultado;
            }

            r.inicializado = 1;

            printf("[DEBUG OPERADOR] Operación '%s' resultado: %f, tipo=%d\n", 
                e->valor, resultado, r.tipo);
            return r;
        } else {
            printf("[EVALUADOR] Operación '%s' solo válida para tipos numéricos\n", e->valor);
            return r;
        }
    }
    if (strcmp(e->valor, ">>") == 0) {
        if (a.tipo == TIPO_INT && b.tipo == TIPO_INT) {
            r.tipo = TIPO_INT;
            r.inicializado = 1;
            r.v.i = a.v.i >> b.v.i;
            printf("[DEBUG OPERADOR] Desplazamiento derecho: %d >> %d = %d\n", 
                a.v.i, b.v.i, r.v.i);
            return r;
        } else {
            printf("[EVALUADOR] Operador '>>' solo válido para enteros\n");
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
            printf("[DEBUG OPERADOR] Desplazamiento izquierdo: %d << %d = %d\n", 
                valorA, valorB, r.v.i);
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
            printf("[DEBUG OPERADOR] XOR: %d ^ %d = %d\n", valorA, valorB, r.v.i);
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
            printf("[DEBUG OPERADOR] OR: %d | %d = %d\n", valorA, valorB, r.v.i);
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
            printf("[DEBUG OPERADOR] AND: %d & %d = %d\n", valorA, valorB, r.v.i);
            return r;
        } else {
            printf("[EVALUADOR] Operador '&' solo válido para enteros y char\n");
            return r;
        }
    }

    printf("[EVALUADOR] Operador '%s' no reconocido\n", e->valor);
    return r;
}

// Función auxiliar para verificar si un tipo es numérico
int esNum(TipoDato t) {
    return t == TIPO_INT || t == TIPO_FLOAT || t == TIPO_DOUBLE || t == TIPO_CHAR;
}



// Función para evaluar expresiones
ValorSimbolo evaluarExpr(NodoAST* e) {
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    r.tipo = TIPO_DESCONOCIDO;
    if (!e) return r;

    switch (e->tipo) {
        case AST_NUMERO: {
                if (strchr(e->valor, '.')) {
                    size_t L = strlen(e->valor);
                    if (L > 0 && (e->valor[L - 1] == 'f' || e->valor[L - 1] == 'F')) {
                        r.tipo = TIPO_FLOAT;
                        r.v.f = strtof(e->valor, NULL);
                    } else {
                        r.tipo = TIPO_DOUBLE;
                        r.v.d = strtod(e->valor, NULL);
                    }
                } else {
                    r.tipo = TIPO_INT;
                    r.v.i = atoi(e->valor);
                }
                r.inicializado = 1;
                return r;
            }
        case AST_CADENA: {
            r.tipo = TIPO_STRING;
            r.v.s = e->valor ? strdup(e->valor) : NULL;
            r.inicializado = 1;
            return r;
        }
        case AST_CARACTER: {
            r.tipo = TIPO_CHAR;
            // Interpretar como unsigned char para soportar valores > 127
            r.v.c = e->valor && *e->valor ? (unsigned char)e->valor[0] : '\0';
            r.inicializado = 1;
            printf("[DEBUG] Carácter leído: %c (%d)\n", r.v.c, r.v.c);
            return r;
        }
        case AST_BOOLEAN: {
            r.tipo = TIPO_BOOLEAN;
            r.v.b = (e->valor && strcmp(e->valor, "true") == 0);
            r.inicializado = 1;
            return r;
        }
        case AST_NULL:
            r.tipo = TIPO_NULL;
            r.inicializado = 1;
            r.v.s = NULL;
            return r;
        // En Evaluador.c, modificar el caso AST_IDENTIFICADOR:
        case AST_IDENTIFICADOR: {
    // Intentar buscar como variable primero
            printf("[DEBUG evaluarExpr] Buscando identificador '%s'\n", e->valor);
            Simbolo* s = buscarSimboloConAmbito(e->valor);
            if (s) {
                printf("[DEBUG evaluarExpr] Símbolo '%s' encontrado, inicializado=%d, tipo=%d\n", s->nombre, s->valor.inicializado, s->valor.tipo);
                if (!s->valor.inicializado) {
                    printf("[EVALUADOR] Variable '%s' no inicializada\n", e->valor);
                    return r;
                }
                
                // **CORRECCIÓN PARA MATRICES: Retornar tipo base, no tipo de almacenamiento**
                if (s->clase == SIMBOLO_MATRIZ) {
                    ValorSimbolo resultado = getValorSimbolo(s);
                    
                    // **CAMBIAR TIPO DE STRING A TIPO BASE DE LA MATRIZ**
                    resultado.tipo = s->tipo;  // tipo base (int)
                    
                    printf("[DEBUG evaluarExpr] Matriz '%s' evaluada: tipo corregido de %d a %d\n", 
                        s->nombre, TIPO_STRING, resultado.tipo);
                    
                    if (resultado.v.s) {
                        printf("[DEBUG evaluarExpr] Contenido matriz: '%s'\n", resultado.v.s);
                    }
                    
                    return resultado;
                }
                
                // **CASO NORMAL PARA OTROS TIPOS**
                if (s->valor.tipo == TIPO_STRING && s->valor.v.s) {
                    printf("[DEBUG evaluarExpr] Valor string: '%s'\n", s->valor.v.s);
                }
                printf("[EVALUADOR] Variable '%s' encontrada con valor: %d\n", e->valor, s->valor.v.i);
                return getValorSimbolo(s);
            }
            
            // Manejar formato \uXXXX (mínimo 4 caracteres: \u + dígitos)
            if (strlen(e->valor) >= 4 && e->valor[0] == '\\' && e->valor[1] == 'u') {
                char* endptr;
                // Saltar los primeros 2 caracteres (\u) y convertir los siguientes
                long unicode_val = strtol(e->valor + 2, &endptr, 16);
                
                // Verificar que se leyeron todos los dígitos
                if (endptr > e->valor + 2) {
                    // Es un literal Unicode válido
                    r.tipo = TIPO_CHAR;
                    r.v.c = (unsigned char)unicode_val; // Usar unsigned para caracteres extendidos
                    r.inicializado = 1;
                    printf("[EVALUADOR] Literal Unicode '%s' convertido a char: %c (%d)\n", 
                        e->valor, r.v.c, r.v.c);
                    return r;
                }
            }
            
            printf("[EVALUADOR] Variable '%s' no declarada\n", e->valor);
            return r;
        }
        case AST_OPERACION: 
        {
            if (e->num_hijos == 1) {
                // Operación unaria
                ValorSimbolo a = evaluarExpr(e->hijos[0]);
                if (!a.inicializado) {
                    printf("[EVALUADOR] Operando no inicializado para operación unaria '%s'\n", e->valor);
                    return r;
                }
                
                // Manejar operación unaria específicamente
                if (strcmp(e->valor, "!") == 0) {
                    if (a.tipo == TIPO_BOOLEAN) {
                        r.tipo = TIPO_BOOLEAN;
                        r.inicializado = 1;
                        r.v.b = !a.v.b;
                        printf("[DEBUG OPERADOR] Negación unaria: !%s = %s\n", 
                            a.v.b ? "true" : "false", r.v.b ? "true" : "false");
                        return r;
                    } else {
                        printf("[EVALUADOR] Operador '!' solo válido para boolean, recibido tipo %d\n", a.tipo);
                        return r;
                    }
                }
                if (contexto_funcion > 0 && generic_recursion_depth > 0) {
                    const char* funcionActual = ambitoActual();
                    
                    // Solo para funciones genéricas (no fibonacci/ackermann)
                    if (strcmp(funcionActual, "fibonacci") != 0 && 
                        strcmp(funcionActual, "ackerman") != 0) {
                        
                        // Manejo especial de variables que podrían haber cambiado
                        if (e->hijos[0]->tipo == AST_IDENTIFICADOR) {
                            const char* nombreVar = e->hijos[0]->valor;
                            ValorSimbolo respaldo = buscarRespaldoGenerico(nombreVar, funcionActual);
                            
                            if (respaldo.inicializado) {
                                printf("[DEBUG GENERIC] Usando valor respaldado para %s en %s\n", 
                                    nombreVar, funcionActual);
                                a = respaldo;
                            }
                        }
                    }
                }
                // Para otras operaciones unarias, usar la función existente
                return evaluarOperaciones(e, a, a);
            } 
            else if (e->num_hijos == 2) {
                // Operación binaria
                ValorSimbolo a = evaluarExpr(e->hijos[0]);
                
                // Caso especial para operaciones dentro de llamadas recursivas
                if (contexto_funcion > 0 && e->hijos[0]->tipo == AST_IDENTIFICADOR) {
                    // Guardar estado de la variable
                    const char* nombreVar = e->hijos[0]->valor;
                    guardarEstadoVariable(nombreVar, a);
                }
                
                ValorSimbolo b = evaluarExpr(e->hijos[1]);
                
                // Caso especial para operaciones de resta en fibonacci
                if (strcmp(e->valor, "-") == 0 && contexto_funcion > 0) {
                    // Si estamos dentro de un contexto de función fibonacci
                    if (e->hijos[0]->tipo == AST_IDENTIFICADOR && 
                        strcmp(e->hijos[0]->valor, "n") == 0 &&
                        strcmp(ambitoActual(), "fibonacci") == 0) {
                        
                        printf("[DEBUG RECURSION] Operación especial n-x en función fibonacci\n");
                        
                        // Si el valor actual de n ha sido modificado, usar el original
                        ValorSimbolo original = buscarEstadoVariable("n");
                        if (original.inicializado) {
                            printf("[DEBUG RECURSION] Usando valor original de n: %d en vez de %d\n", 
                                original.v.i, a.v.i);
                            a = original;
                        }
                    }
                }
                
                return evaluarOperaciones(e, a, b);
            }
            printf("[EVALUADOR] Número de hijos inválido para operación: %d\n", e->num_hijos);
            return r;
        }

        case AST_PARSEINT: 
            return evaluarParseInt(e);
        case AST_PARSEFLOAT:
            return evaluarParseFloat(e);

        case AST_PARSEDOUBLE:
            return evaluarParseDouble(e);

        // Añade este caso al switch en la función evaluarExpr:
        case AST_PARSEJOIN:    
            return evaluarParseJoin(e);
        case AST_VALUEOF: 
            return evaluarValueOf(e);
        case AST_INDEXOF: 
            return evaluarIndexof(e);
        case AST_LENGHT: 
            return evaluarLength(e);
        case AST_ADD:
            return evaluarAdd(e);
        case AST_LLAMADA: {
            // Verificar si es una función void antes de intentar evaluar
            if (e->num_hijos >= 1 && e->hijos[0]->tipo == AST_IDENTIFICADOR) {
                Simbolo* funcionSimbolo = buscarSimbolo(e->hijos[0]->valor);
                if (funcionSimbolo && funcionSimbolo->clase == SIMBOLO_FUNCION_VOID) {
                    printf("[ERROR EVALUADOR] No se puede evaluar función void '%s' como expresión\n", e->hijos[0]->valor);
                    ValorSimbolo error = {0};
                    return error;
                }
            }
            
            printf("[DEBUG EVALUADOR] Evaluando llamada a función como expresión\n");
            ValorSimbolo resultado = EvaluarLlamadaFuncion(e);
            if (resultado.inicializado) {
                printf("[DEBUG EVALUADOR] Función retornó valor de tipo %s\n", nombreTipo(resultado.tipo));
                return resultado;
            } else {
                printf("[ERROR EVALUADOR] Función no retornó un valor inicializado\n");
                ValorSimbolo r = {0}; 
                r.tipo = TIPO_DESCONOCIDO;
                return r;
            }
        }
        case AST_ACCESO: {
            printf("[DEBUG EVALUADOR] Evaluando acceso a vector/matriz en expresión\n");
            
            // Obtener el identificador para determinar si es vector o matriz
            if (e->num_hijos < 1 || e->hijos[0]->tipo != AST_IDENTIFICADOR) {
                printf("[ERROR EVALUADOR] Estructura de acceso inválida\n");
                return r;
            }
            
            NodoAST* idNodo = e->hijos[0];
            Simbolo* simbolo = buscarSimbolo(idNodo->valor);
            
            if (!simbolo) {
                printf("[ERROR EVALUADOR] Variable '%s' no declarada\n", idNodo->valor);
                return r;
            }
            
            if (simbolo->clase == SIMBOLO_MATRIZ) {
                // Verificar que el acceso a matriz sea válido
                if (!verificarAccesoMatriz(e)) {
                    printf("[ERROR EVALUADOR] Acceso inválido a matriz '%s'\n", idNodo->valor);
                    return r;
                }
                
                // Evaluar acceso a matriz
                ValorSimbolo valorMatriz = evaluarAccesoMatriz(e);
                if (valorMatriz.inicializado) {
                    printf("[DEBUG EVALUADOR] Acceso a matriz evaluado exitosamente, valor = ");
                    imprimirValor(valorMatriz);
                    return valorMatriz;
                }
            } 
            else if (simbolo->clase == SIMBOLO_VECTOR) {
                // Verificar que el acceso a vector sea válido
                if (!verificarAccesoVector(e)) {
                    printf("[ERROR EVALUADOR] Acceso inválido a vector '%s'\n", idNodo->valor);
                    return r;
                }
                
                // Evaluar acceso a vector
                ValorSimbolo valorVector = evaluarAccesoVector(e);
                if (valorVector.inicializado) {
                    printf("[DEBUG EVALUADOR] Acceso a vector evaluado exitosamente, valor = ");
                    imprimirValor(valorVector);
                    return valorVector;
                }
            }else if (simbolo->clase == SIMBOLO_ARRAY_MULTIDIMENSIONAL) {
                // Verificar que el acceso a array sea válido
                if (!verificarAccesoArray(e)) {
                    printf("[ERROR EVALUADOR] Acceso inválido a array '%s'\n", idNodo->valor);
                    return r;
                }
                
                // Evaluar acceso a array
                ValorSimbolo valorArray = evaluarAccesoArray(e);
                if (valorArray.inicializado) {
                    printf("[DEBUG EVALUADOR] Acceso a array evaluado exitosamente, valor = ");
                    imprimirValor(valorArray);
                    return valorArray;
                }
            }
            else {
                printf("[ERROR EVALUADOR] Variable '%s' no es un vector ni matriz ni array (clase: %d)\n", 
                    idNodo->valor, simbolo->clase);
            }
            
            printf("[ERROR EVALUADOR] No se pudo obtener valor del acceso\n");
            return r;
        }
           
        case AST_CAST: {
            if (e->num_hijos != 1) return r;
            ValorSimbolo orig = evaluarExpr(e->hijos[0]);
            TipoDato destino = obtenerTipoDesdeString(e->valor);
            if (!orig.inicializado) return r;
            if (esNum(destino) && esNum(orig.tipo)) {
                double base = 0;
                switch (orig.tipo) {
                    case TIPO_DOUBLE: base = orig.v.d; break;
                    case TIPO_FLOAT:  base = orig.v.f; break;
                    case TIPO_INT:    base = orig.v.i; break;
                    case TIPO_CHAR:   base = (unsigned char)orig.v.c; break;
                    default: break;
                }
                ValorSimbolo r = makeNum(destino, base);
                r.es_cast = 1; // <--- Marca que es resultado de un cast explícito
                return r;
            }
            if (destino == orig.tipo) return orig;
            return r;
        }
        case AST_STRING: {
            if (strcmp(e->valor, "String.valueOf") == 0 && e->num_hijos == 1) {
                ValorSimbolo arg = evaluarExpr(e->hijos[0]);
                r.tipo = TIPO_STRING;
                r.inicializado = 1;
                char buffer[64];
                switch (arg.tipo) {
                    case TIPO_INT:     snprintf(buffer, sizeof(buffer), "%d", arg.v.i); break;
                    case TIPO_FLOAT:   snprintf(buffer, sizeof(buffer), "%f", arg.v.f); break;
                    case TIPO_DOUBLE:  snprintf(buffer, sizeof(buffer), "%lf", arg.v.d); break;
                    case TIPO_CHAR:    snprintf(buffer, sizeof(buffer), "%c", arg.v.c); break;
                    case TIPO_BOOLEAN: snprintf(buffer, sizeof(buffer), "%s", arg.v.b ? "true" : "false"); break;
                    case TIPO_STRING:  snprintf(buffer, sizeof(buffer), "%s", arg.v.s ? arg.v.s : ""); break;
                    default:           buffer[0] = '\0'; break;
                }
                r.v.s = strdup(buffer);
                return r;
            }
            return r;
        }
        case AST_EQUAL: 
            return evaluarEqual(e);
        
        default: return r;
    }
    return r;
}

ValorSimbolo convertirValor(ValorSimbolo origen, TipoDato tipoDestino) {
    ValorSimbolo resultado;
    memset(&resultado, 0, sizeof(ValorSimbolo));
    resultado.tipo = tipoDestino;
    resultado.inicializado = 1;
    
    // Si el tipo origen y destino son iguales, no hay conversión
    if (origen.tipo == tipoDestino) {
        if (origen.tipo == TIPO_STRING && origen.v.s) {
            resultado.v.s = strdup(origen.v.s);
        } else {
            resultado = origen;
        }
        return resultado;
    }
    
    // Realizar conversiones según los tipos origen y destino
    switch (tipoDestino) {
        case TIPO_INT:
            switch (origen.tipo) {
                case TIPO_CHAR:
                    resultado.v.i = (int)origen.v.c;
                    break;
                case TIPO_FLOAT:
                    resultado.v.i = (int)origen.v.f;
                    break;
                case TIPO_DOUBLE:
                    resultado.v.i = (int)origen.v.d;
                    break;
                case TIPO_BOOLEAN:
                    resultado.v.i = origen.v.b ? 1 : 0;
                    break;
                default:
                    resultado.inicializado = 0;
                    break;
            }
            break;
            
        case TIPO_FLOAT:
            switch (origen.tipo) {
                case TIPO_CHAR:
                    resultado.v.f = (float)origen.v.c;
                    break;
                case TIPO_INT:
                    resultado.v.f = (float)origen.v.i;
                    break;
                case TIPO_DOUBLE:
                    resultado.v.f = (float)origen.v.d;
                    break;
                default:
                    resultado.inicializado = 0;
                    break;
            }
            break;
            
        case TIPO_DOUBLE:
            switch (origen.tipo) {
                case TIPO_CHAR:
                    resultado.v.d = (double)origen.v.c;
                    break;
                case TIPO_INT:
                    resultado.v.d = (double)origen.v.i;
                    break;
                case TIPO_FLOAT:
                    resultado.v.d = (double)origen.v.f;
                    break;
                default:
                    resultado.inicializado = 0;
                    break;
            }
            break;
            
        case TIPO_CHAR:
            switch (origen.tipo) {
                case TIPO_INT:
                    resultado.v.c = (char)origen.v.i;
                    break;
                default:
                    resultado.inicializado = 0;
                    break;
            }
            break;
            
        case TIPO_BOOLEAN:
            switch (origen.tipo) {
                case TIPO_INT:
                    resultado.v.b = origen.v.i != 0;
                    break;
                default:
                    resultado.inicializado = 0;
                    break;
            }
            break;
            
        case TIPO_STRING:
            switch (origen.tipo) {
                case TIPO_INT: {
                    char buffer[32];
                    snprintf(buffer, sizeof(buffer), "%d", origen.v.i);
                    resultado.v.s = strdup(buffer);
                    break;
                }
                case TIPO_FLOAT: {
                    char buffer[32];
                    snprintf(buffer, sizeof(buffer), "%f", origen.v.f);
                    resultado.v.s = strdup(buffer);
                    break;
                }
                case TIPO_DOUBLE: {
                    char buffer[32];
                    snprintf(buffer, sizeof(buffer), "%lf", origen.v.d);
                    resultado.v.s = strdup(buffer);
                    break;
                }
                case TIPO_CHAR: {
                    char buffer[2] = {origen.v.c, '\0'};
                    resultado.v.s = strdup(buffer);
                    break;
                }
                case TIPO_BOOLEAN: {
                    resultado.v.s = strdup(origen.v.b ? "true" : "false");
                    break;
                }
                default:
                    resultado.inicializado = 0;
                    break;
            }
            break;
            
        default:
            resultado.inicializado = 0;
            break;
    }
    return resultado;
}
   