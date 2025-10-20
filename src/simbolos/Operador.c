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
