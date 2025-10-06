#ifndef PARSER_API_H
#define PARSER_API_H
#include "../AST/ast.h"

typedef struct {
    int ok;
    char* errores;      // texto reporte errores
    char* simbolos;     // texto tabla símbolos
    char* astDot;       // contenido DOT
    char* salida;  
} ParseResult;

ParseResult parse_source(const char* code);
void free_parse_result(ParseResult* r);

#endif