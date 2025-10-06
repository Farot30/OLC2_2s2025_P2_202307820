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
    
    /* Función auxiliar para crear asignaciones compuestas */
    NodoAST* crear_asignacion_compuesta(char* id, char* operador, NodoAST* expr, 
                                       int id_linea, int id_columna, int op_linea, int op_columna);

#line 88 "parser.tab.c"

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
  YYSYMBOL_TOKEN_DBOOLEAN = 13,            /* TOKEN_DBOOLEAN  */
  YYSYMBOL_TOKEN_BOOLEANO = 14,            /* TOKEN_BOOLEANO  */
  YYSYMBOL_TOKEN_IDENTIFICADOR = 15,       /* TOKEN_IDENTIFICADOR  */
  YYSYMBOL_TOKEN_DINT = 16,                /* TOKEN_DINT  */
  YYSYMBOL_TOKEN_DFLOAT = 17,              /* TOKEN_DFLOAT  */
  YYSYMBOL_TOKEN_DSTRING = 18,             /* TOKEN_DSTRING  */
  YYSYMBOL_TOKEN_DCHAR = 19,               /* TOKEN_DCHAR  */
  YYSYMBOL_TOKEN_DOUBLE = 20,              /* TOKEN_DOUBLE  */
  YYSYMBOL_TOKEN_IGUAL = 21,               /* TOKEN_IGUAL  */
  YYSYMBOL_TOKEN_PUNTOCOMA = 22,           /* TOKEN_PUNTOCOMA  */
  YYSYMBOL_TOKEN_DOS_PUNTOS = 23,          /* TOKEN_DOS_PUNTOS  */
  YYSYMBOL_TOKEN_COMA = 24,                /* TOKEN_COMA  */
  YYSYMBOL_TOKEN_NUMERO = 25,              /* TOKEN_NUMERO  */
  YYSYMBOL_TOKEN_CADENA = 26,              /* TOKEN_CADENA  */
  YYSYMBOL_TOKEN_CARACTER = 27,            /* TOKEN_CARACTER  */
  YYSYMBOL_TOKEN_FLOAT_LITERAL = 28,       /* TOKEN_FLOAT_LITERAL  */
  YYSYMBOL_TOKEN_DOUBLE_LITERAL = 29,      /* TOKEN_DOUBLE_LITERAL  */
  YYSYMBOL_TOKEN_PRINT = 30,               /* TOKEN_PRINT  */
  YYSYMBOL_TOKEN_PARIZQ = 31,              /* TOKEN_PARIZQ  */
  YYSYMBOL_TOKEN_PARDER = 32,              /* TOKEN_PARDER  */
  YYSYMBOL_TOKEN_BIT_AND = 33,             /* TOKEN_BIT_AND  */
  YYSYMBOL_TOKEN_BIT_OR = 34,              /* TOKEN_BIT_OR  */
  YYSYMBOL_TOKEN_XOR = 35,                 /* TOKEN_XOR  */
  YYSYMBOL_TOKEN_DESIZQ = 36,              /* TOKEN_DESIZQ  */
  YYSYMBOL_TOKEN_DESDER = 37,              /* TOKEN_DESDER  */
  YYSYMBOL_TOKEN_IGUAL_IGUAL = 38,         /* TOKEN_IGUAL_IGUAL  */
  YYSYMBOL_TOKEN_NO_IGUAL = 39,            /* TOKEN_NO_IGUAL  */
  YYSYMBOL_TOKEN_MAYOR_IGUAL = 40,         /* TOKEN_MAYOR_IGUAL  */
  YYSYMBOL_TOKEN_MENOR_IGUAL = 41,         /* TOKEN_MENOR_IGUAL  */
  YYSYMBOL_TOKEN_MAYOR = 42,               /* TOKEN_MAYOR  */
  YYSYMBOL_TOKEN_MENOR = 43,               /* TOKEN_MENOR  */
  YYSYMBOL_TOKEN_AND = 44,                 /* TOKEN_AND  */
  YYSYMBOL_TOKEN_OR = 45,                  /* TOKEN_OR  */
  YYSYMBOL_TOKEN_NOT = 46,                 /* TOKEN_NOT  */
  YYSYMBOL_TOKEN_IF = 47,                  /* TOKEN_IF  */
  YYSYMBOL_TOKEN_ELSE = 48,                /* TOKEN_ELSE  */
  YYSYMBOL_TOKEN_SWITCH = 49,              /* TOKEN_SWITCH  */
  YYSYMBOL_TOKEN_CASE = 50,                /* TOKEN_CASE  */
  YYSYMBOL_TOKEN_DEFAULT = 51,             /* TOKEN_DEFAULT  */
  YYSYMBOL_TOKEN_BREAK = 52,               /* TOKEN_BREAK  */
  YYSYMBOL_TOKEN_WHILE = 53,               /* TOKEN_WHILE  */
  YYSYMBOL_TOKEN_CONTINUE = 54,            /* TOKEN_CONTINUE  */
  YYSYMBOL_TOKEN_FOR = 55,                 /* TOKEN_FOR  */
  YYSYMBOL_TOKEN_STRING_VALUEOF = 56,      /* TOKEN_STRING_VALUEOF  */
  YYSYMBOL_TOKEN_EQUAL = 57,               /* TOKEN_EQUAL  */
  YYSYMBOL_TOKEN_MAS_IGUAL = 58,           /* TOKEN_MAS_IGUAL  */
  YYSYMBOL_TOKEN_MENOS_IGUAL = 59,         /* TOKEN_MENOS_IGUAL  */
  YYSYMBOL_TOKEN_POR_IGUAL = 60,           /* TOKEN_POR_IGUAL  */
  YYSYMBOL_TOKEN_DIV_IGUAL = 61,           /* TOKEN_DIV_IGUAL  */
  YYSYMBOL_TOKEN_MOD_IGUAL = 62,           /* TOKEN_MOD_IGUAL  */
  YYSYMBOL_TOKEN_AND_IGUAL = 63,           /* TOKEN_AND_IGUAL  */
  YYSYMBOL_TOKEN_OR_IGUAL = 64,            /* TOKEN_OR_IGUAL  */
  YYSYMBOL_TOKEN_XOR_IGUAL = 65,           /* TOKEN_XOR_IGUAL  */
  YYSYMBOL_TOKEN_DESIZQ_IGUAL = 66,        /* TOKEN_DESIZQ_IGUAL  */
  YYSYMBOL_TOKEN_DESDER_IGUAL = 67,        /* TOKEN_DESDER_IGUAL  */
  YYSYMBOL_UMINUS = 68,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 69,                  /* $accept  */
  YYSYMBOL_s = 70,                         /* s  */
  YYSYMBOL_programa = 71,                  /* programa  */
  YYSYMBOL_lista_sentencias = 72,          /* lista_sentencias  */
  YYSYMBOL_sentencia = 73,                 /* sentencia  */
  YYSYMBOL_declaracion = 74,               /* declaracion  */
  YYSYMBOL_declaracion_vector = 75,        /* declaracion_vector  */
  YYSYMBOL_lista_elementos = 76,           /* lista_elementos  */
  YYSYMBOL_lista_filas = 77,               /* lista_filas  */
  YYSYMBOL_declaracion_matriz = 78,        /* declaracion_matriz  */
  YYSYMBOL_declaracion_array_multidimensional = 79, /* declaracion_array_multidimensional  */
  YYSYMBOL_lista_bloques = 80,             /* lista_bloques  */
  YYSYMBOL_DimensionesCorchetes = 81,      /* DimensionesCorchetes  */
  YYSYMBOL_Dimensiones = 82,               /* Dimensiones  */
  YYSYMBOL_tipo_primitivo = 83,            /* tipo_primitivo  */
  YYSYMBOL_asignacion = 84,                /* asignacion  */
  YYSYMBOL_expresion = 85,                 /* expresion  */
  YYSYMBOL_cast = 86,                      /* cast  */
  YYSYMBOL_instruccion_print = 87,         /* instruccion_print  */
  YYSYMBOL_bloque = 88,                    /* bloque  */
  YYSYMBOL_instruccion_break = 89,         /* instruccion_break  */
  YYSYMBOL_instruccion_continue = 90,      /* instruccion_continue  */
  YYSYMBOL_instruccion_if = 91,            /* instruccion_if  */
  YYSYMBOL_intruccion_switch = 92,         /* intruccion_switch  */
  YYSYMBOL_lista_casos = 93,               /* lista_casos  */
  YYSYMBOL_caso = 94,                      /* caso  */
  YYSYMBOL_instruccion_while = 95,         /* instruccion_while  */
  YYSYMBOL_instruccion_for = 96            /* instruccion_for  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  73
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1278

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  104
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  250

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   323


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
      65,    66,    67,    68
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    98,    98,   110,   115,   126,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     152,   161,   172,   187,   194,   204,   215,   222,   237,   246,
     254,   264,   271,   281,   296,   304,   315,   321,   325,   332,
     333,   334,   335,   336,   337,   342,   352,   355,   358,   361,
     364,   367,   370,   373,   376,   379,   386,   391,   396,   403,
     410,   415,   420,   425,   430,   435,   440,   445,   450,   455,
     460,   465,   470,   475,   480,   485,   490,   496,   501,   506,
     509,   513,   517,   521,   525,   529,   533,   537,   540,   549,
     558,   566,   574,   581,   588,   594,   603,   616,   625,   636,
     643,   652,   660,   669,   677
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
  "TOKEN_DBOOLEAN", "TOKEN_BOOLEANO", "TOKEN_IDENTIFICADOR", "TOKEN_DINT",
  "TOKEN_DFLOAT", "TOKEN_DSTRING", "TOKEN_DCHAR", "TOKEN_DOUBLE",
  "TOKEN_IGUAL", "TOKEN_PUNTOCOMA", "TOKEN_DOS_PUNTOS", "TOKEN_COMA",
  "TOKEN_NUMERO", "TOKEN_CADENA", "TOKEN_CARACTER", "TOKEN_FLOAT_LITERAL",
  "TOKEN_DOUBLE_LITERAL", "TOKEN_PRINT", "TOKEN_PARIZQ", "TOKEN_PARDER",
  "TOKEN_BIT_AND", "TOKEN_BIT_OR", "TOKEN_XOR", "TOKEN_DESIZQ",
  "TOKEN_DESDER", "TOKEN_IGUAL_IGUAL", "TOKEN_NO_IGUAL",
  "TOKEN_MAYOR_IGUAL", "TOKEN_MENOR_IGUAL", "TOKEN_MAYOR", "TOKEN_MENOR",
  "TOKEN_AND", "TOKEN_OR", "TOKEN_NOT", "TOKEN_IF", "TOKEN_ELSE",
  "TOKEN_SWITCH", "TOKEN_CASE", "TOKEN_DEFAULT", "TOKEN_BREAK",
  "TOKEN_WHILE", "TOKEN_CONTINUE", "TOKEN_FOR", "TOKEN_STRING_VALUEOF",
  "TOKEN_EQUAL", "TOKEN_MAS_IGUAL", "TOKEN_MENOS_IGUAL", "TOKEN_POR_IGUAL",
  "TOKEN_DIV_IGUAL", "TOKEN_MOD_IGUAL", "TOKEN_AND_IGUAL",
  "TOKEN_OR_IGUAL", "TOKEN_XOR_IGUAL", "TOKEN_DESIZQ_IGUAL",
  "TOKEN_DESDER_IGUAL", "UMINUS", "$accept", "s", "programa",
  "lista_sentencias", "sentencia", "declaracion", "declaracion_vector",
  "lista_elementos", "lista_filas", "declaracion_matriz",
  "declaracion_array_multidimensional", "lista_bloques",
  "DimensionesCorchetes", "Dimensiones", "tipo_primitivo", "asignacion",
  "expresion", "cast", "instruccion_print", "bloque", "instruccion_break",
  "instruccion_continue", "instruccion_if", "intruccion_switch",
  "lista_casos", "caso", "instruccion_while", "instruccion_for", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-140)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-102)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     384,   -19,   384,  1222,  -140,  -140,    12,  -140,  -140,  -140,
    -140,  -140,  -140,  -140,  -140,  -140,  -140,   -17,  1200,  1222,
     -12,    32,  -140,    33,  -140,    74,    86,    22,  -140,   234,
    -140,   -10,    97,    99,   100,    15,   101,   535,  -140,   122,
    -140,   123,   127,  -140,  -140,  -140,  -140,  -140,   337,     6,
     -20,   148,   150,  1222,  1222,  1222,  1222,  1222,  1222,  1222,
    1222,  1222,  1222,  1222,  1222,   133,   573,   -20,  1222,  1222,
    1222,    23,  1222,  -140,  -140,  -140,  -140,  -140,  -140,   168,
     145,    20,  -140,  1222,  1222,  1222,  1222,  1222,  -140,  1222,
    1222,  1222,  1222,  1222,  1222,  1222,  1222,  1222,  1222,  1222,
    1222,  1222,   144,  -140,  -140,  -140,  -140,  -140,  -140,  1029,
    1029,  1029,  1029,  1029,  1029,  1029,  1029,  1029,  1029,  1029,
     611,  1222,  -140,   649,   687,   725,   165,   155,   164,   170,
     763,   115,  1222,   199,   185,   -20,   -20,   -20,   138,   138,
     160,  1117,  1129,   103,   103,  1167,  1167,   116,   116,   116,
     116,  1079,  1067,  1222,  -140,  1029,   204,   205,   204,  1222,
    1222,  -140,   208,   189,  1029,  -140,    95,   801,   163,    -6,
    -140,   839,   877,   200,   147,   212,   171,  -140,     0,  1222,
     195,   113,  -140,  1222,  1222,   198,  1222,   171,  1222,     2,
     215,  -140,  -140,   915,   384,  -140,  -140,   953,   991,   154,
       3,  1029,   217,     4,  -140,   232,   216,   235,   384,   281,
     204,   204,   233,   171,  -140,  1222,  1222,  -140,   212,   236,
     218,   431,  -140,  -140,  1222,     5,   240,  1029,    98,     7,
    -140,   249,   244,     8,  -140,   252,  1222,  -140,  -140,  -140,
    -140,  -140,  1222,   457,    10,   253,  -140,  1222,   497,  -140
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,    44,    85,    86,    39,    42,    43,
      40,    41,    80,    83,    84,    81,    82,     0,     0,     0,
       0,     0,    92,     0,    93,     0,     0,     0,     2,     0,
       5,     0,     0,     0,     0,     0,     0,     0,    87,     0,
      16,     0,     0,     9,    10,    11,    12,    20,     0,    86,
      78,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    77,     0,     0,
       0,     0,     0,     1,     4,     6,    13,    14,    15,     0,
      22,     0,     7,     0,     0,     0,     0,     0,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     8,    18,    19,    91,    58,    59,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
       0,     0,    79,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,    62,    56,    57,
      71,    72,    73,    74,    75,    63,    64,    68,    67,    66,
      65,    69,    70,     0,    90,    89,     0,     0,     0,     0,
       0,    88,     0,     0,    21,    35,     0,     0,    94,     0,
     102,     0,     0,     0,     0,     0,     0,    76,     0,     0,
       0,     0,    99,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,     0,     0,    97,    98,     0,     0,     0,
       0,    26,     0,     0,    32,     0,     0,    31,     0,     0,
       0,     0,     0,     0,    24,     0,     0,    34,     0,     0,
       0,     0,   103,   104,     0,     0,     0,    25,     0,     0,
      37,     0,     0,     0,    30,     0,     0,    23,    33,    38,
     100,    28,     0,     0,     0,     0,    27,     0,     0,    29
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -140,  -140,  -140,    -1,   -27,   186,  -140,  -139,  -140,  -140,
    -140,    49,  -140,  -140,    -9,   197,    -3,  -140,  -140,  -110,
    -140,  -140,    91,  -140,  -140,    89,  -140,  -140
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    27,    28,    29,    30,    31,    32,   200,   225,    33,
      34,   189,    81,   207,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,   181,   182,    45,    46
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      50,    48,    74,    47,     2,   204,   214,   217,   234,    65,
     238,   241,    75,   246,    64,    66,    67,    51,    52,    68,
      79,    74,    73,    51,    52,   133,   205,   215,   215,   235,
      80,   205,   215,    53,   215,   134,     4,   102,   126,     7,
       8,     9,    10,    11,   179,   180,   168,    20,   170,   203,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   128,    69,    70,   123,   124,   125,   191,   130,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
     135,   136,   137,   138,   139,   233,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   175,
     222,   223,   176,   244,   237,    71,    83,    84,    85,    86,
      87,    83,    84,    85,    86,    87,   195,    72,   155,    76,
     162,    77,    78,    82,    83,    84,    85,    86,    87,   164,
     163,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   103,   104,    83,    84,    85,   105,
     167,   186,    92,    93,   187,   102,   171,   172,   212,   107,
     102,   213,   108,   179,   180,   121,   132,   190,    83,    84,
      85,    86,    87,   102,   131,   153,   193,   159,   202,    80,
     197,   198,    74,   201,     4,   201,    53,     7,     8,     9,
      10,    11,   160,   209,    74,   102,    92,    93,    94,    95,
      96,    97,    98,    99,   226,   165,   166,   221,     2,   169,
     174,   178,   227,   228,   173,   185,   188,   102,   194,   199,
     206,   201,   216,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,   243,    -3,     1,   218,   224,     2,   201,
     220,   219,   230,   231,   248,   236,     3,     4,     5,     6,
       7,     8,     9,    10,    11,   239,   242,   127,   247,    12,
      13,    14,    15,    16,    17,    18,   240,   229,   129,   192,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,    20,     1,    21,  -101,     2,    22,    23,    24,    25,
      26,     0,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,     0,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    19,    20,     0,
      21,  -101,  -101,    22,    23,    24,    25,    26,     1,     0,
     106,     2,     0,     0,     0,     0,     0,     0,     0,     3,
       4,     5,     6,     7,     8,     9,    10,    11,     0,     0,
       0,     0,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    19,    20,     1,    21,     0,     2,    22,
      23,    24,    25,    26,     0,     0,     3,     4,     5,     6,
       7,     8,     9,    10,    11,     0,     0,     0,     0,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,    20,     1,    21,     0,     2,    22,    23,    24,    25,
      26,     0,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,     0,     0,     0,     0,    12,    13,    14,    15,
      16,    17,    18,   245,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,     0,     0,     0,    19,    20,     0,
      21,     0,     0,   232,    23,    24,    25,    26,     0,     0,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   249,     0,    83,    84,    85,    86,    87,
       0,     0,     0,     0,   102,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,    83,    84,    85,    86,    87,     0,     0,
       0,     0,     0,     0,   102,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    83,    84,    85,    86,    87,     0,     0,     0,     0,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   122,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,    83,
      84,    85,    86,    87,     0,     0,     0,     0,     0,     0,
     102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   154,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    83,    84,    85,
      86,    87,     0,     0,     0,     0,     0,     0,   102,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   156,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,    83,    84,    85,    86,    87,
       0,     0,     0,     0,     0,     0,   102,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   157,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,    83,    84,    85,    86,    87,     0,     0,
       0,     0,     0,     0,   102,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   158,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    83,    84,    85,    86,    87,     0,     0,     0,     0,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   161,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,    83,
      84,    85,    86,    87,     0,     0,     0,     0,     0,     0,
     102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    83,    84,    85,
      86,    87,     0,     0,     0,     0,     0,     0,   102,     0,
       0,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,    83,    84,    85,    86,    87,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   184,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,    83,    84,    85,    86,    87,     0,     0,
       0,     0,     0,     0,   102,     0,     0,     0,   208,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,    83,    84,    85,    86,    87,     0,     0,     0,     0,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   210,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,    83,
      84,    85,    86,    87,     0,     0,     0,     0,     0,     0,
     102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   211,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,    83,    84,    85,
      86,    87,     0,     0,     0,     0,     0,     0,   102,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,    83,    84,    85,    86,    87,
       0,     0,     0,     0,     0,     0,   102,    83,    84,    85,
      86,    87,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,     0,   102,    83,    84,    85,    86,    87,
       0,     0,     0,     0,     0,     0,   102,    83,    84,    85,
      86,    87,     0,     0,     0,     0,     0,     0,     0,     0,
      89,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,     0,    89,     0,     0,    92,    93,    94,    95,    96,
      97,    98,    99,     0,   102,    83,    84,    85,    86,    87,
       0,     0,     0,     0,     0,     0,   102,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,     0,     0,    96,    97,    98,
      99,     0,     3,     4,     5,    49,     7,     8,     9,    10,
      11,     0,     0,     0,   102,    12,    13,    14,    15,    16,
       0,    18,     0,     0,     3,     0,     5,    49,     0,     0,
       0,     0,     0,     0,     0,     0,    19,    12,    13,    14,
      15,    16,     0,    18,     0,     0,    26,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    26
};

static const yytype_int16 yycheck[] =
{
       3,     2,    29,    22,     4,     3,     3,     3,     3,    18,
       3,     3,    22,     3,    31,    18,    19,    11,    12,    31,
       5,    48,     0,    11,    12,     5,    24,    24,    24,    24,
      15,    24,    24,    21,    24,    15,    13,    57,    15,    16,
      17,    18,    19,    20,    50,    51,   156,    47,   158,   188,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    71,    31,    31,    68,    69,    70,   178,    72,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      83,    84,    85,    86,    87,   224,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,     4,
     210,   211,     7,   242,     6,    31,     8,     9,    10,    11,
      12,     8,     9,    10,    11,    12,     3,    31,   121,    22,
       5,    22,    22,    22,     8,     9,    10,    11,    12,   132,
      15,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    22,    22,     8,     9,    10,    22,
     153,     4,    36,    37,     7,    57,   159,   160,     4,    11,
      57,     7,    12,    50,    51,    32,    21,   176,     8,     9,
      10,    11,    12,    57,     6,    31,   179,    22,   187,    15,
     183,   184,   209,   186,    13,   188,    21,    16,    17,    18,
      19,    20,    22,   194,   221,    57,    36,    37,    38,    39,
      40,    41,    42,    43,   213,     6,    21,   208,     4,     4,
      21,    48,   215,   216,     6,    15,     4,    57,    23,    21,
       5,   224,     5,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,   236,     0,     1,     4,     4,     4,   242,
       5,    25,     6,    25,   247,     5,    12,    13,    14,    15,
      16,    17,    18,    19,    20,     6,     4,    71,     5,    25,
      26,    27,    28,    29,    30,    31,    22,   218,    71,   178,
     181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    47,     1,    49,     3,     4,    52,    53,    54,    55,
      56,    -1,    -1,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,     1,    -1,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    30,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    47,     1,    49,    -1,     4,    52,
      53,    54,    55,    56,    -1,    -1,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    47,     1,    49,    -1,     4,    52,    53,    54,    55,
      56,    -1,    -1,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    30,    31,     6,    -1,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      49,    -1,    -1,    52,    53,    54,    55,    56,    -1,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     6,    -1,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    57,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    57,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    57,     8,     9,    10,
      11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    33,    -1,    -1,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    57,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    -1,    40,    41,    42,
      43,    -1,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    57,    25,    26,    27,    28,    29,
      -1,    31,    -1,    -1,    12,    -1,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    25,    26,    27,
      28,    29,    -1,    31,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     4,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    25,    26,    27,    28,    29,    30,    31,    46,
      47,    49,    52,    53,    54,    55,    56,    70,    71,    72,
      73,    74,    75,    78,    79,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    95,    96,    22,    72,    15,
      85,    11,    12,    21,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    31,    83,    85,    85,    31,    31,
      31,    31,    31,     0,    73,    22,    22,    22,    22,     5,
      15,    81,    22,     8,     9,    10,    11,    12,    22,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    57,    22,    22,    22,     3,    11,    12,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    32,    32,    85,    85,    85,    15,    74,    83,    84,
      85,     6,    21,     5,    15,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    31,    32,    85,    32,    32,    32,    22,
      22,    32,     5,    15,    85,     6,    21,    85,    88,     4,
      88,    85,    85,     6,    21,     4,     7,    32,    48,    50,
      51,    93,    94,    22,    22,    15,     4,     7,     4,    80,
      83,    88,    91,    85,    23,     3,    94,    85,    85,    21,
      76,    85,    83,    76,     3,    24,     5,    82,    23,    72,
      32,    32,     4,     7,     3,    24,     5,     3,     4,    25,
       5,    72,    88,    88,     4,    77,    83,    85,    85,    80,
       6,    25,    52,    76,     3,    24,     5,     6,     3,     6,
      22,     3,     4,    85,    76,     6,     3,     5,    85,     6
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    69,    70,    71,    72,    72,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    74,    74,    75,    75,    76,    76,    77,    77,    78,
      78,    79,    79,    80,    80,    81,    81,    82,    82,    83,
      83,    83,    83,    83,    83,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    86,
      87,    88,    89,    90,    91,    91,    91,    92,    93,    93,
      94,    94,    95,    96,    96
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     2,     2,     2,     1,
       1,     1,     1,     2,     2,     2,     1,     2,     2,     2,
       2,     4,     2,    10,     8,     3,     1,     5,     3,    15,
      10,     7,     7,     5,     3,     3,     2,     3,     4,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     5,     2,     2,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     3,     1,     1,     5,     7,     7,     7,     2,     1,
       6,     3,     5,     9,     9
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
#line 98 "src/parser/parser.y"
            {
    printf("\n=== ESTRUCTURA DEL AST ===\n");
    imprimirAST((yyvsp[0].nodo), 0);
    interpretarPrograma((yyvsp[0].nodo));
    printf("ejecutando exportarAST\n");
    exportarAST((yyvsp[0].nodo), "ast.dot", "ast.png");
    printf("\n=== LIBERANDO MEMORIA ===\n");
    liberarAST((yyvsp[0].nodo));
    printf("[DEBUG] Procesamiento completado.\n");
}
#line 1943 "parser.tab.c"
    break;

  case 3: /* programa: lista_sentencias  */
