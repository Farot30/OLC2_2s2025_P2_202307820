/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 11 "src/parser/parser.y"

    #include "../comun/strutil.h"
    #include <stdio.h>
    #include <string.h>
    #include "../semantico/semantico.h"
    #include "../AST/err.h"  
    #include "../AST/errorTIpos.h" 
    void yyerror(const char *s);
    extern int yylex(void);
    extern int yylineno;
    extern int yycolumn;

    extern NodoAST* astGlobal;
    
    /* Función auxiliar para crear asignaciones compuestas */
    NodoAST* crear_asignacion_compuesta(char* id, char* operador, NodoAST* expr, 
                                       int id_linea, int id_columna, int op_linea, int op_columna);

#line 90 "src/parser/parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOKEN_LLAVEDER = 3,             /* TOKEN_LLAVEDER  */
  YYSYMBOL_TOKEN_LLAVEIZQ = 4,             /* TOKEN_LLAVEIZQ  */
  YYSYMBOL_TOKEN_CORCHETEIZQ = 5,          /* TOKEN_CORCHETEIZQ  */
  YYSYMBOL_TOKEN_CORCHETEDER = 6,          /* TOKEN_CORCHETEDER  */
  YYSYMBOL_TOKEN_NEW = 7,                  /* TOKEN_NEW  */
  YYSYMBOL_TOKEN_MULTI = 8,                /* TOKEN_MULTI  */
  YYSYMBOL_TOKEN_DIV = 9,                  /* TOKEN_DIV  */
  YYSYMBOL_TOKEN_MODULO = 10,              /* TOKEN_MODULO  */
  YYSYMBOL_TOKEN_MAS = 11,                 /* TOKEN_MAS  */
  YYSYMBOL_TOKEN_MENOS = 12,               /* TOKEN_MENOS  */
  YYSYMBOL_TOKEN_INCREMENTO = 13,          /* TOKEN_INCREMENTO  */
  YYSYMBOL_TOKEN_DECREMENTO = 14,          /* TOKEN_DECREMENTO  */
  YYSYMBOL_TOKEN_DBOOLEAN = 15,            /* TOKEN_DBOOLEAN  */
  YYSYMBOL_TOKEN_BOOLEANO = 16,            /* TOKEN_BOOLEANO  */
  YYSYMBOL_TOKEN_IDENTIFICADOR = 17,       /* TOKEN_IDENTIFICADOR  */
  YYSYMBOL_TOKEN_DINT = 18,                /* TOKEN_DINT  */
  YYSYMBOL_TOKEN_DFLOAT = 19,              /* TOKEN_DFLOAT  */
  YYSYMBOL_TOKEN_DSTRING = 20,             /* TOKEN_DSTRING  */
  YYSYMBOL_TOKEN_DCHAR = 21,               /* TOKEN_DCHAR  */
  YYSYMBOL_TOKEN_DOUBLE = 22,              /* TOKEN_DOUBLE  */
  YYSYMBOL_TOKEN_IGUAL = 23,               /* TOKEN_IGUAL  */
  YYSYMBOL_TOKEN_PUNTOCOMA = 24,           /* TOKEN_PUNTOCOMA  */
  YYSYMBOL_TOKEN_DOS_PUNTOS = 25,          /* TOKEN_DOS_PUNTOS  */
  YYSYMBOL_TOKEN_COMA = 26,                /* TOKEN_COMA  */
  YYSYMBOL_TOKEN_NUMERO = 27,              /* TOKEN_NUMERO  */
  YYSYMBOL_TOKEN_CADENA = 28,              /* TOKEN_CADENA  */
  YYSYMBOL_TOKEN_CARACTER = 29,            /* TOKEN_CARACTER  */
  YYSYMBOL_TOKEN_FLOAT_LITERAL = 30,       /* TOKEN_FLOAT_LITERAL  */
  YYSYMBOL_TOKEN_DOUBLE_LITERAL = 31,      /* TOKEN_DOUBLE_LITERAL  */
  YYSYMBOL_TOKEN_PRINT = 32,               /* TOKEN_PRINT  */
  YYSYMBOL_TOKEN_PARIZQ = 33,              /* TOKEN_PARIZQ  */
  YYSYMBOL_TOKEN_PARDER = 34,              /* TOKEN_PARDER  */
  YYSYMBOL_TOKEN_BIT_AND = 35,             /* TOKEN_BIT_AND  */
  YYSYMBOL_TOKEN_BIT_OR = 36,              /* TOKEN_BIT_OR  */
  YYSYMBOL_TOKEN_XOR = 37,                 /* TOKEN_XOR  */
  YYSYMBOL_TOKEN_DESIZQ = 38,              /* TOKEN_DESIZQ  */
  YYSYMBOL_TOKEN_DESDER = 39,              /* TOKEN_DESDER  */
  YYSYMBOL_TOKEN_IGUAL_IGUAL = 40,         /* TOKEN_IGUAL_IGUAL  */
  YYSYMBOL_TOKEN_NO_IGUAL = 41,            /* TOKEN_NO_IGUAL  */
  YYSYMBOL_TOKEN_MAYOR_IGUAL = 42,         /* TOKEN_MAYOR_IGUAL  */
  YYSYMBOL_TOKEN_MENOR_IGUAL = 43,         /* TOKEN_MENOR_IGUAL  */
  YYSYMBOL_TOKEN_MAYOR = 44,               /* TOKEN_MAYOR  */
  YYSYMBOL_TOKEN_MENOR = 45,               /* TOKEN_MENOR  */
  YYSYMBOL_TOKEN_AND = 46,                 /* TOKEN_AND  */
  YYSYMBOL_TOKEN_OR = 47,                  /* TOKEN_OR  */
  YYSYMBOL_TOKEN_NOT = 48,                 /* TOKEN_NOT  */
  YYSYMBOL_TOKEN_IF = 49,                  /* TOKEN_IF  */
  YYSYMBOL_TOKEN_ELSE = 50,                /* TOKEN_ELSE  */
  YYSYMBOL_TOKEN_SWITCH = 51,              /* TOKEN_SWITCH  */
  YYSYMBOL_TOKEN_CASE = 52,                /* TOKEN_CASE  */
  YYSYMBOL_TOKEN_DEFAULT = 53,             /* TOKEN_DEFAULT  */
  YYSYMBOL_TOKEN_BREAK = 54,               /* TOKEN_BREAK  */
  YYSYMBOL_TOKEN_WHILE = 55,               /* TOKEN_WHILE  */
  YYSYMBOL_TOKEN_CONTINUE = 56,            /* TOKEN_CONTINUE  */
  YYSYMBOL_TOKEN_FOR = 57,                 /* TOKEN_FOR  */
  YYSYMBOL_TOKEN_STRING_VALUEOF = 58,      /* TOKEN_STRING_VALUEOF  */
  YYSYMBOL_TOKEN_EQUAL = 59,               /* TOKEN_EQUAL  */
  YYSYMBOL_TOKEN_PARSEINT = 60,            /* TOKEN_PARSEINT  */
  YYSYMBOL_TOKEN_PARSEFLOAT = 61,          /* TOKEN_PARSEFLOAT  */
  YYSYMBOL_TOKEN_PARSEDOUBLE = 62,         /* TOKEN_PARSEDOUBLE  */
  YYSYMBOL_TOKEN_PARSEJOIN = 63,           /* TOKEN_PARSEJOIN  */
  YYSYMBOL_TOKEN_INDEXOF = 64,             /* TOKEN_INDEXOF  */
  YYSYMBOL_TOKEN_LENGHT = 65,              /* TOKEN_LENGHT  */
  YYSYMBOL_TOKEN_ADD = 66,                 /* TOKEN_ADD  */
  YYSYMBOL_TOKEN_PUNTO = 67,               /* TOKEN_PUNTO  */
  YYSYMBOL_TOKEN_VOID = 68,                /* TOKEN_VOID  */
  YYSYMBOL_TOKEN_RETURN = 69,              /* TOKEN_RETURN  */
  YYSYMBOL_TOKEN_PUBLIC = 70,              /* TOKEN_PUBLIC  */
  YYSYMBOL_TOKEN_STATIC = 71,              /* TOKEN_STATIC  */
  YYSYMBOL_TOKEN_main = 72,                /* TOKEN_main  */
  YYSYMBOL_TOKEN_NULL = 73,                /* TOKEN_NULL  */
  YYSYMBOL_TOKEN_FINAL = 74,               /* TOKEN_FINAL  */
  YYSYMBOL_TOKEN_MAS_IGUAL = 75,           /* TOKEN_MAS_IGUAL  */
  YYSYMBOL_TOKEN_MENOS_IGUAL = 76,         /* TOKEN_MENOS_IGUAL  */
  YYSYMBOL_TOKEN_POR_IGUAL = 77,           /* TOKEN_POR_IGUAL  */
  YYSYMBOL_TOKEN_DIV_IGUAL = 78,           /* TOKEN_DIV_IGUAL  */
  YYSYMBOL_TOKEN_MOD_IGUAL = 79,           /* TOKEN_MOD_IGUAL  */
  YYSYMBOL_TOKEN_AND_IGUAL = 80,           /* TOKEN_AND_IGUAL  */
  YYSYMBOL_TOKEN_OR_IGUAL = 81,            /* TOKEN_OR_IGUAL  */
  YYSYMBOL_TOKEN_XOR_IGUAL = 82,           /* TOKEN_XOR_IGUAL  */
  YYSYMBOL_TOKEN_DESIZQ_IGUAL = 83,        /* TOKEN_DESIZQ_IGUAL  */
  YYSYMBOL_TOKEN_DESDER_IGUAL = 84,        /* TOKEN_DESDER_IGUAL  */
  YYSYMBOL_UMINUS = 85,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 86,                  /* $accept  */
  YYSYMBOL_s = 87,                         /* s  */
  YYSYMBOL_programa = 88,                  /* programa  */
  YYSYMBOL_lista_funciones = 89,           /* lista_funciones  */
  YYSYMBOL_main_class = 90,                /* main_class  */
  YYSYMBOL_lista_sentencias = 91,          /* lista_sentencias  */
  YYSYMBOL_sentencia = 92,                 /* sentencia  */
  YYSYMBOL_expresion_sentencia = 93,       /* expresion_sentencia  */
  YYSYMBOL_postfix_incremento = 94,        /* postfix_incremento  */
  YYSYMBOL_declaracion = 95,               /* declaracion  */
  YYSYMBOL_declaracion_vector = 96,        /* declaracion_vector  */
  YYSYMBOL_lista_elementos = 97,           /* lista_elementos  */
  YYSYMBOL_lista_filas = 98,               /* lista_filas  */
  YYSYMBOL_declaracion_matriz = 99,        /* declaracion_matriz  */
  YYSYMBOL_declaracion_array_multidimensional = 100, /* declaracion_array_multidimensional  */
  YYSYMBOL_lista_bloques = 101,            /* lista_bloques  */
  YYSYMBOL_lista_elementos_o_bloques = 102, /* lista_elementos_o_bloques  */
  YYSYMBOL_DimensionesCorchetes = 103,     /* DimensionesCorchetes  */
  YYSYMBOL_Dimensiones = 104,              /* Dimensiones  */
  YYSYMBOL_acceso = 105,                   /* acceso  */
  YYSYMBOL_DimensionesAcceso = 106,        /* DimensionesAcceso  */
  YYSYMBOL_acceso_escritura = 107,         /* acceso_escritura  */
  YYSYMBOL_Funcion_void = 108,             /* Funcion_void  */
  YYSYMBOL_Llamadas = 109,                 /* Llamadas  */
  YYSYMBOL_lista_parametros = 110,         /* lista_parametros  */
  YYSYMBOL_Funcion_parametros = 111,       /* Funcion_parametros  */
  YYSYMBOL_parametros = 112,               /* parametros  */
  YYSYMBOL_TipoConCorchetes = 113,         /* TipoConCorchetes  */
  YYSYMBOL_tipo_primitivo = 114,           /* tipo_primitivo  */
  YYSYMBOL_asignacion = 115,               /* asignacion  */
  YYSYMBOL_expresion = 116,                /* expresion  */
  YYSYMBOL_expresion_or = 117,             /* expresion_or  */
  YYSYMBOL_expresion_and = 118,            /* expresion_and  */
  YYSYMBOL_expresion_bit_or = 119,         /* expresion_bit_or  */
  YYSYMBOL_expresion_bit_xor = 120,        /* expresion_bit_xor  */
  YYSYMBOL_expresion_bit_and = 121,        /* expresion_bit_and  */
  YYSYMBOL_expresion_equality = 122,       /* expresion_equality  */
  YYSYMBOL_expresion_relacional = 123,     /* expresion_relacional  */
  YYSYMBOL_expresion_shift = 124,          /* expresion_shift  */
  YYSYMBOL_expresion_aditiva = 125,        /* expresion_aditiva  */
  YYSYMBOL_expresion_multiplicativa = 126, /* expresion_multiplicativa  */
  YYSYMBOL_expresion_unaria = 127,         /* expresion_unaria  */
  YYSYMBOL_postfix = 128,                  /* postfix  */
  YYSYMBOL_primary = 129,                  /* primary  */
  YYSYMBOL_lista_expresiones_join = 130,   /* lista_expresiones_join  */
  YYSYMBOL_instruccion_print = 131,        /* instruccion_print  */
  YYSYMBOL_bloque = 132,                   /* bloque  */
  YYSYMBOL_instruccion_break = 133,        /* instruccion_break  */
  YYSYMBOL_instruccion_continue = 134,     /* instruccion_continue  */
  YYSYMBOL_instruccion_return = 135,       /* instruccion_return  */
  YYSYMBOL_instruccion_if = 136,           /* instruccion_if  */
  YYSYMBOL_intruccion_switch = 137,        /* intruccion_switch  */
  YYSYMBOL_lista_casos = 138,              /* lista_casos  */
  YYSYMBOL_caso = 139,                     /* caso  */
  YYSYMBOL_instruccion_while = 140,        /* instruccion_while  */
  YYSYMBOL_instruccion_for = 141           /* instruccion_for  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1127

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  195
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  467

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   340


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   112,   112,   116,   122,   130,   141,   152,   157,   162,
     168,   178,   185,   189,   193,   197,   206,   217,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   247,   255,   256,   260,
     265,   274,   284,   292,   301,   309,   329,   340,   372,   383,
     390,   401,   408,   432,   451,   472,   499,   519,   530,   537,
     543,   552,   571,   577,   581,   591,   603,   619,   629,   639,
     654,   666,   678,   694,   702,   712,   724,   735,   742,   757,
     775,   793,   810,   829,   846,   867,   899,   935,   952,   971,
     974,   979,   984,   992,   993,   994,   995,   996,   997,  1002,
    1012,  1015,  1018,  1021,  1024,  1027,  1030,  1033,  1036,  1039,
    1049,  1053,  1054,  1062,  1063,  1071,  1072,  1080,  1081,  1089,
    1090,  1098,  1099,  1104,  1112,  1113,  1118,  1123,  1128,  1136,
    1137,  1142,  1150,  1151,  1156,  1164,  1165,  1170,  1175,  1183,
    1188,  1193,  1201,  1205,  1206,  1211,  1220,  1225,  1230,  1235,
    1240,  1245,  1249,  1253,  1258,  1263,  1268,  1273,  1279,  1286,
    1294,  1302,  1310,  1316,  1326,  1331,  1335,  1338,  1341,  1347,
    1358,  1366,  1372,  1384,  1392,  1397,  1407,  1414,  1421,  1431,
    1437,  1447,  1456,  1468,  1477,  1488,  1495,  1504,  1512,  1518,
    1529,  1537,  1545,  1552,  1571,  1581
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOKEN_LLAVEDER",
  "TOKEN_LLAVEIZQ", "TOKEN_CORCHETEIZQ", "TOKEN_CORCHETEDER", "TOKEN_NEW",
  "TOKEN_MULTI", "TOKEN_DIV", "TOKEN_MODULO", "TOKEN_MAS", "TOKEN_MENOS",
  "TOKEN_INCREMENTO", "TOKEN_DECREMENTO", "TOKEN_DBOOLEAN",
  "TOKEN_BOOLEANO", "TOKEN_IDENTIFICADOR", "TOKEN_DINT", "TOKEN_DFLOAT",
  "TOKEN_DSTRING", "TOKEN_DCHAR", "TOKEN_DOUBLE", "TOKEN_IGUAL",
  "TOKEN_PUNTOCOMA", "TOKEN_DOS_PUNTOS", "TOKEN_COMA", "TOKEN_NUMERO",
  "TOKEN_CADENA", "TOKEN_CARACTER", "TOKEN_FLOAT_LITERAL",
  "TOKEN_DOUBLE_LITERAL", "TOKEN_PRINT", "TOKEN_PARIZQ", "TOKEN_PARDER",
  "TOKEN_BIT_AND", "TOKEN_BIT_OR", "TOKEN_XOR", "TOKEN_DESIZQ",
  "TOKEN_DESDER", "TOKEN_IGUAL_IGUAL", "TOKEN_NO_IGUAL",
  "TOKEN_MAYOR_IGUAL", "TOKEN_MENOR_IGUAL", "TOKEN_MAYOR", "TOKEN_MENOR",
  "TOKEN_AND", "TOKEN_OR", "TOKEN_NOT", "TOKEN_IF", "TOKEN_ELSE",
  "TOKEN_SWITCH", "TOKEN_CASE", "TOKEN_DEFAULT", "TOKEN_BREAK",
  "TOKEN_WHILE", "TOKEN_CONTINUE", "TOKEN_FOR", "TOKEN_STRING_VALUEOF",
  "TOKEN_EQUAL", "TOKEN_PARSEINT", "TOKEN_PARSEFLOAT", "TOKEN_PARSEDOUBLE",
  "TOKEN_PARSEJOIN", "TOKEN_INDEXOF", "TOKEN_LENGHT", "TOKEN_ADD",
  "TOKEN_PUNTO", "TOKEN_VOID", "TOKEN_RETURN", "TOKEN_PUBLIC",
  "TOKEN_STATIC", "TOKEN_main", "TOKEN_NULL", "TOKEN_FINAL",
  "TOKEN_MAS_IGUAL", "TOKEN_MENOS_IGUAL", "TOKEN_POR_IGUAL",
  "TOKEN_DIV_IGUAL", "TOKEN_MOD_IGUAL", "TOKEN_AND_IGUAL",
  "TOKEN_OR_IGUAL", "TOKEN_XOR_IGUAL", "TOKEN_DESIZQ_IGUAL",
  "TOKEN_DESDER_IGUAL", "UMINUS", "$accept", "s", "programa",
  "lista_funciones", "main_class", "lista_sentencias", "sentencia",
  "expresion_sentencia", "postfix_incremento", "declaracion",
  "declaracion_vector", "lista_elementos", "lista_filas",
  "declaracion_matriz", "declaracion_array_multidimensional",
  "lista_bloques", "lista_elementos_o_bloques", "DimensionesCorchetes",
  "Dimensiones", "acceso", "DimensionesAcceso", "acceso_escritura",
  "Funcion_void", "Llamadas", "lista_parametros", "Funcion_parametros",
  "parametros", "TipoConCorchetes", "tipo_primitivo", "asignacion",
  "expresion", "expresion_or", "expresion_and", "expresion_bit_or",
  "expresion_bit_xor", "expresion_bit_and", "expresion_equality",
  "expresion_relacional", "expresion_shift", "expresion_aditiva",
  "expresion_multiplicativa", "expresion_unaria", "postfix", "primary",
  "lista_expresiones_join", "instruccion_print", "bloque",
  "instruccion_break", "instruccion_continue", "instruccion_return",
  "instruccion_if", "intruccion_switch", "lista_casos", "caso",
  "instruccion_while", "instruccion_for", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-376)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-188)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     170,  -376,  -376,  -376,  -376,  -376,  -376,  -376,   -14,    15,
      35,  -376,   170,  -376,  -376,  -376,    46,    49,    47,  -376,
       3,  -376,  -376,  -376,  -376,    86,    81,   101,   275,  -376,
       5,   389,   146,   850,   177,   163,   178,   215,    95,  -376,
     147,   205,   248,   199,   260,   239,   271,   200,   163,   631,
    -376,   259,   163,  -376,  -376,   257,   294,   674,   296,   303,
     308,   674,    61,  -376,    17,   140,   283,   284,   289,  -376,
     290,  -376,   293,  1054,   259,   398,  -376,   304,  -376,   305,
     307,   314,   316,   309,   317,  -376,  -376,  -376,   104,   318,
     319,  -376,   320,   321,   323,  -376,  -376,  -376,  -376,   310,
     103,  -376,   161,   674,   324,   424,   674,   343,   674,   467,
    -376,   674,  -376,  -376,  -376,  1054,  -376,  -376,  1054,   749,
    1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,
     345,  1016,   772,  1054,   825,   157,  1054,  -376,   941,  -376,
     295,  -376,  -376,  -376,   994,  1054,   322,   325,   326,   328,
     330,   334,  -376,   285,  -376,  -376,   306,   327,   333,   335,
     336,    98,   241,     6,   171,   279,  -376,   173,  -376,   339,
    -376,  -376,  -376,  -376,  -376,  -376,  -376,  1054,  -376,   346,
     368,     4,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,   493,  -376,   536,  -376,   562,  -376,   605,   369,  -376,
     349,  -376,   201,  -376,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,  -376,  -376,  -376,  1054,    -9,  -376,   353,   354,   355,
     357,   358,   359,   370,   119,   189,   110,   374,  -376,   367,
     164,   372,   378,   390,  -376,  1054,  1054,  1054,  1054,  1054,
    1054,   361,  1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,
    1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,  1054,
    -376,  -376,   356,  -376,  -376,   105,   423,  1054,   408,  -376,
    -376,  -376,  -376,  -376,  -376,  1054,  -376,   426,  1054,  -376,
     163,   163,   429,   163,   163,  1054,   847,  1054,   430,   412,
    1054,  1054,  -376,   404,  1054,  1054,  -376,   406,   414,   416,
     417,   431,   432,  -376,   327,   333,   335,   336,    98,   241,
     241,     6,     6,     6,     6,   171,   171,   279,   279,  -376,
    -376,  -376,  1054,   453,   415,   455,  -376,   145,  -376,  -376,
     427,  -376,   413,   206,  -376,  -376,   438,   440,   441,   435,
     457,   452,   443,  1054,   449,  -376,  -376,  -376,  -376,  -376,
    1054,  1054,  -376,   112,   133,  -376,   486,   259,  -376,    11,
    1054,   466,    54,  -376,  1054,  1054,  1054,   163,   470,  1054,
    -376,   468,  -376,   469,   202,   471,   477,  1054,   259,   434,
     900,    14,  -376,   499,  -376,  -376,   481,   674,  -376,  -376,
     473,   475,   483,  -376,  1054,   485,  -376,  -376,  1054,  -376,
    -376,   139,    20,  -376,   515,   460,    21,    23,  -376,   486,
     500,   523,   674,   250,   163,   163,   163,   495,   163,  -376,
     526,   259,   498,  -376,  -376,  1054,  1054,   501,  -376,  -376,
    -376,   527,   505,   700,  -376,  -376,  -376,   163,  -376,  1054,
      26,   533,  -376,   537,  1054,  -376,   539,   522,  -376,    29,
    -376,   548,  1054,  -376,   525,  -376,  -376,  -376,  1054,   554,
    -376,    33,   559,  -376,  1054,   563,  -376
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    15,    98,    93,    96,    97,    94,    95,     0,     0,
       0,     2,     0,     4,     7,     8,     0,     0,     0,    14,
       0,     1,     3,     5,     6,     0,     0,     0,     0,    13,
       0,     0,    90,     0,     0,     0,     0,     0,     0,    12,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      70,     0,     0,    87,    88,     0,     0,     0,     0,    91,
       0,     0,     0,    72,     0,     0,     0,     0,     0,   176,
       0,   177,     0,     0,     0,     0,    17,     0,    37,     0,
       0,     0,     0,     0,     0,    29,    38,    31,     0,     0,
       0,    35,     0,     0,     0,    22,    23,    24,    25,     0,
       0,    71,     0,     0,     0,     0,     0,     0,     0,     0,
      80,     0,   175,    36,    66,     0,    39,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,     0,     0,     0,     0,     0,   152,     0,   146,
     149,   150,   148,   147,     0,     0,     0,     0,     0,     0,
       0,     0,   165,   166,   167,   178,   110,   111,   113,   115,
     117,   119,   121,   124,   129,   132,   135,   142,   143,     0,
     174,    16,    33,    18,    26,    27,    28,     0,    20,     0,
       0,    42,    19,    21,    32,    34,    30,    86,    85,    11,
      10,     0,    84,     0,    92,     0,    83,     0,     0,    99,
       0,    73,     0,    77,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,   149,   173,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   140,     0,
       0,     0,     0,     0,   139,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     144,   145,     0,    69,    44,     0,     0,     0,     0,    82,
      79,    81,    78,    68,    75,     0,    74,     0,     0,   171,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    42,
       0,     0,   158,     0,     0,     0,   168,     0,     0,     0,
       0,     0,     0,   164,   112,   114,   116,   118,   120,   122,
     123,   128,   126,   127,   125,   130,   131,   133,   134,   136,
     137,   138,     0,     0,     0,    62,    41,     0,    76,    67,
       0,   182,   179,     0,   189,   188,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   141,   156,   153,   155,   154,
       0,     0,    43,     0,     0,    61,     0,     0,   172,     0,
       0,     0,     0,   185,     0,     0,     0,     0,     0,     0,
     161,     0,   160,   170,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,   180,   181,     0,     0,   183,   184,
       0,     0,     0,   192,     0,     0,   159,   163,     0,   162,
     157,     0,     0,    49,     0,     0,     0,     0,    56,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,     0,    54,    46,     0,     0,     0,    59,    60,
      57,     0,     0,     0,   194,   195,   190,     0,   191,     0,
       0,     0,    48,     0,     0,    63,     0,     0,   193,     0,
      53,     0,     0,    45,     0,    64,   186,    51,     0,     0,
      47,     0,     0,    50,     0,     0,    52
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -376,  -376,  -376,  -376,   558,   -50,   -67,  -376,  -376,   436,
    -376,  -375,  -376,  -376,  -376,   192,   165,   251,  -376,   -27,
    -376,  -376,   148,   -36,  -376,   181,    92,    85,     0,   451,
     -30,  -376,   331,   332,   344,   350,   351,    63,    -8,     9,
      59,  -126,  -376,  -376,  -376,  -376,   -34,  -376,  -376,  -376,
     219,  -376,  -376,   227,  -376,  -376
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    10,    11,    12,    13,    75,    76,    77,    78,    79,
      80,   402,   440,    81,    82,   381,   382,   268,   411,   153,
     130,    84,    85,   154,   202,    87,    36,    16,    88,    89,
     403,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   374,    90,    91,    92,    93,    94,
      95,    96,   362,   363,    97,    98
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    50,   278,    18,    29,   406,    39,   105,   171,   266,
     228,   109,    17,    86,    63,    49,    19,   408,   101,   234,
     112,    86,    83,   424,   428,    86,   429,   267,    38,   450,
      83,    38,   457,    38,    83,    21,   463,    33,   171,    86,
     409,   113,   171,   155,   253,   254,   425,   425,    83,   409,
     231,   100,   451,   191,    26,   425,   193,   388,   195,   425,
      67,   197,   110,    25,   449,   111,    27,    86,   190,    86,
      86,    30,    86,    86,   169,    86,    83,    40,    83,    83,
      28,    83,    83,   461,    83,   198,    20,    32,   199,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
      26,   217,   219,   220,   222,   179,   360,   361,    26,   180,
     323,   179,    54,    37,   233,   288,    37,   107,    37,    31,
     188,   181,   324,    43,   171,    47,   171,   289,   171,   376,
     171,   319,   320,   321,    33,   226,    99,   377,   247,   248,
     378,   114,   118,   420,   232,   115,   421,   263,    14,   356,
     379,    44,   357,   116,   117,    86,   422,    86,   223,    86,
      23,    86,   189,   118,    83,    49,    83,    49,    83,   345,
      83,     1,     2,   119,   224,     3,     4,     5,     6,     7,
      55,    15,   255,   256,   277,     2,   260,   261,     3,     4,
       5,     6,     7,    24,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,    51,   297,   298,   299,   300,   301,
     302,    48,    52,   286,   287,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,    51,    51,   275,   398,   292,
     293,    74,    53,    58,    62,   276,   399,   326,     8,    56,
       9,   311,   312,   313,   314,   328,   331,   332,   330,   334,
     335,   104,    57,  -187,    49,   336,   338,   339,   360,   361,
     341,   342,   315,   316,   344,     2,    59,    65,     3,     4,
       5,     6,     7,    60,     2,    61,    34,     3,     4,     5,
       6,     7,    66,   249,   250,   251,   252,   257,   258,   259,
       2,   102,   352,     3,     4,     5,     6,     7,   103,    67,
     106,    68,  -187,  -187,    69,    70,    71,    72,   107,    35,
     309,   310,   108,   371,   317,   318,   131,   132,     8,    73,
     373,   375,   133,   134,    74,   384,   135,   187,   172,   173,
     386,   174,   177,   393,   390,   391,   392,   413,   175,   395,
     176,   178,   182,   183,   184,   185,   171,   186,   113,   194,
     214,    86,   241,   242,   231,   235,   262,   383,   236,   237,
      83,   238,   433,   239,   417,   423,   171,   240,   419,   244,
     264,   246,   245,   243,   265,   273,    86,    86,   404,   322,
     434,   435,   436,   274,   438,    83,    83,   279,   280,   281,
      41,   282,   283,   284,   285,   442,   443,    86,   290,   104,
     291,   170,    49,   448,     2,   294,    83,     3,     4,     5,
       6,     7,   295,     2,   454,    65,     3,     4,     5,     6,
       7,   441,   459,    42,   296,   104,   303,   192,    49,   325,
      66,   327,   329,   333,   465,   267,   340,   343,   354,     2,
     346,    65,     3,     4,     5,     6,     7,    67,   347,    68,
     348,   349,    69,    70,    71,    72,    66,   350,   351,   353,
     266,   358,   364,   359,   365,   366,     8,    73,   104,   367,
     196,    49,    74,    67,   368,    68,   369,   370,    69,    70,
      71,    72,     2,   372,    65,     3,     4,     5,     6,     7,
     380,   387,     8,    73,   104,   394,   269,    49,    74,    66,
     401,   405,   396,   397,   410,   400,   412,   414,     2,   415,
      65,     3,     4,     5,     6,     7,    67,   416,    68,   418,
     426,    69,    70,    71,    72,    66,   427,   431,   432,   437,
     439,   119,   446,   445,   444,     8,    73,   104,   452,   270,
      49,    74,    67,   453,    68,   455,   456,    69,    70,    71,
      72,     2,   458,    65,     3,     4,     5,     6,     7,   460,
     462,     8,    73,   104,   464,   271,    49,    74,    66,   466,
      22,   225,   407,   304,   430,   305,   355,     2,   385,    65,
       3,     4,     5,     6,     7,    67,   227,    68,   306,   389,
      69,    70,    71,    72,    66,   307,     0,   308,     0,     0,
       0,     0,     0,     0,     8,    73,   104,     0,   272,    49,
      74,    67,     0,    68,     0,     0,    69,    70,    71,    72,
       2,     0,    65,     3,     4,     5,     6,     7,     0,     0,
       8,    73,    64,     0,     0,    49,    74,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     2,     0,    65,     3,
       4,     5,     6,     7,    67,     0,    68,     0,     0,    69,
      70,    71,    72,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     8,    73,   104,     0,     0,    49,    74,
      67,     0,    68,     0,     0,    69,    70,    71,    72,     2,
       0,    65,     3,     4,     5,     6,     7,     0,     0,     8,
      73,   104,     0,     0,    49,    74,    66,     0,     0,     0,
       0,     0,     0,     0,     0,     2,     0,    65,     3,     4,
       5,     6,     7,    67,     0,    68,     0,     0,    69,    70,
      71,    72,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     8,    73,     0,     0,     0,     0,    74,    67,
     200,    68,     0,     0,   447,    70,    71,    72,     0,     0,
       0,   136,     0,     0,     0,   137,   138,     0,     8,    73,
       0,     0,     0,   218,    74,     0,   139,   140,   141,   142,
     143,     0,   144,   201,   136,     0,     0,     0,   137,   138,
       0,     0,     0,     0,     0,     0,     0,   145,     0,   139,
     140,   141,   142,   143,     0,   144,     0,   146,     0,   147,
     148,   149,   150,   151,     0,     0,     0,     0,     0,     0,
     145,     0,   152,     0,     0,     0,   221,     0,     0,     0,
     146,     0,   147,   148,   149,   150,   151,   136,     0,     0,
       0,   137,   138,     0,     0,   152,     0,     0,   337,     0,
       0,    45,   139,   140,   141,   142,   143,     0,   144,   136,
       0,     0,     0,   137,   138,     2,     0,     0,     3,     4,
       5,     6,     7,   145,   139,   140,   141,   142,   143,     0,
     144,     0,     0,   146,    46,   147,   148,   149,   150,   151,
       0,     0,     0,     0,     0,   145,     0,     0,   152,     0,
       0,     0,     0,     0,   380,   146,     0,   147,   148,   149,
     150,   151,   136,     0,     0,     0,   137,   138,     0,     0,
     152,     0,     0,     0,     0,     0,     0,   139,   140,   141,
     142,   143,     0,   144,     0,     0,     0,     0,     0,     0,
       0,     0,   114,     0,  -151,     0,   115,  -151,   145,  -151,
    -151,  -151,  -151,  -151,  -151,  -151,     0,     0,   146,     0,
     147,   148,   149,   150,   151,  -151,  -151,  -151,     0,     0,
       0,     0,     0,   152,   119,  -151,  -151,  -151,  -151,  -151,
    -151,  -151,  -151,  -151,  -151,  -151,  -151,  -151,  -151,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     229,     0,     0,     0,     0,     0,   136,     0,   230,     2,
     137,   138,     3,     4,     5,     6,     7,     0,     0,     0,
       0,   139,   140,   141,   142,   143,     0,   144,   136,     0,
       0,     0,   137,   138,     0,     0,     0,     0,     0,     0,
       0,     0,   145,   139,   215,   141,   142,   143,     0,   144,
     216,     0,   146,     0,   147,   148,   149,   150,   151,     0,
       0,     0,     0,     0,   145,     0,   136,   152,     0,     0,
     137,   138,     0,     0,   146,     0,   147,   148,   149,   150,
     151,   139,   140,   141,   142,   143,     0,   144,     0,   152,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   146,     0,   147,   148,   149,   150,   151,     0,
       0,     0,     0,     0,     0,     0,     0,   152
};

static const yytype_int16 yycheck[] =
{
       0,    35,    11,    17,     1,   380,     1,    57,    75,     5,
     136,    61,    12,    49,    48,     4,     1,     3,    52,   145,
       3,    57,    49,     3,     3,    61,     3,    23,    28,     3,
      57,    31,     3,    33,    61,     0,     3,    33,   105,    75,
      26,    24,   109,    73,    38,    39,    26,    26,    75,    26,
      59,    51,    26,   103,     5,    26,   106,     3,   108,    26,
      49,   111,     1,    17,   439,     4,    17,   103,   102,   105,
     106,    68,   108,   109,    74,   111,   103,    72,   105,   106,
      33,   108,   109,   458,   111,   115,    71,     6,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
       5,   131,   132,   133,   134,     1,    52,    53,     5,     5,
       5,     1,    17,    28,   144,     5,    31,     5,    33,    33,
      17,    17,    17,    31,   191,    33,   193,    17,   195,    17,
     197,   257,   258,   259,    33,   135,    51,     4,    40,    41,
       7,     1,    23,     4,   144,     5,     7,   177,     0,     4,
      17,     5,     7,    13,    14,   191,    17,   193,     1,   195,
      12,   197,     1,    23,   191,     4,   193,     4,   195,   295,
     197,     1,    15,    33,    17,    18,    19,    20,    21,    22,
      33,     0,    11,    12,   214,    15,    13,    14,    18,    19,
      20,    21,    22,    12,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    26,   235,   236,   237,   238,   239,
     240,    34,    34,    24,    25,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    26,    26,    26,    26,    65,
      66,    74,    17,    34,    34,    34,    34,   267,    68,    34,
      70,   249,   250,   251,   252,   275,   280,   281,   278,   283,
     284,     1,     4,     3,     4,   285,   286,   287,    52,    53,
     290,   291,   253,   254,   294,    15,     6,    17,    18,    19,
      20,    21,    22,    34,    15,     4,     1,    18,    19,    20,
      21,    22,    32,    42,    43,    44,    45,     8,     9,    10,
      15,    34,   322,    18,    19,    20,    21,    22,     4,    49,
       4,    51,    52,    53,    54,    55,    56,    57,     5,    34,
     247,   248,     4,   343,   255,   256,    33,    33,    68,    69,
     350,   351,    33,    33,    74,   359,    33,    17,    24,    24,
     360,    24,    23,   367,   364,   365,   366,   387,    24,   369,
      24,    24,    24,    24,    24,    24,   413,    24,    24,     6,
       5,   387,    67,    47,    59,    33,    17,   357,    33,    33,
     387,    33,   412,    33,   394,   401,   433,    33,   398,    36,
      24,    35,    37,    46,     6,     6,   412,   413,   378,    23,
     414,   415,   416,    34,   418,   412,   413,    34,    34,    34,
       1,    34,    34,    34,    24,   425,   426,   433,    24,     1,
      33,     3,     4,   437,    15,    33,   433,    18,    19,    20,
      21,    22,    34,    15,   444,    17,    18,    19,    20,    21,
      22,   421,   452,    34,    34,     1,    65,     3,     4,     6,
      32,    23,     6,     4,   464,    23,     6,    33,    23,    15,
      34,    17,    18,    19,    20,    21,    22,    49,    34,    51,
      34,    34,    54,    55,    56,    57,    32,    26,    26,     6,
       5,    34,    24,    50,    24,    24,    68,    69,     1,    34,
       3,     4,    74,    49,    17,    51,    24,    34,    54,    55,
      56,    57,    15,    34,    17,    18,    19,    20,    21,    22,
       4,    25,    68,    69,     1,    25,     3,     4,    74,    32,
      23,    67,    34,    34,     5,    34,    25,    34,    15,    34,
      17,    18,    19,    20,    21,    22,    49,    34,    51,    34,
       5,    54,    55,    56,    57,    32,    66,    27,     5,    34,
       4,    33,    27,     6,    33,    68,    69,     1,     5,     3,
       4,    74,    49,     6,    51,     6,    24,    54,    55,    56,
      57,    15,     4,    17,    18,    19,    20,    21,    22,    34,
       6,    68,    69,     1,     5,     3,     4,    74,    32,     6,
      12,   135,   380,   242,   409,   243,   325,    15,   359,    17,
      18,    19,    20,    21,    22,    49,   135,    51,   244,   362,
      54,    55,    56,    57,    32,   245,    -1,   246,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    69,     1,    -1,     3,     4,
      74,    49,    -1,    51,    -1,    -1,    54,    55,    56,    57,
      15,    -1,    17,    18,    19,    20,    21,    22,    -1,    -1,
      68,    69,     1,    -1,    -1,     4,    74,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    17,    18,
      19,    20,    21,    22,    49,    -1,    51,    -1,    -1,    54,
      55,    56,    57,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    69,     1,    -1,    -1,     4,    74,
      49,    -1,    51,    -1,    -1,    54,    55,    56,    57,    15,
      -1,    17,    18,    19,    20,    21,    22,    -1,    -1,    68,
      69,     1,    -1,    -1,     4,    74,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    17,    18,    19,
      20,    21,    22,    49,    -1,    51,    -1,    -1,    54,    55,
      56,    57,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,    49,
       1,    51,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    16,    17,    -1,    68,    69,
      -1,    -1,    -1,     1,    74,    -1,    27,    28,    29,    30,
      31,    -1,    33,    34,    12,    -1,    -1,    -1,    16,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    27,
      28,    29,    30,    31,    -1,    33,    -1,    58,    -1,    60,
      61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    73,    -1,    -1,    -1,     1,    -1,    -1,    -1,
      58,    -1,    60,    61,    62,    63,    64,    12,    -1,    -1,
      -1,    16,    17,    -1,    -1,    73,    -1,    -1,     1,    -1,
      -1,     1,    27,    28,    29,    30,    31,    -1,    33,    12,
      -1,    -1,    -1,    16,    17,    15,    -1,    -1,    18,    19,
      20,    21,    22,    48,    27,    28,    29,    30,    31,    -1,
      33,    -1,    -1,    58,    34,    60,    61,    62,    63,    64,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,     4,    58,    -1,    60,    61,    62,
      63,    64,    12,    -1,    -1,    -1,    16,    17,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,
      30,    31,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,    -1,     5,     6,    48,     8,
       9,    10,    11,    12,    13,    14,    -1,    -1,    58,    -1,
      60,    61,    62,    63,    64,    24,    25,    26,    -1,    -1,
      -1,    -1,    -1,    73,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    12,    -1,    67,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    27,    28,    29,    30,    31,    -1,    33,    12,    -1,
      -1,    -1,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    58,    -1,    60,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    12,    73,    -1,    -1,
      16,    17,    -1,    -1,    58,    -1,    60,    61,    62,    63,
      64,    27,    28,    29,    30,    31,    -1,    33,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    60,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    15,    18,    19,    20,    21,    22,    68,    70,
      87,    88,    89,    90,   108,   111,   113,   114,    17,     1,
      71,     0,    90,   108,   111,    17,     5,    17,    33,     1,
      68,    33,     6,    33,     1,    34,   112,   113,   114,     1,
      72,     1,    34,   112,     5,     1,    34,   112,    34,     4,
     132,    26,    34,    17,    17,    33,    34,     4,    34,     6,
      34,     4,    34,   132,     1,    17,    32,    49,    51,    54,
      55,    56,    57,    69,    74,    91,    92,    93,    94,    95,
      96,    99,   100,   105,   107,   108,   109,   111,   114,   115,
     131,   132,   133,   134,   135,   136,   137,   140,   141,   113,
     114,   132,    34,     4,     1,    91,     4,     5,     4,    91,
       1,     4,     3,    24,     1,     5,    13,    14,    23,    33,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
     106,    33,    33,    33,    33,    33,    12,    16,    17,    27,
      28,    29,    30,    31,    33,    48,    58,    60,    61,    62,
      63,    64,    73,   105,   109,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   114,
       3,    92,    24,    24,    24,    24,    24,    23,    24,     1,
       5,    17,    24,    24,    24,    24,    24,    17,    17,     1,
     132,    91,     3,    91,     6,    91,     3,    91,   116,   116,
       1,    34,   110,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,     5,    28,    34,   116,     1,   116,
     116,     1,   116,     1,    17,    95,   114,   115,   127,    59,
      67,    59,   114,   116,   127,    33,    33,    33,    33,    33,
      33,    67,    47,    46,    36,    37,    35,    40,    41,    42,
      43,    44,    45,    38,    39,    11,    12,     8,     9,    10,
      13,    14,    17,   116,    24,     6,     5,    23,   103,     3,
       3,     3,     3,     6,    34,    26,    34,   116,    11,    34,
      34,    34,    34,    34,    34,    24,    24,    25,     5,    17,
      24,    33,    65,    66,    33,    34,    34,   116,   116,   116,
     116,   116,   116,    65,   118,   119,   120,   121,   122,   123,
     123,   124,   124,   124,   124,   125,   125,   126,   126,   127,
     127,   127,    23,     5,    17,     6,   116,    23,   116,     6,
     116,   132,   132,     4,   132,   132,   116,     1,   116,   116,
       6,   116,   116,    33,   116,   127,    34,    34,    34,    34,
      26,    26,   116,     6,    23,   103,     4,     7,    34,    50,
      52,    53,   138,   139,    24,    24,    24,    34,    17,    24,
      34,   116,    34,   116,   130,   116,    17,     4,     7,    17,
       4,   101,   102,   114,   132,   136,   116,    25,     3,   139,
     116,   116,   116,   132,    25,   116,    34,    34,    26,    34,
      34,    23,    97,   116,   114,    67,    97,   101,     3,    26,
       5,   104,    25,    91,    34,    34,    34,   116,    34,   116,
       4,     7,    17,   109,     3,    26,     5,    66,     3,     3,
     102,    27,     5,    91,   132,   132,   132,    34,   132,     4,
      98,   114,   116,   116,    33,     6,    27,    54,   132,    97,
       3,    26,     5,     6,   116,     6,    24,     3,     4,   116,
      34,    97,     6,     3,     5,   116,     6
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    86,    87,    88,    88,    89,    89,    89,    89,    89,
      90,    90,    90,    90,    90,    90,    91,    91,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    93,    93,    94,
      94,    95,    95,    95,    95,    96,    96,    96,    97,    97,
      98,    98,    99,    99,    99,   100,   100,   101,   101,   102,
     102,   103,   103,   104,   104,   105,   105,   106,   106,   107,
     108,   108,   108,   109,   109,   109,   110,   110,   111,   111,
     111,   111,   111,   111,   111,   112,   112,   112,   112,   113,
     113,   113,   113,   114,   114,   114,   114,   114,   114,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     116,   117,   117,   118,   118,   119,   119,   120,   120,   121,
     121,   122,   122,   122,   123,   123,   123,   123,   123,   124,
     124,   124,   125,   125,   125,   126,   126,   126,   126,   127,
     127,   127,   127,   128,   128,   128,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   130,
     130,   131,   131,   131,   132,   132,   133,   134,   135,   136,
     136,   136,   136,   137,   138,   138,   139,   139,   140,   140,
     141,   141,   141,   141,   141,   141
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     2,     1,     1,     0,
       7,     7,     4,     3,     2,     1,     2,     1,     2,     2,
       2,     2,     1,     1,     1,     1,     2,     2,     2,     1,
       2,     1,     2,     2,     2,     1,     2,     1,     1,     2,
       2,     4,     2,     5,     3,    10,     8,    11,     3,     1,
       5,     3,    15,    10,     8,     7,     7,     3,     1,     3,
       3,     3,     2,     3,     4,     2,     2,     4,     3,     3,
       5,     6,     6,     3,     4,     4,     3,     1,     8,     8,
       6,     8,     8,     7,     7,     4,     4,     2,     2,     1,
       3,     5,     7,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     2,
       2,     4,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     4,     4,     6,     3,     6,
       5,     5,     6,     6,     3,     1,     1,     1,     3,     3,
       1,     4,     6,     3,     3,     3,     1,     1,     2,     5,
       7,     7,     5,     7,     2,     1,     6,     3,     5,     5,
       9,     9,     7,    10,     9,     9
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* s: programa  */
#line 112 "src/parser/parser.y"
             { astGlobal = (yyvsp[0].nodo); }
