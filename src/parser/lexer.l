%{
#define _POSIX_C_SOURCE 200809L
#include "parser.tab.h"
#include "../comun/strutil.h"
#include "../AST/errorTIpos.h" 
#include "../AST/err.h"  
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc.first_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_line = yylineno; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

static char map_escape(char c){
    switch(c){
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case '\\': return '\\';
        case '"': return '"';
        case '\'': return '\'';
        case 'u': return 'u'; // Para manejar Unicode
        default: return c;
    }
}

static char* unescape(const char* yytext) {
    size_t len = strlen(yytext);
    if(len < 2) return strdup("");
    const char* p = yytext + 1; // salta comilla inicial
    const char* end = yytext + len - 1; // antes de comilla final
    char* out = (char*)malloc(len + 1);
    size_t j = 0;
    
    while(p < end) {
        if(*p == '\\' && p + 1 < end) {
            if(*(p+1) == 'u' && p + 2 < end) { // Manejar \u con 1-6 dígitos
                p += 2; // Saltar \u
                int digitCount = 0;
                char dec[7] = {0}; // Máximo 6 dígitos + null terminator
                
                // Leer hasta 6 dígitos decimales
                while(digitCount < 6 && p < end && 
                      (*p >= '0' && *p <= '9')) {
                    dec[digitCount++] = *p++;
                }
                
                if(digitCount > 0) {
                    char* endptr;
                    long unicode_val = strtol(dec, &endptr, 16); // Base 10 para decimal
                    if(endptr == dec + digitCount) {
                        // Truncar a 8 bits si es necesario
                        if(unicode_val > 255) {
                            unicode_val = unicode_val & 0xFF;
                        }
                        out[j++] = (unsigned char)unicode_val;
                        continue;
                    }
                }
                // Si falla la conversión, tratar como carácter normal
                out[j++] = 'u';
                p -= 2; // Retroceder para reprocesar
            } else {
                out[j++] = map_escape(*(p+1));
                p += 2;
            }
        } else {
            out[j++] = *p++;
        }
    }
    out[j] = 0;
    return out;
}
%}
%option yylineno
%option noinput nounput

%%
System.out.println          { yylval.str = strdup("System.out.println"); return TOKEN_PRINT;}
int                         { yylval.str = strdup("int");    return TOKEN_DINT; }
float                       { yylval.str = strdup("float");  return TOKEN_DFLOAT; }
String                      { yylval.str = strdup("string"); return TOKEN_DSTRING; }
char                        { yylval.str = strdup("char");   return TOKEN_DCHAR; }
double                      { yylval.str = strdup("double"); return TOKEN_DOUBLE; }
boolean                     { yylval.str = strdup("boolean"); return TOKEN_DBOOLEAN; }
true                        { yylval.str = strdup("true"); return TOKEN_BOOLEANO; }
if                          { yylval.str = strdup("if"); return TOKEN_IF; }
else                        { yylval.str = strdup("else"); return TOKEN_ELSE; }
false                       { yylval.str = strdup("false"); return TOKEN_BOOLEANO; }
String\.valueOf             { return TOKEN_STRING_VALUEOF; }
\.equals                    { yylval.str = strdup("equals"); return TOKEN_EQUAL; }
Integer\.parseInt           { yylval.str = strdup("Integer.parseInt"); return TOKEN_PARSEINT; }
Double\.parseDouble         { yylval.str = strdup("Double.parseDouble"); return TOKEN_PARSEDOUBLE; }
Float\.parseFloat           { yylval.str = strdup("Float.parseFloat"); return TOKEN_PARSEFLOAT; }
String\.join                { yylval.str = strdup("String.join"); return TOKEN_PARSEJOIN; }
Arrays\.indexOf             { yylval.str = strdup("Arrays.indexOf"); return TOKEN_INDEXOF; }
length                      { yylval.str = strdup("Array.length"); return TOKEN_LENGHT; }
add                         { yylval.str = strdup("Array.add"); return TOKEN_ADD; }
switch                      { yylval.str = strdup("switch"); return TOKEN_SWITCH; }
case                        { yylval.str = strdup("case"); return TOKEN_CASE; }
default                     { yylval.str = strdup("default"); return TOKEN_DEFAULT; }
break                       { yylval.str = strdup("break"); return TOKEN_BREAK; }
while                       { yylval.str = strdup("while"); return TOKEN_WHILE; }
continue                    { yylval.str = strdup("continue"); return TOKEN_CONTINUE; }
for                         { yylval.str = strdup("for"); return TOKEN_FOR; }
new                         { yylval.str = strdup("new"); return TOKEN_NEW; }
void                        { yylval.str = strdup("void"); return TOKEN_VOID; }
return                      { yylval.str = strdup("return"); return TOKEN_RETURN; }
public                      { yylval.str = strdup("public"); return TOKEN_PUBLIC; }
static                      { yylval.str = strdup("static"); return TOKEN_STATIC; }
main                        { yylval.str = strdup("main"); return TOKEN_main; }
null                        { yylval.str = strdup("null"); return TOKEN_NULL; }
final                       { yylval.str = strdup("final"); return TOKEN_FINAL; }