#line 110 "src/parser/parser.y"
                           { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1949 "parser.tab.c"
    break;

  case 4: /* lista_sentencias: lista_sentencias sentencia  */
#line 115 "src/parser/parser.y"
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
#line 1965 "parser.tab.c"
    break;

  case 5: /* lista_sentencias: sentencia  */
#line 126 "src/parser/parser.y"
                { 
        if ((yyvsp[0].nodo)) {
            (yyval.nodo) = (yyvsp[0].nodo);
        } else {
            (yyval.nodo) = crearNodo(AST_LISTA, "Programa");
        }
    }
#line 1977 "parser.tab.c"
    break;

  case 6: /* sentencia: declaracion TOKEN_PUNTOCOMA  */
#line 137 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 1983 "parser.tab.c"
    break;

  case 7: /* sentencia: asignacion TOKEN_PUNTOCOMA  */
#line 138 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 1989 "parser.tab.c"
    break;

  case 8: /* sentencia: instruccion_print TOKEN_PUNTOCOMA  */
#line 139 "src/parser/parser.y"
                                        { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 1995 "parser.tab.c"
    break;

  case 9: /* sentencia: instruccion_if  */
#line 140 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2001 "parser.tab.c"
    break;

  case 10: /* sentencia: intruccion_switch  */
#line 141 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2007 "parser.tab.c"
    break;

  case 11: /* sentencia: instruccion_while  */
#line 142 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2013 "parser.tab.c"
    break;

  case 12: /* sentencia: instruccion_for  */
#line 143 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2019 "parser.tab.c"
    break;

  case 13: /* sentencia: declaracion_vector TOKEN_PUNTOCOMA  */
#line 144 "src/parser/parser.y"
                                              { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2025 "parser.tab.c"
    break;

  case 14: /* sentencia: declaracion_matriz TOKEN_PUNTOCOMA  */
#line 145 "src/parser/parser.y"
                                              { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2031 "parser.tab.c"
    break;

  case 15: /* sentencia: declaracion_array_multidimensional TOKEN_PUNTOCOMA  */
#line 146 "src/parser/parser.y"
                                                         { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2037 "parser.tab.c"
    break;

  case 16: /* sentencia: bloque  */
#line 147 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2043 "parser.tab.c"
    break;

  case 17: /* sentencia: expresion TOKEN_PUNTOCOMA  */
#line 148 "src/parser/parser.y"
                                    { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2049 "parser.tab.c"
    break;

  case 18: /* sentencia: instruccion_break TOKEN_PUNTOCOMA  */
#line 149 "src/parser/parser.y"
                                        {(yyval.nodo) = (yyvsp[-1].nodo); }
#line 2055 "parser.tab.c"
    break;

  case 19: /* sentencia: instruccion_continue TOKEN_PUNTOCOMA  */
#line 150 "src/parser/parser.y"
                                           {(yyval.nodo) = (yyvsp[-1].nodo); }
#line 2061 "parser.tab.c"
    break;

  case 20: /* sentencia: error TOKEN_PUNTOCOMA  */
#line 152 "src/parser/parser.y"
                                    { 
        addErrorSintactico("Instrucción inválida", "global", (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        yyerrok; 
        (yyval.nodo) = NULL;
    }
#line 2071 "parser.tab.c"
    break;

  case 21: /* declaracion: tipo_primitivo TOKEN_IDENTIFICADOR TOKEN_IGUAL expresion  */
#line 161 "src/parser/parser.y"
                                                             {
        (yyval.nodo) = crearNodo(AST_DECLARACION, NULL);
        (yyval.nodo)->valor = strdup((yyvsp[-3].str));
        (yyval.nodo)->linea = (yylsp[-2]).first_line; 
        (yyval.nodo)->columna = (yylsp[-2]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; // Cambia @3 a @2
        id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2087 "parser.tab.c"
    break;

  case 22: /* declaracion: tipo_primitivo TOKEN_IDENTIFICADOR  */
#line 172 "src/parser/parser.y"
                                         {
        (yyval.nodo) = crearNodo(AST_DECLARACION, NULL);
        (yyval.nodo)->valor = strdup((yyvsp[-1].str));
        (yyval.nodo)->linea = (yylsp[0]).first_line; 
        (yyval.nodo)->columna = (yylsp[0]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
        id->linea = (yylsp[0]).first_line; // Cambia @3 a @2
        id->columna = (yylsp[0]).first_column;
        NodoAST* def = crearNodo(AST_CADENA, (char*)valorPorDefectoTipo(obtenerTipoDesdeString((yyvsp[-1].str))));
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), def);
    }
#line 2104 "parser.tab.c"
    break;

  case 23: /* declaracion_vector: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_NEW tipo_primitivo TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER  */
#line 187 "src/parser/parser.y"
                                                                                                                                                              {
        (yyval.nodo) = crearNodo(AST_DECLARACION_VECTOR, "Declaración Vector");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-9].str)));
        agregarHijo((yyval.nodo), crearNodo(AST_IDENTIFICADOR, (yyvsp[-6].str)));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo)); // Dimensión
    }
#line 2116 "parser.tab.c"
    break;

  case 24: /* declaracion_vector: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 194 "src/parser/parser.y"
                                                                                                                                      {
        (yyval.nodo) = crearNodo(AST_DECLARACION_VECTOR, "Declaración Vector");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-7].str)));
        agregarHijo((yyval.nodo), crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str)));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo)); // lista_elementos
    }
#line 2128 "parser.tab.c"
    break;

  case 25: /* lista_elementos: lista_elementos TOKEN_COMA expresion  */
#line 204 "src/parser/parser.y"
                                         {
        if ((yyvsp[-2].nodo)->tipo != AST_LISTA) {
            NodoAST* lista = crearNodo(AST_LISTA, "Lista Elementos");
            agregarHijo(lista, (yyvsp[-2].nodo));
            agregarHijo(lista, (yyvsp[0].nodo));
            (yyval.nodo) = lista;
        } else {
            agregarHijo((yyvsp[-2].nodo), (yyvsp[0].nodo));
            (yyval.nodo) = (yyvsp[-2].nodo);
        }
    }
#line 2144 "parser.tab.c"
    break;

  case 26: /* lista_elementos: expresion  */
#line 215 "src/parser/parser.y"
                {
        (yyval.nodo) = crearNodo(AST_LISTA, "Lista Elementos");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2153 "parser.tab.c"
    break;

  case 27: /* lista_filas: lista_filas TOKEN_COMA TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 222 "src/parser/parser.y"
                                                                         {
        if ((yyvsp[-4].nodo)->tipo != AST_LISTA) {
            NodoAST* lista = crearNodo(AST_LISTA, "Lista Filas");
            agregarHijo(lista, (yyvsp[-4].nodo));
            NodoAST* fila = crearNodo(AST_LISTA, "Fila");
            agregarHijo(fila, (yyvsp[-1].nodo));
            agregarHijo(lista, fila);
            (yyval.nodo) = lista;
        } else {
            NodoAST* fila = crearNodo(AST_LISTA, "Fila");
            agregarHijo(fila, (yyvsp[-1].nodo));
            agregarHijo((yyvsp[-4].nodo), fila);
            (yyval.nodo) = (yyvsp[-4].nodo);
        }
    }
#line 2173 "parser.tab.c"
    break;

  case 28: /* lista_filas: TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 237 "src/parser/parser.y"
                                                    {
        NodoAST* fila = crearNodo(AST_LISTA, "Fila");
        agregarHijo(fila, (yyvsp[-1].nodo));
        (yyval.nodo) = crearNodo(AST_LISTA, "Lista Filas");
        agregarHijo((yyval.nodo), fila);
    }
#line 2184 "parser.tab.c"
    break;

  case 29: /* declaracion_matriz: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_NEW tipo_primitivo TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ expresion TOKEN_CORCHETEDER  */
#line 246 "src/parser/parser.y"
                                                                                                                                                                                                                                                {
        (yyval.nodo) = crearNodo(AST_DECLARACION_MATRIZ, "Declaración Matriz");
        (yyval.nodo)->linea = (yylsp[-9]).first_line; (yyval.nodo)->columna = (yylsp[-9]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-14].str)));
        agregarHijo((yyval.nodo), crearNodo(AST_IDENTIFICADOR, (yyvsp[-9].str)));
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo)); // Filas
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo)); // Columnas
    }