#line 2066 "src/parser/parser.tab.c"
    break;

  case 3: /* programa: lista_funciones main_class  */
#line 116 "src/parser/parser.y"
                                 {
          NodoAST* raiz = crearNodo(AST_PROGRAMA, "Programa");
          agregarHijo(raiz, (yyvsp[-1].nodo));
          agregarHijo(raiz, (yyvsp[0].nodo));
          (yyval.nodo) = raiz;
      }
#line 2077 "src/parser/parser.tab.c"
    break;

  case 4: /* programa: main_class  */
#line 122 "src/parser/parser.y"
                 {
          NodoAST* raiz = crearNodo(AST_PROGRAMA, "Programa");
          agregarHijo(raiz, (yyvsp[0].nodo));
          (yyval.nodo) = raiz;
      }
#line 2087 "src/parser/parser.tab.c"
    break;

  case 5: /* lista_funciones: lista_funciones Funcion_void  */
#line 130 "src/parser/parser.y"
                                   {
          if ((yyvsp[-1].nodo)->tipo != AST_LISTA) {
              NodoAST* lista = crearNodo(AST_LISTA, "Funciones");
              agregarHijo(lista, (yyvsp[-1].nodo));
              agregarHijo(lista, (yyvsp[0].nodo));
              (yyval.nodo) = lista;
          } else {
              agregarHijo((yyvsp[-1].nodo), (yyvsp[0].nodo));
              (yyval.nodo) = (yyvsp[-1].nodo);
          }
      }
