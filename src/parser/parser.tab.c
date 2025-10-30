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
  YYSYMBOL_TOKEN_DO = 75,                  /* TOKEN_DO  */
  YYSYMBOL_TOKEN_MAS_IGUAL = 76,           /* TOKEN_MAS_IGUAL  */
  YYSYMBOL_TOKEN_MENOS_IGUAL = 77,         /* TOKEN_MENOS_IGUAL  */
  YYSYMBOL_TOKEN_POR_IGUAL = 78,           /* TOKEN_POR_IGUAL  */
  YYSYMBOL_TOKEN_DIV_IGUAL = 79,           /* TOKEN_DIV_IGUAL  */
  YYSYMBOL_TOKEN_MOD_IGUAL = 80,           /* TOKEN_MOD_IGUAL  */
  YYSYMBOL_TOKEN_AND_IGUAL = 81,           /* TOKEN_AND_IGUAL  */
  YYSYMBOL_TOKEN_OR_IGUAL = 82,            /* TOKEN_OR_IGUAL  */
  YYSYMBOL_TOKEN_XOR_IGUAL = 83,           /* TOKEN_XOR_IGUAL  */
  YYSYMBOL_TOKEN_DESIZQ_IGUAL = 84,        /* TOKEN_DESIZQ_IGUAL  */
  YYSYMBOL_TOKEN_DESDER_IGUAL = 85,        /* TOKEN_DESDER_IGUAL  */
  YYSYMBOL_UMINUS = 86,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 87,                  /* $accept  */
  YYSYMBOL_s = 88,                         /* s  */
  YYSYMBOL_programa = 89,                  /* programa  */
  YYSYMBOL_lista_funciones = 90,           /* lista_funciones  */
  YYSYMBOL_main_class = 91,                /* main_class  */
  YYSYMBOL_lista_sentencias = 92,          /* lista_sentencias  */
  YYSYMBOL_sentencia = 93,                 /* sentencia  */
  YYSYMBOL_expresion_sentencia = 94,       /* expresion_sentencia  */
  YYSYMBOL_postfix_incremento = 95,        /* postfix_incremento  */
  YYSYMBOL_declaracion = 96,               /* declaracion  */
  YYSYMBOL_declaracion_vector = 97,        /* declaracion_vector  */
  YYSYMBOL_lista_elementos = 98,           /* lista_elementos  */
  YYSYMBOL_lista_filas = 99,               /* lista_filas  */
  YYSYMBOL_declaracion_matriz = 100,       /* declaracion_matriz  */
  YYSYMBOL_declaracion_array_multidimensional = 101, /* declaracion_array_multidimensional  */
  YYSYMBOL_lista_bloques = 102,            /* lista_bloques  */
  YYSYMBOL_lista_elementos_o_bloques = 103, /* lista_elementos_o_bloques  */
  YYSYMBOL_DimensionesCorchetes = 104,     /* DimensionesCorchetes  */
  YYSYMBOL_Dimensiones = 105,              /* Dimensiones  */
  YYSYMBOL_instruccion_do_while = 106,     /* instruccion_do_while  */
  YYSYMBOL_acceso = 107,                   /* acceso  */
  YYSYMBOL_DimensionesAcceso = 108,        /* DimensionesAcceso  */
  YYSYMBOL_acceso_escritura = 109,         /* acceso_escritura  */
  YYSYMBOL_Funcion_void = 110,             /* Funcion_void  */
  YYSYMBOL_Llamadas = 111,                 /* Llamadas  */
  YYSYMBOL_lista_parametros = 112,         /* lista_parametros  */
  YYSYMBOL_Funcion_parametros = 113,       /* Funcion_parametros  */
  YYSYMBOL_parametros = 114,               /* parametros  */
  YYSYMBOL_TipoConCorchetes = 115,         /* TipoConCorchetes  */
  YYSYMBOL_tipo_primitivo = 116,           /* tipo_primitivo  */
  YYSYMBOL_asignacion = 117,               /* asignacion  */
  YYSYMBOL_expresion = 118,                /* expresion  */
  YYSYMBOL_expresion_or = 119,             /* expresion_or  */
  YYSYMBOL_expresion_and = 120,            /* expresion_and  */
  YYSYMBOL_expresion_bit_or = 121,         /* expresion_bit_or  */
  YYSYMBOL_expresion_bit_xor = 122,        /* expresion_bit_xor  */
  YYSYMBOL_expresion_bit_and = 123,        /* expresion_bit_and  */
  YYSYMBOL_expresion_equality = 124,       /* expresion_equality  */
  YYSYMBOL_expresion_relacional = 125,     /* expresion_relacional  */
  YYSYMBOL_expresion_shift = 126,          /* expresion_shift  */
  YYSYMBOL_expresion_aditiva = 127,        /* expresion_aditiva  */
  YYSYMBOL_expresion_multiplicativa = 128, /* expresion_multiplicativa  */
  YYSYMBOL_expresion_unaria = 129,         /* expresion_unaria  */
  YYSYMBOL_postfix = 130,                  /* postfix  */
  YYSYMBOL_primary = 131,                  /* primary  */
  YYSYMBOL_lista_expresiones_join = 132,   /* lista_expresiones_join  */
  YYSYMBOL_instruccion_print = 133,        /* instruccion_print  */
  YYSYMBOL_bloque = 134,                   /* bloque  */
  YYSYMBOL_instruccion_break = 135,        /* instruccion_break  */
  YYSYMBOL_instruccion_continue = 136,     /* instruccion_continue  */
  YYSYMBOL_instruccion_return = 137,       /* instruccion_return  */
  YYSYMBOL_instruccion_if = 138,           /* instruccion_if  */
  YYSYMBOL_intruccion_switch = 139,        /* intruccion_switch  */
  YYSYMBOL_lista_casos = 140,              /* lista_casos  */
  YYSYMBOL_caso = 141,                     /* caso  */
  YYSYMBOL_instruccion_while = 142,        /* instruccion_while  */
  YYSYMBOL_instruccion_for = 143           /* instruccion_for  */
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
#define YYLAST   1281

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  87
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  198
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  478

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   341


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
      85,    86
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   114,   114,   118,   124,   132,   143,   154,   159,   164,
     170,   180,   187,   191,   195,   199,   208,   219,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   250,   260,   261,
     265,   270,   279,   289,   297,   306,   314,   334,   345,   377,
     388,   395,   406,   413,   437,   456,   477,   504,   524,   535,
     542,   548,   557,   576,   582,   586,   593,   600,   614,   626,
     642,   652,   662,   677,   689,   701,   717,   725,   735,   747,
     758,   765,   780,   798,   816,   833,   852,   869,   890,   922,
     958,   975,   994,   997,  1002,  1007,  1015,  1016,  1017,  1018,
    1019,  1020,  1025,  1035,  1038,  1041,  1044,  1047,  1050,  1053,
    1056,  1059,  1062,  1072,  1076,  1077,  1085,  1086,  1094,  1095,
    1103,  1104,  1112,  1113,  1121,  1122,  1127,  1135,  1136,  1141,
    1146,  1151,  1159,  1160,  1165,  1173,  1174,  1179,  1187,  1188,
    1193,  1198,  1206,  1211,  1216,  1224,  1228,  1229,  1234,  1243,
    1248,  1253,  1258,  1263,  1268,  1272,  1276,  1281,  1286,  1291,
    1296,  1302,  1309,  1317,  1325,  1333,  1339,  1349,  1354,  1358,
    1361,  1364,  1370,  1381,  1389,  1395,  1407,  1415,  1420,  1430,
    1437,  1444,  1454,  1460,  1469,  1478,  1489,  1498,  1509,  1516,
    1525,  1533,  1539,  1550,  1558,  1566,  1573,  1592,  1602
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
  "TOKEN_STATIC", "TOKEN_main", "TOKEN_NULL", "TOKEN_FINAL", "TOKEN_DO",
  "TOKEN_MAS_IGUAL", "TOKEN_MENOS_IGUAL", "TOKEN_POR_IGUAL",
  "TOKEN_DIV_IGUAL", "TOKEN_MOD_IGUAL", "TOKEN_AND_IGUAL",
  "TOKEN_OR_IGUAL", "TOKEN_XOR_IGUAL", "TOKEN_DESIZQ_IGUAL",
  "TOKEN_DESDER_IGUAL", "UMINUS", "$accept", "s", "programa",
  "lista_funciones", "main_class", "lista_sentencias", "sentencia",
  "expresion_sentencia", "postfix_incremento", "declaracion",
  "declaracion_vector", "lista_elementos", "lista_filas",
  "declaracion_matriz", "declaracion_array_multidimensional",
  "lista_bloques", "lista_elementos_o_bloques", "DimensionesCorchetes",
  "Dimensiones", "instruccion_do_while", "acceso", "DimensionesAcceso",
  "acceso_escritura", "Funcion_void", "Llamadas", "lista_parametros",
  "Funcion_parametros", "parametros", "TipoConCorchetes", "tipo_primitivo",
  "asignacion", "expresion", "expresion_or", "expresion_and",
  "expresion_bit_or", "expresion_bit_xor", "expresion_bit_and",
  "expresion_equality", "expresion_relacional", "expresion_shift",
  "expresion_aditiva", "expresion_multiplicativa", "expresion_unaria",
  "postfix", "primary", "lista_expresiones_join", "instruccion_print",
  "bloque", "instruccion_break", "instruccion_continue",
  "instruccion_return", "instruccion_if", "intruccion_switch",
  "lista_casos", "caso", "instruccion_while", "instruccion_for", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-387)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-191)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     123,  -387,  -387,  -387,  -387,  -387,  -387,  -387,    -1,     7,
      40,  -387,   123,  -387,  -387,  -387,    37,    42,    -3,  -387,
       9,  -387,  -387,  -387,  -387,    43,    39,   120,   181,  -387,
       1,   400,   161,   524,   154,   203,    46,   223,   150,  -387,
     233,   160,   272,    87,   280,   253,   286,    89,   203,   779,
    -387,   217,   203,  -387,  -387,   257,   288,   823,   294,   295,
     297,   823,   113,  -387,    26,   146,   266,   284,   287,  -387,
     289,  -387,   290,  1208,   217,   203,   453,  -387,   300,  -387,
     303,   305,   306,   307,  -387,   296,   309,  -387,  -387,  -387,
     131,   310,   311,  -387,   314,   315,   321,  -387,  -387,  -387,
    -387,   304,   157,  -387,   167,   823,   323,   515,   823,   342,
     823,   559,  -387,   823,  -387,  -387,  -387,  1208,  -387,  -387,
    1208,   916,  1208,  1208,  1208,  1208,  1208,  1208,  1208,  1208,
    1208,  1208,   344,  1170,   939,  1208,   992,     5,  1208,  -387,
     269,  -387,   291,  -387,  -387,  -387,  1148,  1208,   318,   319,
     320,   322,   324,   325,  -387,   293,  -387,  -387,   316,   326,
     329,   330,   327,    78,   174,   101,   193,   238,  -387,   207,
    -387,   339,   313,  -387,  -387,  -387,  -387,  -387,  -387,  -387,
    1208,  -387,   337,   360,    16,  -387,  -387,  -387,  -387,  -387,
    -387,  -387,  -387,  -387,   603,  -387,   647,  -387,   691,  -387,
     735,   363,  -387,   340,  -387,   164,  -387,  -387,  -387,  -387,
    -387,  -387,  -387,  -387,  -387,  -387,  -387,  1208,    25,  -387,
     341,   346,   349,   350,   351,   352,   354,   472,   209,   156,
     364,  -387,   343,   190,   356,   362,   365,  -387,  1208,  1208,
    1208,  1208,  1208,  1208,   332,  1208,  1208,  1208,  1208,  1208,
    1208,  1208,  1208,  1208,  1208,  1208,  1208,  1208,  1208,  1208,
    1208,  1208,  1208,  -387,  -387,   367,   361,  -387,  -387,   158,
     392,  1208,   377,  -387,  -387,  -387,  -387,  -387,  -387,  1208,
    -387,   397,  1208,  -387,   203,   203,   401,   203,   203,  1208,
    1014,  1208,   398,   385,  1208,  1208,  -387,   376,  1208,  1208,
    -387,   382,   383,   389,   402,   411,   412,  -387,   326,   329,
     330,   327,    78,   174,   174,   101,   101,   101,   101,   193,
     193,   238,   238,  -387,  -387,  -387,  1208,  1067,   405,   416,
     430,  -387,   188,  -387,  -387,   406,  -387,   393,   205,  -387,
    -387,   418,   420,   422,   413,   431,   425,   417,  1208,   419,
    -387,  -387,  -387,  -387,  -387,  1208,  1208,  -387,   421,   433,
     159,    31,  -387,   446,   217,  -387,    13,  1208,   427,    29,
    -387,  1208,  1208,  1208,   203,   434,  1208,  -387,   442,  -387,
     445,   180,   447,   456,   458,   463,  1208,   217,   423,  1089,
      34,  -387,   482,  -387,  -387,   464,   823,  -387,  -387,   454,
     457,   459,  -387,  1208,   460,  -387,  -387,  1208,  -387,  -387,
    -387,  -387,   103,    41,  -387,   487,   432,    82,    83,  -387,
     446,   473,   494,   823,   409,   203,   203,   203,   467,   203,
    -387,   499,   217,   478,  -387,  -387,  1208,  1208,   479,  -387,
    -387,  -387,   500,   486,   867,  -387,  -387,  -387,   203,  -387,
    1208,    85,   509,  -387,   514,  1208,  -387,   517,   481,  -387,
      86,  -387,   511,  1208,  -387,   490,  -387,  -387,  -387,  1208,
     520,  -387,    99,   526,  -387,  1208,   523,  -387
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    15,   101,    96,    99,   100,    97,    98,     0,     0,
       0,     2,     0,     4,     7,     8,     0,     0,     0,    14,
       0,     1,     3,     5,     6,     0,     0,     0,     0,    13,
       0,     0,    93,     0,     0,     0,     0,     0,     0,    12,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      73,     0,     0,    90,    91,     0,     0,     0,     0,    94,
       0,     0,     0,    75,     0,     0,     0,     0,     0,   179,
       0,   180,     0,     0,     0,     0,     0,    17,     0,    38,
       0,     0,     0,     0,    26,     0,     0,    30,    39,    32,
       0,     0,     0,    36,     0,     0,     0,    22,    23,    24,
      25,     0,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,    83,     0,   178,    37,    69,     0,    40,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,     0,     0,     0,     0,     0,   155,
       0,   149,   152,   153,   151,   150,     0,     0,     0,     0,
       0,     0,     0,     0,   168,   169,   170,   181,   113,   114,
     116,   118,   120,   122,   124,   127,   132,   135,   138,   145,
     146,     0,     0,   177,    16,    34,    18,    27,    28,    29,
       0,    20,     0,     0,    43,    19,    21,    33,    35,    31,
      89,    88,    11,    10,     0,    87,     0,    95,     0,    86,
       0,     0,   102,     0,    76,     0,    80,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,     0,   152,   176,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,     0,     0,     0,     0,     0,   142,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   147,   148,     0,     0,    72,    45,     0,
       0,     0,     0,    85,    82,    84,    81,    71,    78,     0,
      77,     0,     0,   174,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,   161,     0,     0,     0,
     171,     0,     0,     0,     0,     0,     0,   167,   115,   117,
     119,   121,   123,   125,   126,   131,   129,   130,   128,   133,
     134,   136,   137,   139,   140,   141,     0,     0,     0,     0,
      63,    42,     0,    79,    70,     0,   185,   182,     0,   192,
     191,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     144,   159,   156,   158,   157,     0,     0,    44,     0,     0,
       0,     0,    62,     0,     0,   175,     0,     0,     0,     0,
     188,     0,     0,     0,     0,     0,     0,   164,     0,   163,
     173,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,   183,   184,     0,     0,   186,   187,     0,
       0,     0,   195,     0,     0,   162,   166,     0,   165,   160,
      67,    66,     0,     0,    50,     0,     0,     0,     0,    57,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
     172,     0,     0,     0,    55,    47,     0,     0,     0,    60,
      61,    58,     0,     0,     0,   197,   198,   193,     0,   194,
       0,     0,     0,    49,     0,     0,    64,     0,     0,   196,
       0,    54,     0,     0,    46,     0,    65,   189,    52,     0,
       0,    48,     0,     0,    51,     0,     0,    53
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -387,  -387,  -387,  -387,   528,   -52,   -65,  -387,  -387,   404,
    -387,  -386,  -387,  -387,  -387,   149,   141,   229,  -387,  -387,
      73,  -387,  -387,   177,   -42,  -387,   185,   139,    24,     0,
     428,   -30,  -387,   328,   336,   338,   345,   347,    12,   -10,
      28,    30,  -134,  -387,  -387,  -387,  -387,   -34,  -387,  -387,
    -387,   201,  -387,  -387,   199,  -387,  -387
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    10,    11,    12,    13,    76,    77,    78,    79,    80,
      81,   413,   451,    82,    83,   390,   391,   272,   422,    84,
     155,   132,    86,    87,   156,   205,    89,    36,    16,    90,
      91,   414,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   381,    92,    93,    94,    95,
      96,    97,    98,   369,   370,    99,   100
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      17,    50,    39,   417,   231,   107,   226,    88,    19,   111,
      29,   174,    17,   237,    63,    88,    18,    49,   103,    88,
       2,   270,   227,     3,     4,     5,     6,     7,    38,   114,
      28,    38,   397,    38,    88,   386,   282,   419,   387,   271,
      21,   172,   174,   157,   435,    32,   174,    26,   388,    33,
     115,   102,    37,   194,    25,    37,   196,    37,   198,    27,
     420,   200,    67,    88,   460,    88,    88,   436,    88,    88,
     193,    88,    51,    40,   171,   101,    31,    30,    20,    74,
      52,   367,   368,   472,   234,   439,   440,   201,   461,   468,
     202,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   474,   220,   222,   223,   225,   431,   436,   420,
     432,   462,   436,    51,   112,    51,   236,   113,   250,   251,
     433,    58,    85,    62,     1,   436,   323,   324,   325,   174,
      85,   174,   182,   174,    85,   174,   183,   229,     2,   256,
     257,     3,     4,     5,     6,     7,   235,   116,   184,    85,
     267,   117,    88,    33,    88,    26,    88,   182,    88,   118,
     119,   292,    26,   328,   109,   350,    44,    54,   192,   120,
      43,    49,    47,   293,   191,   329,   385,    14,    85,   121,
      85,    85,    34,    85,    85,    15,    85,   281,    48,    23,
     279,     8,   363,     9,    56,   364,     2,    24,   280,     3,
       4,     5,     6,     7,   258,   259,   407,    49,   301,   302,
     303,   304,   305,   306,   408,    35,   252,   253,   254,   255,
     263,   264,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     2,   290,   291,     3,     4,     5,     6,     7,
      53,   331,   315,   316,   317,   318,   260,   261,   262,   333,
     336,   337,   335,   339,   340,   296,   297,   367,   368,   341,
     343,   344,   313,   314,   346,   347,    55,    85,   349,    85,
     116,    85,  -154,    85,   117,  -154,    57,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,   319,   320,    59,    60,   321,   322,
      61,   104,   105,  -154,  -154,  -154,   357,   359,   108,   133,
     109,   110,   121,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,   134,   378,   180,
     135,   190,   136,   137,   175,   380,   382,   176,   232,   177,
     178,   179,   393,   181,   185,   186,   233,   395,   187,   188,
     402,   399,   400,   401,   424,   189,   404,   115,   197,   217,
     234,   238,   239,   240,    88,   241,   265,   242,   243,   174,
     244,   268,   249,   245,   392,   247,   269,   248,   266,   277,
     434,   444,   246,   428,   278,   283,   295,   430,   289,   174,
     284,    88,    88,   285,   286,   287,   288,   415,   294,   298,
     326,   445,   446,   447,   327,   449,   299,   307,   330,   300,
     332,    41,    88,   334,   345,   338,   453,   454,   271,   348,
     106,   360,  -190,    49,   459,     2,   351,   352,     3,     4,
       5,     6,     7,   353,     2,   465,    65,     3,     4,     5,
       6,     7,   452,   470,    42,   270,   354,   355,   356,   361,
     365,    66,   371,   366,   372,   476,   373,   374,   375,   376,
     389,   377,   396,   379,   106,   383,   173,    49,    67,   403,
      68,  -190,  -190,    69,    70,    71,    72,   384,     2,    85,
      65,     3,     4,     5,     6,     7,   405,     8,    73,   406,
     410,   409,   411,    74,    75,    66,   412,   421,   425,   423,
     416,   426,   437,   427,   429,   120,    85,    85,   438,   443,
     442,   448,    67,   450,    68,   467,   456,    69,    70,    71,
      72,   121,   455,   457,   463,   469,   106,    85,   195,    49,
     464,     8,    73,   466,   471,    45,   473,    74,    75,   477,
       2,   475,    65,     3,     4,     5,     6,     7,   418,     2,
      22,   228,     3,     4,     5,     6,     7,    66,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,    46,   362,
     106,   441,   199,    49,    67,   230,    68,   394,   398,    69,
      70,    71,    72,   308,     2,     0,    65,     3,     4,     5,
       6,     7,   309,     8,    73,   310,     0,     0,     0,    74,
      75,    66,     0,   311,     0,     0,   312,     0,     0,     0,
       0,     0,     0,     0,   106,     0,   273,    49,    67,     0,
      68,     0,     0,    69,    70,    71,    72,     0,     2,     0,
      65,     3,     4,     5,     6,     7,     0,     8,    73,     0,
       0,     0,     0,    74,    75,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,     0,
     274,    49,    67,     0,    68,     0,     0,    69,    70,    71,
      72,     0,     2,     0,    65,     3,     4,     5,     6,     7,
       0,     8,    73,     0,     0,     0,     0,    74,    75,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106,     0,   275,    49,    67,     0,    68,     0,
       0,    69,    70,    71,    72,     0,     2,     0,    65,     3,
       4,     5,     6,     7,     0,     8,    73,     0,     0,     0,
       0,    74,    75,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   106,     0,   276,    49,
      67,     0,    68,     0,     0,    69,    70,    71,    72,     0,
       2,     0,    65,     3,     4,     5,     6,     7,     0,     8,
      73,     0,     0,     0,     0,    74,    75,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,    49,    67,     0,    68,     0,     0,    69,
      70,    71,    72,     0,     2,     0,    65,     3,     4,     5,
       6,     7,     0,     8,    73,     0,     0,     0,     0,    74,
      75,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   106,     0,     0,    49,    67,     0,
      68,     0,     0,    69,    70,    71,    72,     0,     2,     0,
      65,     3,     4,     5,     6,     7,     0,     8,    73,     0,
       0,     0,     0,    74,    75,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,     0,
       0,    49,    67,     0,    68,     0,     0,    69,    70,    71,
      72,     0,     2,     0,    65,     3,     4,     5,     6,     7,
       0,     8,    73,     0,     0,     0,     0,    74,    75,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,   203,    68,     0,
       0,   458,    70,    71,    72,     0,     0,     0,   138,     0,
       0,     0,   139,   140,     0,     8,    73,     0,     0,     0,
     221,    74,    75,   141,   142,   143,   144,   145,     0,   146,
     204,   138,     0,     0,     0,   139,   140,     0,     0,     0,
       0,     0,     0,     0,   147,     0,   141,   142,   143,   144,
     145,     0,   146,     0,   148,     0,   149,   150,   151,   152,
     153,     0,     0,     0,     0,     0,     0,   147,     0,   154,
       0,     0,     0,   224,     0,     0,     0,   148,     0,   149,
     150,   151,   152,   153,   138,     0,     0,     0,   139,   140,
       0,     0,   154,     0,     0,   342,     0,     0,     0,   141,
     142,   143,   144,   145,     0,   146,   138,     0,     0,     0,
     139,   140,     0,     0,     0,     0,     0,     0,     0,     0,
     147,   141,   142,   143,   144,   145,     0,   146,     0,     0,
     148,     0,   149,   150,   151,   152,   153,     0,     0,     0,
       0,     0,   147,     0,     0,   154,     0,     0,   358,     0,
       0,     0,   148,     0,   149,   150,   151,   152,   153,   138,
       0,     0,     0,   139,   140,     0,     0,   154,     0,     0,
       0,     0,     0,   389,   141,   142,   143,   144,   145,     0,
     146,   138,     0,     0,     0,   139,   140,     0,     0,     0,
       0,     0,     0,     0,     0,   147,   141,   142,   143,   144,
     145,     0,   146,     0,     0,   148,     0,   149,   150,   151,
     152,   153,     0,     0,     0,     0,     0,   147,     0,     0,
     154,     0,     0,     0,     0,     0,     0,   148,     0,   149,
     150,   151,   152,   153,     0,     0,     0,     0,     0,     0,
     138,     0,   154,     2,   139,   140,     3,     4,     5,     6,
       7,     0,     0,     0,     0,   141,   142,   143,   144,   145,
       0,   146,   138,     0,     0,     0,   139,   140,     0,     0,
       0,     0,     0,     0,     0,     0,   147,   141,   218,   143,
     144,   145,     0,   146,   219,     0,   148,     0,   149,   150,
     151,   152,   153,     0,     0,     0,     0,     0,   147,     0,
     138,   154,     0,     0,   139,   140,     0,     0,   148,     0,
     149,   150,   151,   152,   153,   141,   142,   143,   144,   145,
       0,   146,     0,   154,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   147,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   148,     0,   149,   150,
     151,   152,   153,     0,     0,     0,     0,     0,     0,     0,
       0,   154
};