#line 2197 "parser.tab.c"
    break;

  case 30: /* declaracion_matriz: tipo_primitivo TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_LLAVEIZQ lista_filas TOKEN_LLAVEDER  */
#line 254 "src/parser/parser.y"
                                                                                                                                                                       {
        (yyval.nodo) = crearNodo(AST_DECLARACION_MATRIZ, "Declaración Matriz");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-9].str)));
        agregarHijo((yyval.nodo), crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str)));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2209 "parser.tab.c"
    break;

  case 31: /* declaracion_array_multidimensional: tipo_primitivo DimensionesCorchetes TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_NEW tipo_primitivo Dimensiones  */
#line 264 "src/parser/parser.y"
                                                                                                             {
        (yyval.nodo) = crearNodo(AST_DECLARACION_ARRAY_MULTIDIMENSIONAL, "Declaración Array Multidimensional");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-6].str)));
        agregarHijo((yyval.nodo), crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str)));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo)); // Dimensiones
    }
#line 2221 "parser.tab.c"
    break;

  case 32: /* declaracion_array_multidimensional: tipo_primitivo DimensionesCorchetes TOKEN_IDENTIFICADOR TOKEN_IGUAL TOKEN_LLAVEIZQ lista_bloques TOKEN_LLAVEDER  */