[0-9]+                      { yylval.str = strdup(yytext); return TOKEN_NUMERO; }
[0-9]+\.[0-9]+[fF]          { yylval.str = strdup(yytext); return TOKEN_FLOAT_LITERAL; }
[0-9]+\.[0-9]+              { yylval.str = strdup(yytext); return TOKEN_DOUBLE_LITERAL; }


\"([^\\\"\n]|\\[nrt\"\'\\])*\" {
    yylval.str = unescape(yytext);
    return TOKEN_CADENA;
}


'([^\\'\n]|\\[nrt\"\'\\]|\\u[0-9a-fA-F]{1,6})' {
    char* tmp = unescape(yytext);
    if(strlen(tmp) != 1){
        fprintf(stderr, "Carácter inválido en línea %d: debe ser un solo carácter\n", yylineno);
        free(tmp);
        yylval.str = strdup("?");
    } else {
        yylval.str = tmp;
    }
    return TOKEN_CARACTER;
}

[a-zA-Z_][a-zA-Z0-9_]*      { yylval.str = strdup(yytext); return TOKEN_IDENTIFICADOR; }

\/\/[^\n]*                  { }
\/\*([^*]|\*+[^*/])*\*+\/   { }


"{"              { return TOKEN_LLAVEIZQ; }
"}"              { return TOKEN_LLAVEDER; }
"\+\+"           { return TOKEN_INCREMENTO; }
"\-\-"           { return TOKEN_DECREMENTO; }
"+"              { return TOKEN_MAS; }
"-"              { return TOKEN_MENOS; }
"="              { return TOKEN_IGUAL; }
";"              { return TOKEN_PUNTOCOMA; }
":"              { return TOKEN_DOS_PUNTOS; }
"+="             { return TOKEN_MAS_IGUAL; }
"-="             { return TOKEN_MENOS_IGUAL; }
"*="             { return TOKEN_POR_IGUAL; }
"/="             { return TOKEN_DIV_IGUAL; }
"%="             { return TOKEN_MOD_IGUAL; }
"&="             { return TOKEN_AND_IGUAL; }
"|="             { return TOKEN_OR_IGUAL; }
"^="             { return TOKEN_XOR_IGUAL; }
"<<="            { return TOKEN_DESIZQ_IGUAL; }
">>="            { return TOKEN_DESDER_IGUAL; }
"("              { return TOKEN_PARIZQ; }
")"              { return TOKEN_PARDER; }
"&"              { return TOKEN_BIT_AND; }
"|"              { return TOKEN_BIT_OR; }
"^"              { return TOKEN_XOR; }
"<<"             { return TOKEN_DESIZQ; }
">>"             { return TOKEN_DESDER; }
"=="             { return TOKEN_IGUAL_IGUAL; }
"!="             { return TOKEN_NO_IGUAL; }
">="             { return TOKEN_MAYOR_IGUAL; }
"<="             { return TOKEN_MENOR_IGUAL; }
">"              { return TOKEN_MAYOR; }
"<"              { return TOKEN_MENOR; }
"&&"             { return TOKEN_AND; }
"||"             { return TOKEN_OR; }
"!"              { return TOKEN_NOT; }
"*"              { return TOKEN_MULTI; }
"/"              { return TOKEN_DIV; }
"%"              { return TOKEN_MODULO; }
"["              { return TOKEN_CORCHETEIZQ; }
"]"              { return TOKEN_CORCHETEDER; }
","              { return TOKEN_COMA; }
"."              { return TOKEN_PUNTO; }




[ \t]+                      { }
\n                          { yycolumn = 1; }

.                           { fprintf(stderr, "Carácter no reconocido '%c' en línea %d, columna %d\n", 
                              yytext[0], yylineno, yycolumn);
                              addError("Carácter no reconocido", "léxico", yylineno, yycolumn);
                              yycolumn += yyleng;
                            }

%%

int yywrap(void){ return 1; }