#line 2103 "src/parser/parser.tab.c"
    break;

  case 6: /* lista_funciones: lista_funciones Funcion_parametros  */
#line 141 "src/parser/parser.y"
                                         {
          if ((yyvsp[-1].nodo)->tipo != AST_LISTA) {
              NodoAST* lista = crearNodo(AST_LISTA, "Funciones");
              agregarHijo(lista, (yyvsp[-1].nodo));
              agregarHijo(lista, (yyvsp[0].nodo));
              (yyval.nodo) = lista;
          } else {
              agregarHijo((yyvsp[-1].nodo), (yyvsp[0].nodo));
              (yyval.nodo) = (yyvsp[-1].nodo);
          }
      }
#line 2119 "src/parser/parser.tab.c"
    break;

  case 7: /* lista_funciones: Funcion_void  */
#line 152 "src/parser/parser.y"
                   {
          NodoAST* lista = crearNodo(AST_LISTA, "Funciones");
          agregarHijo(lista, (yyvsp[0].nodo));
          (yyval.nodo) = lista;
      }
#line 2129 "src/parser/parser.tab.c"
    break;

  case 8: /* lista_funciones: Funcion_parametros  */
#line 157 "src/parser/parser.y"
                         {
          NodoAST* lista = crearNodo(AST_LISTA, "Funciones");
          agregarHijo(lista, (yyvsp[0].nodo));
          (yyval.nodo) = lista;
      }
