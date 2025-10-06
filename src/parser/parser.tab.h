/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_PARSER_PARSER_TAB_H_INCLUDED
# define YY_YY_SRC_PARSER_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/parser/parser.y"

    #include "../AST/ast.h"
    #include "../AST/errorTIpos.h" 
    #include "../semantico/Instrucciones/Print.h"
    #include "../AST/ejecutar_ast.h"

#line 56 "src/parser/parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOKEN_LLAVEDER = 258,          /* TOKEN_LLAVEDER  */
    TOKEN_LLAVEIZQ = 259,          /* TOKEN_LLAVEIZQ  */
    TOKEN_CORCHETEIZQ = 260,       /* TOKEN_CORCHETEIZQ  */
    TOKEN_CORCHETEDER = 261,       /* TOKEN_CORCHETEDER  */
    TOKEN_NEW = 262,               /* TOKEN_NEW  */
    TOKEN_MULTI = 263,             /* TOKEN_MULTI  */
    TOKEN_DIV = 264,               /* TOKEN_DIV  */
    TOKEN_MODULO = 265,            /* TOKEN_MODULO  */
    TOKEN_MAS = 266,               /* TOKEN_MAS  */
    TOKEN_MENOS = 267,             /* TOKEN_MENOS  */
    TOKEN_INCREMENTO = 268,        /* TOKEN_INCREMENTO  */
    TOKEN_DECREMENTO = 269,        /* TOKEN_DECREMENTO  */
    TOKEN_DBOOLEAN = 270,          /* TOKEN_DBOOLEAN  */
    TOKEN_BOOLEANO = 271,          /* TOKEN_BOOLEANO  */
    TOKEN_IDENTIFICADOR = 272,     /* TOKEN_IDENTIFICADOR  */
    TOKEN_DINT = 273,              /* TOKEN_DINT  */
    TOKEN_DFLOAT = 274,            /* TOKEN_DFLOAT  */
    TOKEN_DSTRING = 275,           /* TOKEN_DSTRING  */
    TOKEN_DCHAR = 276,             /* TOKEN_DCHAR  */
    TOKEN_DOUBLE = 277,            /* TOKEN_DOUBLE  */
    TOKEN_IGUAL = 278,             /* TOKEN_IGUAL  */
    TOKEN_PUNTOCOMA = 279,         /* TOKEN_PUNTOCOMA  */
    TOKEN_DOS_PUNTOS = 280,        /* TOKEN_DOS_PUNTOS  */
    TOKEN_COMA = 281,              /* TOKEN_COMA  */
    TOKEN_NUMERO = 282,            /* TOKEN_NUMERO  */
    TOKEN_CADENA = 283,            /* TOKEN_CADENA  */
    TOKEN_CARACTER = 284,          /* TOKEN_CARACTER  */
    TOKEN_FLOAT_LITERAL = 285,     /* TOKEN_FLOAT_LITERAL  */
    TOKEN_DOUBLE_LITERAL = 286,    /* TOKEN_DOUBLE_LITERAL  */
    TOKEN_PRINT = 287,             /* TOKEN_PRINT  */
    TOKEN_PARIZQ = 288,            /* TOKEN_PARIZQ  */
    TOKEN_PARDER = 289,            /* TOKEN_PARDER  */
    TOKEN_BIT_AND = 290,           /* TOKEN_BIT_AND  */
    TOKEN_BIT_OR = 291,            /* TOKEN_BIT_OR  */
    TOKEN_XOR = 292,               /* TOKEN_XOR  */
    TOKEN_DESIZQ = 293,            /* TOKEN_DESIZQ  */
    TOKEN_DESDER = 294,            /* TOKEN_DESDER  */
    TOKEN_IGUAL_IGUAL = 295,       /* TOKEN_IGUAL_IGUAL  */
    TOKEN_NO_IGUAL = 296,          /* TOKEN_NO_IGUAL  */
    TOKEN_MAYOR_IGUAL = 297,       /* TOKEN_MAYOR_IGUAL  */
    TOKEN_MENOR_IGUAL = 298,       /* TOKEN_MENOR_IGUAL  */
    TOKEN_MAYOR = 299,             /* TOKEN_MAYOR  */
    TOKEN_MENOR = 300,             /* TOKEN_MENOR  */
    TOKEN_AND = 301,               /* TOKEN_AND  */
    TOKEN_OR = 302,                /* TOKEN_OR  */
    TOKEN_NOT = 303,               /* TOKEN_NOT  */
    TOKEN_IF = 304,                /* TOKEN_IF  */
    TOKEN_ELSE = 305,              /* TOKEN_ELSE  */
    TOKEN_SWITCH = 306,            /* TOKEN_SWITCH  */
    TOKEN_CASE = 307,              /* TOKEN_CASE  */
    TOKEN_DEFAULT = 308,           /* TOKEN_DEFAULT  */
    TOKEN_BREAK = 309,             /* TOKEN_BREAK  */
    TOKEN_WHILE = 310,             /* TOKEN_WHILE  */
    TOKEN_CONTINUE = 311,          /* TOKEN_CONTINUE  */
    TOKEN_FOR = 312,               /* TOKEN_FOR  */
    TOKEN_STRING_VALUEOF = 313,    /* TOKEN_STRING_VALUEOF  */
    TOKEN_EQUAL = 314,             /* TOKEN_EQUAL  */
    TOKEN_PARSEINT = 315,          /* TOKEN_PARSEINT  */
    TOKEN_PARSEFLOAT = 316,        /* TOKEN_PARSEFLOAT  */
    TOKEN_PARSEDOUBLE = 317,       /* TOKEN_PARSEDOUBLE  */
    TOKEN_PARSEJOIN = 318,         /* TOKEN_PARSEJOIN  */
    TOKEN_INDEXOF = 319,           /* TOKEN_INDEXOF  */
    TOKEN_LENGHT = 320,            /* TOKEN_LENGHT  */
    TOKEN_ADD = 321,               /* TOKEN_ADD  */
    TOKEN_PUNTO = 322,             /* TOKEN_PUNTO  */
    TOKEN_VOID = 323,              /* TOKEN_VOID  */
    TOKEN_RETURN = 324,            /* TOKEN_RETURN  */
    TOKEN_PUBLIC = 325,            /* TOKEN_PUBLIC  */
    TOKEN_STATIC = 326,            /* TOKEN_STATIC  */
    TOKEN_main = 327,              /* TOKEN_main  */
    TOKEN_NULL = 328,              /* TOKEN_NULL  */
    TOKEN_FINAL = 329,             /* TOKEN_FINAL  */
    TOKEN_MAS_IGUAL = 330,         /* TOKEN_MAS_IGUAL  */
    TOKEN_MENOS_IGUAL = 331,       /* TOKEN_MENOS_IGUAL  */
    TOKEN_POR_IGUAL = 332,         /* TOKEN_POR_IGUAL  */
    TOKEN_DIV_IGUAL = 333,         /* TOKEN_DIV_IGUAL  */
    TOKEN_MOD_IGUAL = 334,         /* TOKEN_MOD_IGUAL  */
    TOKEN_AND_IGUAL = 335,         /* TOKEN_AND_IGUAL  */
    TOKEN_OR_IGUAL = 336,          /* TOKEN_OR_IGUAL  */
    TOKEN_XOR_IGUAL = 337,         /* TOKEN_XOR_IGUAL  */
    TOKEN_DESIZQ_IGUAL = 338,      /* TOKEN_DESIZQ_IGUAL  */
    TOKEN_DESDER_IGUAL = 339,      /* TOKEN_DESDER_IGUAL  */
    UMINUS = 340                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 30 "src/parser/parser.y"

    char* str;
    NodoAST* nodo;

#line 163 "src/parser/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_SRC_PARSER_PARSER_TAB_H_INCLUDED  */
