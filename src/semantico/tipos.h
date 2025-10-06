#ifndef TIPOS_H
#define TIPOS_H

#include "../AST/ast.h"
#include "../comun/strutil.h"
typedef enum {
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_DOUBLE,
    TIPO_CHAR,
    TIPO_STRING,
    TIPO_BOOLEAN,
    TIPO_DESCONOCIDO,
    TIPO_VOID,
    TIPO_FUNCION,
    TIPO_VECTOR,
    TIPO_MATRIZ,
    TIPO_ARRAY_MULTIDIMENSIONAL,
    TIPO_NULL,
} TipoDato;

TipoDato obtenerTipoDesdeString(const char* tipostr);
TipoDato deducirTipoExpr(NodoAST* expr); 

const char* valorPorDefectoTipo(TipoDato t);
int esWidening(TipoDato origen, TipoDato destino);
int esNarrowing(TipoDato origen, TipoDato destino);


#endif