#line 2139 "src/parser/parser.tab.c"
    break;

  case 9: /* lista_funciones: %empty  */
#line 162 "src/parser/parser.y"
                   {
          (yyval.nodo) = crearNodo(AST_LISTA, "Funciones");
      }
#line 2147 "src/parser/parser.tab.c"
    break;

  case 10: /* main_class: TOKEN_PUBLIC TOKEN_STATIC TOKEN_VOID TOKEN_main TOKEN_PARIZQ TOKEN_PARDER bloque  */
#line 168 "src/parser/parser.y"
                                                                                     {
        if (strcmp((yyvsp[-3].str), "main") != 0) {
            addErrorSintactico("El método principal debe llamarse 'main'", "global", (yylsp[-3]).first_line, (yylsp[-3]).first_column);
            (yyval.nodo) = NULL;
        } else {
            (yyval.nodo) = crearNodo(AST_METODO_MAIN, "public static void main");
            (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
            agregarHijo((yyval.nodo), (yyvsp[0].nodo)); // Agregar el bloque como hijo
        }
    }
#line 2162 "src/parser/parser.tab.c"
    break;

  case 11: /* main_class: TOKEN_PUBLIC TOKEN_STATIC TOKEN_VOID TOKEN_main TOKEN_PARIZQ TOKEN_PARDER error  */
#line 178 "src/parser/parser.y"
                                                                                      {
        addErrorSintactico("Error en el bloque del método 'main'", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = crearNodo(AST_METODO_MAIN, "public static void main");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        NodoAST* bloqueVacio = crearNodo(AST_BLOQUE, "Bloque vacío");
        agregarHijo((yyval.nodo), bloqueVacio);
    }
#line 2174 "src/parser/parser.tab.c"
    break;

  case 12: /* main_class: TOKEN_PUBLIC TOKEN_STATIC TOKEN_VOID error  */
#line 185 "src/parser/parser.y"
                                                 {
        addErrorSintactico("Falta el nombre del método principal 'main'", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = NULL;
    }
#line 2183 "src/parser/parser.tab.c"
    break;

  case 13: /* main_class: TOKEN_PUBLIC TOKEN_STATIC error  */
#line 189 "src/parser/parser.y"
                                      {
        addErrorSintactico("Falta el tipo de retorno 'void' en el método principal", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = NULL;
    }
#line 2192 "src/parser/parser.tab.c"
    break;

  case 14: /* main_class: TOKEN_PUBLIC error  */
#line 193 "src/parser/parser.y"
                         {
        addErrorSintactico("Falta la palabra clave 'static' en el método principal", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = NULL;
    }
#line 2201 "src/parser/parser.tab.c"
    break;

  case 15: /* main_class: error  */
#line 197 "src/parser/parser.y"
            {
        addErrorSintactico("Error en la declaración del método principal", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = NULL;
    }
#line 2210 "src/parser/parser.tab.c"
    break;

  case 16: /* lista_sentencias: lista_sentencias sentencia  */
#line 206 "src/parser/parser.y"
                               { 
        if ((yyvsp[-1].nodo)->tipo != AST_LISTA) {
            NodoAST* lista = crearNodo(AST_LISTA, "Programa");
            agregarHijo(lista, (yyvsp[-1].nodo));
            if ((yyvsp[0].nodo)) agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            if ((yyvsp[0].nodo)) agregarHijo((yyvsp[-1].nodo), (yyvsp[0].nodo));
            (yyval.nodo) = (yyvsp[-1].nodo);
        }
    }
#line 2226 "src/parser/parser.tab.c"
    break;

  case 17: /* lista_sentencias: sentencia  */
#line 217 "src/parser/parser.y"
                { 
        if ((yyvsp[0].nodo)) {
            (yyval.nodo) = (yyvsp[0].nodo);
        } else {
            (yyval.nodo) = crearNodo(AST_LISTA, "Programa");
        }
    }
#line 2238 "src/parser/parser.tab.c"
    break;

  case 18: /* sentencia: declaracion TOKEN_PUNTOCOMA  */
#line 228 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2244 "src/parser/parser.tab.c"
    break;

  case 19: /* sentencia: asignacion TOKEN_PUNTOCOMA  */
#line 229 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2250 "src/parser/parser.tab.c"
    break;

  case 20: /* sentencia: acceso_escritura TOKEN_PUNTOCOMA  */
#line 230 "src/parser/parser.y"
                                       { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2256 "src/parser/parser.tab.c"
    break;

  case 21: /* sentencia: instruccion_print TOKEN_PUNTOCOMA  */
#line 231 "src/parser/parser.y"
                                        { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2262 "src/parser/parser.tab.c"
    break;

  case 22: /* sentencia: instruccion_if  */
#line 232 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2268 "src/parser/parser.tab.c"
    break;

  case 23: /* sentencia: intruccion_switch  */
#line 233 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2274 "src/parser/parser.tab.c"
    break;

  case 24: /* sentencia: instruccion_while  */
#line 234 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2280 "src/parser/parser.tab.c"
    break;

  case 25: /* sentencia: instruccion_for  */
#line 235 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2286 "src/parser/parser.tab.c"
    break;

  case 26: /* sentencia: declaracion_vector TOKEN_PUNTOCOMA  */
#line 236 "src/parser/parser.y"
                                              { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2292 "src/parser/parser.tab.c"
    break;

  case 27: /* sentencia: declaracion_matriz TOKEN_PUNTOCOMA  */
#line 237 "src/parser/parser.y"
                                              { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2298 "src/parser/parser.tab.c"
    break;

  case 28: /* sentencia: declaracion_array_multidimensional TOKEN_PUNTOCOMA  */
#line 238 "src/parser/parser.y"
                                                         { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2304 "src/parser/parser.tab.c"
    break;

  case 29: /* sentencia: Funcion_void  */
#line 239 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2310 "src/parser/parser.tab.c"
    break;

  case 30: /* sentencia: instruccion_return TOKEN_PUNTOCOMA  */
#line 240 "src/parser/parser.y"
                                         { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2316 "src/parser/parser.tab.c"
    break;

  case 31: /* sentencia: Funcion_parametros  */
#line 241 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2322 "src/parser/parser.tab.c"
    break;

  case 32: /* sentencia: instruccion_break TOKEN_PUNTOCOMA  */
#line 242 "src/parser/parser.y"
                                        {(yyval.nodo) = (yyvsp[-1].nodo); }
#line 2328 "src/parser/parser.tab.c"
    break;

  case 33: /* sentencia: expresion_sentencia TOKEN_PUNTOCOMA  */
#line 243 "src/parser/parser.y"
                                              { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2334 "src/parser/parser.tab.c"
    break;

  case 34: /* sentencia: instruccion_continue TOKEN_PUNTOCOMA  */
#line 244 "src/parser/parser.y"
                                           {(yyval.nodo) = (yyvsp[-1].nodo); }
#line 2340 "src/parser/parser.tab.c"
    break;

  case 35: /* sentencia: bloque  */
#line 245 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2346 "src/parser/parser.tab.c"
    break;

  case 36: /* sentencia: error TOKEN_PUNTOCOMA  */
#line 247 "src/parser/parser.y"
                                    { 
        addErrorSintactico("Instrucción inválida", "global", (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        yyerrok; 
        (yyval.nodo) = NULL;
    }
#line 2356 "src/parser/parser.tab.c"
    break;

  case 39: /* postfix_incremento: TOKEN_IDENTIFICADOR TOKEN_INCREMENTO  */
#line 260 "src/parser/parser.y"
                                           {
          (yyval.nodo) = crearNodo(AST_INCREMENTO, "++");
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
          agregarHijo((yyval.nodo), crearNodo(AST_IDENTIFICADOR, (yyvsp[-1].str)));
      }
#line 2366 "src/parser/parser.tab.c"
    break;

  case 40: /* postfix_incremento: TOKEN_IDENTIFICADOR TOKEN_DECREMENTO  */
#line 265 "src/parser/parser.y"
                                           {
          (yyval.nodo) = crearNodo(AST_DECREMENTO, "--");
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
          agregarHijo((yyval.nodo), crearNodo(AST_IDENTIFICADOR, (yyvsp[-1].str)));
      }
#line 2376 "src/parser/parser.tab.c"
    break;

  case 41: /* declaracion: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_IGUAL expresion  */
#line 274 "src/parser/parser.y"
                                                             {
        (yyval.nodo) = crearNodo(AST_DECLARACION, "declaracion");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-3].str)));
        
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2391 "src/parser/parser.tab.c"
    break;

  case 42: /* declaracion: tipo_primitivo TOKEN_IDENTIFICADOR  */
#line 284 "src/parser/parser.y"
                                         {
        (yyval.nodo) = crearNodo(AST_DECLARACION, "declaracion");
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-1].str)));
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
        id->linea = (yylsp[0]).first_line; id->columna = (yylsp[0]).first_column;
        agregarHijo((yyval.nodo), id);
    }
#line 2404 "src/parser/parser.tab.c"
    break;

  case 43: /* declaracion: TOKEN_FINAL tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_IGUAL expresion  */
#line 292 "src/parser/parser.y"
                                                                           {
        (yyval.nodo) = crearNodo(AST_DECLARACION_CONST, "declaracion_const");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-3].str)));
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2418 "src/parser/parser.tab.c"
    break;

  case 44: /* declaracion: tipo_primitivo error TOKEN_PUNTOCOMA  */
#line 301 "src/parser/parser.y"
                                           {
        addErrorSintactico("Error en declaración", "global", (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        yyerrok;
        (yyval.nodo) = NULL;
    }
#line 2428 "src/parser/parser.tab.c"
    break;

  case 45: /* declaracion_vector: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_NEW tipo_primitivo TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER  */
#line 309 "src/parser/parser.y"
                                                                                                                                                              {
        (yyval.nodo) = crearNodo(AST_DECLARACION_VECTOR, "Declaración Vector");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-9].str)));
        
        // Crea el nodo identificador con línea y columna
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-6].str));
        id->linea = (yylsp[-6]).first_line; 
        id->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), id);

        NodoAST* id2 = crearNodo(AST_TIPO, (yyvsp[-3].str));
        id2->linea = (yylsp[-3]).first_line; 
        id2->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), id2);
        
        NodoAST* dimension = crearNodo(AST_1DIMENSION, "Dimensión");
        agregarHijo(dimension, (yyvsp[-1].nodo));
        agregarHijo((yyval.nodo), dimension);
    }
#line 2453 "src/parser/parser.tab.c"
    break;

  case 46: /* declaracion_vector: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 329 "src/parser/parser.y"
                                                                                                                                       {
        (yyval.nodo) = crearNodo(AST_DECLARACION_VECTOR, "Declaración Vector");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-7].str)));
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str));
        id->linea = (yylsp[-4]).first_line; id->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), id);
        NodoAST* lista = crearNodo(AST_LISTA_ELEMENTOS, "Lista Elementos");
        agregarHijo(lista, (yyvsp[-1].nodo));
        agregarHijo((yyval.nodo), lista);
    }
#line 2469 "src/parser/parser.tab.c"
    break;

  case 47: /* declaracion_vector: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_IDENTIFICADOR TOKEN_PUNTO TOKEN_ADD TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 340 "src/parser/parser.y"
                                                                                                                                                                       {
        (yyval.nodo) = crearNodo(AST_DECLARACION_VECTOR, "Declaración Vector");
        (yyval.nodo)->linea = (yylsp[-7]).first_line; (yyval.nodo)->columna = (yylsp[-7]).first_column;
        
        // Tipo declarado
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-10].str)));
        
        // Identificador del nuevo vector
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-7].str));
        id->linea = (yylsp[-7]).first_line; id->columna = (yylsp[-7]).first_column;
        agregarHijo((yyval.nodo), id);
        
        // Crear el nodo para la operación add
        NodoAST* addNode = crearNodo(AST_ADD, "Array.add");
        addNode->linea = (yylsp[-4]).first_line; addNode->columna = (yylsp[-4]).first_column;
        
        // Identificador del vector origen
        NodoAST* idOrig = crearNodo(AST_IDENTIFICADOR, (yyvsp[-5].str));
        idOrig->linea = (yylsp[-5]).first_line; idOrig->columna = (yylsp[-5]).first_column;
        agregarHijo(addNode, idOrig);
        
        // Valor a añadir
        agregarHijo(addNode, (yyvsp[-1].nodo));
        
        // Añadir el nodo add como lista de elementos
        NodoAST* lista = crearNodo(AST_LISTA_ELEMENTOS, "Lista Elementos");
        agregarHijo(lista, addNode);
        agregarHijo((yyval.nodo), lista);
    }
#line 2503 "src/parser/parser.tab.c"
    break;

  case 48: /* lista_elementos: lista_elementos TOKEN_COMA expresion  */
