#include "evaluarString.h"
#include "../../simbolos/Operador.h"
#include "../../AST/ast.h"
#include "../tabla_simbolo.h"
#include "../semantico.h"
#include "../../AST/err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>



ValorSimbolo evaluarParseInt(NodoAST* e) {
        ValorSimbolo r;
        memset(&r, 0, sizeof(r));
        r.tipo = TIPO_INT;
        r.inicializado = 0;

    // Verificar que el nodo sea válido y tenga un hijo
    if (!e || strcmp(e->valor, "Integer.parseInt") != 0 || e->num_hijos != 1) {
        printf("[ERROR] Nodo inválido para Integer.parseInt\n");
        return r;
    }

    // Evaluar el argumento
    ValorSimbolo argVal = evaluarExpr(e->hijos[0]);

    // Verificar que el argumento esté inicializado
    if (!argVal.inicializado) {
        char msg[200];
        snprintf(msg, sizeof(msg), "Argumento de Integer.parseInt no inicializado");
        printf("[ERROR] %s\n", msg);
        addError(msg, ambitoActual(), e->linea, e->columna);
        return r;
    }

    // Verificar que el argumento sea una cadena
    if (argVal.tipo != TIPO_STRING) {
        char msg[200];
        snprintf(msg, sizeof(msg), "Integer.parseInt requiere una cadena como argumento, se recibió '%s'", 
                nombreTipo(argVal.tipo));
        printf("[ERROR] %s\n", msg);
        addError(msg, ambitoActual(), e->linea, e->columna);
        return r;
    }

    // Verificar que la cadena no sea NULL
    if (!argVal.v.s) {
        char msg[200];
        snprintf(msg, sizeof(msg), "Integer.parseInt recibió una cadena NULL");
        printf("[ERROR] %s\n", msg);
        addError(msg, ambitoActual(), e->linea, e->columna);
        return r;
    }

    // Intentar convertir la cadena a entero
    char* endPtr;
    long int valor = strtol(argVal.v.s, &endPtr, 10);

    // Verificar errores de conversión
    if (endPtr == argVal.v.s || *endPtr != '\0') {
        char msg[200];
        snprintf(msg, sizeof(msg), "La cadena '%s' no es un número válido para Integer.parseInt", 
                argVal.v.s);
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }

    // Verificar rango de int
    if (valor < INT_MIN || valor > INT_MAX) {
        char msg[200];
        snprintf(msg, sizeof(msg), "El número '%s' está fuera del rango válido para int", 
                argVal.v.s);
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }

    // Asignar el valor convertido al resultado
    r.v.i = (int)valor;
    r.inicializado = 1;

    // Liberar memoria si es necesario
    if (argVal.tipo == TIPO_STRING && argVal.v.s) {
        //free(argVal.v.s);
    }

    return r;
}

ValorSimbolo evaluarParseFloat(NodoAST* e) {
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    r.tipo = TIPO_FLOAT;
    r.inicializado = 0;

    if (e->num_hijos < 1) {
        printf("[ERROR] Float.parseFloat requiere un argumento\n");
        return r;
    }

    ValorSimbolo argVal = evaluarExpr(e->hijos[0]);

    if (!argVal.inicializado) {
        char msg[200];
        snprintf(msg, sizeof(msg), "Argumento de Float.parseFloat no inicializado\n");
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }

    if (argVal.tipo != TIPO_STRING) {
        char msg[200];
        snprintf(msg, sizeof(msg), "Float.parseFloat requiere una cadena como argumento, se recibió %s", 
                nombreTipo(argVal.tipo));
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }

    char* endptr;
    float val = strtof(argVal.v.s, &endptr);

    if( *endptr != '\0') {
        char msg[200];
        snprintf(msg, sizeof(msg), "La cadena '%s' no es un número válido para Float.parseFloat", 
                argVal.v.s);
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }

    if( val == HUGE_VALF ) {
        char msg[200];
        snprintf(msg, sizeof(msg), "El número '%s' está fuera del rango válido para float", 
                argVal.v.s);
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }

    r = makeNum(TIPO_FLOAT, (double)val);
    r.tipo = TIPO_FLOAT;
    printf("[DEBUG] Float.parseFloat('%s') = %f\n", argVal.v.s, r.v.f);

    return r;
}

ValorSimbolo evaluarParseDouble(NodoAST* e) {
    ValorSimbolo r;
    memset(&r, 0, sizeof(r));
    r.tipo = TIPO_DOUBLE;
    r.inicializado = 0;

    if (e->num_hijos < 1) {
        printf("[ERROR] Double.parseDouble requiere un argumento\n");
        return r;
    }
    
    ValorSimbolo argVal = evaluarExpr(e->hijos[0]);
    
    if (!argVal.inicializado) {
        char msg[200];
        snprintf(msg, sizeof(msg), "Argumento de Double.parseDouble no inicializado\n");
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }
    
    if (argVal.tipo != TIPO_STRING) {
        char msg[200];
        snprintf(msg, sizeof(msg), "Double.parseDouble requiere una cadena como argumento, se recibió %s", 
                nombreTipo(argVal.tipo));
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }
    
    char* endptr;
    double val = strtod(argVal.v.s, &endptr);
    
    if (*endptr != '\0') {
        char msg[200];
        snprintf(msg, sizeof(msg), "La cadena '%s' no es un número válido para Double.parseDouble", 
                argVal.v.s);
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }
    
    if (val == HUGE_VAL) {
        char msg[200];
        snprintf(msg, sizeof(msg), "El número '%s' está fuera del rango válido para double", 
                argVal.v.s);
        addError(msg, ambitoActual(), e->linea, e->columna);
        printf("[ERROR] %s\n", msg);
        return r;
    }
    
    r = makeNum(TIPO_DOUBLE, val);
    r.tipo = TIPO_DOUBLE;
    printf("[DEBUG] Double.parseDouble('%s') = %f\n", argVal.v.s, r.v.d);

    return r;
}