static const yytype_int16 yycheck[] =
{
       0,    35,     1,   389,   138,    57,     1,    49,     1,    61,
       1,    76,    12,   147,    48,    57,    17,     4,    52,    61,
      15,     5,    17,    18,    19,    20,    21,    22,    28,     3,
      33,    31,     3,    33,    76,     4,    11,     3,     7,    23,
       0,    75,   107,    73,     3,     6,   111,     5,    17,    33,
      24,    51,    28,   105,    17,    31,   108,    33,   110,    17,
      26,   113,    49,   105,   450,   107,   108,    26,   110,   111,
     104,   113,    26,    72,    74,    51,    33,    68,    71,    74,
      34,    52,    53,   469,    59,     3,     3,   117,     3,     3,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,     3,   133,   134,   135,   136,     4,    26,    26,
       7,    26,    26,    26,     1,    26,   146,     4,    40,    41,
      17,    34,    49,    34,     1,    26,   260,   261,   262,   194,
      57,   196,     1,   198,    61,   200,     5,   137,    15,    38,
      39,    18,    19,    20,    21,    22,   146,     1,    17,    76,
     180,     5,   194,    33,   196,     5,   198,     1,   200,    13,
      14,     5,     5,     5,     5,   299,     5,    17,     1,    23,
      31,     4,    33,    17,    17,    17,    17,     0,   105,    33,
     107,   108,     1,   110,   111,     0,   113,   217,    34,    12,
      26,    68,     4,    70,    34,     7,    15,    12,    34,    18,
      19,    20,    21,    22,    11,    12,    26,     4,   238,   239,
     240,   241,   242,   243,    34,    34,    42,    43,    44,    45,
      13,    14,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    15,    24,    25,    18,    19,    20,    21,    22,
      17,   271,   252,   253,   254,   255,     8,     9,    10,   279,
     284,   285,   282,   287,   288,    65,    66,    52,    53,   289,
     290,   291,   250,   251,   294,   295,    33,   194,   298,   196,
       1,   198,     3,   200,     5,     6,     4,     8,     9,    10,
      11,    12,    13,    14,   256,   257,     6,    34,   258,   259,
       4,    34,     4,    24,    25,    26,   326,   327,     4,    33,
       5,     4,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    33,   348,    23,
      33,    17,    33,    33,    24,   355,   356,    24,    59,    24,
      24,    24,   366,    24,    24,    24,    67,   367,    24,    24,
     374,   371,   372,   373,   396,    24,   376,    24,     6,     5,
      59,    33,    33,    33,   396,    33,    17,    33,    33,   424,
      67,    24,    35,    47,   364,    36,     6,    37,    55,     6,
     412,   423,    46,   403,    34,    34,    33,   407,    24,   444,
      34,   423,   424,    34,    34,    34,    34,   387,    24,    33,
      23,   425,   426,   427,    33,   429,    34,    65,     6,    34,
      23,     1,   444,     6,     6,     4,   436,   437,    23,    33,
       1,     6,     3,     4,   448,    15,    34,    34,    18,    19,
      20,    21,    22,    34,    15,   455,    17,    18,    19,    20,
      21,    22,   432,   463,    34,     5,    34,    26,    26,    23,
      34,    32,    24,    50,    24,   475,    24,    34,    17,    24,
       4,    34,    25,    34,     1,    34,     3,     4,    49,    25,
      51,    52,    53,    54,    55,    56,    57,    34,    15,   396,
      17,    18,    19,    20,    21,    22,    34,    68,    69,    34,
      24,    34,    24,    74,    75,    32,    23,     5,    34,    25,
      67,    34,     5,    34,    34,    23,   423,   424,    66,     5,
      27,    34,    49,     4,    51,    24,     6,    54,    55,    56,
      57,    33,    33,    27,     5,     4,     1,   444,     3,     4,
       6,    68,    69,     6,    34,     1,     6,    74,    75,     6,
      15,     5,    17,    18,    19,    20,    21,    22,   389,    15,
      12,   137,    18,    19,    20,    21,    22,    32,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    34,   330,
       1,   420,     3,     4,    49,   137,    51,   366,   369,    54,
      55,    56,    57,   245,    15,    -1,    17,    18,    19,    20,
      21,    22,   246,    68,    69,   247,    -1,    -1,    -1,    74,
      75,    32,    -1,   248,    -1,    -1,   249,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,     4,    49,    -1,
      51,    -1,    -1,    54,    55,    56,    57,    -1,    15,    -1,
      17,    18,    19,    20,    21,    22,    -1,    68,    69,    -1,
      -1,    -1,    -1,    74,    75,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,    49,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    -1,    15,    -1,    17,    18,    19,    20,    21,    22,
      -1,    68,    69,    -1,    -1,    -1,    -1,    74,    75,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,    49,    -1,    51,    -1,
      -1,    54,    55,    56,    57,    -1,    15,    -1,    17,    18,
      19,    20,    21,    22,    -1,    68,    69,    -1,    -1,    -1,
      -1,    74,    75,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
      49,    -1,    51,    -1,    -1,    54,    55,    56,    57,    -1,
      15,    -1,    17,    18,    19,    20,    21,    22,    -1,    68,
      69,    -1,    -1,    -1,    -1,    74,    75,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,     4,    49,    -1,    51,    -1,    -1,    54,
      55,    56,    57,    -1,    15,    -1,    17,    18,    19,    20,
      21,    22,    -1,    68,    69,    -1,    -1,    -1,    -1,    74,
      75,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,    -1,     4,    49,    -1,
      51,    -1,    -1,    54,    55,    56,    57,    -1,    15,    -1,
      17,    18,    19,    20,    21,    22,    -1,    68,    69,    -1,
      -1,    -1,    -1,    74,    75,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
      -1,     4,    49,    -1,    51,    -1,    -1,    54,    55,    56,
      57,    -1,    15,    -1,    17,    18,    19,    20,    21,    22,
      -1,    68,    69,    -1,    -1,    -1,    -1,    74,    75,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,     1,    51,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    16,    17,    -1,    68,    69,    -1,    -1,    -1,
       1,    74,    75,    27,    28,    29,    30,    31,    -1,    33,
      34,    12,    -1,    -1,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    27,    28,    29,    30,
      31,    -1,    33,    -1,    58,    -1,    60,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    73,
      -1,    -1,    -1,     1,    -1,    -1,    -1,    58,    -1,    60,
      61,    62,    63,    64,    12,    -1,    -1,    -1,    16,    17,
      -1,    -1,    73,    -1,    -1,     1,    -1,    -1,    -1,    27,
      28,    29,    30,    31,    -1,    33,    12,    -1,    -1,    -1,
      16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    27,    28,    29,    30,    31,    -1,    33,    -1,    -1,
      58,    -1,    60,    61,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    73,    -1,    -1,     1,    -1,
      -1,    -1,    58,    -1,    60,    61,    62,    63,    64,    12,
      -1,    -1,    -1,    16,    17,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    -1,     4,    27,    28,    29,    30,    31,    -1,
      33,    12,    -1,    -1,    -1,    16,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    27,    28,    29,    30,
      31,    -1,    33,    -1,    -1,    58,    -1,    60,    61,    62,
      63,    64,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    60,
      61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    73,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    27,    28,    29,    30,    31,
      -1,    33,    12,    -1,    -1,    -1,    16,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    58,    -1,    60,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      12,    73,    -1,    -1,    16,    17,    -1,    -1,    58,    -1,
      60,    61,    62,    63,    64,    27,    28,    29,    30,    31,
      -1,    33,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    60,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    15,    18,    19,    20,    21,    22,    68,    70,
      88,    89,    90,    91,   110,   113,   115,   116,    17,     1,
      71,     0,    91,   110,   113,    17,     5,    17,    33,     1,
      68,    33,     6,    33,     1,    34,   114,   115,   116,     1,
      72,     1,    34,   114,     5,     1,    34,   114,    34,     4,
     134,    26,    34,    17,    17,    33,    34,     4,    34,     6,
      34,     4,    34,   134,     1,    17,    32,    49,    51,    54,
      55,    56,    57,    69,    74,    75,    92,    93,    94,    95,
      96,    97,   100,   101,   106,   107,   109,   110,   111,   113,
     116,   117,   133,   134,   135,   136,   137,   138,   139,   142,
     143,   115,   116,   134,    34,     4,     1,    92,     4,     5,
       4,    92,     1,     4,     3,    24,     1,     5,    13,    14,
      23,    33,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,   108,    33,    33,    33,    33,    33,    12,    16,
      17,    27,    28,    29,    30,    31,    33,    48,    58,    60,
      61,    62,    63,    64,    73,   107,   111,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   116,   134,     3,    93,    24,    24,    24,    24,    24,
      23,    24,     1,     5,    17,    24,    24,    24,    24,    24,
      17,    17,     1,   134,    92,     3,    92,     6,    92,     3,
      92,   118,   118,     1,    34,   112,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   118,     5,    28,    34,
     118,     1,   118,   118,     1,   118,     1,    17,    96,   116,
     117,   129,    59,    67,    59,   116,   118,   129,    33,    33,
      33,    33,    33,    33,    67,    47,    46,    36,    37,    35,
      40,    41,    42,    43,    44,    45,    38,    39,    11,    12,
       8,     9,    10,    13,    14,    17,    55,   118,    24,     6,
       5,    23,   104,     3,     3,     3,     3,     6,    34,    26,
      34,   118,    11,    34,    34,    34,    34,    34,    34,    24,
      24,    25,     5,    17,    24,    33,    65,    66,    33,    34,
      34,   118,   118,   118,   118,   118,   118,    65,   120,   121,
     122,   123,   124,   125,   125,   126,   126,   126,   126,   127,
     127,   128,   128,   129,   129,   129,    23,    33,     5,    17,
       6,   118,    23,   118,     6,   118,   134,   134,     4,   134,
     134,   118,     1,   118,   118,     6,   118,   118,    33,   118,
     129,    34,    34,    34,    34,    26,    26,   118,     1,   118,
       6,    23,   104,     4,     7,    34,    50,    52,    53,   140,
     141,    24,    24,    24,    34,    17,    24,    34,   118,    34,
     118,   132,   118,    34,    34,    17,     4,     7,    17,     4,
     102,   103,   116,   134,   138,   118,    25,     3,   141,   118,
     118,   118,   134,    25,   118,    34,    34,    26,    34,    34,
      24,    24,    23,    98,   118,   116,    67,    98,   102,     3,
      26,     5,   105,    25,    92,    34,    34,    34,   118,    34,
     118,     4,     7,    17,   111,     3,    26,     5,    66,     3,
       3,   103,    27,     5,    92,   134,   134,   134,    34,   134,
       4,    99,   116,   118,   118,    33,     6,    27,    54,   134,
      98,     3,    26,     5,     6,   118,     6,    24,     3,     4,
     118,    34,    98,     6,     3,     5,   118,     6
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    87,    88,    89,    89,    90,    90,    90,    90,    90,
      91,    91,    91,    91,    91,    91,    92,    92,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    94,    94,
      95,    95,    96,    96,    96,    96,    97,    97,    97,    98,
      98,    99,    99,   100,   100,   100,   101,   101,   102,   102,
     103,   103,   104,   104,   105,   105,   106,   106,   107,   107,
     108,   108,   109,   110,   110,   110,   111,   111,   111,   112,
     112,   113,   113,   113,   113,   113,   113,   113,   114,   114,
     114,   114,   115,   115,   115,   115,   116,   116,   116,   116,
     116,   116,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   118,   119,   119,   120,   120,   121,   121,
     122,   122,   123,   123,   124,   124,   124,   125,   125,   125,
     125,   125,   126,   126,   126,   127,   127,   127,   128,   128,
     128,   128,   129,   129,   129,   129,   130,   130,   130,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   132,   132,   133,   133,   133,   134,   134,   135,
     136,   137,   138,   138,   138,   138,   139,   140,   140,   141,
     141,   142,   142,   143,   143,   143,   143,   143,   143
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     2,     2,     1,     1,     0,
       7,     7,     4,     3,     2,     1,     2,     1,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     2,     2,     2,
       1,     2,     1,     2,     2,     2,     1,     2,     1,     1,
       2,     2,     4,     2,     5,     3,    10,     8,    11,     3,
       1,     5,     3,    15,    10,     8,     7,     7,     3,     1,
       3,     3,     3,     2,     3,     4,     7,     7,     2,     2,
       4,     3,     3,     5,     6,     6,     3,     4,     4,     3,
       1,     8,     8,     6,     8,     8,     7,     7,     4,     4,
       2,     2,     1,     3,     5,     7,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     2,     2,     4,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     4,     4,
       6,     3,     6,     5,     5,     6,     6,     3,     1,     1,
       1,     3,     3,     1,     4,     6,     3,     3,     3,     1,
       1,     2,     5,     7,     7,     5,     7,     2,     1,     6,
       3,     5,     5,     9,     9,     7,    10,     9,     9
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
#line 114 "src/parser/parser.y"
             { astGlobal = (yyvsp[0].nodo); }
#line 2103 "src/parser/parser.tab.c"
    break;

  case 3: /* programa: lista_funciones main_class  */
#line 118 "src/parser/parser.y"
                                 {
          NodoAST* raiz = crearNodo(AST_PROGRAMA, "Programa");
          agregarHijo(raiz, (yyvsp[-1].nodo));
          agregarHijo(raiz, (yyvsp[0].nodo));
          (yyval.nodo) = raiz;
      }
#line 2114 "src/parser/parser.tab.c"
    break;

  case 4: /* programa: main_class  */
#line 124 "src/parser/parser.y"
                 {
          NodoAST* raiz = crearNodo(AST_PROGRAMA, "Programa");
          agregarHijo(raiz, (yyvsp[0].nodo));
          (yyval.nodo) = raiz;
      }
#line 2124 "src/parser/parser.tab.c"
    break;

  case 5: /* lista_funciones: lista_funciones Funcion_void  */
#line 132 "src/parser/parser.y"
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
#line 2140 "src/parser/parser.tab.c"
    break;

  case 6: /* lista_funciones: lista_funciones Funcion_parametros  */
#line 143 "src/parser/parser.y"
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
#line 2156 "src/parser/parser.tab.c"
    break;

  case 7: /* lista_funciones: Funcion_void  */
#line 154 "src/parser/parser.y"
                   {
          NodoAST* lista = crearNodo(AST_LISTA, "Funciones");
          agregarHijo(lista, (yyvsp[0].nodo));
          (yyval.nodo) = lista;
      }
#line 2166 "src/parser/parser.tab.c"
    break;

  case 8: /* lista_funciones: Funcion_parametros  */
#line 159 "src/parser/parser.y"
                         {
          NodoAST* lista = crearNodo(AST_LISTA, "Funciones");
          agregarHijo(lista, (yyvsp[0].nodo));
          (yyval.nodo) = lista;
      }
#line 2176 "src/parser/parser.tab.c"
    break;

  case 9: /* lista_funciones: %empty  */
#line 164 "src/parser/parser.y"
                   {
          (yyval.nodo) = crearNodo(AST_LISTA, "Funciones");
      }
#line 2184 "src/parser/parser.tab.c"
    break;

  case 10: /* main_class: TOKEN_PUBLIC TOKEN_STATIC TOKEN_VOID TOKEN_main TOKEN_PARIZQ TOKEN_PARDER bloque  */
#line 170 "src/parser/parser.y"
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
#line 2199 "src/parser/parser.tab.c"
    break;

  case 11: /* main_class: TOKEN_PUBLIC TOKEN_STATIC TOKEN_VOID TOKEN_main TOKEN_PARIZQ TOKEN_PARDER error  */
#line 180 "src/parser/parser.y"
                                                                                      {
        addErrorSintactico("Error en el bloque del método 'main'", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = crearNodo(AST_METODO_MAIN, "public static void main");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        NodoAST* bloqueVacio = crearNodo(AST_BLOQUE, "Bloque vacío");
        agregarHijo((yyval.nodo), bloqueVacio);
    }
#line 2211 "src/parser/parser.tab.c"
    break;

  case 12: /* main_class: TOKEN_PUBLIC TOKEN_STATIC TOKEN_VOID error  */
#line 187 "src/parser/parser.y"
                                                 {
        addErrorSintactico("Falta el nombre del método principal 'main'", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = NULL;
    }
#line 2220 "src/parser/parser.tab.c"
    break;

  case 13: /* main_class: TOKEN_PUBLIC TOKEN_STATIC error  */
#line 191 "src/parser/parser.y"
                                      {
        addErrorSintactico("Falta el tipo de retorno 'void' en el método principal", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = NULL;
    }
#line 2229 "src/parser/parser.tab.c"
    break;

  case 14: /* main_class: TOKEN_PUBLIC error  */
#line 195 "src/parser/parser.y"
                         {
        addErrorSintactico("Falta la palabra clave 'static' en el método principal", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = NULL;
    }
#line 2238 "src/parser/parser.tab.c"
    break;

  case 15: /* main_class: error  */
#line 199 "src/parser/parser.y"
            {
        addErrorSintactico("Error en la declaración del método principal", "global", (yylsp[0]).first_line, (yylsp[0]).first_column);
        (yyval.nodo) = NULL;
    }
#line 2247 "src/parser/parser.tab.c"
    break;

  case 16: /* lista_sentencias: lista_sentencias sentencia  */
#line 208 "src/parser/parser.y"
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
#line 2263 "src/parser/parser.tab.c"
    break;

  case 17: /* lista_sentencias: sentencia  */
#line 219 "src/parser/parser.y"
                { 
        if ((yyvsp[0].nodo)) {
            (yyval.nodo) = (yyvsp[0].nodo);
        } else {
            (yyval.nodo) = crearNodo(AST_LISTA, "Programa");
        }
    }
#line 2275 "src/parser/parser.tab.c"
    break;

  case 18: /* sentencia: declaracion TOKEN_PUNTOCOMA  */
#line 230 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2281 "src/parser/parser.tab.c"
    break;

  case 19: /* sentencia: asignacion TOKEN_PUNTOCOMA  */
#line 231 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2287 "src/parser/parser.tab.c"
    break;

  case 20: /* sentencia: acceso_escritura TOKEN_PUNTOCOMA  */
#line 232 "src/parser/parser.y"
                                       { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2293 "src/parser/parser.tab.c"
    break;

  case 21: /* sentencia: instruccion_print TOKEN_PUNTOCOMA  */
#line 233 "src/parser/parser.y"
                                        { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2299 "src/parser/parser.tab.c"
    break;

  case 22: /* sentencia: instruccion_if  */
#line 234 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2305 "src/parser/parser.tab.c"
    break;

  case 23: /* sentencia: intruccion_switch  */
#line 235 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2311 "src/parser/parser.tab.c"
    break;

  case 24: /* sentencia: instruccion_while  */
#line 236 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2317 "src/parser/parser.tab.c"
    break;

  case 25: /* sentencia: instruccion_for  */
#line 237 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2323 "src/parser/parser.tab.c"
    break;

  case 26: /* sentencia: instruccion_do_while  */
#line 238 "src/parser/parser.y"
                                { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2329 "src/parser/parser.tab.c"
    break;

  case 27: /* sentencia: declaracion_vector TOKEN_PUNTOCOMA  */
#line 239 "src/parser/parser.y"
                                              { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2335 "src/parser/parser.tab.c"
    break;

  case 28: /* sentencia: declaracion_matriz TOKEN_PUNTOCOMA  */
#line 240 "src/parser/parser.y"
                                              { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2341 "src/parser/parser.tab.c"
    break;

  case 29: /* sentencia: declaracion_array_multidimensional TOKEN_PUNTOCOMA  */
#line 241 "src/parser/parser.y"
                                                         { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2347 "src/parser/parser.tab.c"
    break;

  case 30: /* sentencia: Funcion_void  */
#line 242 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2353 "src/parser/parser.tab.c"
    break;

  case 31: /* sentencia: instruccion_return TOKEN_PUNTOCOMA  */
#line 243 "src/parser/parser.y"
                                         { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2359 "src/parser/parser.tab.c"
    break;

  case 32: /* sentencia: Funcion_parametros  */
#line 244 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2365 "src/parser/parser.tab.c"
    break;

  case 33: /* sentencia: instruccion_break TOKEN_PUNTOCOMA  */
#line 245 "src/parser/parser.y"
                                        {(yyval.nodo) = (yyvsp[-1].nodo); }
#line 2371 "src/parser/parser.tab.c"
    break;

  case 34: /* sentencia: expresion_sentencia TOKEN_PUNTOCOMA  */
#line 246 "src/parser/parser.y"
                                              { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2377 "src/parser/parser.tab.c"
    break;

  case 35: /* sentencia: instruccion_continue TOKEN_PUNTOCOMA  */
#line 247 "src/parser/parser.y"
                                           {(yyval.nodo) = (yyvsp[-1].nodo); }
#line 2383 "src/parser/parser.tab.c"
    break;

  case 36: /* sentencia: bloque  */
#line 248 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2389 "src/parser/parser.tab.c"
    break;

  case 37: /* sentencia: error TOKEN_PUNTOCOMA  */
#line 250 "src/parser/parser.y"
                                    { 
        addErrorSintactico("Instrucción inválida", "global", (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        yyerrok; 
        (yyval.nodo) = NULL;
    }
#line 2399 "src/parser/parser.tab.c"
    break;

  case 40: /* postfix_incremento: TOKEN_IDENTIFICADOR TOKEN_INCREMENTO  */
#line 265 "src/parser/parser.y"
                                           {
          (yyval.nodo) = crearNodo(AST_INCREMENTO, "++");
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
          agregarHijo((yyval.nodo), crearNodo(AST_IDENTIFICADOR, (yyvsp[-1].str)));
      }
#line 2409 "src/parser/parser.tab.c"
    break;

  case 41: /* postfix_incremento: TOKEN_IDENTIFICADOR TOKEN_DECREMENTO  */
#line 270 "src/parser/parser.y"
                                           {
          (yyval.nodo) = crearNodo(AST_DECREMENTO, "--");
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
          agregarHijo((yyval.nodo), crearNodo(AST_IDENTIFICADOR, (yyvsp[-1].str)));
      }
#line 2419 "src/parser/parser.tab.c"
    break;

  case 42: /* declaracion: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_IGUAL expresion  */
#line 279 "src/parser/parser.y"
                                                             {
        (yyval.nodo) = crearNodo(AST_DECLARACION, "declaracion");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-3].str)));
        
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2434 "src/parser/parser.tab.c"
    break;

  case 43: /* declaracion: tipo_primitivo TOKEN_IDENTIFICADOR  */
#line 289 "src/parser/parser.y"
                                         {
        (yyval.nodo) = crearNodo(AST_DECLARACION, "declaracion");
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-1].str)));
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
        id->linea = (yylsp[0]).first_line; id->columna = (yylsp[0]).first_column;
        agregarHijo((yyval.nodo), id);
    }
#line 2447 "src/parser/parser.tab.c"
    break;

  case 44: /* declaracion: TOKEN_FINAL tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_IGUAL expresion  */
#line 297 "src/parser/parser.y"
                                                                           {
        (yyval.nodo) = crearNodo(AST_DECLARACION_CONST, "declaracion_const");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-3].str)));
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2461 "src/parser/parser.tab.c"
    break;

  case 45: /* declaracion: tipo_primitivo error TOKEN_PUNTOCOMA  */
#line 306 "src/parser/parser.y"
                                           {
        addErrorSintactico("Error en declaración", "global", (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        yyerrok;
        (yyval.nodo) = NULL;
    }
#line 2471 "src/parser/parser.tab.c"
    break;

  case 46: /* declaracion_vector: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_NEW tipo_primitivo TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER  */
#line 314 "src/parser/parser.y"
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
#line 2496 "src/parser/parser.tab.c"
    break;

  case 47: /* declaracion_vector: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 334 "src/parser/parser.y"
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
#line 2512 "src/parser/parser.tab.c"
    break;

  case 48: /* declaracion_vector: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_IDENTIFICADOR TOKEN_PUNTO TOKEN_ADD TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 345 "src/parser/parser.y"
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
#line 2546 "src/parser/parser.tab.c"
    break;

  case 49: /* lista_elementos: lista_elementos TOKEN_COMA expresion  */
#line 377 "src/parser/parser.y"
                                         {
        if ((yyvsp[-2].nodo)->tipo != AST_INTERNO) {
            NodoAST* lista = crearNodo(AST_INTERNO, "Lista Interna");
            agregarHijo(lista, (yyvsp[-2].nodo));
            agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            agregarHijo((yyvsp[-2].nodo), (yyvsp[0].nodo));
            (yyval.nodo) = (yyvsp[-2].nodo);
        }
    }
#line 2562 "src/parser/parser.tab.c"
    break;

  case 50: /* lista_elementos: expresion  */
#line 388 "src/parser/parser.y"
                {
        (yyval.nodo) = crearNodo(AST_INTERNO, "Lista Interna");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2571 "src/parser/parser.tab.c"
    break;

  case 51: /* lista_filas: lista_filas TOKEN_COMA TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 395 "src/parser/parser.y"
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
#line 2587 "src/parser/parser.tab.c"
    break;

  case 52: /* lista_filas: TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 406 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crearNodo(AST_LISTA_ELEMENTOS, "Lista Filas");
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2596 "src/parser/parser.tab.c"
    break;

  case 53: /* declaracion_matriz: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_NEW tipo_primitivo TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER  */
#line 413 "src/parser/parser.y"
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
#line 2625 "src/parser/parser.tab.c"
    break;

  case 54: /* declaracion_matriz: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_LLAVEIZQ lista_filas TOKEN_LLAVEDER  */
#line 437 "src/parser/parser.y"
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
#line 2649 "src/parser/parser.tab.c"
    break;

  case 55: /* declaracion_matriz: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL Llamadas  */
#line 456 "src/parser/parser.y"
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
#line 2672 "src/parser/parser.tab.c"
    break;

  case 56: /* declaracion_array_multidimensional: tipo_primitivo TOKEN_IDENTIFICADOR DimensionesCorchetes TOKEN_IGUAL TOKEN_NEW tipo_primitivo Dimensiones  */
#line 477 "src/parser/parser.y"
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
#line 2704 "src/parser/parser.tab.c"
    break;

  case 57: /* declaracion_array_multidimensional: tipo_primitivo TOKEN_IDENTIFICADOR DimensionesCorchetes TOKEN_IGUAL TOKEN_LLAVEIZQ lista_bloques TOKEN_LLAVEDER  */
#line 504 "src/parser/parser.y"
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
#line 2726 "src/parser/parser.tab.c"
    break;

  case 58: /* lista_bloques: lista_bloques TOKEN_COMA lista_elementos_o_bloques  */
#line 524 "src/parser/parser.y"
                                                       {
        if ((yyvsp[-2].nodo)->tipo != AST_INTERNO) {
            NodoAST* lista = crearNodo(AST_INTERNO, "Lista Bloques (INTERNO)");
            agregarHijo(lista, (yyvsp[-2].nodo));
            agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            agregarHijo((yyvsp[-2].nodo), (yyvsp[0].nodo));
            (yyval.nodo) = (yyvsp[-2].nodo);
        }
    }
#line 2742 "src/parser/parser.tab.c"
    break;

  case 59: /* lista_bloques: lista_elementos_o_bloques  */
#line 535 "src/parser/parser.y"
                                {
        (yyval.nodo) = crearNodo(AST_INTERNO, "Lista Bloques (INTERNO)");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2751 "src/parser/parser.tab.c"
    break;

  case 60: /* lista_elementos_o_bloques: TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 542 "src/parser/parser.y"
                                                  {
        // Caso: lista de elementos
        NodoAST* bloque = crearNodo(AST_INTERNO, "Bloque (INTERNO)");
        agregarHijo(bloque, (yyvsp[-1].nodo));
        (yyval.nodo) = bloque;
    }
#line 2762 "src/parser/parser.tab.c"
    break;

  case 61: /* lista_elementos_o_bloques: TOKEN_LLAVEIZQ lista_bloques TOKEN_LLAVEDER  */
#line 548 "src/parser/parser.y"
                                                  {
        // Caso: bloques anidados
        NodoAST* bloque = crearNodo(AST_INTERNO, "Bloque (INTERNO)");
        agregarHijo(bloque, (yyvsp[-1].nodo));
        (yyval.nodo) = bloque;
    }
#line 2773 "src/parser/parser.tab.c"
    break;

  case 62: /* DimensionesCorchetes: TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER DimensionesCorchetes  */
#line 557 "src/parser/parser.y"
                                                             {
        NodoAST* dim = crearNodo(AST_DIMENSIONES, "[]");
        if ((yyvsp[0].nodo) == NULL) {
            (yyval.nodo) = dim;
        } else if ((yyvsp[0].nodo)->tipo != AST_INTERNO) {
            NodoAST* lista = crearNodo(AST_INTERNO, "Dimensiones (INTERNO)");
            agregarHijo(lista, dim);
            agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            /* Insertar al principio de la lista */
            NodoAST* nueva = crearNodo(AST_INTERNO, "Dimensiones (INTERNO)");
            agregarHijo(nueva, dim);
            for (int i = 0; i < (yyvsp[0].nodo)->num_hijos; i++) {
                agregarHijo(nueva, (yyvsp[0].nodo)->hijos[i]);
            }
            (yyval.nodo) = nueva;
        }
    }
#line 2797 "src/parser/parser.tab.c"
    break;

  case 63: /* DimensionesCorchetes: TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER  */
#line 576 "src/parser/parser.y"
                                          {
        (yyval.nodo) = crearNodo(AST_DIMENSIONES, "[]");
    }
#line 2805 "src/parser/parser.tab.c"
    break;

  case 64: /* Dimensiones: TOKEN_CORCHETEIZQ TOKEN_NUMERO TOKEN_CORCHETEDER  */
#line 582 "src/parser/parser.y"
                                                     {
        (yyval.nodo) = crearNodo(AST_DIMENSIONES, "[]");
        agregarHijo((yyval.nodo), crearNodo(AST_NUMERO, (yyvsp[-1].str)));
    }
#line 2814 "src/parser/parser.tab.c"
    break;

  case 65: /* Dimensiones: Dimensiones TOKEN_CORCHETEIZQ TOKEN_NUMERO TOKEN_CORCHETEDER  */
#line 586 "src/parser/parser.y"
                                                                   {
        agregarHijo((yyvsp[-3].nodo), crearNodo(AST_NUMERO, (yyvsp[-1].str)));
        (yyval.nodo) = (yyvsp[-3].nodo);
    }
#line 2823 "src/parser/parser.tab.c"
    break;

  case 66: /* instruccion_do_while: TOKEN_DO bloque TOKEN_WHILE TOKEN_PARIZQ expresion TOKEN_PARDER TOKEN_PUNTOCOMA  */
#line 593 "src/parser/parser.y"
                                                                                    {
        (yyval.nodo) = crearNodo(AST_DO_WHILE, "do-while");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; 
        (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-5].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
    }
#line 2835 "src/parser/parser.tab.c"
    break;

  case 67: /* instruccion_do_while: TOKEN_DO bloque TOKEN_WHILE TOKEN_PARIZQ error TOKEN_PARDER TOKEN_PUNTOCOMA  */
#line 600 "src/parser/parser.y"
                                                                                  {
        addErrorSintactico("Expresión inválida en condición do-while", "global", (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        (yyval.nodo) = crearNodo(AST_DO_WHILE, "do-while");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; 
        (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-5].nodo));
        agregarHijo((yyval.nodo), crearNodo(AST_BOOLEAN, "false"));
    }
#line 2848 "src/parser/parser.tab.c"
    break;

  case 68: /* acceso: TOKEN_IDENTIFICADOR DimensionesAcceso  */
#line 614 "src/parser/parser.y"
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
#line 2865 "src/parser/parser.tab.c"
    break;

  case 69: /* acceso: TOKEN_IDENTIFICADOR error  */
#line 626 "src/parser/parser.y"
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
#line 2883 "src/parser/parser.tab.c"
    break;

  case 70: /* DimensionesAcceso: DimensionesAcceso TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER  */
#line 642 "src/parser/parser.y"
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
#line 2898 "src/parser/parser.tab.c"
    break;

  case 71: /* DimensionesAcceso: TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER  */
#line 652 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crearNodo(AST_DIMENSIONES, "Dimensiones");
        NodoAST* dim = crearNodo(AST_1DIMENSION, "Dimensión");
        agregarHijo(dim, (yyvsp[-1].nodo)); // Agregar la expresión como una dimensión
        agregarHijo((yyval.nodo), dim);
    }
#line 2909 "src/parser/parser.tab.c"
    break;

  case 72: /* acceso_escritura: acceso TOKEN_IGUAL expresion  */
#line 662 "src/parser/parser.y"
                                 {
        (yyval.nodo) = crearNodo(AST_ACCESO_ESCRITURA, "Acceso Escritura");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;

        // Nodo para el acceso (identificador + dimensiones)
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));

        // Nodo para la expresión (valor a asignar)
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2924 "src/parser/parser.tab.c"
    break;

  case 73: /* Funcion_void: TOKEN_VOID TOKEN_IDENTIFICADOR TOKEN_PARIZQ TOKEN_PARDER bloque  */
#line 677 "src/parser/parser.y"
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
#line 2941 "src/parser/parser.tab.c"
    break;

  case 74: /* Funcion_void: TOKEN_VOID TOKEN_IDENTIFICADOR TOKEN_PARIZQ parametros TOKEN_PARDER bloque  */
#line 689 "src/parser/parser.y"
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
#line 2958 "src/parser/parser.tab.c"
    break;

  case 75: /* Funcion_void: TOKEN_VOID TOKEN_IDENTIFICADOR TOKEN_PARIZQ error TOKEN_PARDER bloque  */
#line 701 "src/parser/parser.y"
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
#line 2976 "src/parser/parser.tab.c"
    break;

  case 76: /* Llamadas: TOKEN_IDENTIFICADOR TOKEN_PARIZQ TOKEN_PARDER  */
#line 717 "src/parser/parser.y"
                                                  {
        (yyval.nodo) = crearNodo(AST_LLAMADA_VOID, "Llamada funcion void");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;

        NodoAST* id1 = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id1->linea = (yylsp[-2]).first_line; id1->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id1);
    }
#line 2989 "src/parser/parser.tab.c"
    break;

  case 77: /* Llamadas: TOKEN_IDENTIFICADOR TOKEN_PARIZQ lista_parametros TOKEN_PARDER  */
#line 725 "src/parser/parser.y"
                                                                     {
        (yyval.nodo) = crearNodo(AST_LLAMADA, "Llamada funcion");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;

        NodoAST* id1 = crearNodo(AST_IDENTIFICADOR, (yyvsp[-3].str));
        id1->linea = (yylsp[-3]).first_line; id1->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), id1);

        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3004 "src/parser/parser.tab.c"
    break;

  case 78: /* Llamadas: TOKEN_IDENTIFICADOR TOKEN_PARIZQ error TOKEN_PARDER  */
#line 735 "src/parser/parser.y"
                                                          {
        addErrorSintactico("Parámetros inválidos en llamada a función", "global", (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        (yyval.nodo) = crearNodo(AST_LLAMADA_VOID, "Llamada funcion void");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        
        NodoAST* id1 = crearNodo(AST_IDENTIFICADOR, (yyvsp[-3].str));
        id1->linea = (yylsp[-3]).first_line; id1->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), id1);
    }
#line 3018 "src/parser/parser.tab.c"
    break;

  case 79: /* lista_parametros: lista_parametros TOKEN_COMA expresion  */
#line 747 "src/parser/parser.y"
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
#line 3034 "src/parser/parser.tab.c"
    break;

  case 80: /* lista_parametros: expresion  */
#line 758 "src/parser/parser.y"
                {
        (yyval.nodo) = crearNodo(AST_LISTA_PARAMETROS, "Lista Parametros");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 3043 "src/parser/parser.tab.c"
    break;

  case 81: /* Funcion_parametros: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_PARIZQ parametros TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 765 "src/parser/parser.y"
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
#line 3063 "src/parser/parser.tab.c"
    break;

  case 82: /* Funcion_parametros: TipoConCorchetes TOKEN_IDENTIFICADOR TOKEN_PARIZQ parametros TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 780 "src/parser/parser.y"
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
#line 3086 "src/parser/parser.tab.c"
    break;

  case 83: /* Funcion_parametros: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_PARIZQ parametros TOKEN_PARDER error  */
#line 798 "src/parser/parser.y"
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
#line 3109 "src/parser/parser.tab.c"
    break;

  case 84: /* Funcion_parametros: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_PARIZQ error TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 816 "src/parser/parser.y"
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
#line 3131 "src/parser/parser.tab.c"
    break;

  case 85: /* Funcion_parametros: TipoConCorchetes TOKEN_IDENTIFICADOR TOKEN_PARIZQ error TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 833 "src/parser/parser.y"
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
#line 3155 "src/parser/parser.tab.c"
    break;

  case 86: /* Funcion_parametros: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_PARIZQ TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 852 "src/parser/parser.y"
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
#line 3177 "src/parser/parser.tab.c"
    break;

  case 87: /* Funcion_parametros: TipoConCorchetes TOKEN_IDENTIFICADOR TOKEN_PARIZQ TOKEN_PARDER TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 869 "src/parser/parser.y"
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
#line 3201 "src/parser/parser.tab.c"
    break;

  case 88: /* parametros: parametros TOKEN_COMA tipo_primitivo TOKEN_IDENTIFICADOR  */
#line 890 "src/parser/parser.y"
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
#line 3238 "src/parser/parser.tab.c"
    break;

  case 89: /* parametros: parametros TOKEN_COMA TipoConCorchetes TOKEN_IDENTIFICADOR  */
#line 922 "src/parser/parser.y"
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
#line 3279 "src/parser/parser.tab.c"
    break;

  case 90: /* parametros: TipoConCorchetes TOKEN_IDENTIFICADOR  */
#line 958 "src/parser/parser.y"
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
#line 3301 "src/parser/parser.tab.c"
    break;

  case 91: /* parametros: tipo_primitivo TOKEN_IDENTIFICADOR  */
#line 975 "src/parser/parser.y"
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
#line 3321 "src/parser/parser.tab.c"
    break;

  case 92: /* TipoConCorchetes: tipo_primitivo  */
#line 994 "src/parser/parser.y"
                   {
        (yyval.str) = strdup((yyvsp[0].str));
    }
#line 3329 "src/parser/parser.tab.c"
    break;

  case 93: /* TipoConCorchetes: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER  */
#line 997 "src/parser/parser.y"
                                                         {
        char* buffer = malloc(strlen((yyvsp[-2].str)) + 3);
        sprintf(buffer, "%s[]", (yyvsp[-2].str));
        (yyval.str) = buffer;
    }
#line 3339 "src/parser/parser.tab.c"
    break;

  case 94: /* TipoConCorchetes: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER  */
#line 1002 "src/parser/parser.y"
                                                                                             {
        char* buffer = malloc(strlen((yyvsp[-4].str)) + 5);
        sprintf(buffer, "%s[][]", (yyvsp[-4].str));
        (yyval.str) = buffer;
    }
#line 3349 "src/parser/parser.tab.c"
    break;

  case 95: /* TipoConCorchetes: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER  */
#line 1007 "src/parser/parser.y"
                                                                                                                                 {
        char* buffer = malloc(strlen((yyvsp[-6].str)) + 7);
        sprintf(buffer, "%s[][][]", (yyvsp[-6].str));
        (yyval.str) = buffer;
    }
#line 3359 "src/parser/parser.tab.c"
    break;

  case 96: /* tipo_primitivo: TOKEN_DINT  */
#line 1015 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3365 "src/parser/parser.tab.c"
    break;

  case 97: /* tipo_primitivo: TOKEN_DCHAR  */
#line 1016 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3371 "src/parser/parser.tab.c"
    break;

  case 98: /* tipo_primitivo: TOKEN_DOUBLE  */
#line 1017 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3377 "src/parser/parser.tab.c"
    break;

  case 99: /* tipo_primitivo: TOKEN_DFLOAT  */
#line 1018 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3383 "src/parser/parser.tab.c"
    break;

  case 100: /* tipo_primitivo: TOKEN_DSTRING  */
#line 1019 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3389 "src/parser/parser.tab.c"
    break;

  case 101: /* tipo_primitivo: TOKEN_DBOOLEAN  */
#line 1020 "src/parser/parser.y"
                      { (yyval.str) = strdup((yyvsp[0].str)); }
#line 3395 "src/parser/parser.tab.c"
    break;

  case 102: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_IGUAL expresion  */
#line 1025 "src/parser/parser.y"
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
#line 3410 "src/parser/parser.tab.c"
    break;

  case 103: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_MAS_IGUAL expresion  */
#line 1035 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "+=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3418 "src/parser/parser.tab.c"
    break;

  case 104: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_MENOS_IGUAL expresion  */
#line 1038 "src/parser/parser.y"
                                                      {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "-=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3426 "src/parser/parser.tab.c"
    break;

  case 105: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_POR_IGUAL expresion  */
#line 1041 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "*=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3434 "src/parser/parser.tab.c"
    break;

  case 106: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_DIV_IGUAL expresion  */
#line 1044 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "/=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3442 "src/parser/parser.tab.c"
    break;

  case 107: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_MOD_IGUAL expresion  */
#line 1047 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "%=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3450 "src/parser/parser.tab.c"
    break;

  case 108: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_AND_IGUAL expresion  */
#line 1050 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "&=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3458 "src/parser/parser.tab.c"
    break;

  case 109: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_OR_IGUAL expresion  */
#line 1053 "src/parser/parser.y"
                                                   {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "|=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3466 "src/parser/parser.tab.c"
    break;

  case 110: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_XOR_IGUAL expresion  */
#line 1056 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "^=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3474 "src/parser/parser.tab.c"
    break;

  case 111: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_DESIZQ_IGUAL expresion  */
#line 1059 "src/parser/parser.y"
                                                       {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "<<=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3482 "src/parser/parser.tab.c"
    break;

  case 112: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_DESDER_IGUAL expresion  */
#line 1062 "src/parser/parser.y"
                                                       {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), ">>=", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 3490 "src/parser/parser.tab.c"
    break;

  case 113: /* expresion: expresion_or  */
#line 1072 "src/parser/parser.y"
                   { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3496 "src/parser/parser.tab.c"
    break;

  case 114: /* expresion_or: expresion_and  */
#line 1076 "src/parser/parser.y"
                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3502 "src/parser/parser.tab.c"
    break;

  case 115: /* expresion_or: expresion_or TOKEN_OR expresion_and  */
#line 1077 "src/parser/parser.y"
                                          {
          (yyval.nodo) = crearNodo(AST_OPERACION, "||");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3512 "src/parser/parser.tab.c"
    break;

  case 116: /* expresion_and: expresion_bit_or  */
#line 1085 "src/parser/parser.y"
                       { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3518 "src/parser/parser.tab.c"
    break;

  case 117: /* expresion_and: expresion_and TOKEN_AND expresion_bit_or  */
#line 1086 "src/parser/parser.y"
                                               {
          (yyval.nodo) = crearNodo(AST_OPERACION, "&&");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3528 "src/parser/parser.tab.c"
    break;

  case 118: /* expresion_bit_or: expresion_bit_xor  */
#line 1094 "src/parser/parser.y"
                        { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3534 "src/parser/parser.tab.c"
    break;

  case 119: /* expresion_bit_or: expresion_bit_or TOKEN_BIT_OR expresion_bit_xor  */
#line 1095 "src/parser/parser.y"
                                                      {
          (yyval.nodo) = crearNodo(AST_OPERACION, "|");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3544 "src/parser/parser.tab.c"
    break;

  case 120: /* expresion_bit_xor: expresion_bit_and  */
#line 1103 "src/parser/parser.y"
                        { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3550 "src/parser/parser.tab.c"
    break;

  case 121: /* expresion_bit_xor: expresion_bit_xor TOKEN_XOR expresion_bit_and  */
#line 1104 "src/parser/parser.y"
                                                    {
          (yyval.nodo) = crearNodo(AST_OPERACION, "^");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3560 "src/parser/parser.tab.c"
    break;

  case 122: /* expresion_bit_and: expresion_equality  */
#line 1112 "src/parser/parser.y"
                         { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3566 "src/parser/parser.tab.c"
    break;

  case 123: /* expresion_bit_and: expresion_bit_and TOKEN_BIT_AND expresion_equality  */
#line 1113 "src/parser/parser.y"
                                                         {
          (yyval.nodo) = crearNodo(AST_OPERACION, "&");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3576 "src/parser/parser.tab.c"
    break;

  case 124: /* expresion_equality: expresion_relacional  */
#line 1121 "src/parser/parser.y"
                           { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3582 "src/parser/parser.tab.c"
    break;

  case 125: /* expresion_equality: expresion_equality TOKEN_IGUAL_IGUAL expresion_relacional  */
#line 1122 "src/parser/parser.y"
                                                                {
          (yyval.nodo) = crearNodo(AST_OPERACION, "==");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3592 "src/parser/parser.tab.c"
    break;

  case 126: /* expresion_equality: expresion_equality TOKEN_NO_IGUAL expresion_relacional  */
#line 1127 "src/parser/parser.y"
                                                             {
          (yyval.nodo) = crearNodo(AST_OPERACION, "!=");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3602 "src/parser/parser.tab.c"
    break;

  case 127: /* expresion_relacional: expresion_shift  */
#line 1135 "src/parser/parser.y"
                      { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3608 "src/parser/parser.tab.c"
    break;

  case 128: /* expresion_relacional: expresion_relacional TOKEN_MENOR expresion_shift  */
#line 1136 "src/parser/parser.y"
                                                       {
          (yyval.nodo) = crearNodo(AST_OPERACION, "<");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3618 "src/parser/parser.tab.c"
    break;

  case 129: /* expresion_relacional: expresion_relacional TOKEN_MENOR_IGUAL expresion_shift  */
#line 1141 "src/parser/parser.y"
                                                             {
          (yyval.nodo) = crearNodo(AST_OPERACION, "<=");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3628 "src/parser/parser.tab.c"
    break;

  case 130: /* expresion_relacional: expresion_relacional TOKEN_MAYOR expresion_shift  */
#line 1146 "src/parser/parser.y"
                                                       {
          (yyval.nodo) = crearNodo(AST_OPERACION, ">");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3638 "src/parser/parser.tab.c"
    break;

  case 131: /* expresion_relacional: expresion_relacional TOKEN_MAYOR_IGUAL expresion_shift  */
#line 1151 "src/parser/parser.y"
                                                             {
          (yyval.nodo) = crearNodo(AST_OPERACION, ">=");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3648 "src/parser/parser.tab.c"
    break;

  case 132: /* expresion_shift: expresion_aditiva  */
#line 1159 "src/parser/parser.y"
                        { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3654 "src/parser/parser.tab.c"
    break;

  case 133: /* expresion_shift: expresion_shift TOKEN_DESIZQ expresion_aditiva  */
#line 1160 "src/parser/parser.y"
                                                     {
          (yyval.nodo) = crearNodo(AST_OPERACION, "<<");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3664 "src/parser/parser.tab.c"
    break;

  case 134: /* expresion_shift: expresion_shift TOKEN_DESDER expresion_aditiva  */
#line 1165 "src/parser/parser.y"
                                                     {
          (yyval.nodo) = crearNodo(AST_OPERACION, ">>");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3674 "src/parser/parser.tab.c"
    break;

  case 135: /* expresion_aditiva: expresion_multiplicativa  */
#line 1173 "src/parser/parser.y"
                               { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3680 "src/parser/parser.tab.c"
    break;

  case 136: /* expresion_aditiva: expresion_aditiva TOKEN_MAS expresion_multiplicativa  */
#line 1174 "src/parser/parser.y"
                                                           {
          (yyval.nodo) = crearNodo(AST_OPERACION, "+");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3690 "src/parser/parser.tab.c"
    break;

  case 137: /* expresion_aditiva: expresion_aditiva TOKEN_MENOS expresion_multiplicativa  */
#line 1179 "src/parser/parser.y"
                                                             {
          (yyval.nodo) = crearNodo(AST_OPERACION, "-");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3700 "src/parser/parser.tab.c"
    break;

  case 138: /* expresion_multiplicativa: expresion_unaria  */
#line 1187 "src/parser/parser.y"
                       { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3706 "src/parser/parser.tab.c"
    break;

  case 139: /* expresion_multiplicativa: expresion_multiplicativa TOKEN_MULTI expresion_unaria  */
#line 1188 "src/parser/parser.y"
                                                            {
          (yyval.nodo) = crearNodo(AST_OPERACION, "*");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3716 "src/parser/parser.tab.c"
    break;

  case 140: /* expresion_multiplicativa: expresion_multiplicativa TOKEN_DIV expresion_unaria  */
#line 1193 "src/parser/parser.y"
                                                          {
          (yyval.nodo) = crearNodo(AST_OPERACION, "/");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3726 "src/parser/parser.tab.c"
    break;

  case 141: /* expresion_multiplicativa: expresion_multiplicativa TOKEN_MODULO expresion_unaria  */
#line 1198 "src/parser/parser.y"
                                                             {
          (yyval.nodo) = crearNodo(AST_OPERACION, "%");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3736 "src/parser/parser.tab.c"
    break;

  case 142: /* expresion_unaria: TOKEN_NOT expresion_unaria  */
#line 1206 "src/parser/parser.y"
                                 {
          (yyval.nodo) = crearNodo(AST_OPERACION, "!");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3746 "src/parser/parser.tab.c"
    break;

  case 143: /* expresion_unaria: TOKEN_MENOS expresion_unaria  */
#line 1211 "src/parser/parser.y"
                                                {
          (yyval.nodo) = crearNodo(AST_OPERACION, "uminus");
          (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[0].nodo));
      }
#line 3756 "src/parser/parser.tab.c"
    break;

  case 144: /* expresion_unaria: TOKEN_PARIZQ tipo_primitivo TOKEN_PARDER expresion_unaria  */
#line 1216 "src/parser/parser.y"
                                                                {
          /* crear un nodo específico para cast; guardamos el tipo en ->valor */
          (yyval.nodo) = crearNodo(AST_CAST, "cast");
          (yyval.nodo)->valor = strdup((yyvsp[-2].str));    
          (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[0].nodo));
          free((yyvsp[-2].str)); 
      }
#line 3769 "src/parser/parser.tab.c"
    break;

  case 145: /* expresion_unaria: postfix  */
#line 1224 "src/parser/parser.y"
              { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3775 "src/parser/parser.tab.c"
    break;

  case 146: /* postfix: primary  */
#line 1228 "src/parser/parser.y"
              { (yyval.nodo) = (yyvsp[0].nodo); }
#line 3781 "src/parser/parser.tab.c"
    break;

  case 147: /* postfix: postfix TOKEN_INCREMENTO  */
#line 1229 "src/parser/parser.y"
                               {
          (yyval.nodo) = crearNodo(AST_INCREMENTO, "++");
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
      }
#line 3791 "src/parser/parser.tab.c"
    break;

  case 148: /* postfix: postfix TOKEN_DECREMENTO  */
#line 1234 "src/parser/parser.y"
                               {
          (yyval.nodo) = crearNodo(AST_DECREMENTO, "--");
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
          agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
      }
#line 3801 "src/parser/parser.tab.c"
    break;

  case 149: /* primary: TOKEN_NUMERO  */
#line 1243 "src/parser/parser.y"
                   {
          (yyval.nodo) = crearNodo(AST_NUMERO, NULL);
          (yyval.nodo)->valor = strdup((yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3811 "src/parser/parser.tab.c"
    break;

  case 150: /* primary: TOKEN_DOUBLE_LITERAL  */
#line 1248 "src/parser/parser.y"
                           {
          (yyval.nodo) = crearNodo(AST_NUMERO, NULL);
          (yyval.nodo)->valor = strdup((yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3821 "src/parser/parser.tab.c"
    break;

  case 151: /* primary: TOKEN_FLOAT_LITERAL  */
#line 1253 "src/parser/parser.y"
                          {
          (yyval.nodo) = crearNodo(AST_NUMERO, NULL);
          (yyval.nodo)->valor = strdup((yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3831 "src/parser/parser.tab.c"
    break;

  case 152: /* primary: TOKEN_CADENA  */
#line 1258 "src/parser/parser.y"
                   {
          (yyval.nodo) = crearNodo(AST_CADENA, NULL);
          (yyval.nodo)->valor = strdup((yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3841 "src/parser/parser.tab.c"
    break;

  case 153: /* primary: TOKEN_CARACTER  */
#line 1263 "src/parser/parser.y"
                     {
          (yyval.nodo) = crearNodo(AST_CARACTER, NULL);
          (yyval.nodo)->valor = strdup((yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3851 "src/parser/parser.tab.c"
    break;

  case 154: /* primary: TOKEN_IDENTIFICADOR  */
#line 1268 "src/parser/parser.y"
                          {
          (yyval.nodo) = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
          (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
      }
#line 3860 "src/parser/parser.tab.c"
    break;

  case 155: /* primary: TOKEN_BOOLEANO  */
#line 1272 "src/parser/parser.y"
                     {
        (yyval.nodo) = crearNodo(AST_BOOLEAN, (yyvsp[0].str));
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 3869 "src/parser/parser.tab.c"
    break;

  case 156: /* primary: TOKEN_PARSEINT TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1276 "src/parser/parser.y"
                                                         {
        (yyval.nodo) = crearNodo(AST_PARSEINT, "Integer.parseInt");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3879 "src/parser/parser.tab.c"
    break;

  case 157: /* primary: TOKEN_PARSEDOUBLE TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1281 "src/parser/parser.y"
                                                            {
        (yyval.nodo) = crearNodo(AST_PARSEDOUBLE, "Double.parseDouble");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3889 "src/parser/parser.tab.c"
    break;

  case 158: /* primary: TOKEN_PARSEFLOAT TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1286 "src/parser/parser.y"
                                                           {
        (yyval.nodo) = crearNodo(AST_PARSEFLOAT, "Float.parseFloat");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3899 "src/parser/parser.tab.c"
    break;

  case 159: /* primary: TOKEN_STRING_VALUEOF TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1291 "src/parser/parser.y"
                                                               {
        (yyval.nodo) = crearNodo(AST_VALUEOF, "String.valueOf");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3909 "src/parser/parser.tab.c"
    break;

  case 160: /* primary: TOKEN_INDEXOF TOKEN_PARIZQ expresion TOKEN_COMA expresion TOKEN_PARDER  */
#line 1296 "src/parser/parser.y"
                                                                             {
        (yyval.nodo) = crearNodo(AST_INDEXOF, "Arrays.indexOf");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-3].nodo));  // El array/vector
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));  // La clave a buscar
    }
#line 3920 "src/parser/parser.tab.c"
    break;

  case 161: /* primary: TOKEN_IDENTIFICADOR TOKEN_PUNTO TOKEN_LENGHT  */
#line 1302 "src/parser/parser.y"
                                                   {
        (yyval.nodo) = crearNodo(AST_LENGHT, "Array.length");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
    }
#line 3932 "src/parser/parser.tab.c"
    break;

  case 162: /* primary: TOKEN_IDENTIFICADOR TOKEN_PUNTO TOKEN_ADD TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1309 "src/parser/parser.y"
                                                                                    {
        (yyval.nodo) = crearNodo(AST_ADD, "Array.add");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-5].str));
        id->linea = (yylsp[-5]).first_line; id->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));  // El elemento a añadir
    }
#line 3945 "src/parser/parser.tab.c"
    break;

  case 163: /* primary: TOKEN_CADENA TOKEN_EQUAL TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1317 "src/parser/parser.y"
                                                                   {
        (yyval.nodo) = crearNodo(AST_EQUAL, ".equals");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        NodoAST* cadena = crearNodo(AST_CADENA, (yyvsp[-4].str));
        cadena->linea = (yylsp[-4]).first_line; cadena->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), cadena);
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3958 "src/parser/parser.tab.c"
    break;

  case 164: /* primary: TOKEN_IDENTIFICADOR TOKEN_EQUAL TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1325 "src/parser/parser.y"
                                                                          {
        (yyval.nodo) = crearNodo(AST_EQUAL, ".equals");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str));
        id->linea = (yylsp[-4]).first_line; id->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 3971 "src/parser/parser.tab.c"
    break;

  case 165: /* primary: TOKEN_PARSEJOIN TOKEN_PARIZQ expresion TOKEN_COMA lista_expresiones_join TOKEN_PARDER  */
#line 1333 "src/parser/parser.y"
                                                                                            {
        (yyval.nodo) = crearNodo(AST_PARSEJOIN, "Strings.join");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-3].nodo));  // El delimitador
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));  // Lista de expresiones a unir
    }
#line 3982 "src/parser/parser.tab.c"
    break;

  case 166: /* primary: TOKEN_PARSEJOIN TOKEN_PARIZQ expresion TOKEN_COMA expresion TOKEN_PARDER  */
#line 1339 "src/parser/parser.y"
                                                                               {
        (yyval.nodo) = crearNodo(AST_PARSEJOIN, "Strings.join");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-3].nodo));  // El delimitador
        
        // Para un solo elemento, creamos una lista con ese elemento
        NodoAST* lista = crearNodo(AST_LISTA, "Lista Strings");
        agregarHijo(lista, (yyvsp[-1].nodo));
        agregarHijo((yyval.nodo), lista);
    }
#line 3997 "src/parser/parser.tab.c"
    break;

  case 167: /* primary: acceso TOKEN_PUNTO TOKEN_LENGHT  */
#line 1349 "src/parser/parser.y"
                                      {
        (yyval.nodo) = crearNodo(AST_LENGHT, "Array.length");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
    }
#line 4007 "src/parser/parser.tab.c"
    break;

  case 168: /* primary: TOKEN_NULL  */
#line 1354 "src/parser/parser.y"
                 {
        (yyval.nodo) = crearNodo(AST_NULL, "null");
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 4016 "src/parser/parser.tab.c"
    break;

  case 169: /* primary: acceso  */
#line 1358 "src/parser/parser.y"
             {
          (yyval.nodo) = (yyvsp[0].nodo);
      }
#line 4024 "src/parser/parser.tab.c"
    break;

  case 170: /* primary: Llamadas  */
#line 1361 "src/parser/parser.y"
              {
        (yyval.nodo) = (yyvsp[0].nodo);
    }
#line 4032 "src/parser/parser.tab.c"
    break;

  case 171: /* primary: TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1364 "src/parser/parser.y"
                                          {
          (yyval.nodo) = (yyvsp[-1].nodo);
      }
#line 4040 "src/parser/parser.tab.c"
    break;

  case 172: /* lista_expresiones_join: lista_expresiones_join TOKEN_COMA expresion  */
#line 1370 "src/parser/parser.y"
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
#line 4056 "src/parser/parser.tab.c"
    break;

  case 173: /* lista_expresiones_join: expresion  */
#line 1381 "src/parser/parser.y"
                {
        (yyval.nodo) = crearNodo(AST_LISTA, "Lista Strings");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4065 "src/parser/parser.tab.c"
    break;

  case 174: /* instruccion_print: TOKEN_PRINT TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 1389 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crearNodo(AST_PRINT, "System.out.println");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 4075 "src/parser/parser.tab.c"
    break;

  case 175: /* instruccion_print: TOKEN_PRINT TOKEN_PARIZQ TOKEN_CADENA TOKEN_MAS expresion TOKEN_PARDER  */
#line 1395 "src/parser/parser.y"
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
#line 4092 "src/parser/parser.tab.c"
    break;

  case 176: /* instruccion_print: TOKEN_PRINT TOKEN_PARIZQ TOKEN_PARDER  */
#line 1407 "src/parser/parser.y"
                                            {
        (yyval.nodo) = crearNodo(AST_PRINT, "System.out.println");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        // No hijos, print vacío
    }
#line 4102 "src/parser/parser.tab.c"
    break;

  case 177: /* bloque: TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 1415 "src/parser/parser.y"
                                                   {
        (yyval.nodo) = crearNodo(AST_BLOQUE, "Bloque");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 4112 "src/parser/parser.tab.c"
    break;

  case 178: /* bloque: TOKEN_LLAVEIZQ error TOKEN_LLAVEDER  */
#line 1420 "src/parser/parser.y"
                                          {
        addErrorSintactico("Error sintáctico en el bloque", "global", (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        (yyval.nodo) = crearNodo(AST_BLOQUE, "Bloque");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        // Crear un bloque vacío para continuar
        agregarHijo((yyval.nodo), crearNodo(AST_LISTA, "Programa"));
    }
#line 4124 "src/parser/parser.tab.c"
    break;

  case 179: /* instruccion_break: TOKEN_BREAK  */
#line 1430 "src/parser/parser.y"
                {
        (yyval.nodo) = crearNodo(AST_BREAK, "break");
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 4133 "src/parser/parser.tab.c"
    break;

  case 180: /* instruccion_continue: TOKEN_CONTINUE  */
#line 1437 "src/parser/parser.y"
                   {
        (yyval.nodo) = crearNodo(AST_CONTINUE, "continue");
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 4142 "src/parser/parser.tab.c"
    break;

  case 181: /* instruccion_return: TOKEN_RETURN expresion  */
#line 1444 "src/parser/parser.y"
                           {
        (yyval.nodo) = crearNodo(AST_RETURN, "return");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4152 "src/parser/parser.tab.c"
    break;

  case 182: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ expresion TOKEN_PARDER bloque  */
#line 1454 "src/parser/parser.y"
                                                                       {
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));   // condición
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));   // bloque if
    }
#line 4163 "src/parser/parser.tab.c"
    break;

  case 183: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ expresion TOKEN_PARDER bloque TOKEN_ELSE bloque  */
#line 1460 "src/parser/parser.y"
                                                                            {
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));   // condición
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));   // bloque if
        NodoAST* else_node = crearNodo(AST_ELSE, "else");
        agregarHijo(else_node, (yyvsp[0].nodo));
        agregarHijo((yyval.nodo), else_node);
    }
#line 4177 "src/parser/parser.tab.c"
    break;

  case 184: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ expresion TOKEN_PARDER bloque TOKEN_ELSE instruccion_if  */
#line 1469 "src/parser/parser.y"
                                                                                    {
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));   // condición
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));   // bloque if
        NodoAST* elseif_node = crearNodo(AST_ELSEIF, "elseif");
        agregarHijo(elseif_node, (yyvsp[0].nodo));
        agregarHijo((yyval.nodo), elseif_node);
    }
#line 4191 "src/parser/parser.tab.c"
    break;

  case 185: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ error TOKEN_PARDER bloque  */
#line 1478 "src/parser/parser.y"
                                                                     {
        addErrorSintactico("Expresión inválida en condición if", "global", (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_BOOLEAN, "false"));  // condición predeterminada
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));  // bloque if
    }
#line 4203 "src/parser/parser.tab.c"
    break;

  case 186: /* intruccion_switch: TOKEN_SWITCH TOKEN_PARIZQ expresion TOKEN_PARDER TOKEN_LLAVEIZQ lista_casos TOKEN_LLAVEDER  */
#line 1489 "src/parser/parser.y"
                                                                                               {
        (yyval.nodo) = crearNodo(AST_SWITCH, "switch");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 4214 "src/parser/parser.tab.c"
    break;

  case 187: /* lista_casos: lista_casos caso  */
#line 1498 "src/parser/parser.y"
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
#line 4230 "src/parser/parser.tab.c"
    break;

  case 188: /* lista_casos: caso  */
#line 1509 "src/parser/parser.y"
           {
        (yyval.nodo) = crearNodo(AST_LISTA, "Casos");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4239 "src/parser/parser.tab.c"
    break;

  case 189: /* caso: TOKEN_CASE expresion TOKEN_DOS_PUNTOS lista_sentencias TOKEN_BREAK TOKEN_PUNTOCOMA  */
#line 1516 "src/parser/parser.y"
                                                                                      {
        (yyval.nodo) = crearNodo(AST_CASE, "CASE");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        NodoAST* nodoBreak = crearNodo(AST_BREAK, "break");
        nodoBreak->linea = (yylsp[-1]).first_line; nodoBreak->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), nodoBreak);
    }
#line 4253 "src/parser/parser.tab.c"
    break;

  case 190: /* caso: TOKEN_DEFAULT TOKEN_DOS_PUNTOS lista_sentencias  */
#line 1525 "src/parser/parser.y"
                                                      {
        (yyval.nodo) = crearNodo(AST_DEFAULT, "DEFAULT");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4263 "src/parser/parser.tab.c"
    break;

  case 191: /* instruccion_while: TOKEN_WHILE TOKEN_PARIZQ expresion TOKEN_PARDER bloque  */
#line 1533 "src/parser/parser.y"
                                                           {
        (yyval.nodo) = crearNodo(AST_WHILE, "while");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4274 "src/parser/parser.tab.c"
    break;

  case 192: /* instruccion_while: TOKEN_WHILE TOKEN_PARIZQ error TOKEN_PARDER bloque  */
#line 1539 "src/parser/parser.y"
                                                         {
        addErrorSintactico("Expresión inválida en condición while", "global", (yylsp[-2]).first_line, (yylsp[-2]).first_column);
        (yyval.nodo) = crearNodo(AST_WHILE, "while");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        // Agregar una condición predeterminada (false)
        agregarHijo((yyval.nodo), crearNodo(AST_BOOLEAN, "false"));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));  // bloque while
    }
#line 4287 "src/parser/parser.tab.c"
    break;

  case 193: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ declaracion TOKEN_PUNTOCOMA expresion TOKEN_PUNTOCOMA expresion TOKEN_PARDER bloque  */
#line 1550 "src/parser/parser.y"
                                                                                                              {
        (yyval.nodo) = crearNodo(AST_FOR, "FOR");
        (yyval.nodo)->linea = (yylsp[-8]).first_line; (yyval.nodo)->columna = (yylsp[-8]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-6].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4300 "src/parser/parser.tab.c"
    break;

  case 194: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ asignacion TOKEN_PUNTOCOMA expresion TOKEN_PUNTOCOMA expresion TOKEN_PARDER bloque  */
#line 1558 "src/parser/parser.y"
                                                                                                               {
        (yyval.nodo) = crearNodo(AST_FOR, "FOR");
        (yyval.nodo)->linea = (yylsp[-8]).first_line; (yyval.nodo)->columna = (yylsp[-8]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-6].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4313 "src/parser/parser.tab.c"
    break;

  case 195: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ declaracion TOKEN_DOS_PUNTOS expresion TOKEN_PARDER bloque  */
#line 1566 "src/parser/parser.y"
                                                                                        {
        (yyval.nodo) = crearNodo(AST_FOR_VECTOR, "FOR EACH");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 4325 "src/parser/parser.tab.c"
    break;

  case 196: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_DOS_PUNTOS expresion TOKEN_PARDER bloque  */
#line 1573 "src/parser/parser.y"
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
#line 4349 "src/parser/parser.tab.c"
    break;

  case 197: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ error TOKEN_PUNTOCOMA expresion TOKEN_PUNTOCOMA expresion TOKEN_PARDER bloque  */
#line 1592 "src/parser/parser.y"
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
#line 4364 "src/parser/parser.tab.c"
    break;

  case 198: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ declaracion TOKEN_PUNTOCOMA error TOKEN_PUNTOCOMA expresion TOKEN_PARDER bloque  */
#line 1602 "src/parser/parser.y"
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
#line 4379 "src/parser/parser.tab.c"
    break;


#line 4383 "src/parser/parser.tab.c"

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

#line 1615 "src/parser/parser.y"


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