#line 271 "src/parser/parser.y"
                                                                                                                      {
        (yyval.nodo) = crearNodo(AST_DECLARACION_ARRAY_MULTIDIMENSIONAL, "Declaración Array Multidimensional");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), crearNodo(AST_TIPO, (yyvsp[-6].str)));
        agregarHijo((yyval.nodo), crearNodo(AST_IDENTIFICADOR, (yyvsp[-4].str)));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2233 "parser.tab.c"
    break;

  case 33: /* lista_bloques: lista_bloques TOKEN_COMA TOKEN_LLAVEIZQ lista_bloques TOKEN_LLAVEDER  */
#line 281 "src/parser/parser.y"
                                                                         {
        if ((yyvsp[-4].nodo)->tipo != AST_LISTA) {
            NodoAST* lista = crearNodo(AST_LISTA, "Lista Bloques");
            agregarHijo(lista, (yyvsp[-4].nodo));
            NodoAST* bloque = crearNodo(AST_LISTA, "Bloque");
            agregarHijo(bloque, (yyvsp[-1].nodo));
            agregarHijo(lista, bloque);
            (yyval.nodo) = lista;
        } else {
            NodoAST* bloque = crearNodo(AST_LISTA, "Bloque");
            agregarHijo(bloque, (yyvsp[-1].nodo));
            agregarHijo((yyvsp[-4].nodo), bloque);
            (yyval.nodo) = (yyvsp[-4].nodo);
        }
    }