#line 372 "src/parser/parser.y"
                                         {
        if ((yyvsp[-2].nodo)->tipo != AST_INTERNO) {
            NodoAST* lista = crearNodo(AST_INTERNO, "Lista Elementos");
            agregarHijo(lista, (yyvsp[-2].nodo));
            agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            agregarHijo((yyvsp[-2].nodo), (yyvsp[0].nodo));
            (yyval.nodo) = (yyvsp[-2].nodo);
        }
    }
#line 2519 "src/parser/parser.tab.c"
    break;

  case 49: /* lista_elementos: expresion  */
#line 383 "src/parser/parser.y"
                {
        (yyval.nodo) = crearNodo(AST_INTERNO, "Lista Elementos");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2528 "src/parser/parser.tab.c"
    break;

  case 50: /* lista_filas: lista_filas TOKEN_COMA TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 390 "src/parser/parser.y"
                                                                         {
        if ((yyvsp[-4].nodo)->tipo != AST_LISTA_ELEMENTOS) {
            NodoAST* lista = crearNodo(AST_LISTA_ELEMENTOS, "Lista Filas");
            agregarHijo(lista, (yyvsp[-4].nodo));
            agregarHijo(lista, (yyvsp[-1].nodo));
            (yyval.nodo) = lista;
        } else {
            agregarHijo((yyvsp[-4].nodo), (yyvsp[-1].nodo));
            (yyval.nodo) = (yyvsp[-4].nodo);
        }
    }
#line 2544 "src/parser/parser.tab.c"
    break;

  case 51: /* lista_filas: TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 401 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crearNodo(AST_LISTA_ELEMENTOS, "Lista Filas");
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2553 "src/parser/parser.tab.c"
    break;

  case 52: /* declaracion_matriz: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_NEW tipo_primitivo TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER  */
#line 408 "src/parser/parser.y"
                                                                                                                                                                                                                                                {
        (yyval.nodo) = crearNodo(AST_DECLARACION_MATRIZ, "Declaración Matriz");
        (yyval.nodo)->linea = (yylsp[-9]).first_line; (yyval.nodo)->columna = (yylsp[-9]).first_column;
        
        // Tipo declarado
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-14].str));
        tipo->linea = (yylsp[-14]).first_line; tipo->columna = (yylsp[-14]).first_column;
        agregarHijo((yyval.nodo), tipo);
        
        // Identificador
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-9].str));
        id->linea = (yylsp[-9]).first_line; id->columna = (yylsp[-9]).first_column;
        agregarHijo((yyval.nodo), id);
        
        // Tipo en new
        NodoAST* tipoNew = crearNodo(AST_TIPO, (yyvsp[-6].str));
        tipoNew->linea = (yylsp[-6]).first_line; tipoNew->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), tipoNew);
        
        NodoAST* dimension = crearNodo(AST_DIMENSIONES, "Dimensiones");
        agregarHijo(dimension, (yyvsp[-4].nodo));
        agregarHijo(dimension, (yyvsp[-1].nodo));
        agregarHijo((yyval.nodo), dimension);
    }
#line 2582 "src/parser/parser.tab.c"
    break;

  case 53: /* declaracion_matriz: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_LLAVEIZQ lista_filas TOKEN_LLAVEDER  */
#line 432 "src/parser/parser.y"
                                                                                                                                                                       {
        (yyval.nodo) = crearNodo(AST_DECLARACION_MATRIZ, "Declaración Matriz");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        
        // Tipo declarado
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-9].str));
        tipo->linea = (yylsp[-9]).first_line; tipo->columna = (yylsp[-9]).first_column;
        agregarHijo((yyval.nodo), tipo);
        
        // Identificador
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str));
        id->linea = (yylsp[-4]).first_line; id->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), id);
        
        // Lista de elementos
        NodoAST* lista = crearNodo(AST_LISTA_ELEMENTOS, "Lista Elementos");
        agregarHijo(lista, (yyvsp[-1].nodo));
        agregarHijo((yyval.nodo), lista);
    }
#line 2606 "src/parser/parser.tab.c"
    break;

  case 54: /* declaracion_matriz: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL Llamadas  */
#line 451 "src/parser/parser.y"
                                                                                                                                      {
        // ✓ NUEVA REGLA: Declaración de matriz con resultado de función
        (yyval.nodo) = crearNodo(AST_DECLARACION_MATRIZ, "Declaración Matriz");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        
        // Tipo declarado
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-7].str));
        tipo->linea = (yylsp[-7]).first_line; tipo->columna = (yylsp[-7]).first_column;
        agregarHijo((yyval.nodo), tipo);
        
        // Identificador
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
        
        // Llamada a función (resultado de la función)
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2629 "src/parser/parser.tab.c"
    break;

  case 55: /* declaracion_array_multidimensional: tipo_primitivo TOKEN_IDENTIFICADOR DimensionesCorchetes TOKEN_IGUAL TOKEN_NEW tipo_primitivo Dimensiones  */
#line 472 "src/parser/parser.y"
                                                                                                             {
        (yyval.nodo) = crearNodo(AST_DECLARACION_ARRAY_MULTIDIMENSIONAL, "Declaración Array Multidimensional");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;

        // Tipo declarado
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-6].str));
        tipo->linea = (yylsp[-6]).first_line; tipo->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), tipo);

        // Identificador
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-5].str));
        id->linea = (yylsp[-5]).first_line; id->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), id);

        // Tipo en new
        NodoAST* tipoNew = crearNodo(AST_TIPO, (yyvsp[-1].str));
        tipoNew->linea = (yylsp[-1]).first_line; tipoNew->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), tipoNew);

        // Dimensiones explícitas
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));

        // Validación de consistencia entre corchetes y dimensiones
        if ((yyvsp[-4].nodo)->num_hijos != (yyvsp[0].nodo)->num_hijos) {
            addErrorSintactico("El número de corchetes no coincide con las dimensiones especificadas", "global", (yylsp[-5]).first_line, (yylsp[-5]).first_column);
        }
    }
#line 2661 "src/parser/parser.tab.c"
    break;

  case 56: /* declaracion_array_multidimensional: tipo_primitivo TOKEN_IDENTIFICADOR DimensionesCorchetes TOKEN_IGUAL TOKEN_LLAVEIZQ lista_bloques TOKEN_LLAVEDER  */
#line 499 "src/parser/parser.y"
                                                                                                                      {
        (yyval.nodo) = crearNodo(AST_DECLARACION_ARRAY_MULTIDIMENSIONAL, "Declaración Array Multidimensional");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;

        // Tipo declarado
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-6].str));
        tipo->linea = (yylsp[-6]).first_line; tipo->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), tipo);

        // Identificador
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-5].str));
        id->linea = (yylsp[-5]).first_line; id->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), id);

        // Lista de bloques anidados
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2683 "src/parser/parser.tab.c"
    break;

  case 57: /* lista_bloques: lista_bloques TOKEN_COMA lista_elementos_o_bloques  */
#line 519 "src/parser/parser.y"
                                                       {
        if ((yyvsp[-2].nodo)->tipo != AST_INTERNO) {
            NodoAST* lista = crearNodo(AST_INTERNO, "Lista Bloques");
            agregarHijo(lista, (yyvsp[-2].nodo));
            agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            agregarHijo((yyvsp[-2].nodo), (yyvsp[0].nodo));
            (yyval.nodo) = (yyvsp[-2].nodo);
        }
    }
#line 2699 "src/parser/parser.tab.c"
    break;

  case 58: /* lista_bloques: lista_elementos_o_bloques  */
#line 530 "src/parser/parser.y"
                                {
        (yyval.nodo) = crearNodo(AST_INTERNO, "Lista Bloques");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2708 "src/parser/parser.tab.c"
    break;

  case 59: /* lista_elementos_o_bloques: TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 537 "src/parser/parser.y"
                                                  {
        // Caso: lista de elementos
        NodoAST* bloque = crearNodo(AST_INTERNO, "Bloque");
        agregarHijo(bloque, (yyvsp[-1].nodo));
        (yyval.nodo) = bloque;
    }
#line 2719 "src/parser/parser.tab.c"
    break;

  case 60: /* lista_elementos_o_bloques: TOKEN_LLAVEIZQ lista_bloques TOKEN_LLAVEDER  */
#line 543 "src/parser/parser.y"
                                                  {
        // Caso: bloques anidados
        NodoAST* bloque = crearNodo(AST_INTERNO, "Bloque");
        agregarHijo(bloque, (yyvsp[-1].nodo));
        (yyval.nodo) = bloque;
    }
#line 2730 "src/parser/parser.tab.c"
    break;

  case 61: /* DimensionesCorchetes: TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER DimensionesCorchetes  */
#line 552 "src/parser/parser.y"
                                                             {
        NodoAST* dim = crearNodo(AST_DIMENSIONES, "[]");
        if ((yyvsp[0].nodo) == NULL) {
            (yyval.nodo) = dim;
        } else if ((yyvsp[0].nodo)->tipo != AST_INTERNO) {
            NodoAST* lista = crearNodo(AST_INTERNO, "Dimensiones");
            agregarHijo(lista, dim);
            agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            /* Insertar al principio de la lista */
            NodoAST* nueva = crearNodo(AST_INTERNO, "Dimensiones");
            agregarHijo(nueva, dim);
            for (int i = 0; i < (yyvsp[0].nodo)->num_hijos; i++) {
                agregarHijo(nueva, (yyvsp[0].nodo)->hijos[i]);
            }
            (yyval.nodo) = nueva;
        }
    }
#line 2754 "src/parser/parser.tab.c"
    break;

  case 62: /* DimensionesCorchetes: TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER  */
#line 571 "src/parser/parser.y"
                                          {
        (yyval.nodo) = crearNodo(AST_DIMENSIONES, "[]");
    }
#line 2762 "src/parser/parser.tab.c"
    break;

  case 63: /* Dimensiones: TOKEN_CORCHETEIZQ TOKEN_NUMERO TOKEN_CORCHETEDER  */
#line 577 "src/parser/parser.y"
                                                     {
        (yyval.nodo) = crearNodo(AST_DIMENSIONES, "[]");
        agregarHijo((yyval.nodo), crearNodo(AST_NUMERO, (yyvsp[-1].str)));
    }
#line 2771 "src/parser/parser.tab.c"
    break;

  case 64: /* Dimensiones: Dimensiones TOKEN_CORCHETEIZQ TOKEN_NUMERO TOKEN_CORCHETEDER  */
#line 581 "src/parser/parser.y"
                                                                   {
        agregarHijo((yyvsp[-3].nodo), crearNodo(AST_NUMERO, (yyvsp[-1].str)));
        (yyval.nodo) = (yyvsp[-3].nodo);
    }
#line 2780 "src/parser/parser.tab.c"
    break;

  case 65: /* acceso: TOKEN_IDENTIFICADOR DimensionesAcceso  */
#line 591 "src/parser/parser.y"
                                          {
        (yyval.nodo) = crearNodo(AST_ACCESO, "Acceso");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;

        // Nodo para el identificador
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-1].str));
        id->linea = (yylsp[-1]).first_line; id->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), id);

        // Nodo para las dimensiones de acceso
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2797 "src/parser/parser.tab.c"
    break;

  case 66: /* acceso: TOKEN_IDENTIFICADOR error  */
#line 603 "src/parser/parser.y"
                                {
        addErrorSintactico("Acceso inválido a array", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = crearNodo(AST_ACCESO, "Acceso");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-1].str));
        id->linea = (yylsp[-1]).first_line; id->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), id);
        
        // Crear una dimensión predeterminada
        NodoAST* dim = crearNodo(AST_DIMENSIONES, "Dimensiones");
        agregarHijo((yyval.nodo), dim);
    }
#line 2815 "src/parser/parser.tab.c"
    break;

  case 67: /* DimensionesAcceso: DimensionesAcceso TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER  */
#line 619 "src/parser/parser.y"
                                                                    {
        if ((yyvsp[-3].nodo) == NULL) {
            (yyval.nodo) = crearNodo(AST_DIMENSIONES, "Dimensiones");
        } else {
            (yyval.nodo) = (yyvsp[-3].nodo);
        }
        NodoAST* dim = crearNodo(AST_1DIMENSION, "Dimensión");
        agregarHijo(dim, (yyvsp[-1].nodo)); // Agregar la expresión como una dimensión
        agregarHijo((yyval.nodo), dim);
    }
#line 2830 "src/parser/parser.tab.c"
    break;

  case 68: /* DimensionesAcceso: TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER  */
#line 629 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crearNodo(AST_DIMENSIONES, "Dimensiones");
        NodoAST* dim = crearNodo(AST_1DIMENSION, "Dimensión");
        agregarHijo(dim, (yyvsp[-1].nodo)); // Agregar la expresión como una dimensión
        agregarHijo((yyval.nodo), dim);
    }
#line 2841 "src/parser/parser.tab.c"
    break;

  case 69: /* acceso_escritura: acceso TOKEN_IGUAL expresion  */
#line 639 "src/parser/parser.y"
                                 {
        (yyval.nodo) = crearNodo(AST_ACCESO_ESCRITURA, "Acceso Escritura");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;

        // Nodo para el acceso (identificador + dimensiones)
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));

        // Nodo para la expresión (valor a asignar)
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2856 "src/parser/parser.tab.c"
    break;

  case 70: /* Funcion_void: TOKEN_VOID TOKEN_IDENTIFICADOR TOKEN_PARIZQ TOKEN_PARDER bloque  */
#line 654 "src/parser/parser.y"
                                                                    {
        (yyval.nodo) = crearNodo(AST_FUNCION_VOID, "Función void");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;

        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-3].str));
        id->linea = (yylsp[-3]).first_line; id->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), id);

        // Lista de parámetros vacía
        agregarHijo((yyval.nodo), crearNodo(AST_PARAMETROS, "parametros"));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2873 "src/parser/parser.tab.c"
    break;

  case 71: /* Funcion_void: TOKEN_VOID TOKEN_IDENTIFICADOR TOKEN_PARIZQ parametros TOKEN_PARDER bloque  */
#line 666 "src/parser/parser.y"
                                                                                 {
        (yyval.nodo) = crearNodo(AST_FUNCION_VOID, "Función void con parámetros");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;

        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str));
        id->linea = (yylsp[-4]).first_line; id->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), id);

        // Lista de parámetros
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2890 "src/parser/parser.tab.c"
    break;

  case 72: /* Funcion_void: TOKEN_VOID TOKEN_IDENTIFICADOR TOKEN_PARIZQ error TOKEN_PARDER bloque  */
#line 678 "src/parser/parser.y"
                                                                            {
        addErrorSintactico("Lista de parámetros inválida en función void", "global", (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        (yyval.nodo) = crearNodo(AST_FUNCION_VOID, "Función void");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str));
        id->linea = (yylsp[-4]).first_line; id->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), id);
        
        // Crear una lista de parámetros vacía
        agregarHijo((yyval.nodo), crearNodo(AST_PARAMETROS, "parametros"));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2908 "src/parser/parser.tab.c"
    break;

  case 73: /* Llamadas: TOKEN_IDENTIFICADOR TOKEN_PARIZQ TOKEN_PARDER  */
#line 694 "src/parser/parser.y"
                                                  {
        (yyval.nodo) = crearNodo(AST_LLAMADA_VOID, "Llamada funcion void");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;

        NodoAST* id1 = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id1->linea = (yylsp[-2]).first_line; id1->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id1);
    }
