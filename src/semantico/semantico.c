#include "semantico.h"
#include "tabla_simbolo.h"
#include "tipos.h"
#include <stdio.h>
#include <string.h>
#include "Instrucciones/declaracion.h"
#include "Instrucciones/asignacion.h"
#include "Expresiones/aritmeticas.h"
#include "Instrucciones/Print.h"
#include "../AST/err.h"

int error_semantico = 0;

const char* nombreTipo(TipoDato tipo) {
    switch(tipo) {
        case TIPO_INT: return "int";
        case TIPO_FLOAT: return "float";
        case TIPO_DOUBLE: return "double";
        case TIPO_CHAR: return "char";
        case TIPO_STRING: return "string";
        case TIPO_BOOLEAN: return "boolean";
        default: return "NULL";
    }
}