#line 2253 "parser.tab.c"
    break;

  case 34: /* lista_bloques: TOKEN_LLAVEIZQ lista_elementos TOKEN_LLAVEDER  */
#line 296 "src/parser/parser.y"
                                                    {
        NodoAST* bloque = crearNodo(AST_LISTA, "Bloque");
        agregarHijo(bloque, (yyvsp[-1].nodo));
        (yyval.nodo) = bloque;
    }
#line 2263 "parser.tab.c"
    break;

  case 35: /* DimensionesCorchetes: DimensionesCorchetes TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER  */
#line 304 "src/parser/parser.y"
                                                             {
        if ((yyvsp[-2].nodo)->tipo == AST_LISTA) {
            agregarHijo((yyvsp[-2].nodo), crearNodo(AST_DIMENSION, "[]"));
            (yyval.nodo) = (yyvsp[-2].nodo);
        } else {
            NodoAST* lista = crearNodo(AST_LISTA, "Dimensiones");
            agregarHijo(lista, (yyvsp[-2].nodo));
            agregarHijo(lista, crearNodo(AST_DIMENSION, "[]"));
            (yyval.nodo) = lista;
        }
    }
#line 2279 "parser.tab.c"
    break;

  case 36: /* DimensionesCorchetes: TOKEN_CORCHETEIZQ TOKEN_CORCHETEDER  */