#line 2921 "src/parser/parser.tab.c"
    break;

  case 74: /* Llamadas: TOKEN_IDENTIFICADOR TOKEN_PARIZQ lista_parametros TOKEN_PARDER  */
#line 702 "src/parser/parser.y"
                                                                     {
        (yyval.nodo) = crearNodo(AST_LLAMADA, "Llamada funcion");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;

        NodoAST* id1 = crearNodo(AST_IDENTIFICADOR, (yyvsp[-3].str));
        id1->linea = (yylsp[-3]).first_line; id1->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), id1);

        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2936 "src/parser/parser.tab.c"
    break;

  case 75: /* Llamadas: TOKEN_IDENTIFICADOR TOKEN_PARIZQ error TOKEN_PARDER  */
#line 712 "src/parser/parser.y"
                                                          {
        addErrorSintactico("Parámetros inválidos en llamada a función", "global", (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        (yyval.nodo) = crearNodo(AST_LLAMADA_VOID, "Llamada funcion void");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        
        NodoAST* id1 = crearNodo(AST_IDENTIFICADOR, (yyvsp[-3].str));
        id1->linea = (yylsp[-3]).first_line; id1->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), id1);
    }
#line 2950 "src/parser/parser.tab.c"
    break;

  case 76: /* lista_parametros: lista_parametros TOKEN_COMA expresion  */
#line 724 "src/parser/parser.y"
                                          {
        if ((yyvsp[-2].nodo)->tipo != AST_LISTA_PARAMETROS) {
            NodoAST* lista = crearNodo(AST_LISTA_PARAMETROS, "Lista Parametros");
            agregarHijo(lista, (yyvsp[-2].nodo));
            agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            agregarHijo((yyvsp[-2].nodo), (yyvsp[0].nodo));
            (yyval.nodo) = (yyvsp[-2].nodo);
        }
    }
#line 2966 "src/parser/parser.tab.c"
    break;

  case 77: /* lista_parametros: expresion  */
#line 735 "src/parser/parser.y"
                {
        (yyval.nodo) = crearNodo(AST_LISTA_PARAMETROS, "Lista Parametros");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2975 "src/parser/parser.tab.c"
    break;

  case 78: /* Funcion_parametros: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_PARIZQ parametros TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 742 "src/parser/parser.y"
                                                                                                                           {
        (yyval.nodo) = crearNodo(AST_FUNCION, "Función con parametros");
        (yyval.nodo)->linea = (yylsp[-7]).first_line; (yyval.nodo)->columna = (yylsp[-7]).first_column;

        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-7].str));
        tipo->linea = (yylsp[-7]).first_line; tipo->columna = (yylsp[-7]).first_column;
        agregarHijo((yyval.nodo), tipo);

        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-6].str));
        id->linea = (yylsp[-6]).first_line; id->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), id);

        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2995 "src/parser/parser.tab.c"
    break;

  case 79: /* Funcion_parametros: TipoConCorchetes TOKEN_IDENTIFICADOR TOKEN_PARIZQ parametros TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 757 "src/parser/parser.y"
                                                                                                                               {
        (yyval.nodo) = crearNodo(AST_FUNCION, "Función con parametros");
        (yyval.nodo)->linea = (yylsp[-7]).first_line; (yyval.nodo)->columna = (yylsp[-7]).first_column;

        // ✓ $1 es char* desde TipoConCorchetes
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-7].str));
        tipo->linea = (yylsp[-7]).first_line; tipo->columna = (yylsp[-7]).first_column;
        agregarHijo((yyval.nodo), tipo);

        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-6].str));
        id->linea = (yylsp[-6]).first_line; id->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), id);

        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
        
        free((yyvsp[-7].str));  // Liberar la memoria del string
    }
#line 3018 "src/parser/parser.tab.c"
    break;

  case 80: /* Funcion_parametros: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_PARIZQ parametros TOKEN_PARDER error  */
#line 775 "src/parser/parser.y"
                                                                                    {
        addErrorSintactico("Error en el cuerpo de la función", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = crearNodo(AST_FUNCION, "Función con parametros");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-5].str));
        tipo->linea = (yylsp[-5]).first_line; tipo->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), tipo);
        
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str));
        id->linea = (yylsp[-4]).first_line; id->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), id);
        
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        // Crear un cuerpo vacío
        NodoAST* bloqueVacio = crearNodo(AST_LISTA, "Programa");
        agregarHijo((yyval.nodo), bloqueVacio);
    }
#line 3041 "src/parser/parser.tab.c"
    break;

  case 81: /* Funcion_parametros: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_PARIZQ error TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 793 "src/parser/parser.y"
                                                                                                                        {
        addErrorSintactico("Lista de parámetros inválida en definición de función", "global", (yylsp[-4]).first_line, (yylsp[-4]).first_column);
        (yyval.nodo) = crearNodo(AST_FUNCION, "Función con parametros");
        (yyval.nodo)->linea = (yylsp[-7]).first_line; (yyval.nodo)->columna = (yylsp[-7]).first_column;
        
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-7].str));
        tipo->linea = (yylsp[-7]).first_line; tipo->columna = (yylsp[-7]).first_column;
        agregarHijo((yyval.nodo), tipo);
        
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-6].str));
        id->linea = (yylsp[-6]).first_line; id->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), id);
        
        // Crear una lista de parámetros vacía
        agregarHijo((yyval.nodo), crearNodo(AST_PARAMETROS, "parametros"));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3063 "src/parser/parser.tab.c"
    break;

  case 82: /* Funcion_parametros: TipoConCorchetes TOKEN_IDENTIFICADOR TOKEN_PARIZQ error TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 810 "src/parser/parser.y"
                                                                                                                          {
        addErrorSintactico("Lista de parámetros inválida en definición de función", "global", (yylsp[-4]).first_line, (yylsp[-4]).first_column);
        (yyval.nodo) = crearNodo(AST_FUNCION, "Función con parametros");
        (yyval.nodo)->linea = (yylsp[-7]).first_line; (yyval.nodo)->columna = (yylsp[-7]).first_column;
        
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-7].str));
        tipo->linea = (yylsp[-7]).first_line; tipo->columna = (yylsp[-7]).first_column;
        agregarHijo((yyval.nodo), tipo);
        
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-6].str));
        id->linea = (yylsp[-6]).first_line; id->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), id);
        
        // Crear una lista de parámetros vacía
        agregarHijo((yyval.nodo), crearNodo(AST_PARAMETROS, "parametros"));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
        
        free((yyvsp[-7].str));
    }
#line 3087 "src/parser/parser.tab.c"
    break;

  case 83: /* Funcion_parametros: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_PARIZQ TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 829 "src/parser/parser.y"
                                                                                                                  {
        // ✓ NUEVO: Función sin parámetros con tipo primitivo
        (yyval.nodo) = crearNodo(AST_FUNCION, "Función sin parametros");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;

        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-6].str));
        tipo->linea = (yylsp[-6]).first_line; tipo->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), tipo);

        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-5].str));
        id->linea = (yylsp[-5]).first_line; id->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), id);

        // Lista de parámetros vacía
        agregarHijo((yyval.nodo), crearNodo(AST_PARAMETROS, "parametros"));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3109 "src/parser/parser.tab.c"
    break;

  case 84: /* Funcion_parametros: TipoConCorchetes TOKEN_IDENTIFICADOR TOKEN_PARIZQ TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 846 "src/parser/parser.y"
                                                                                                                    {
        // ✓ NUEVO: Función sin parámetros con tipo matriz/vector
        (yyval.nodo) = crearNodo(AST_FUNCION, "Función sin parametros");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;

        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-6].str));
        tipo->linea = (yylsp[-6]).first_line; tipo->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), tipo);

        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-5].str));
        id->linea = (yylsp[-5]).first_line; id->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), id);

        // Lista de parámetros vacía
        agregarHijo((yyval.nodo), crearNodo(AST_PARAMETROS, "parametros"));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
        
        free((yyvsp[-6].str));
    }
#line 3133 "src/parser/parser.tab.c"
    break;

  case 85: /* parametros: parametros TOKEN_COMA tipo_primitivo TOKEN_IDENTIFICADOR  */
#line 867 "src/parser/parser.y"
                                                             {
        if((yyvsp[-3].nodo)->tipo != AST_PARAMETROS) {
            NodoAST* lista = crearNodo(AST_PARAMETROS, "parametros");
            agregarHijo(lista, (yyvsp[-3].nodo));
            
            NodoAST* param = crearNodo(AST_PARAMETRO, "parametro");
            
            NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-1].str));
            tipo->linea = (yylsp[-1]).first_line; tipo->columna = (yylsp[-1]).first_column;
            agregarHijo(param, tipo);
            
            NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
            id->linea = (yylsp[0]).first_line; id->columna = (yylsp[0]).first_column;
            agregarHijo(param, id);
            
            agregarHijo(lista, param);
            (yyval.nodo) = lista;
        } else {
            NodoAST* param = crearNodo(AST_PARAMETRO, "parametro");
            
            NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-1].str));
            tipo->linea = (yylsp[-1]).first_line; tipo->columna = (yylsp[-1]).first_column;
            agregarHijo(param, tipo);
            
            NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
            id->linea = (yylsp[0]).first_line; id->columna = (yylsp[0]).first_column;
            agregarHijo(param, id);
            
            agregarHijo((yyvsp[-3].nodo), param);
            (yyval.nodo) = (yyvsp[-3].nodo);
        }
    }
#line 3170 "src/parser/parser.tab.c"
    break;

  case 86: /* parametros: parametros TOKEN_COMA TipoConCorchetes TOKEN_IDENTIFICADOR  */
#line 899 "src/parser/parser.y"
                                                                 {
        if((yyvsp[-3].nodo)->tipo != AST_PARAMETROS) {
            NodoAST* lista = crearNodo(AST_PARAMETROS, "parametros");
            agregarHijo(lista, (yyvsp[-3].nodo));
            
            NodoAST* param = crearNodo(AST_PARAMETRO, "parametro");
            
            // ✓ CORRECCIÓN: $3 es char*, no NodoAST*
            NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-1].str));
            tipo->linea = (yylsp[-1]).first_line; tipo->columna = (yylsp[-1]).first_column;
            agregarHijo(param, tipo);
            
            NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
            id->linea = (yylsp[0]).first_line; id->columna = (yylsp[0]).first_column;
            agregarHijo(param, id);
            
            agregarHijo(lista, param);
            free((yyvsp[-1].str));  // ✓ Liberar la memoria del string
            (yyval.nodo) = lista;
        } else {
            NodoAST* param = crearNodo(AST_PARAMETRO, "parametro");
            
            // ✓ CORRECCIÓN: $3 es char*, no NodoAST*
            NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-1].str));
            tipo->linea = (yylsp[-1]).first_line; tipo->columna = (yylsp[-1]).first_column;
            agregarHijo(param, tipo);
            
            NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
            id->linea = (yylsp[0]).first_line; id->columna = (yylsp[0]).first_column;
            agregarHijo(param, id);
            
            agregarHijo((yyvsp[-3].nodo), param);
            free((yyvsp[-1].str));  // ✓ Liberar la memoria del string
            (yyval.nodo) = (yyvsp[-3].nodo);
        }
    }
#line 3211 "src/parser/parser.tab.c"
    break;

  case 87: /* parametros: TipoConCorchetes TOKEN_IDENTIFICADOR  */
#line 935 "src/parser/parser.y"
                                           {
        (yyval.nodo) = crearNodo(AST_PARAMETROS, "parametros");
        
        NodoAST* param = crearNodo(AST_PARAMETRO, "parametro");
        
        // ✓ CORRECCIÓN: $1 es char*, no NodoAST*
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-1].str));
        tipo->linea = (yylsp[-1]).first_line; tipo->columna = (yylsp[-1]).first_column;
        agregarHijo(param, tipo);
        
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
        id->linea = (yylsp[0]).first_line; id->columna = (yylsp[0]).first_column;
        agregarHijo(param, id);
        
        agregarHijo((yyval.nodo), param);
        free((yyvsp[-1].str));  // ✓ Liberar la memoria del string
    }
#line 3233 "src/parser/parser.tab.c"
    break;

  case 88: /* parametros: tipo_primitivo TOKEN_IDENTIFICADOR  */
#line 952 "src/parser/parser.y"
                                         {
        (yyval.nodo) = crearNodo(AST_PARAMETROS, "parametros");
        
        NodoAST* param = crearNodo(AST_PARAMETRO, "parametro");
        
        NodoAST* tipo = crearNodo(AST_TIPO, (yyvsp[-1].str));
        tipo->linea = (yylsp[-1]).first_line; tipo->columna = (yylsp[-1]).first_column;
        agregarHijo(param, tipo);
        
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
        id->linea = (yylsp[0]).first_line; id->columna = (yylsp[0]).first_column;
        agregarHijo(param, id);
        
        agregarHijo((yyval.nodo), param);
    }
#line 3253 "src/parser/parser.tab.c"
    break;

  case 89: /* TipoConCorchetes: tipo_primitivo  */
#line 971 "src/parser/parser.y"
                   {
        (yyval.str) = strdup((yyvsp[0].str));
    }
#line 3261 "src/parser/parser.tab.c"
    break;

  case 90: /* TipoConCorchetes: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER  */
#line 974 "src/parser/parser.y"
                                                         {
        char* buffer = malloc(strlen((yyvsp[-2].str)) + 3);
        sprintf(buffer, "%s[]", (yyvsp[-2].str));
        (yyval.str) = buffer;
    }
#line 3271 "src/parser/parser.tab.c"
    break;

  case 91: /* TipoConCorchetes: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER  */
#line 979 "src/parser/parser.y"
                                                                                             {
        char* buffer = malloc(strlen((yyvsp[-4].str)) + 5);
        sprintf(buffer, "%s[][]", (yyvsp[-4].str));
        (yyval.str) = buffer;
    }
#line 3281 "src/parser/parser.tab.c"
    break;

  case 92: /* TipoConCorchetes: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER  */
#line 984 "src/parser/parser.y"
                                                                                                                                 {
        char* buffer = malloc(strlen((yyvsp[-6].str)) + 7);
        sprintf(buffer, "%s[][][]", (yyvsp[-6].str));
        (yyval.str) = buffer;
    }
#line 3291 "src/parser/parser.tab.c"
    break;

  case 93: /* tipo_primitivo: TOKEN_DINT  */
#line 992 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3297 "src/parser/parser.tab.c"
    break;

  case 94: /* tipo_primitivo: TOKEN_DCHAR  */
#line 993 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3303 "src/parser/parser.tab.c"
    break;

  case 95: /* tipo_primitivo: TOKEN_DOUBLE  */
#line 994 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3309 "src/parser/parser.tab.c"
    break;

  case 96: /* tipo_primitivo: TOKEN_DFLOAT  */
#line 995 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3315 "src/parser/parser.tab.c"
    break;

  case 97: /* tipo_primitivo: TOKEN_DSTRING  */
#line 996 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3321 "src/parser/parser.tab.c"
    break;

  case 98: /* tipo_primitivo: TOKEN_DBOOLEAN  */
#line 997 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3327 "src/parser/parser.tab.c"
    break;

  case 99: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_IGUAL expresion  */