#line 315 "src/parser/parser.y"
                                          {
        (yyval.nodo) = crearNodo(AST_DIMENSION, "[]");
    }
#line 2287 "parser.tab.c"
    break;

  case 37: /* Dimensiones: TOKEN_CORCHETEIZQ TOKEN_NUMERO TOKEN_CORCHETEDER  */
#line 321 "src/parser/parser.y"
                                                     {
        (yyval.nodo) = crearNodo(AST_DIMENSION, "[]");
        agregarHijo((yyval.nodo), crearNodo(AST_NUMERO, (yyvsp[-1].str)));
    }
#line 2296 "parser.tab.c"
    break;

  case 38: /* Dimensiones: Dimensiones TOKEN_CORCHETEIZQ TOKEN_NUMERO TOKEN_CORCHETEDER  */
#line 325 "src/parser/parser.y"
                                                                   {
        agregarHijo((yyvsp[-3].nodo), crearNodo(AST_NUMERO, (yyvsp[-1].str)));
        (yyval.nodo) = (yyvsp[-3].nodo);
    }
#line 2305 "parser.tab.c"
    break;

  case 39: /* tipo_primitivo: TOKEN_DINT  */
#line 332 "src/parser/parser.y"
                    { (yyval.str) = (yyvsp[0].str); }
#line 2311 "parser.tab.c"
    break;

  case 40: /* tipo_primitivo: TOKEN_DCHAR  */
#line 333 "src/parser/parser.y"
                    { (yyval.str) = (yyvsp[0].str); }
#line 2317 "parser.tab.c"
    break;

  case 41: /* tipo_primitivo: TOKEN_DOUBLE  */
#line 334 "src/parser/parser.y"
                    { (yyval.str) = (yyvsp[0].str); }
#line 2323 "parser.tab.c"
    break;

  case 42: /* tipo_primitivo: TOKEN_DFLOAT  */
#line 335 "src/parser/parser.y"
                    { (yyval.str) = (yyvsp[0].str); }
#line 2329 "parser.tab.c"
    break;

  case 43: /* tipo_primitivo: TOKEN_DSTRING  */
#line 336 "src/parser/parser.y"
                    { (yyval.str) = (yyvsp[0].str); }
#line 2335 "parser.tab.c"
    break;

  case 44: /* tipo_primitivo: TOKEN_DBOOLEAN  */
#line 337 "src/parser/parser.y"
                     { (yyval.str) = (yyvsp[0].str); }
#line 2341 "parser.tab.c"
    break;

  case 45: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_IGUAL expresion  */
#line 342 "src/parser/parser.y"
                                              {
        (yyval.nodo) = crearNodo(AST_ASIGNACION, "");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; 
        (yyval.nodo)->columna = (yylsp[-2]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; 
        id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2356 "parser.tab.c"
    break;

  case 46: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_MAS_IGUAL expresion  */
#line 352 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "+", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2364 "parser.tab.c"
    break;

  case 47: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_MENOS_IGUAL expresion  */
#line 355 "src/parser/parser.y"
                                                      {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "-", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2372 "parser.tab.c"
    break;

  case 48: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_POR_IGUAL expresion  */
#line 358 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "*", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2380 "parser.tab.c"
    break;

  case 49: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_DIV_IGUAL expresion  */
#line 361 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "/", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2388 "parser.tab.c"
    break;

  case 50: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_MOD_IGUAL expresion  */
#line 364 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "%", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2396 "parser.tab.c"
    break;

  case 51: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_AND_IGUAL expresion  */
#line 367 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "&", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2404 "parser.tab.c"
    break;

  case 52: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_OR_IGUAL expresion  */
#line 370 "src/parser/parser.y"
                                                   {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "|", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2412 "parser.tab.c"
    break;

  case 53: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_XOR_IGUAL expresion  */
#line 373 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "^", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2420 "parser.tab.c"
    break;

  case 54: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_DESIZQ_IGUAL expresion  */
#line 376 "src/parser/parser.y"
                                                       {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), "<<", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2428 "parser.tab.c"
    break;

  case 55: /* asignacion: TOKEN_IDENTIFICADOR TOKEN_DESDER_IGUAL expresion  */
#line 379 "src/parser/parser.y"
                                                       {
        (yyval.nodo) = crear_asignacion_compuesta((yyvsp[-2].str), ">>", (yyvsp[0].nodo), (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2436 "parser.tab.c"
    break;

  case 56: /* expresion: expresion TOKEN_MAS expresion  */
#line 386 "src/parser/parser.y"
                                  {
        (yyval.nodo) = crearNodo(AST_OPERACION, "+");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2446 "parser.tab.c"
    break;

  case 57: /* expresion: expresion TOKEN_MENOS expresion  */
#line 391 "src/parser/parser.y"
                                      {
        (yyval.nodo) = crearNodo(AST_OPERACION, "-");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2456 "parser.tab.c"
    break;

  case 58: /* expresion: TOKEN_IDENTIFICADOR TOKEN_MAS TOKEN_MAS  */
#line 396 "src/parser/parser.y"
                                              {
        (yyval.nodo) = crearNodo(AST_INCREMENTO, "++");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
    }
#line 2468 "parser.tab.c"
    break;

  case 59: /* expresion: TOKEN_IDENTIFICADOR TOKEN_MENOS TOKEN_MENOS  */
#line 403 "src/parser/parser.y"
                                                  {
        (yyval.nodo) = crearNodo(AST_DECREMENTO, "--");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        NodoAST* id = crearNodo(AST_IDENTIFICADOR, (yyvsp[-2].str));
        id->linea = (yylsp[-2]).first_line; id->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), id);
    }
#line 2480 "parser.tab.c"
    break;

  case 60: /* expresion: expresion TOKEN_MULTI expresion  */
#line 410 "src/parser/parser.y"
                                      {
        (yyval.nodo) = crearNodo(AST_OPERACION, "*");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2490 "parser.tab.c"
    break;

  case 61: /* expresion: expresion TOKEN_DIV expresion  */
#line 415 "src/parser/parser.y"
                                    {
        (yyval.nodo) = crearNodo(AST_OPERACION, "/");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2500 "parser.tab.c"
    break;

  case 62: /* expresion: expresion TOKEN_MODULO expresion  */
#line 420 "src/parser/parser.y"
                                       {
        (yyval.nodo) = crearNodo(AST_OPERACION, "%");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2510 "parser.tab.c"
    break;

  case 63: /* expresion: expresion TOKEN_IGUAL_IGUAL expresion  */
#line 425 "src/parser/parser.y"
                                            {
        (yyval.nodo) = crearNodo(AST_OPERACION, "==");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2520 "parser.tab.c"
    break;

  case 64: /* expresion: expresion TOKEN_NO_IGUAL expresion  */
#line 430 "src/parser/parser.y"
                                         {
        (yyval.nodo) = crearNodo(AST_OPERACION, "!=");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2530 "parser.tab.c"
    break;

  case 65: /* expresion: expresion TOKEN_MENOR expresion  */
#line 435 "src/parser/parser.y"
                                      {
        (yyval.nodo) = crearNodo(AST_OPERACION, "<");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2540 "parser.tab.c"
    break;

  case 66: /* expresion: expresion TOKEN_MAYOR expresion  */
#line 440 "src/parser/parser.y"
                                      {
        (yyval.nodo) = crearNodo(AST_OPERACION, ">");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2550 "parser.tab.c"
    break;

  case 67: /* expresion: expresion TOKEN_MENOR_IGUAL expresion  */
#line 445 "src/parser/parser.y"
                                            {
        (yyval.nodo) = crearNodo(AST_OPERACION, "<=");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2560 "parser.tab.c"
    break;

  case 68: /* expresion: expresion TOKEN_MAYOR_IGUAL expresion  */
#line 450 "src/parser/parser.y"
                                            {
        (yyval.nodo) = crearNodo(AST_OPERACION, ">=");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2570 "parser.tab.c"
    break;

  case 69: /* expresion: expresion TOKEN_AND expresion  */
#line 455 "src/parser/parser.y"
                                    {
        (yyval.nodo) = crearNodo(AST_OPERACION, "&&");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2580 "parser.tab.c"
    break;

  case 70: /* expresion: expresion TOKEN_OR expresion  */
#line 460 "src/parser/parser.y"
                                   {
        (yyval.nodo) = crearNodo(AST_OPERACION, "||");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2590 "parser.tab.c"
    break;

  case 71: /* expresion: expresion TOKEN_BIT_AND expresion  */
#line 465 "src/parser/parser.y"
                                        {
        (yyval.nodo) = crearNodo(AST_OPERACION, "&");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2600 "parser.tab.c"
    break;

  case 72: /* expresion: expresion TOKEN_BIT_OR expresion  */
#line 470 "src/parser/parser.y"
                                       {
        (yyval.nodo) = crearNodo(AST_OPERACION, "|");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2610 "parser.tab.c"
    break;

  case 73: /* expresion: expresion TOKEN_XOR expresion  */
#line 475 "src/parser/parser.y"
                                    {
        (yyval.nodo) = crearNodo(AST_OPERACION, "^");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2620 "parser.tab.c"
    break;

  case 74: /* expresion: expresion TOKEN_DESIZQ expresion  */
#line 480 "src/parser/parser.y"
                                       {
        (yyval.nodo) = crearNodo(AST_OPERACION, "<<");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2630 "parser.tab.c"
    break;

  case 75: /* expresion: expresion TOKEN_DESDER expresion  */
#line 485 "src/parser/parser.y"
                                       {
        (yyval.nodo) = crearNodo(AST_OPERACION, ">>");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo)); agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2640 "parser.tab.c"
    break;

  case 76: /* expresion: expresion TOKEN_EQUAL TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 490 "src/parser/parser.y"
                                                                {
        (yyval.nodo) = crearNodo(AST_EQUAL, ".equals");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2651 "parser.tab.c"
    break;

  case 77: /* expresion: TOKEN_NOT expresion  */
#line 496 "src/parser/parser.y"
                          {
        (yyval.nodo) = crearNodo(AST_OPERACION, "!");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2661 "parser.tab.c"
    break;

  case 78: /* expresion: TOKEN_MENOS expresion  */
#line 501 "src/parser/parser.y"
                                         {
        (yyval.nodo) = crearNodo(AST_OPERACION, "-");
        (yyval.nodo)->linea = (yylsp[-1]).first_line; (yyval.nodo)->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2671 "parser.tab.c"
    break;

  case 79: /* expresion: TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 506 "src/parser/parser.y"
                                          {
        (yyval.nodo) = (yyvsp[-1].nodo);
    }
#line 2679 "parser.tab.c"
    break;

  case 80: /* expresion: TOKEN_NUMERO  */
#line 509 "src/parser/parser.y"
                   {
        (yyval.nodo) = crearNodo(AST_NUMERO, (yyvsp[0].str));
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 2688 "parser.tab.c"
    break;

  case 81: /* expresion: TOKEN_FLOAT_LITERAL  */
#line 513 "src/parser/parser.y"
                          {
        (yyval.nodo) = crearNodo(AST_NUMERO, (yyvsp[0].str));
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 2697 "parser.tab.c"
    break;

  case 82: /* expresion: TOKEN_DOUBLE_LITERAL  */
#line 517 "src/parser/parser.y"
                           {
        (yyval.nodo) = crearNodo(AST_NUMERO, (yyvsp[0].str));
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 2706 "parser.tab.c"
    break;

  case 83: /* expresion: TOKEN_CADENA  */
#line 521 "src/parser/parser.y"
                   {
        (yyval.nodo) = crearNodo(AST_CADENA, (yyvsp[0].str));
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 2715 "parser.tab.c"
    break;

  case 84: /* expresion: TOKEN_CARACTER  */
#line 525 "src/parser/parser.y"
                     {
        (yyval.nodo) = crearNodo(AST_CARACTER, (yyvsp[0].str));
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 2724 "parser.tab.c"
    break;

  case 85: /* expresion: TOKEN_BOOLEANO  */
#line 529 "src/parser/parser.y"
                     {
        (yyval.nodo) = crearNodo(AST_BOOLEAN, (yyvsp[0].str));
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 2733 "parser.tab.c"
    break;

  case 86: /* expresion: TOKEN_IDENTIFICADOR  */
#line 533 "src/parser/parser.y"
                          {
        (yyval.nodo) = crearNodo(AST_IDENTIFICADOR, (yyvsp[0].str));
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 2742 "parser.tab.c"
    break;

  case 87: /* expresion: cast  */
#line 537 "src/parser/parser.y"
           {
        (yyval.nodo) = (yyvsp[0].nodo);
    }
#line 2750 "parser.tab.c"
    break;

  case 88: /* expresion: TOKEN_STRING_VALUEOF TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 540 "src/parser/parser.y"
                                                               {
        (yyval.nodo) = crearNodo(AST_STRING, "String.valueOf");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2760 "parser.tab.c"
    break;

  case 89: /* cast: TOKEN_PARIZQ tipo_primitivo TOKEN_PARDER expresion  */
#line 549 "src/parser/parser.y"
                                                       {
        (yyval.nodo) = crearNodo(AST_CAST, (yyvsp[-2].str));
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2770 "parser.tab.c"
    break;

  case 90: /* instruccion_print: TOKEN_PRINT TOKEN_PARIZQ expresion TOKEN_PARDER  */
#line 558 "src/parser/parser.y"
                                                    {
        (yyval.nodo) = crearNodo(AST_PRINT, "System.out.println");
        (yyval.nodo)->linea = (yylsp[-3]).first_line; (yyval.nodo)->columna = (yylsp[-3]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2780 "parser.tab.c"
    break;

  case 91: /* bloque: TOKEN_LLAVEIZQ lista_sentencias TOKEN_LLAVEDER  */
#line 566 "src/parser/parser.y"
                                                   {
        (yyval.nodo) = crearNodo(AST_BLOQUE, NULL);
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2790 "parser.tab.c"
    break;

  case 92: /* instruccion_break: TOKEN_BREAK  */
#line 574 "src/parser/parser.y"
                {
        (yyval.nodo) = crearNodo(AST_BREAK, "break");
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 2799 "parser.tab.c"
    break;

  case 93: /* instruccion_continue: TOKEN_CONTINUE  */
#line 581 "src/parser/parser.y"
                   {
        (yyval.nodo) = crearNodo(AST_CONTINUE, "continue");
        (yyval.nodo)->linea = (yylsp[0]).first_line; (yyval.nodo)->columna = (yylsp[0]).first_column;
    }
#line 2808 "parser.tab.c"
    break;

  case 94: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ expresion TOKEN_PARDER bloque  */
#line 588 "src/parser/parser.y"
                                                                       {
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));   // condición
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));   // bloque if
    }
#line 2819 "parser.tab.c"
    break;

  case 95: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ expresion TOKEN_PARDER bloque TOKEN_ELSE bloque  */
#line 594 "src/parser/parser.y"
                                                                            {
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));   // condición
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));   // bloque if
        NodoAST* else_node = crearNodo(AST_ELSE, "else");
        agregarHijo(else_node, (yyvsp[0].nodo));
        agregarHijo((yyval.nodo), else_node);
    }
#line 2833 "parser.tab.c"
    break;

  case 96: /* instruccion_if: TOKEN_IF TOKEN_PARIZQ expresion TOKEN_PARDER bloque TOKEN_ELSE instruccion_if  */
#line 603 "src/parser/parser.y"
                                                                                    {
        (yyval.nodo) = crearNodo(AST_IF, "if");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));   // condición
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));   // bloque if
        NodoAST* elseif_node = crearNodo(AST_ELSEIF, "elseif");
        agregarHijo(elseif_node, (yyvsp[0].nodo));
        agregarHijo((yyval.nodo), elseif_node);
    }
#line 2847 "parser.tab.c"
    break;

  case 97: /* intruccion_switch: TOKEN_SWITCH TOKEN_PARIZQ expresion TOKEN_PARDER TOKEN_LLAVEIZQ lista_casos TOKEN_LLAVEDER  */
#line 616 "src/parser/parser.y"
                                                                                               {
        (yyval.nodo) = crearNodo(AST_SWITCH, "switch");
        (yyval.nodo)->linea = (yylsp[-6]).first_line; (yyval.nodo)->columna = (yylsp[-6]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-1].nodo));
    }
#line 2858 "parser.tab.c"
    break;

  case 98: /* lista_casos: lista_casos caso  */
#line 625 "src/parser/parser.y"
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
#line 2874 "parser.tab.c"
    break;

  case 99: /* lista_casos: caso  */
#line 636 "src/parser/parser.y"
           {
        (yyval.nodo) = crearNodo(AST_LISTA, "Casos");
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2883 "parser.tab.c"
    break;

  case 100: /* caso: TOKEN_CASE expresion TOKEN_DOS_PUNTOS lista_sentencias TOKEN_BREAK TOKEN_PUNTOCOMA  */
#line 643 "src/parser/parser.y"
                                                                                      {
        (yyval.nodo) = crearNodo(AST_CASE, "CASE");
        (yyval.nodo)->linea = (yylsp[-5]).first_line; (yyval.nodo)->columna = (yylsp[-5]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        NodoAST* nodoBreak = crearNodo(AST_BREAK, "break");
        nodoBreak->linea = (yylsp[-1]).first_line; nodoBreak->columna = (yylsp[-1]).first_column;
        agregarHijo((yyval.nodo), nodoBreak);
    }
#line 2897 "parser.tab.c"
    break;

  case 101: /* caso: TOKEN_DEFAULT TOKEN_DOS_PUNTOS lista_sentencias  */
#line 652 "src/parser/parser.y"
                                                      {
        (yyval.nodo) = crearNodo(AST_DEFAULT, "DEFAULT");
        (yyval.nodo)->linea = (yylsp[-2]).first_line; (yyval.nodo)->columna = (yylsp[-2]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2907 "parser.tab.c"
    break;

  case 102: /* instruccion_while: TOKEN_WHILE TOKEN_PARIZQ expresion TOKEN_PARDER bloque  */
#line 660 "src/parser/parser.y"
                                                           {
        (yyval.nodo) = crearNodo(AST_WHILE, "while");
        (yyval.nodo)->linea = (yylsp[-4]).first_line; (yyval.nodo)->columna = (yylsp[-4]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2918 "parser.tab.c"
    break;

  case 103: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ declaracion TOKEN_PUNTOCOMA expresion TOKEN_PUNTOCOMA expresion TOKEN_PARDER bloque  */
#line 669 "src/parser/parser.y"
                                                                                                              {
        (yyval.nodo) = crearNodo(AST_FOR, "FOR");
        (yyval.nodo)->linea = (yylsp[-8]).first_line; (yyval.nodo)->columna = (yylsp[-8]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-6].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2931 "parser.tab.c"
    break;

  case 104: /* instruccion_for: TOKEN_FOR TOKEN_PARIZQ asignacion TOKEN_PUNTOCOMA expresion TOKEN_PUNTOCOMA expresion TOKEN_PARDER bloque  */
#line 677 "src/parser/parser.y"
                                                                                                               {
        (yyval.nodo) = crearNodo(AST_FOR, "FOR");
        (yyval.nodo)->linea = (yylsp[-8]).first_line; (yyval.nodo)->columna = (yylsp[-8]).first_column;
        agregarHijo((yyval.nodo), (yyvsp[-6].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-4].nodo));
        agregarHijo((yyval.nodo), (yyvsp[-2].nodo));
        agregarHijo((yyval.nodo), (yyvsp[0].nodo));
    }
#line 2944 "parser.tab.c"
    break;


#line 2948 "parser.tab.c"

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

#line 687 "src/parser/parser.y"


/* ========== FUNCIONES AUXILIARES ========== */

NodoAST* crear_asignacion_compuesta(char* id, char* operador, NodoAST* expr, 
                                   int id_linea, int id_columna, int op_linea, int op_columna) {
    NodoAST* asig = crearNodo(AST_ASIGNACION, "");
    asig->linea = id_linea; 
    asig->columna = id_columna;
    
    NodoAST* idRef = crearNodo(AST_IDENTIFICADOR, strdup(id));
    idRef->linea = id_linea; 
    idRef->columna = id_columna;
    
    NodoAST* op = crearNodo(AST_OPERACION, strdup(operador));
    op->linea = op_linea; 
    op->columna = op_columna;
    
    NodoAST* idOp = crearNodo(AST_IDENTIFICADOR, strdup(id));
    idOp->linea = id_linea; 
    idOp->columna = id_columna;
    
    agregarHijo(op, idOp);
    agregarHijo(op, expr);
    agregarHijo(asig, idRef);
    agregarHijo(asig, op);
    
    return asig;
}

void yyerror(const char *s) {
    extern YYLTYPE yylloc;
    int linea = (yylloc.first_line > 0) ? yylloc.first_line : yylineno;
    int columna = (yylloc.first_column > 0) ? yylloc.first_column : yycolumn;
    fprintf(stderr, "%s en línea %d, columna %d\n", s, linea, columna);
}