#line 1002 "src/parser/parser.y"
                                              {
        (yyval.nodo) = crearNodo(AST_ASIGNACION, "=");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; 
        (yyval.nodo)->columna = (yylsp[-2]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; 
        id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 3342 "src/parser/parser.tab.c"
    break;

  case 100: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_MAS_IGUAL expresion  */
#line 1012 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "+=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3350 "src/parser/parser.tab.c"
    break;

  case 101: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_MENOS_IGUAL expresion  */
#line 1015 "src/parser/parser.y"
                                                      {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "-=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3358 "src/parser/parser.tab.c"
    break;

  case 102: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_POR_IGUAL expresion  */
#line 1018 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "*=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3366 "src/parser/parser.tab.c"
    break;

  case 103: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_DIV_IGUAL expresion  */
#line 1021 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "/=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3374 "src/parser/parser.tab.c"
    break;

  case 104: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_MOD_IGUAL expresion  */
#line 1024 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "%=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3382 "src/parser/parser.tab.c"
    break;

  case 105: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_AND_IGUAL expresion  */
#line 1027 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "&=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3390 "src/parser/parser.tab.c"
    break;

  case 106: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_OR_IGUAL expresion  */
#line 1030 "src/parser/parser.y"
                                                   {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "|=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3398 "src/parser/parser.tab.c"
    break;

  case 107: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_XOR_IGUAL expresion  */
#line 1033 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "^=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3406 "src/parser/parser.tab.c"
    break;

  case 108: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_DESIZQ_IGUAL expresion  */
#line 1036 "src/parser/parser.y"
                                                       {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "<<=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3414 "src/parser/parser.tab.c"
    break;

  case 109: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_DESDER_IGUAL expresion  */
#line 1039 "src/parser/parser.y"
                                                       {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), ">>=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3422 "src/parser/parser.tab.c"
    break;

  case 110: /* expresion: expresion_or  */
#line 1049 "src/parser/parser.y"
                   { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3428 "src/parser/parser.tab.c"
    break;

  case 111: /* expresion_or: expresion_and  */
#line 1053 "src/parser/parser.y"
                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3434 "src/parser/parser.tab.c"
    break;

  case 112: /* expresion_or: expresion_or TOKEN_OR expresion_and  */
#line 1054 "src/parser/parser.y"
                                          {
          (yyval.nodo) = crearNodo(AST_OPERACION, "||");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3444 "src/parser/parser.tab.c"
    break;

  case 113: /* expresion_and: expresion_bit_or  */
#line 1062 "src/parser/parser.y"
                       { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3450 "src/parser/parser.tab.c"
    break;

  case 114: /* expresion_and: expresion_and TOKEN_AND expresion_bit_or  */
#line 1063 "src/parser/parser.y"
                                               {
          (yyval.nodo) = crearNodo(AST_OPERACION, "&&");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3460 "src/parser/parser.tab.c"
    break;

  case 115: /* expresion_bit_or: expresion_bit_xor  */
#line 1071 "src/parser/parser.y"
                        { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3466 "src/parser/parser.tab.c"
    break;

  case 116: /* expresion_bit_or: expresion_bit_or TOKEN_BIT_OR expresion_bit_xor  */
#line 1072 "src/parser/parser.y"
                                                      {
          (yyval.nodo) = crearNodo(AST_OPERACION, "|");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3476 "src/parser/parser.tab.c"
    break;

  case 117: /* expresion_bit_xor: expresion_bit_and  */
#line 1080 "src/parser/parser.y"
                        { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3482 "src/parser/parser.tab.c"
    break;

  case 118: /* expresion_bit_xor: expresion_bit_xor TOKEN_XOR expresion_bit_and  */
#line 1081 "src/parser/parser.y"
                                                    {
          (yyval.nodo) = crearNodo(AST_OPERACION, "^");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3492 "src/parser/parser.tab.c"
    break;

  case 119: /* expresion_bit_and: expresion_equality  */
#line 1089 "src/parser/parser.y"
                         { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3498 "src/parser/parser.tab.c"
    break;

  case 120: /* expresion_bit_and: expresion_bit_and TOKEN_BIT_AND expresion_equality  */
#line 1090 "src/parser/parser.y"
                                                         {
          (yyval.nodo) = crearNodo(AST_OPERACION, "&");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3508 "src/parser/parser.tab.c"
    break;

  case 121: /* expresion_equality: expresion_relacional  */
#line 1098 "src/parser/parser.y"
                           { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3514 "src/parser/parser.tab.c"
    break;

  case 122: /* expresion_equality: expresion_equality TOKEN_IGUAL_IGUAL expresion_relacional  */
#line 1099 "src/parser/parser.y"
                                                                {
          (yyval.nodo) = crearNodo(AST_OPERACION, "==");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3524 "src/parser/parser.tab.c"
    break;

  case 123: /* expresion_equality: expresion_equality TOKEN_NO_IGUAL expresion_relacional  */
#line 1104 "src/parser/parser.y"
                                                             {
          (yyval.nodo) = crearNodo(AST_OPERACION, "!=");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3534 "src/parser/parser.tab.c"
    break;

  case 124: /* expresion_relacional: expresion_shift  */
#line 1112 "src/parser/parser.y"
                      { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3540 "src/parser/parser.tab.c"
    break;

  case 125: /* expresion_relacional: expresion_relacional TOKEN_MENOR expresion_shift  */
#line 1113 "src/parser/parser.y"
                                                       {
          (yyval.nodo) = crearNodo(AST_OPERACION, "<");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3550 "src/parser/parser.tab.c"
    break;

  case 126: /* expresion_relacional: expresion_relacional TOKEN_MENOR_IGUAL expresion_shift  */
#line 1118 "src/parser/parser.y"
                                                             {
          (yyval.nodo) = crearNodo(AST_OPERACION, "<=");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3560 "src/parser/parser.tab.c"
    break;

  case 127: /* expresion_relacional: expresion_relacional TOKEN_MAYOR expresion_shift  */
#line 1123 "src/parser/parser.y"
                                                       {
          (yyval.nodo) = crearNodo(AST_OPERACION, ">");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3570 "src/parser/parser.tab.c"
    break;

  case 128: /* expresion_relacional: expresion_relacional TOKEN_MAYOR_IGUAL expresion_shift  */
#line 1128 "src/parser/parser.y"
                                                             {
          (yyval.nodo) = crearNodo(AST_OPERACION, ">=");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3580 "src/parser/parser.tab.c"
    break;

  case 129: /* expresion_shift: expresion_aditiva  */
#line 1136 "src/parser/parser.y"
                        { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3586 "src/parser/parser.tab.c"
    break;

  case 130: /* expresion_shift: expresion_shift TOKEN_DESIZQ expresion_aditiva  */
#line 1137 "src/parser/parser.y"
                                                     {
          (yyval.nodo) = crearNodo(AST_OPERACION, "<<");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3596 "src/parser/parser.tab.c"
    break;

  case 131: /* expresion_shift: expresion_shift TOKEN_DESDER expresion_aditiva  */
#line 1142 "src/parser/parser.y"
                                                     {
          (yyval.nodo) = crearNodo(AST_OPERACION, ">>");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3606 "src/parser/parser.tab.c"
    break;

  case 132: /* expresion_aditiva: expresion_multiplicativa  */
#line 1150 "src/parser/parser.y"
                               { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3612 "src/parser/parser.tab.c"
    break;

  case 133: /* expresion_aditiva: expresion_aditiva TOKEN_MAS expresion_multiplicativa  */
#line 1151 "src/parser/parser.y"
                                                           {
          (yyval.nodo) = crearNodo(AST_OPERACION, "+");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3622 "src/parser/parser.tab.c"
    break;

  case 134: /* expresion_aditiva: expresion_aditiva TOKEN_MENOS expresion_multiplicativa  */
#line 1156 "src/parser/parser.y"
                                                             {
          (yyval.nodo) = crearNodo(AST_OPERACION, "-");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3632 "src/parser/parser.tab.c"
    break;

  case 135: /* expresion_multiplicativa: expresion_unaria  */
#line 1164 "src/parser/parser.y"
                       { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3638 "src/parser/parser.tab.c"
    break;

  case 136: /* expresion_multiplicativa: expresion_multiplicativa TOKEN_MULTI expresion_unaria  */
#line 1165 "src/parser/parser.y"
                                                            {
          (yyval.nodo) = crearNodo(AST_OPERACION, "*");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3648 "src/parser/parser.tab.c"
    break;

  case 137: /* expresion_multiplicativa: expresion_multiplicativa TOKEN_DIV expresion_unaria  */
#line 1170 "src/parser/parser.y"
                                                          {
          (yyval.nodo) = crearNodo(AST_OPERACION, "/");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3658 "src/parser/parser.tab.c"
    break;

  case 138: /* expresion_multiplicativa: expresion_multiplicativa TOKEN_MODULO expresion_unaria  */
#line 1175 "src/parser/parser.y"
                                                             {
          (yyval.nodo) = crearNodo(AST_OPERACION, "%");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3668 "src/parser/parser.tab.c"
    break;

  case 139: /* expresion_unaria: TOKEN_NOT expresion_unaria  */
#line 1183 "src/parser/parser.y"
                                 {
          (yyval.nodo) = crearNodo(AST_OPERACION, "!");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3678 "src/parser/parser.tab.c"
    break;

  case 140: /* expresion_unaria: TOKEN_MENOS expresion_unaria  */
#line 1188 "src/parser/parser.y"
                                                {
          (yyval.nodo) = crearNodo(AST_OPERACION, "uminus");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3688 "src/parser/parser.tab.c"
    break;

  case 141: /* expresion_unaria: TOKEN_PARIZQ tipo_primitivo TOKEN_PARDER expresion_unaria  */
#line 1193 "src/parser/parser.y"
                                                                {
          /* crear un nodo específico para cast; guardamos el tipo en ->valor */
          (yyval.nodo) = crearNodo(AST_CAST, "cast");
          (yyval.nodo)->valor = strdup((yyvsp[-2].str));    
          (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[0].nodo));
          free((yyvsp[-2].str)); 
      }
#line 3701 "src/parser/parser.tab.c"
    break;

  case 142: /* expresion_unaria: postfix  */
#line 1201 "src/parser/parser.y"
              { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3707 "src/parser/parser.tab.c"
    break;

  case 143: /* postfix: primary  */
#line 1205 "src/parser/parser.y"
              { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3713 "src/parser/parser.tab.c"
    break;

  case 144: /* postfix: postfix TOKEN_INCREMENTO  */
#line 1206 "src/parser/parser.y"
                               {
          (yyval.nodo) = crearNodo(AST_INCREMENTO, "++");
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
      }
#line 3723 "src/parser/parser.tab.c"
    break;

  case 145: /* postfix: postfix TOKEN_DECREMENTO  */
#line 1211 "src/parser/parser.y"
                               {
          (yyval.nodo) = crearNodo(AST_DECREMENTO, "--");
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
      }
#line 3733 "src/parser/parser.tab.c"
    break;

  case 146: /* primary: TOKEN_NUMERO  */
#line 1220 "src/parser/parser.y"
                   {
          (yyval.nodo) = crearNodo(AST_NUMERO, NULL);
          (yyval.nodo)->valor = strdup((yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3743 "src/parser/parser.tab.c"
    break;

  case 147: /* primary: TOKEN_DOUBLE_LITERAL  */
#line 1225 "src/parser/parser.y"
                           {
          (yyval.nodo) = crearNodo(AST_NUMERO, NULL);
          (yyval.nodo)->valor = strdup((yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3753 "src/parser/parser.tab.c"
    break;

  case 148: /* primary: TOKEN_FLOAT_LITERAL  */
#line 1230 "src/parser/parser.y"
                          {
          (yyval.nodo) = crearNodo(AST_NUMERO, NULL);
          (yyval.nodo)->valor = strdup((yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3763 "src/parser/parser.tab.c"
    break;

  case 149: /* primary: TOKEN_CADENA  */
#line 1235 "src/parser/parser.y"
                   {
          (yyval.nodo) = crearNodo(AST_CADENA, NULL);
          (yyval.nodo)->valor = strdup((yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3773 "src/parser/parser.tab.c"
    break;

  case 150: /* primary: TOKEN_CARACTER  */
#line 1240 "src/parser/parser.y"
                     {
          (yyval.nodo) = crearNodo(AST_CARACTER, NULL);
          (yyval.nodo)->valor = strdup((yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3783 "src/parser/parser.tab.c"
    break;

  case 151: /* primary: TOKEN_IDENTIFICADOR  */
#line 1245 "src/parser/parser.y"
                          {
          (yyval.nodo) = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3792 "src/parser/parser.tab.c"
    break;

  case 152: /* primary: TOKEN_BOOLEANO  */
#line 1249 "src/parser/parser.y"
                     {
        (yyval.nodo) = crearNodo(AST_BOOLEAN, (yyvsp[0].str));
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 3801 "src/parser/parser.tab.c"
    break;

  case 153: /* primary: TOKEN_PARSEINT TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1253 "src/parser/parser.y"
                                                         {
        (yyval.nodo) = crearNodo(AST_PARSEINT, "Integer.parseInt");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3811 "src/parser/parser.tab.c"
    break;

  case 154: /* primary: TOKEN_PARSEDOUBLE TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1258 "src/parser/parser.y"
                                                            {
        (yyval.nodo) = crearNodo(AST_PARSEDOUBLE, "Double.parseDouble");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3821 "src/parser/parser.tab.c"
    break;

  case 155: /* primary: TOKEN_PARSEFLOAT TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1263 "src/parser/parser.y"
                                                           {
        (yyval.nodo) = crearNodo(AST_PARSEFLOAT, "Float.parseFloat");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3831 "src/parser/parser.tab.c"
    break;

  case 156: /* primary: TOKEN_STRING_VALUEOF TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1268 "src/parser/parser.y"
                                                               {
        (yyval.nodo) = crearNodo(AST_VALUEOF, "String.valueOf");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3841 "src/parser/parser.tab.c"
    break;

  case 157: /* primary: TOKEN_INDEXOF TOKEN_PARIZQ expresion TOKEN_COMA expresion TOKEN_PARDER  */
#line 1273 "src/parser/parser.y"
                                                                             {
        (yyval.nodo) = crearNodo(AST_INDEXOF, "Arrays.indexOf");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-3].nodo));  // El array/vector
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));  // La clave a buscar
    }
#line 3852 "src/parser/parser.tab.c"
    break;

  case 158: /* primary: TOKEN_IDENTIFICADOR TOKEN_PUNTO TOKEN_LENGHT  */
#line 1279 "src/parser/parser.y"
                                                   {
        (yyval.nodo) = crearNodo(AST_LENGHT, "Array.length");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
    }
#line 3864 "src/parser/parser.tab.c"
    break;

  case 159: /* primary: TOKEN_IDENTIFICADOR TOKEN_PUNTO TOKEN_ADD TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1286 "src/parser/parser.y"
                                                                                    {
        (yyval.nodo) = crearNodo(AST_ADD, "Array.add");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-5].str));
        id->linea = (yylsp[-5]).first_line; id->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));  // El elemento a añadir
    }
#line 3877 "src/parser/parser.tab.c"
    break;

  case 160: /* primary: TOKEN_CADENA TOKEN_EQUAL TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1294 "src/parser/parser.y"
                                                                   {
        (yyval.nodo) = crearNodo(AST_EQUAL, ".equals");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        NodoAST* cadena = crearNodo(AST_CADENA, (yyvsp[-4].str));
        cadena->linea = (yylsp[-4]).first_line; cadena->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), cadena);
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3890 "src/parser/parser.tab.c"
    break;

  case 161: /* primary: TOKEN_IDENTIFICADOR TOKEN_EQUAL TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1302 "src/parser/parser.y"
                                                                          {
        (yyval.nodo) = crearNodo(AST_EQUAL, ".equals");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str));
        id->linea = (yylsp[-4]).first_line; id->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3903 "src/parser/parser.tab.c"
    break;

  case 162: /* primary: TOKEN_PARSEJOIN TOKEN_PARIZQ expresion TOKEN_COMA lista_expresiones_join TOKEN_PARDER  */
#line 1310 "src/parser/parser.y"
                                                                                            {
        (yyval.nodo) = crearNodo(AST_PARSEJOIN, "Strings.join");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-3].nodo));  // El delimitador
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));  // Lista de expresiones a unir
    }
#line 3914 "src/parser/parser.tab.c"
    break;

  case 163: /* primary: TOKEN_PARSEJOIN TOKEN_PARIZQ expresion TOKEN_COMA expresion TOKEN_PARDER  */
#line 1316 "src/parser/parser.y"
                                                                               {
        (yyval.nodo) = crearNodo(AST_PARSEJOIN, "Strings.join");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-3].nodo));  // El delimitador
        
        // Para un solo elemento, creamos una lista con ese elemento
        NodoAST* lista = crearNodo(AST_LISTA, "Lista Strings");
        agregarHijo(lista, (yyvsp[-1].nodo));
        agregarHijo((yyval.nodo), lista);
    }
#line 3929 "src/parser/parser.tab.c"
    break;

  case 164: /* primary: acceso TOKEN_PUNTO TOKEN_LENGHT  */
#line 1326 "src/parser/parser.y"
                                      {
        (yyval.nodo) = crearNodo(AST_LENGHT, "Array.length");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
    }
#line 3939 "src/parser/parser.tab.c"
    break;

  case 165: /* primary: TOKEN_NULL  */
#line 1331 "src/parser/parser.y"
                 {
        (yyval.nodo) = crearNodo(AST_NULL, "null");
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 3948 "src/parser/parser.tab.c"
    break;

  case 166: /* primary: acceso  */
#line 1335 "src/parser/parser.y"
             {
          (yyval.nodo) = (yyvsp[0].nodo);
      }
#line 3956 "src/parser/parser.tab.c"
    break;

  case 167: /* primary: Llamadas  */
#line 1338 "src/parser/parser.y"
              {
        (yyval.nodo) = (yyvsp[0].nodo);
    }
#line 3964 "src/parser/parser.tab.c"
    break;

  case 168: /* primary: TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1341 "src/parser/parser.y"
                                          {
          (yyval.nodo) = (yyvsp[-1].nodo);
      }
#line 3972 "src/parser/parser.tab.c"
    break;

  case 169: /* lista_expresiones_join: lista_expresiones_join TOKEN_COMA expresion  */
#line 1347 "src/parser/parser.y"
                                                {
        if ((yyvsp[-2].nodo)->tipo != AST_LISTA) {
            NodoAST* lista = crearNodo(AST_LISTA, "Lista Strings");
            agregarHijo(lista, (yyvsp[-2].nodo));
            agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            agregarHijo((yyvsp[-2].nodo), (yyvsp[0].nodo));
            (yyval.nodo) = (yyvsp[-2].nodo);
        }
    }
#line 3988 "src/parser/parser.tab.c"
    break;

  case 170: /* lista_expresiones_join: expresion  */
#line 1358 "src/parser/parser.y"
                {
        (yyval.nodo) = crearNodo(AST_LISTA, "Lista Strings");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 3997 "src/parser/parser.tab.c"
    break;

  case 171: /* instruccion_print: TOKEN_PRINT TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1366 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crearNodo(AST_PRINT, "System.out.println");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 4007 "src/parser/parser.tab.c"
    break;

  case 172: /* instruccion_print: TOKEN_PRINT TOKEN_PARIZQ TOKEN_CADENA TOKEN_MAS expresion TOKEN_PARDER  */
#line 1372 "src/parser/parser.y"
                                                                             {
        NodoAST* concat = crearNodo(AST_OPERACION, "+");
        concat->linea = (yylsp[-2]).first_line; concat->columna = (yylsp[-2]).first_column;
        NodoAST* cadena = crearNodo(AST_CADENA, (yyvsp[-3].str));
        cadena->linea = (yylsp[-3]).first_line; cadena->columna = (yylsp[-3]).first_column;
        agregarHijo(concat, cadena);
        agregarHijo(concat, (yyvsp[-1].nodo));
        
        (yyval.nodo) = crearNodo(AST_PRINT, "System.out.println");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), concat);
    }
#line 4024 "src/parser/parser.tab.c"
    break;

  case 173: /* instruccion_print: TOKEN_PRINT TOKEN_PARIZQ TOKEN_PARDER  */
#line 1384 "src/parser/parser.y"
                                            {
        (yyval.nodo) = crearNodo(AST_PRINT, "System.out.println");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        // No hijos, print vacío
    }
#line 4034 "src/parser/parser.tab.c"
    break;

  case 174: /* bloque: TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 1392 "src/parser/parser.y"
                                                   {
        (yyval.nodo) = crearNodo(AST_BLOQUE, "Bloque");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 4044 "src/parser/parser.tab.c"
    break;

  case 175: /* bloque: TOKEN_LLAVEIZQ error TOKEN_LLAVEDER  */
#line 1397 "src/parser/parser.y"
                                          {
        addErrorSintactico("Error sintáctico en el bloque", "global", (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        (yyval.nodo) = crearNodo(AST_BLOQUE, "Bloque");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        // Crear un bloque vacío para continuar
        agregarHijo((yyval.nodo), crearNodo(AST_LISTA, "Programa"));
    }
#line 4056 "src/parser/parser.tab.c"
    break;

  case 176: /* instruccion_break: TOKEN_BREAK  */
#line 1407 "src/parser/parser.y"
                {
        (yyval.nodo) = crearNodo(AST_BREAK, "break");
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 4065 "src/parser/parser.tab.c"
    break;

  case 177: /* instruccion_continue: TOKEN_CONTINUE  */
#line 1414 "src/parser/parser.y"
                   {
        (yyval.nodo) = crearNodo(AST_CONTINUE, "continue");
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 4074 "src/parser/parser.tab.c"
    break;

  case 178: /* instruccion_return: TOKEN_RETURN expresion  */
#line 1421 "src/parser/parser.y"
                           {
        (yyval.nodo) = crearNodo(AST_RETURN, "return");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4084 "src/parser/parser.tab.c"
    break;

  case 179: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ expresion TOKEN_PARDER bloque  */
#line 1431 "src/parser/parser.y"
                                                                       {
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));   // condición
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));   // bloque if
    }
#line 4095 "src/parser/parser.tab.c"
    break;

  case 180: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ expresion TOKEN_PARDER bloque TOKEN_ELSE bloque  */
#line 1437 "src/parser/parser.y"
                                                                            {
        printf("[DEBUG] Procesando if-else en línea %d, columna %d\n", (yylsp[-6]).first_line, (yylsp[-6]).first_column);
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));   // condición
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));   // bloque if
        NodoAST* else_node = crearNodo(AST_ELSE, "else");
        agregarHijo(else_node, (yyvsp[0].nodo));
        agregarHijo((yyval.nodo), else_node);
    }
#line 4110 "src/parser/parser.tab.c"
    break;

  case 181: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ expresion TOKEN_PARDER bloque TOKEN_ELSE instruccion_if  */
#line 1447 "src/parser/parser.y"
                                                                                    {
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));   // condición
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));   // bloque if
        NodoAST* elseif_node = crearNodo(AST_ELSEIF, "elseif");
        agregarHijo(elseif_node, (yyvsp[0].nodo));
        agregarHijo((yyval.nodo), elseif_node);
    }
#line 4124 "src/parser/parser.tab.c"
    break;

  case 182: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ error TOKEN_PARDER bloque  */
#line 1456 "src/parser/parser.y"
                                                                     {
        printf("[DEBUG] Error en condición del if en línea %d, columna %d\n", (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        addErrorSintactico("Expresión inválida en condición if", "global", (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_BOOLEAN, "false"));  // condición predeterminada
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));  // bloque if
    }
#line 4137 "src/parser/parser.tab.c"
    break;

  case 183: /* intruccion_switch: TOKEN_SWITCH TOKEN_PARIZQ expresion TOKEN_PARDER TOKEN_LLAVEIZQ lista_casos TOKEN_LLAVEDER  */
#line 1468 "src/parser/parser.y"
                                                                                               {
        (yyval.nodo) = crearNodo(AST_SWITCH, "switch");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 4148 "src/parser/parser.tab.c"
    break;

  case 184: /* lista_casos: lista_casos caso  */
#line 1477 "src/parser/parser.y"
                     {
        if ((yyvsp[-1].nodo)->tipo != AST_LISTA) {
            NodoAST* lista = crearNodo(AST_LISTA, "Casos");
            agregarHijo(lista, (yyvsp[-1].nodo));
            agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            agregarHijo((yyvsp[-1].nodo), (yyvsp[0].nodo));
            (yyval.nodo) = (yyvsp[-1].nodo);
        }
    }
#line 4164 "src/parser/parser.tab.c"
    break;

  case 185: /* lista_casos: caso  */
#line 1488 "src/parser/parser.y"
           {
        (yyval.nodo) = crearNodo(AST_LISTA, "Casos");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4173 "src/parser/parser.tab.c"
    break;

  case 186: /* caso: TOKEN_CASE expresion TOKEN_DOS_PUNTOS lista_sentencias TOKEN_BREAK TOKEN_PUNTOCOMA  */
#line 1495 "src/parser/parser.y"
                                                                                      {
        (yyval.nodo) = crearNodo(AST_CASE, "CASE");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        NodoAST* nodoBreak = crearNodo(AST_BREAK, "break");
        nodoBreak->linea = (yylsp[-1]).first_line; nodoBreak->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), nodoBreak);
    }
#line 4187 "src/parser/parser.tab.c"
    break;

  case 187: /* caso: TOKEN_DEFAULT TOKEN_DOS_PUNTOS lista_sentencias  */
#line 1504 "src/parser/parser.y"
                                                      {
        (yyval.nodo) = crearNodo(AST_DEFAULT, "DEFAULT");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4197 "src/parser/parser.tab.c"
    break;

  case 188: /* instruccion_while: TOKEN_WHILE TOKEN_PARIZQ expresion TOKEN_PARDER bloque  */
#line 1512 "src/parser/parser.y"
                                                           {
        (yyval.nodo) = crearNodo(AST_WHILE, "while");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4208 "src/parser/parser.tab.c"
    break;

  case 189: /* instruccion_while: TOKEN_WHILE TOKEN_PARIZQ error TOKEN_PARDER bloque  */
#line 1518 "src/parser/parser.y"
                                                         {
        addErrorSintactico("Expresión inválida en condición while", "global", (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        (yyval.nodo) = crearNodo(AST_WHILE, "while");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        // Agregar una condición predeterminada (false)
        agregarHijo((yyval.nodo), crearNodo(AST_BOOLEAN, "false"));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));  // bloque while
    }
#line 4221 "src/parser/parser.tab.c"
    break;

  case 190: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ declaracion TOKEN_PUNTOCOMA expresion TOKEN_PUNTOCOMA expresion TOKEN_PARDER bloque  */
#line 1529 "src/parser/parser.y"
                                                                                                              {
        (yyval.nodo) = crearNodo(AST_FOR, "FOR");
        (yyval.nodo)->linea = (yylsp[-8]).first_line; (yyval.nodo)->columna = (yylsp[-8]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-6].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4234 "src/parser/parser.tab.c"
    break;

  case 191: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ asignacion TOKEN_PUNTOCOMA expresion TOKEN_PUNTOCOMA expresion TOKEN_PARDER bloque  */
#line 1537 "src/parser/parser.y"
                                                                                                               {
        (yyval.nodo) = crearNodo(AST_FOR, "FOR");
        (yyval.nodo)->linea = (yylsp[-8]).first_line; (yyval.nodo)->columna = (yylsp[-8]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-6].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4247 "src/parser/parser.tab.c"
    break;

  case 192: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ declaracion TOKEN_DOS_PUNTOS expresion TOKEN_PARDER bloque  */
#line 1545 "src/parser/parser.y"
                                                                                        {
        (yyval.nodo) = crearNodo(AST_FOR_VECTOR, "FOR EACH");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4259 "src/parser/parser.tab.c"
    break;

  case 193: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_DOS_PUNTOS expresion TOKEN_PARDER bloque  */
#line 1552 "src/parser/parser.y"
                                                                                                                                                   {
        (yyval.nodo) = crearNodo(AST_FOR_VECTOR, "FOR EACH");
        (yyval.nodo)->linea = (yylsp[-9]).first_line; (yyval.nodo)->columna = (yylsp[-9]).first_column;

        // Crear nodo de declaración de vector temporal: int[] par
        char* tipo_vec = malloc(strlen((yyvsp[-7].str)) + 3);
        sprintf(tipo_vec, "%s[]", (yyvsp[-7].str));
        NodoAST* decl_vec = crearNodo(AST_DECLARACION_VECTOR_TEMP, "declaracion_vector_temp");
        decl_vec->linea = (yylsp[-7]).first_line; decl_vec->columna = (yylsp[-7]).first_column;
        agregarHijo(decl_vec, crearNodo(AST_TIPO, tipo_vec));
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str));
        id->linea = (yylsp[-4]).first_line; id->columna = (yylsp[-4]).first_column;
        agregarHijo(decl_vec, id);
        free(tipo_vec);

        agregarHijo((yyval.nodo), decl_vec);
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); 
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4283 "src/parser/parser.tab.c"
    break;

  case 194: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ error TOKEN_PUNTOCOMA expresion TOKEN_PUNTOCOMA expresion TOKEN_PARDER bloque  */
#line 1571 "src/parser/parser.y"
                                                                                                           {
        addErrorSintactico("Inicialización inválida en bucle for", "global", (yylsp[-6]).first_line, (yylsp[-6]).first_column);
        (yyval.nodo) = crearNodo(AST_FOR, "FOR");
        (yyval.nodo)->linea = (yylsp[-8]).first_line; (yyval.nodo)->columna = (yylsp[-8]).first_column;
        NodoAST* decl = crearNodo(AST_DECLARACION, "declaracion");
        agregarHijo((yyval.nodo), decl);
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4298 "src/parser/parser.tab.c"
    break;

  case 195: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ declaracion TOKEN_PUNTOCOMA error TOKEN_PUNTOCOMA expresion TOKEN_PARDER bloque  */
#line 1581 "src/parser/parser.y"
                                                                                                             {
        addErrorSintactico("Condición inválida en bucle for", "global", (yylsp[-4]).first_line, (yylsp[-4]).first_column);
        (yyval.nodo) = crearNodo(AST_FOR, "FOR");
        (yyval.nodo)->linea = (yylsp[-8]).first_line; (yyval.nodo)->columna = (yylsp[-8]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-6].nodo));
        // Crear una condición predeterminada (true)
        agregarHijo((yyval.nodo), crearNodo(AST_BOOLEAN, "true"));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4313 "src/parser/parser.tab.c"
    break;


#line 4317 "src/parser/parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1594 "src/parser/parser.y"


/* ========== FUNCIONES AUXILIARES ========== */

NodoAST* crear_asignacion_compuesta(char* id, char* operador, NodoAST* expr, 
                                  int id_linea, int id_columna, int op_linea, int op_columna) {
    (void)op_linea;
    (void)op_columna;
        
    NodoAST* asig = crearNodo(AST_ASIGNACION_COMPUESTA, operador); // <--- Cambia aquí
    asig->linea = id_linea;
    asig->columna = id_columna;

    NodoAST* idNodo = crearNodo(AST_IDENTIFICADOR, strdup(id));
    idNodo->linea = id_linea;
    idNodo->columna = id_columna;

    agregarHijo(asig, idNodo);
    agregarHijo(asig, expr);

    return asig;
}

void yyerror(const char *s) {
    extern YYLTYPE yylloc;
    int linea = (yylloc.first_line > 0) ? yylloc.first_line : yylineno;
    int columna = (yylloc.first_column > 0) ? yylloc.first_column : yycolumn;
    fprintf(stderr, "%s en línea %d, columna %d\n", s, linea, columna);
}
