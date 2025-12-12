/* A Bison parser, made by GNU Bison 3.7.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 28 "asm.y"

#include "int.h"
#include "parse.h"
#include "vec.h"
#include "buf.h"
#include "log.h"
#include <stdio.h>
#define YYERROR_VERBOSE

static struct vec asm_atoms[1];

/* prototypes to silence compiler warnings */
int yylex(void);
void yyerror(const char *s);


#line 88 "asm.tab.c"

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

#include "asm.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INCLUDE = 3,                    /* INCLUDE  */
  YYSYMBOL_IF = 4,                         /* IF  */
  YYSYMBOL_DEFINED = 5,                    /* DEFINED  */
  YYSYMBOL_MACRO = 6,                      /* MACRO  */
  YYSYMBOL_MACRO_STRING = 7,               /* MACRO_STRING  */
  YYSYMBOL_ORG = 8,                        /* ORG  */
  YYSYMBOL_ERROR = 9,                      /* ERROR  */
  YYSYMBOL_ECHO1 = 10,                     /* ECHO1  */
  YYSYMBOL_INCBIN = 11,                    /* INCBIN  */
  YYSYMBOL_INCLEN = 12,                    /* INCLEN  */
  YYSYMBOL_INCWORD = 13,                   /* INCWORD  */
  YYSYMBOL_RES = 14,                       /* RES  */
  YYSYMBOL_WORD = 15,                      /* WORD  */
  YYSYMBOL_BYTE = 16,                      /* BYTE  */
  YYSYMBOL_TEXT = 17,                      /* TEXT  */
  YYSYMBOL_LDA = 18,                       /* LDA  */
  YYSYMBOL_LDX = 19,                       /* LDX  */
  YYSYMBOL_LDY = 20,                       /* LDY  */
  YYSYMBOL_STA = 21,                       /* STA  */
  YYSYMBOL_STX = 22,                       /* STX  */
  YYSYMBOL_STY = 23,                       /* STY  */
  YYSYMBOL_AND = 24,                       /* AND  */
  YYSYMBOL_ORA = 25,                       /* ORA  */
  YYSYMBOL_EOR = 26,                       /* EOR  */
  YYSYMBOL_ADC = 27,                       /* ADC  */
  YYSYMBOL_SBC = 28,                       /* SBC  */
  YYSYMBOL_CMP = 29,                       /* CMP  */
  YYSYMBOL_CPX = 30,                       /* CPX  */
  YYSYMBOL_CPY = 31,                       /* CPY  */
  YYSYMBOL_TSX = 32,                       /* TSX  */
  YYSYMBOL_TXS = 33,                       /* TXS  */
  YYSYMBOL_PHA = 34,                       /* PHA  */
  YYSYMBOL_PLA = 35,                       /* PLA  */
  YYSYMBOL_PHP = 36,                       /* PHP  */
  YYSYMBOL_PLP = 37,                       /* PLP  */
  YYSYMBOL_SEI = 38,                       /* SEI  */
  YYSYMBOL_CLI = 39,                       /* CLI  */
  YYSYMBOL_NOP = 40,                       /* NOP  */
  YYSYMBOL_TYA = 41,                       /* TYA  */
  YYSYMBOL_TAY = 42,                       /* TAY  */
  YYSYMBOL_TXA = 43,                       /* TXA  */
  YYSYMBOL_TAX = 44,                       /* TAX  */
  YYSYMBOL_CLC = 45,                       /* CLC  */
  YYSYMBOL_SEC = 46,                       /* SEC  */
  YYSYMBOL_RTS = 47,                       /* RTS  */
  YYSYMBOL_CLV = 48,                       /* CLV  */
  YYSYMBOL_CLD = 49,                       /* CLD  */
  YYSYMBOL_SED = 50,                       /* SED  */
  YYSYMBOL_JSR = 51,                       /* JSR  */
  YYSYMBOL_JMP = 52,                       /* JMP  */
  YYSYMBOL_BEQ = 53,                       /* BEQ  */
  YYSYMBOL_BNE = 54,                       /* BNE  */
  YYSYMBOL_BCC = 55,                       /* BCC  */
  YYSYMBOL_BCS = 56,                       /* BCS  */
  YYSYMBOL_BPL = 57,                       /* BPL  */
  YYSYMBOL_BMI = 58,                       /* BMI  */
  YYSYMBOL_BVC = 59,                       /* BVC  */
  YYSYMBOL_BVS = 60,                       /* BVS  */
  YYSYMBOL_INX = 61,                       /* INX  */
  YYSYMBOL_DEX = 62,                       /* DEX  */
  YYSYMBOL_INY = 63,                       /* INY  */
  YYSYMBOL_DEY = 64,                       /* DEY  */
  YYSYMBOL_INC = 65,                       /* INC  */
  YYSYMBOL_DEC = 66,                       /* DEC  */
  YYSYMBOL_LSR = 67,                       /* LSR  */
  YYSYMBOL_ASL = 68,                       /* ASL  */
  YYSYMBOL_ROR = 69,                       /* ROR  */
  YYSYMBOL_ROL = 70,                       /* ROL  */
  YYSYMBOL_BIT = 71,                       /* BIT  */
  YYSYMBOL_SYMBOL = 72,                    /* SYMBOL  */
  YYSYMBOL_STRING = 73,                    /* STRING  */
  YYSYMBOL_LAND = 74,                      /* LAND  */
  YYSYMBOL_LOR = 75,                       /* LOR  */
  YYSYMBOL_LNOT = 76,                      /* LNOT  */
  YYSYMBOL_LPAREN = 77,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 78,                    /* RPAREN  */
  YYSYMBOL_COMMA = 79,                     /* COMMA  */
  YYSYMBOL_COLON = 80,                     /* COLON  */
  YYSYMBOL_X = 81,                         /* X  */
  YYSYMBOL_Y = 82,                         /* Y  */
  YYSYMBOL_HASH = 83,                      /* HASH  */
  YYSYMBOL_PLUS = 84,                      /* PLUS  */
  YYSYMBOL_MINUS = 85,                     /* MINUS  */
  YYSYMBOL_MULT = 86,                      /* MULT  */
  YYSYMBOL_DIV = 87,                       /* DIV  */
  YYSYMBOL_MOD = 88,                       /* MOD  */
  YYSYMBOL_LT = 89,                        /* LT  */
  YYSYMBOL_GT = 90,                        /* GT  */
  YYSYMBOL_EQ = 91,                        /* EQ  */
  YYSYMBOL_NEQ = 92,                       /* NEQ  */
  YYSYMBOL_ASSIGN = 93,                    /* ASSIGN  */
  YYSYMBOL_GUESS = 94,                     /* GUESS  */
  YYSYMBOL_NUMBER = 95,                    /* NUMBER  */
  YYSYMBOL_vNEG = 96,                      /* vNEG  */
  YYSYMBOL_LABEL = 97,                     /* LABEL  */
  YYSYMBOL_YYACCEPT = 98,                  /* $accept  */
  YYSYMBOL_stmts = 99,                     /* stmts  */
  YYSYMBOL_stmt = 100,                     /* stmt  */
  YYSYMBOL_atom = 101,                     /* atom  */
  YYSYMBOL_exprs = 102,                    /* exprs  */
  YYSYMBOL_op = 103,                       /* op  */
  YYSYMBOL_am_im = 104,                    /* am_im  */
  YYSYMBOL_am_a = 105,                     /* am_a  */
  YYSYMBOL_am_ax = 106,                    /* am_ax  */
  YYSYMBOL_am_ay = 107,                    /* am_ay  */
  YYSYMBOL_am_zp = 108,                    /* am_zp  */
  YYSYMBOL_am_zpx = 109,                   /* am_zpx  */
  YYSYMBOL_am_zpy = 110,                   /* am_zpy  */
  YYSYMBOL_am_ix = 111,                    /* am_ix  */
  YYSYMBOL_am_iy = 112,                    /* am_iy  */
  YYSYMBOL_expr = 113,                     /* expr  */
  YYSYMBOL_lexpr = 114                     /* lexpr  */
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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  223
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   663

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  202
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  327

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   352


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
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   186,   186,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   200,   201,   202,   203,
     204,   205,   207,   209,   212,   213,   215,   216,   217,   218,
     219,   220,   221,   222,   224,   225,   226,   227,   228,   230,
     231,   232,   233,   234,   236,   237,   238,   239,   240,   241,
     242,   244,   245,   246,   248,   249,   250,   252,   253,   254,
     255,   256,   257,   258,   259,   261,   262,   263,   264,   265,
     266,   267,   268,   270,   271,   272,   273,   274,   275,   276,
     277,   279,   280,   281,   282,   283,   284,   285,   286,   288,
     289,   290,   291,   292,   293,   294,   295,   297,   298,   299,
     300,   301,   302,   303,   304,   306,   307,   308,   309,   310,
     311,   313,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     344,   345,   346,   347,   349,   350,   351,   352,   354,   355,
     356,   357,   359,   360,   361,   362,   363,   365,   366,   367,
     368,   369,   371,   372,   373,   374,   375,   377,   378,   379,
     380,   381,   383,   384,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   406,   407,   409,   410,   411,   412,   413,   414,
     415,   416,   418
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INCLUDE", "IF",
  "DEFINED", "MACRO", "MACRO_STRING", "ORG", "ERROR", "ECHO1", "INCBIN",
  "INCLEN", "INCWORD", "RES", "WORD", "BYTE", "TEXT", "LDA", "LDX", "LDY",
  "STA", "STX", "STY", "AND", "ORA", "EOR", "ADC", "SBC", "CMP", "CPX",
  "CPY", "TSX", "TXS", "PHA", "PLA", "PHP", "PLP", "SEI", "CLI", "NOP",
  "TYA", "TAY", "TXA", "TAX", "CLC", "SEC", "RTS", "CLV", "CLD", "SED",
  "JSR", "JMP", "BEQ", "BNE", "BCC", "BCS", "BPL", "BMI", "BVC", "BVS",
  "INX", "DEX", "INY", "DEY", "INC", "DEC", "LSR", "ASL", "ROR", "ROL",
  "BIT", "SYMBOL", "STRING", "LAND", "LOR", "LNOT", "LPAREN", "RPAREN",
  "COMMA", "COLON", "X", "Y", "HASH", "PLUS", "MINUS", "MULT", "DIV",
  "MOD", "LT", "GT", "EQ", "NEQ", "ASSIGN", "GUESS", "NUMBER", "vNEG",
  "LABEL", "$accept", "stmts", "stmt", "atom", "exprs", "op", "am_im",
  "am_a", "am_ax", "am_ay", "am_zp", "am_zpx", "am_zpy", "am_ix", "am_iy",
  "expr", "lexpr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352
};
#endif

#define YYPACT_NINF (-216)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     293,   -72,   -24,   -11,  -216,    15,    86,    92,   104,   106,
     114,   115,   116,   187,   190,   354,   373,   376,   387,   187,
     187,   187,   187,   187,   187,   357,   357,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,  -216,  -216,  -216,  -216,
     387,   387,   387,   387,   387,   387,   398,   -48,   117,    87,
    -216,  -216,  -216,   121,   155,   122,   149,   123,   128,   131,
     149,   149,   149,   133,   135,   136,  -216,   149,   149,   149,
     149,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
     507,   149,   149,  -216,  -216,  -216,  -216,  -216,   517,  -216,
    -216,  -216,  -216,  -216,   527,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,   -69,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,   149,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
     149,   149,  -216,  -216,  -216,   127,   145,   155,   155,   433,
     -18,   146,   309,   147,   -14,    -8,   537,    -6,   -69,     7,
     150,   156,   157,   132,   -69,  -216,   547,    97,   149,   149,
     149,   149,   149,   320,   557,   141,   152,   -69,   -69,   -69,
    -216,   137,  -216,   -64,   -16,   149,   149,   149,   149,   155,
     155,  -216,  -216,  -216,  -216,  -216,   149,  -216,   149,   149,
    -216,   149,  -216,  -216,   163,   164,   167,   161,   168,  -216,
    -216,   -57,   -57,  -216,  -216,  -216,  -216,   165,   170,  -216,
     -69,   -69,   -69,   -69,  -216,   171,    21,   202,   481,   -69,
    -216,   149,   169,   180,  -216,  -216,  -216,  -216,  -216,   149,
    -216,   486,  -216,  -216,   497,  -216,  -216
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,    15,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   112,   111,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   140,   141,   142,   143,
       0,     0,   152,   157,   162,   167,     0,     0,     0,     0,
       3,    14,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   193,     0,     0,     0,
       0,   192,    26,    29,    30,    31,    27,    28,    32,    33,
     175,     0,     0,    34,    37,    38,    35,    36,   175,    39,
      42,    43,    40,    41,   175,    46,    47,    48,    44,    45,
      49,    50,    53,    51,    52,   175,    56,    54,    55,    57,
      60,    61,    62,    58,    59,    63,    64,    65,    68,    69,
      70,    66,    67,    71,    72,    73,    76,    77,    78,    74,
      75,    79,    80,    81,    84,    85,    86,    82,    83,    87,
      88,    89,    92,    93,    94,    90,    91,    95,    96,    97,
     100,   101,   102,    98,    99,   103,   104,     0,   105,   107,
     106,   108,   110,   109,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   146,   147,   144,   145,   150,   151,
     148,   149,   155,   156,   153,   154,   160,   161,   158,   159,
     165,   166,   163,   164,   170,   171,   168,   169,   173,   172,
       0,     0,     4,     1,     2,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,   174,   188,   178,     0,     0,     0,
       0,     0,     0,     0,   178,     0,     0,   178,     5,     6,
      12,     0,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     7,    13,     8,     9,    10,     0,    21,     0,     0,
      18,     0,    19,    20,     0,     0,   189,     0,     0,   176,
     177,   183,   184,   185,   186,   187,   189,     0,     0,   197,
     198,   199,   200,   201,   195,   194,     0,     0,     0,    24,
     190,     0,     0,     0,   179,   180,   202,    11,    22,     0,
      17,     0,   182,   181,     0,   191,    23
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -216,  -216,   191,  -216,   -78,  -216,   151,   480,   492,   166,
      18,   356,   244,   630,   639,   -13,  -215
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    69,    70,    71,   237,    72,    92,    93,    94,    95,
      96,    97,   107,    98,    99,   125,   230
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     100,   108,   114,   100,   239,    73,   100,   100,   100,   100,
     100,   100,   262,   264,   296,   248,   249,   250,   251,   252,
     248,   249,   250,   251,   252,   265,   266,   267,   268,   250,
     251,   252,   106,   112,   118,   123,   127,   133,   141,   149,
     157,   165,   173,   180,   183,   220,   221,   114,   114,   114,
     114,   114,   114,    74,   304,   305,   269,   270,   269,   270,
     271,   229,   299,   232,   275,   276,    75,   236,   238,   238,
     277,   278,   280,   281,   243,   244,   245,   246,   196,   200,
     204,   208,   212,   216,   219,   282,   281,   223,   253,   254,
       1,     2,    76,     3,     4,     5,     6,     7,     8,   317,
     281,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
     226,    84,    85,    77,   257,   103,   109,    84,    85,    78,
     129,   137,   145,   153,   161,   169,   178,   181,   289,   290,
     105,    79,   117,    80,    68,   132,   140,   148,   156,   164,
     172,    81,    82,    83,   225,   231,   233,   222,   306,    84,
      85,   234,    84,    85,   235,   260,   240,   258,   259,   298,
     286,   287,   241,   242,   229,   263,   248,   249,   250,   251,
     252,    86,   261,   290,   272,   274,   101,    86,   283,   284,
     285,   227,   228,   289,    89,   291,   292,   293,   294,   295,
      89,   310,   313,   311,    91,   269,   312,   315,   316,   314,
      91,   322,   300,   301,   302,   303,   229,   229,   323,    86,
     224,   124,    86,   238,    87,   307,   308,   101,   309,     0,
      88,     0,    89,    88,     0,    89,    90,     0,     0,   102,
     318,   319,    91,     0,     0,    91,   248,   249,   250,   251,
     252,     0,     0,     0,     0,     0,     1,     2,   321,     3,
       4,     5,     6,     7,     8,     0,   324,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    84,    85,     0,    84,
      85,   113,   119,     0,   128,   134,   142,   150,   158,   166,
     174,     0,     0,     0,     0,    84,    85,   273,    84,    85,
      68,     0,     0,   248,   249,   250,   251,   252,   296,    84,
      85,     0,     0,     0,   248,   249,   250,   251,   252,     0,
      84,    85,     0,     0,     0,     0,   197,   201,   205,   209,
     213,   217,     0,     0,     0,     0,    86,     0,     0,    86,
       0,   101,     0,     0,   101,     0,     0,    88,     0,    89,
      88,     0,    89,    90,     0,    86,   177,     0,    86,    91,
      87,     0,    91,   101,     0,     0,     0,     0,    89,    86,
       0,    89,    90,     0,   101,   102,     0,     0,    91,     0,
      86,    91,    89,     0,     0,   101,    90,     0,     0,     0,
       0,     0,    91,    89,     0,     0,     0,   177,     0,     0,
       0,     0,     0,    91,   104,   110,   115,   122,   126,   130,
     138,   146,   154,   162,   170,   179,   182,   111,   116,     0,
       0,   131,   139,   147,   155,   163,   171,   248,   249,   250,
     251,   252,   265,   266,   267,   268,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,     0,     0,     0,     0,
     194,   198,   202,   206,   210,   214,   218,     0,     0,     0,
       0,     0,   195,   199,   203,   207,   211,   215,     0,   320,
       0,     0,     0,     0,   325,   248,   249,   250,   251,   252,
     248,   249,   250,   251,   252,   326,     0,     0,     0,     0,
       0,   248,   249,   250,   251,   252,   247,     0,     0,     0,
       0,   248,   249,   250,   251,   252,   255,     0,     0,     0,
       0,   248,   249,   250,   251,   252,   256,     0,     0,     0,
       0,   248,   249,   250,   251,   252,   279,     0,     0,     0,
       0,   248,   249,   250,   251,   252,   288,     0,     0,     0,
       0,   248,   249,   250,   251,   252,   297,     0,     0,     0,
       0,   248,   249,   250,   251,   252,   120,     0,     0,   135,
     143,   151,   159,   167,   175,   121,     0,     0,   136,   144,
     152,   160,   168,   176
};

static const yytype_int16 yycheck[] =
{
      13,    14,    15,    16,    82,    77,    19,    20,    21,    22,
      23,    24,   227,   228,    78,    84,    85,    86,    87,    88,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    86,
      87,    88,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    93,    94,    60,    61,    62,
      63,    64,    65,    77,   269,   270,    74,    75,    74,    75,
      78,    74,    78,    76,    78,    79,    77,    80,    81,    82,
      78,    79,    78,    79,    87,    88,    89,    90,    60,    61,
      62,    63,    64,    65,    66,    78,    79,     0,   101,   102,
       3,     4,    77,     6,     7,     8,     9,    10,    11,    78,
      79,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
       5,    12,    13,    77,   177,    14,    15,    12,    13,    77,
      19,    20,    21,    22,    23,    24,    25,    26,    81,    82,
      14,    77,    16,    77,    97,    19,    20,    21,    22,    23,
      24,    77,    77,    77,    73,    73,    73,    80,   276,    12,
      13,    73,    12,    13,    73,    78,    73,   220,   221,    72,
      78,    79,    77,    77,   227,   228,    84,    85,    86,    87,
      88,    72,    77,    82,    78,    78,    77,    72,    78,    73,
      73,    76,    77,    81,    85,   248,   249,   250,   251,   252,
      85,    78,    81,    79,    95,    74,    79,    82,    78,    81,
      95,    82,   265,   266,   267,   268,   269,   270,    78,    72,
      69,    17,    72,   276,    77,   278,   279,    77,   281,    -1,
      83,    -1,    85,    83,    -1,    85,    89,    -1,    -1,    89,
      78,    79,    95,    -1,    -1,    95,    84,    85,    86,    87,
      88,    -1,    -1,    -1,    -1,    -1,     3,     4,   311,     6,
       7,     8,     9,    10,    11,    -1,   319,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    12,    13,    -1,    12,
      13,    15,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    12,    13,    78,    12,    13,
      97,    -1,    -1,    84,    85,    86,    87,    88,    78,    12,
      13,    -1,    -1,    -1,    84,    85,    86,    87,    88,    -1,
      12,    13,    -1,    -1,    -1,    -1,    60,    61,    62,    63,
      64,    65,    -1,    -1,    -1,    -1,    72,    -1,    -1,    72,
      -1,    77,    -1,    -1,    77,    -1,    -1,    83,    -1,    85,
      83,    -1,    85,    89,    -1,    72,    89,    -1,    72,    95,
      77,    -1,    95,    77,    -1,    -1,    -1,    -1,    85,    72,
      -1,    85,    89,    -1,    77,    89,    -1,    -1,    95,    -1,
      72,    95,    85,    -1,    -1,    77,    89,    -1,    -1,    -1,
      -1,    -1,    95,    85,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    95,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    -1,
      60,    61,    62,    63,    64,    65,    66,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,    63,    64,    65,    -1,    78,
      -1,    -1,    -1,    -1,    78,    84,    85,    86,    87,    88,
      84,    85,    86,    87,    88,    78,    -1,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    79,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    79,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    79,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    79,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    79,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    79,    -1,    -1,    -1,
      -1,    84,    85,    86,    87,    88,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     6,     7,     8,     9,    10,    11,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    97,    99,
     100,   101,   103,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    12,    13,    72,    77,    83,    85,
      89,    95,   104,   105,   106,   107,   108,   109,   111,   112,
     113,    77,    89,   104,   105,   107,   108,   110,   113,   104,
     105,   106,   108,   109,   113,   105,   106,   107,   108,   109,
     111,   112,   105,   108,   110,   113,   105,   108,   109,   104,
     105,   106,   107,   108,   109,   111,   112,   104,   105,   106,
     107,   108,   109,   111,   112,   104,   105,   106,   107,   108,
     109,   111,   112,   104,   105,   106,   107,   108,   109,   111,
     112,   104,   105,   106,   107,   108,   109,   111,   112,   104,
     105,   106,   107,   108,   109,   111,   112,    89,   104,   105,
     108,   104,   105,   108,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   106,   108,   109,   105,   106,
     108,   109,   105,   106,   108,   109,   105,   106,   108,   109,
     105,   106,   108,   109,   105,   106,   108,   109,   105,   108,
      93,    94,    80,     0,   100,    73,     5,    76,    77,   113,
     114,    73,   113,    73,    73,    73,   113,   102,   113,   102,
      73,    77,    77,   113,   113,   113,   113,    79,    84,    85,
      86,    87,    88,   113,   113,    79,    79,   113,   113,   113,
      78,    77,   114,   113,   114,    89,    90,    91,    92,    74,
      75,    78,    78,    78,    78,    78,    79,    78,    79,    79,
      78,    79,    78,    78,    73,    73,    78,    79,    79,    81,
      82,   113,   113,   113,   113,   113,    78,    79,    72,    78,
     113,   113,   113,   113,   114,   114,   102,   113,   113,   113,
      78,    79,    79,    81,    81,    82,    78,    78,    78,    79,
      78,   113,    82,    78,   113,    78,    78
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    98,    99,    99,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   101,   101,   101,   101,
     101,   101,   101,   101,   102,   102,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   114,   114,   114,   114,   114,   114,
     114,   114,   114
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     2,     3,     3,     4,     4,     4,
       4,     6,     4,     4,     1,     1,     1,     6,     4,     4,
       4,     4,     6,     8,     3,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     1,     1,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     2,     2,     2,     2,     1,     2,     2,
       2,     2,     1,     2,     2,     2,     2,     1,     2,     2,
       2,     2,     2,     2,     2,     1,     3,     3,     2,     4,
       4,     5,     5,     3,     3,     3,     3,     3,     2,     3,
       4,     6,     1,     1,     3,     3,     2,     3,     3,     3,
       3,     3,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
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

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
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
    goto yyexhaustedlab;
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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* stmt: LABEL COLON  */
#line 187 "asm.y"
                    { new_label((yyvsp[-1].str)); }
#line 1491 "asm.tab.c"
    break;

  case 5: /* stmt: SYMBOL ASSIGN expr  */
#line 188 "asm.y"
                           { new_symbol_expr((yyvsp[-2].str), (yyvsp[0].expr)); }
#line 1497 "asm.tab.c"
    break;

  case 6: /* stmt: SYMBOL GUESS expr  */
#line 189 "asm.y"
                          { new_symbol_expr_guess((yyvsp[-2].str), (yyvsp[0].expr)); }
#line 1503 "asm.tab.c"
    break;

  case 7: /* stmt: IF LPAREN lexpr RPAREN  */
#line 190 "asm.y"
                               { push_if_state((yyvsp[-1].expr)); }
#line 1509 "asm.tab.c"
    break;

  case 8: /* stmt: ORG LPAREN expr RPAREN  */
#line 191 "asm.y"
                               { set_org((yyvsp[-1].expr)); }
#line 1515 "asm.tab.c"
    break;

  case 9: /* stmt: ERROR LPAREN STRING RPAREN  */
#line 192 "asm.y"
                                   { asm_error((yyvsp[-1].str)); }
#line 1521 "asm.tab.c"
    break;

  case 10: /* stmt: ECHO1 LPAREN STRING RPAREN  */
#line 193 "asm.y"
                                   { asm_echo((yyvsp[-1].str), NULL); }
#line 1527 "asm.tab.c"
    break;

  case 11: /* stmt: ECHO1 LPAREN STRING COMMA exprs RPAREN  */
#line 194 "asm.y"
                                               { asm_echo((yyvsp[-3].str), (yyvsp[-1].atom)); }
#line 1533 "asm.tab.c"
    break;

  case 12: /* stmt: INCLUDE LPAREN STRING RPAREN  */
#line 195 "asm.y"
                                     { asm_include((yyvsp[-1].str)); }
#line 1539 "asm.tab.c"
    break;

  case 13: /* stmt: MACRO LPAREN STRING RPAREN  */
#line 196 "asm.y"
                                   { push_macro_state((yyvsp[-1].str)); }
#line 1545 "asm.tab.c"
    break;

  case 14: /* stmt: atom  */
#line 197 "asm.y"
             { vec_push(asm_atoms, &(yyvsp[0].atom)); }
#line 1551 "asm.tab.c"
    break;

  case 15: /* stmt: MACRO_STRING  */
#line 198 "asm.y"
                     { macro_append((yyvsp[0].str)); }
#line 1557 "asm.tab.c"
    break;

  case 16: /* atom: op  */
#line 200 "asm.y"
           { (yyval.atom) = (yyvsp[0].atom); }
#line 1563 "asm.tab.c"
    break;

  case 17: /* atom: RES LPAREN expr COMMA expr RPAREN  */
#line 201 "asm.y"
                                          { (yyval.atom) = new_res((yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1569 "asm.tab.c"
    break;

  case 18: /* atom: WORD LPAREN exprs RPAREN  */
#line 202 "asm.y"
                                 { (yyval.atom) = exprs_to_word_exprs((yyvsp[-1].atom)); }
#line 1575 "asm.tab.c"
    break;

  case 19: /* atom: BYTE LPAREN exprs RPAREN  */
#line 203 "asm.y"
                                 { (yyval.atom) = exprs_to_byte_exprs((yyvsp[-1].atom)); }
#line 1581 "asm.tab.c"
    break;

  case 20: /* atom: TEXT LPAREN STRING RPAREN  */
#line 204 "asm.y"
                                  { (yyval.atom) = text_to_byte_exprs((yyvsp[-1].str)); }
#line 1587 "asm.tab.c"
    break;

  case 21: /* atom: INCBIN LPAREN STRING RPAREN  */
#line 205 "asm.y"
                                    {
            (yyval.atom) = new_incbin((yyvsp[-1].str), NULL, NULL); }
#line 1594 "asm.tab.c"
    break;

  case 22: /* atom: INCBIN LPAREN STRING COMMA expr RPAREN  */
#line 207 "asm.y"
                                               {
            (yyval.atom) = new_incbin((yyvsp[-3].str), (yyvsp[-1].expr), NULL); }
#line 1601 "asm.tab.c"
    break;

  case 23: /* atom: INCBIN LPAREN STRING COMMA expr COMMA expr RPAREN  */
#line 209 "asm.y"
                                                          {
            (yyval.atom) = new_incbin((yyvsp[-5].str), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1608 "asm.tab.c"
    break;

  case 24: /* exprs: exprs COMMA expr  */
#line 212 "asm.y"
                         { (yyval.atom) = exprs_add((yyvsp[-2].atom), (yyvsp[0].expr)); }
#line 1614 "asm.tab.c"
    break;

  case 25: /* exprs: expr  */
#line 213 "asm.y"
             { (yyval.atom) = new_exprs((yyvsp[0].expr)); }
#line 1620 "asm.tab.c"
    break;

  case 26: /* op: LDA am_im  */
#line 215 "asm.y"
                   { (yyval.atom) = new_op(0xA9, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); }
#line 1626 "asm.tab.c"
    break;

  case 27: /* op: LDA am_zp  */
#line 216 "asm.y"
                   { (yyval.atom) = new_op(0xA5, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1632 "asm.tab.c"
    break;

  case 28: /* op: LDA am_zpx  */
#line 217 "asm.y"
                   { (yyval.atom) = new_op(0xB5, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1638 "asm.tab.c"
    break;

  case 29: /* op: LDA am_a  */
#line 218 "asm.y"
                   { (yyval.atom) = new_op(0xAD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1644 "asm.tab.c"
    break;

  case 30: /* op: LDA am_ax  */
#line 219 "asm.y"
                   { (yyval.atom) = new_op(0xBD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1650 "asm.tab.c"
    break;

  case 31: /* op: LDA am_ay  */
#line 220 "asm.y"
                   { (yyval.atom) = new_op(0xB9, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1656 "asm.tab.c"
    break;

  case 32: /* op: LDA am_ix  */
#line 221 "asm.y"
                   { (yyval.atom) = new_op(0xA1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1662 "asm.tab.c"
    break;

  case 33: /* op: LDA am_iy  */
#line 222 "asm.y"
                   { (yyval.atom) = new_op(0xB1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1668 "asm.tab.c"
    break;

  case 34: /* op: LDX am_im  */
#line 224 "asm.y"
                   { (yyval.atom) = new_op(0xA2, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); }
#line 1674 "asm.tab.c"
    break;

  case 35: /* op: LDX am_zp  */
#line 225 "asm.y"
                   { (yyval.atom) = new_op(0xA6, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1680 "asm.tab.c"
    break;

  case 36: /* op: LDX am_zpy  */
#line 226 "asm.y"
                   { (yyval.atom) = new_op(0xB6, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1686 "asm.tab.c"
    break;

  case 37: /* op: LDX am_a  */
#line 227 "asm.y"
                   { (yyval.atom) = new_op(0xAE, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1692 "asm.tab.c"
    break;

  case 38: /* op: LDX am_ay  */
#line 228 "asm.y"
                   { (yyval.atom) = new_op(0xBE, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1698 "asm.tab.c"
    break;

  case 39: /* op: LDY am_im  */
#line 230 "asm.y"
                   { (yyval.atom) = new_op(0xA0, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); }
#line 1704 "asm.tab.c"
    break;

  case 40: /* op: LDY am_zp  */
#line 231 "asm.y"
                   { (yyval.atom) = new_op(0xA4, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1710 "asm.tab.c"
    break;

  case 41: /* op: LDY am_zpx  */
#line 232 "asm.y"
                   { (yyval.atom) = new_op(0xB4, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1716 "asm.tab.c"
    break;

  case 42: /* op: LDY am_a  */
#line 233 "asm.y"
                   { (yyval.atom) = new_op(0xAC, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1722 "asm.tab.c"
    break;

  case 43: /* op: LDY am_ax  */
#line 234 "asm.y"
                   { (yyval.atom) = new_op(0xBC, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1728 "asm.tab.c"
    break;

  case 44: /* op: STA am_zp  */
#line 236 "asm.y"
                   { (yyval.atom) = new_op(0x85, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1734 "asm.tab.c"
    break;

  case 45: /* op: STA am_zpx  */
#line 237 "asm.y"
                   { (yyval.atom) = new_op(0x95, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1740 "asm.tab.c"
    break;

  case 46: /* op: STA am_a  */
#line 238 "asm.y"
                   { (yyval.atom) = new_op(0x8D, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1746 "asm.tab.c"
    break;

  case 47: /* op: STA am_ax  */
#line 239 "asm.y"
                   { (yyval.atom) = new_op(0x9D, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1752 "asm.tab.c"
    break;

  case 48: /* op: STA am_ay  */
#line 240 "asm.y"
                   { (yyval.atom) = new_op(0x99, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1758 "asm.tab.c"
    break;

  case 49: /* op: STA am_ix  */
#line 241 "asm.y"
                   { (yyval.atom) = new_op(0x81, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1764 "asm.tab.c"
    break;

  case 50: /* op: STA am_iy  */
#line 242 "asm.y"
                   { (yyval.atom) = new_op(0x91, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1770 "asm.tab.c"
    break;

  case 51: /* op: STX am_zp  */
#line 244 "asm.y"
                   { (yyval.atom) = new_op(0x86, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1776 "asm.tab.c"
    break;

  case 52: /* op: STX am_zpy  */
#line 245 "asm.y"
                   { (yyval.atom) = new_op(0x96, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1782 "asm.tab.c"
    break;

  case 53: /* op: STX am_a  */
#line 246 "asm.y"
                   { (yyval.atom) = new_op(0x8e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1788 "asm.tab.c"
    break;

  case 54: /* op: STY am_zp  */
#line 248 "asm.y"
                   { (yyval.atom) = new_op(0x84, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1794 "asm.tab.c"
    break;

  case 55: /* op: STY am_zpx  */
#line 249 "asm.y"
                   { (yyval.atom) = new_op(0x94, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1800 "asm.tab.c"
    break;

  case 56: /* op: STY am_a  */
#line 250 "asm.y"
                   { (yyval.atom) = new_op(0x8c, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1806 "asm.tab.c"
    break;

  case 57: /* op: AND am_im  */
#line 252 "asm.y"
                   { (yyval.atom) = new_op(0x29, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); }
#line 1812 "asm.tab.c"
    break;

  case 58: /* op: AND am_zp  */
#line 253 "asm.y"
                   { (yyval.atom) = new_op(0x25, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1818 "asm.tab.c"
    break;

  case 59: /* op: AND am_zpx  */
#line 254 "asm.y"
                   { (yyval.atom) = new_op(0x35, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1824 "asm.tab.c"
    break;

  case 60: /* op: AND am_a  */
#line 255 "asm.y"
                   { (yyval.atom) = new_op(0x2d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1830 "asm.tab.c"
    break;

  case 61: /* op: AND am_ax  */
#line 256 "asm.y"
                   { (yyval.atom) = new_op(0x3d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1836 "asm.tab.c"
    break;

  case 62: /* op: AND am_ay  */
#line 257 "asm.y"
                   { (yyval.atom) = new_op(0x39, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1842 "asm.tab.c"
    break;

  case 63: /* op: AND am_ix  */
#line 258 "asm.y"
                   { (yyval.atom) = new_op(0x21, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1848 "asm.tab.c"
    break;

  case 64: /* op: AND am_iy  */
#line 259 "asm.y"
                   { (yyval.atom) = new_op(0x31, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1854 "asm.tab.c"
    break;

  case 65: /* op: ORA am_im  */
#line 261 "asm.y"
                   { (yyval.atom) = new_op(0x09, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); }
#line 1860 "asm.tab.c"
    break;

  case 66: /* op: ORA am_zp  */
#line 262 "asm.y"
                   { (yyval.atom) = new_op(0x05, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1866 "asm.tab.c"
    break;

  case 67: /* op: ORA am_zpx  */
#line 263 "asm.y"
                   { (yyval.atom) = new_op(0x15, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1872 "asm.tab.c"
    break;

  case 68: /* op: ORA am_a  */
#line 264 "asm.y"
                   { (yyval.atom) = new_op(0x0d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1878 "asm.tab.c"
    break;

  case 69: /* op: ORA am_ax  */
#line 265 "asm.y"
                   { (yyval.atom) = new_op(0x1d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1884 "asm.tab.c"
    break;

  case 70: /* op: ORA am_ay  */
#line 266 "asm.y"
                   { (yyval.atom) = new_op(0x19, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1890 "asm.tab.c"
    break;

  case 71: /* op: ORA am_ix  */
#line 267 "asm.y"
                   { (yyval.atom) = new_op(0x01, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1896 "asm.tab.c"
    break;

  case 72: /* op: ORA am_iy  */
#line 268 "asm.y"
                   { (yyval.atom) = new_op(0x11, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1902 "asm.tab.c"
    break;

  case 73: /* op: EOR am_im  */
#line 270 "asm.y"
                   { (yyval.atom) = new_op(0x49, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); }
#line 1908 "asm.tab.c"
    break;

  case 74: /* op: EOR am_zp  */
#line 271 "asm.y"
                   { (yyval.atom) = new_op(0x45, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1914 "asm.tab.c"
    break;

  case 75: /* op: EOR am_zpx  */
#line 272 "asm.y"
                   { (yyval.atom) = new_op(0x55, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1920 "asm.tab.c"
    break;

  case 76: /* op: EOR am_a  */
#line 273 "asm.y"
                   { (yyval.atom) = new_op(0x4d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1926 "asm.tab.c"
    break;

  case 77: /* op: EOR am_ax  */
#line 274 "asm.y"
                   { (yyval.atom) = new_op(0x5d, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1932 "asm.tab.c"
    break;

  case 78: /* op: EOR am_ay  */
#line 275 "asm.y"
                   { (yyval.atom) = new_op(0x59, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1938 "asm.tab.c"
    break;

  case 79: /* op: EOR am_ix  */
#line 276 "asm.y"
                   { (yyval.atom) = new_op(0x41, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1944 "asm.tab.c"
    break;

  case 80: /* op: EOR am_iy  */
#line 277 "asm.y"
                   { (yyval.atom) = new_op(0x51, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1950 "asm.tab.c"
    break;

  case 81: /* op: ADC am_im  */
#line 279 "asm.y"
                   { (yyval.atom) = new_op(0x69, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); }
#line 1956 "asm.tab.c"
    break;

  case 82: /* op: ADC am_zp  */
#line 280 "asm.y"
                   { (yyval.atom) = new_op(0x65, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 1962 "asm.tab.c"
    break;

  case 83: /* op: ADC am_zpx  */
#line 281 "asm.y"
                   { (yyval.atom) = new_op(0x75, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1968 "asm.tab.c"
    break;

  case 84: /* op: ADC am_a  */
#line 282 "asm.y"
                   { (yyval.atom) = new_op(0x6D, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1974 "asm.tab.c"
    break;

  case 85: /* op: ADC am_ax  */
#line 283 "asm.y"
                   { (yyval.atom) = new_op(0x7D, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1980 "asm.tab.c"
    break;

  case 86: /* op: ADC am_ay  */
#line 284 "asm.y"
                   { (yyval.atom) = new_op(0x79, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 1986 "asm.tab.c"
    break;

  case 87: /* op: ADC am_ix  */
#line 285 "asm.y"
                   { (yyval.atom) = new_op(0x61, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1992 "asm.tab.c"
    break;

  case 88: /* op: ADC am_iy  */
#line 286 "asm.y"
                   { (yyval.atom) = new_op(0x71, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 1998 "asm.tab.c"
    break;

  case 89: /* op: SBC am_im  */
#line 288 "asm.y"
                   { (yyval.atom) = new_op(0xe9, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); }
#line 2004 "asm.tab.c"
    break;

  case 90: /* op: SBC am_zp  */
#line 289 "asm.y"
                   { (yyval.atom) = new_op(0xe5, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 2010 "asm.tab.c"
    break;

  case 91: /* op: SBC am_zpx  */
#line 290 "asm.y"
                   { (yyval.atom) = new_op(0xf5, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2016 "asm.tab.c"
    break;

  case 92: /* op: SBC am_a  */
#line 291 "asm.y"
                   { (yyval.atom) = new_op(0xeD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2022 "asm.tab.c"
    break;

  case 93: /* op: SBC am_ax  */
#line 292 "asm.y"
                   { (yyval.atom) = new_op(0xfD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2028 "asm.tab.c"
    break;

  case 94: /* op: SBC am_ay  */
#line 293 "asm.y"
                   { (yyval.atom) = new_op(0xf9, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2034 "asm.tab.c"
    break;

  case 95: /* op: SBC am_ix  */
#line 294 "asm.y"
                   { (yyval.atom) = new_op(0xe1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2040 "asm.tab.c"
    break;

  case 96: /* op: SBC am_iy  */
#line 295 "asm.y"
                   { (yyval.atom) = new_op(0xf1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2046 "asm.tab.c"
    break;

  case 97: /* op: CMP am_im  */
#line 297 "asm.y"
                   { (yyval.atom) = new_op(0xc9, ATOM_TYPE_OP_ARG_UI8, (yyvsp[0].expr)); }
#line 2052 "asm.tab.c"
    break;

  case 98: /* op: CMP am_zp  */
#line 298 "asm.y"
                   { (yyval.atom) = new_op(0xc5, ATOM_TYPE_OP_ARG_U8,  (yyvsp[0].expr)); }
#line 2058 "asm.tab.c"
    break;

  case 99: /* op: CMP am_zpx  */
#line 299 "asm.y"
                   { (yyval.atom) = new_op(0xd5, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2064 "asm.tab.c"
    break;

  case 100: /* op: CMP am_a  */
#line 300 "asm.y"
                   { (yyval.atom) = new_op(0xcD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2070 "asm.tab.c"
    break;

  case 101: /* op: CMP am_ax  */
#line 301 "asm.y"
                   { (yyval.atom) = new_op(0xdD, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2076 "asm.tab.c"
    break;

  case 102: /* op: CMP am_ay  */
#line 302 "asm.y"
                   { (yyval.atom) = new_op(0xd9, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2082 "asm.tab.c"
    break;

  case 103: /* op: CMP am_ix  */
#line 303 "asm.y"
                   { (yyval.atom) = new_op(0xc1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2088 "asm.tab.c"
    break;

  case 104: /* op: CMP am_iy  */
#line 304 "asm.y"
                   { (yyval.atom) = new_op(0xd1, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2094 "asm.tab.c"
    break;

  case 105: /* op: CPX am_im  */
#line 306 "asm.y"
                  { (yyval.atom) = new_op(0xe0, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2100 "asm.tab.c"
    break;

  case 106: /* op: CPX am_zp  */
#line 307 "asm.y"
                  { (yyval.atom) = new_op(0xe4, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2106 "asm.tab.c"
    break;

  case 107: /* op: CPX am_a  */
#line 308 "asm.y"
                  { (yyval.atom) = new_op(0xec, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2112 "asm.tab.c"
    break;

  case 108: /* op: CPY am_im  */
#line 309 "asm.y"
                  { (yyval.atom) = new_op(0xc0, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2118 "asm.tab.c"
    break;

  case 109: /* op: CPY am_zp  */
#line 310 "asm.y"
                  { (yyval.atom) = new_op(0xc4, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2124 "asm.tab.c"
    break;

  case 110: /* op: CPY am_a  */
#line 311 "asm.y"
                  { (yyval.atom) = new_op(0xcc, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2130 "asm.tab.c"
    break;

  case 111: /* op: TXS  */
#line 313 "asm.y"
            { (yyval.atom) = new_op0(0x9A); }
#line 2136 "asm.tab.c"
    break;

  case 112: /* op: TSX  */
#line 314 "asm.y"
            { (yyval.atom) = new_op0(0xBA); }
#line 2142 "asm.tab.c"
    break;

  case 113: /* op: PHA  */
#line 315 "asm.y"
            { (yyval.atom) = new_op0(0x48); }
#line 2148 "asm.tab.c"
    break;

  case 114: /* op: PLA  */
#line 316 "asm.y"
            { (yyval.atom) = new_op0(0x68); }
#line 2154 "asm.tab.c"
    break;

  case 115: /* op: PHP  */
#line 317 "asm.y"
            { (yyval.atom) = new_op0(0x08); }
#line 2160 "asm.tab.c"
    break;

  case 116: /* op: PLP  */
#line 318 "asm.y"
            { (yyval.atom) = new_op0(0x28); }
#line 2166 "asm.tab.c"
    break;

  case 117: /* op: SEI  */
#line 319 "asm.y"
            { (yyval.atom) = new_op0(0x78); }
#line 2172 "asm.tab.c"
    break;

  case 118: /* op: CLI  */
#line 320 "asm.y"
            { (yyval.atom) = new_op0(0x58); }
#line 2178 "asm.tab.c"
    break;

  case 119: /* op: NOP  */
#line 321 "asm.y"
            { (yyval.atom) = new_op0(0xea); }
#line 2184 "asm.tab.c"
    break;

  case 120: /* op: TYA  */
#line 322 "asm.y"
            { (yyval.atom) = new_op0(0x98); }
#line 2190 "asm.tab.c"
    break;

  case 121: /* op: TAY  */
#line 323 "asm.y"
            { (yyval.atom) = new_op0(0xa8); }
#line 2196 "asm.tab.c"
    break;

  case 122: /* op: TXA  */
#line 324 "asm.y"
            { (yyval.atom) = new_op0(0x8a); }
#line 2202 "asm.tab.c"
    break;

  case 123: /* op: TAX  */
#line 325 "asm.y"
            { (yyval.atom) = new_op0(0xaa); }
#line 2208 "asm.tab.c"
    break;

  case 124: /* op: CLC  */
#line 326 "asm.y"
            { (yyval.atom) = new_op0(0x18); }
#line 2214 "asm.tab.c"
    break;

  case 125: /* op: SEC  */
#line 327 "asm.y"
            { (yyval.atom) = new_op0(0x38); }
#line 2220 "asm.tab.c"
    break;

  case 126: /* op: RTS  */
#line 328 "asm.y"
            { (yyval.atom) = new_op0(0x60); }
#line 2226 "asm.tab.c"
    break;

  case 127: /* op: CLV  */
#line 329 "asm.y"
            { (yyval.atom) = new_op0(0xb8); }
#line 2232 "asm.tab.c"
    break;

  case 128: /* op: CLD  */
#line 330 "asm.y"
            { (yyval.atom) = new_op0(0xd8); }
#line 2238 "asm.tab.c"
    break;

  case 129: /* op: SED  */
#line 331 "asm.y"
            { (yyval.atom) = new_op0(0xf0); }
#line 2244 "asm.tab.c"
    break;

  case 130: /* op: JSR am_a  */
#line 333 "asm.y"
                   { (yyval.atom) = new_op(0x20, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2250 "asm.tab.c"
    break;

  case 131: /* op: JMP am_a  */
#line 334 "asm.y"
                   { (yyval.atom) = new_op(0x4c, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2256 "asm.tab.c"
    break;

  case 132: /* op: BEQ am_a  */
#line 335 "asm.y"
                   { (yyval.atom) = new_op(0xf0, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); }
#line 2262 "asm.tab.c"
    break;

  case 133: /* op: BNE am_a  */
#line 336 "asm.y"
                   { (yyval.atom) = new_op(0xd0, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); }
#line 2268 "asm.tab.c"
    break;

  case 134: /* op: BCC am_a  */
#line 337 "asm.y"
                   { (yyval.atom) = new_op(0x90, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); }
#line 2274 "asm.tab.c"
    break;

  case 135: /* op: BCS am_a  */
#line 338 "asm.y"
                   { (yyval.atom) = new_op(0xb0, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); }
#line 2280 "asm.tab.c"
    break;

  case 136: /* op: BPL am_a  */
#line 339 "asm.y"
                   { (yyval.atom) = new_op(0x10, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); }
#line 2286 "asm.tab.c"
    break;

  case 137: /* op: BMI am_a  */
#line 340 "asm.y"
                   { (yyval.atom) = new_op(0x30, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); }
#line 2292 "asm.tab.c"
    break;

  case 138: /* op: BVC am_a  */
#line 341 "asm.y"
                   { (yyval.atom) = new_op(0x50, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); }
#line 2298 "asm.tab.c"
    break;

  case 139: /* op: BVS am_a  */
#line 342 "asm.y"
                   { (yyval.atom) = new_op(0x70, ATOM_TYPE_OP_ARG_I8,  (yyvsp[0].expr)); }
#line 2304 "asm.tab.c"
    break;

  case 140: /* op: INX  */
#line 344 "asm.y"
            { (yyval.atom) = new_op0(0xe8); }
#line 2310 "asm.tab.c"
    break;

  case 141: /* op: DEX  */
#line 345 "asm.y"
            { (yyval.atom) = new_op0(0xca); }
#line 2316 "asm.tab.c"
    break;

  case 142: /* op: INY  */
#line 346 "asm.y"
            { (yyval.atom) = new_op0(0xc8); }
#line 2322 "asm.tab.c"
    break;

  case 143: /* op: DEY  */
#line 347 "asm.y"
            { (yyval.atom) = new_op0(0x88); }
#line 2328 "asm.tab.c"
    break;

  case 144: /* op: INC am_zp  */
#line 349 "asm.y"
                   { (yyval.atom) = new_op(0xe6, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2334 "asm.tab.c"
    break;

  case 145: /* op: INC am_zpx  */
#line 350 "asm.y"
                   { (yyval.atom) = new_op(0xf6, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2340 "asm.tab.c"
    break;

  case 146: /* op: INC am_a  */
#line 351 "asm.y"
                   { (yyval.atom) = new_op(0xee, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2346 "asm.tab.c"
    break;

  case 147: /* op: INC am_ax  */
#line 352 "asm.y"
                   { (yyval.atom) = new_op(0xfe, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2352 "asm.tab.c"
    break;

  case 148: /* op: DEC am_zp  */
#line 354 "asm.y"
                   { (yyval.atom) = new_op(0xc6, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2358 "asm.tab.c"
    break;

  case 149: /* op: DEC am_zpx  */
#line 355 "asm.y"
                   { (yyval.atom) = new_op(0xd6, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2364 "asm.tab.c"
    break;

  case 150: /* op: DEC am_a  */
#line 356 "asm.y"
                   { (yyval.atom) = new_op(0xce, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2370 "asm.tab.c"
    break;

  case 151: /* op: DEC am_ax  */
#line 357 "asm.y"
                   { (yyval.atom) = new_op(0xde, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2376 "asm.tab.c"
    break;

  case 152: /* op: LSR  */
#line 359 "asm.y"
                   { (yyval.atom) = new_op0(0x4a); }
#line 2382 "asm.tab.c"
    break;

  case 153: /* op: LSR am_zp  */
#line 360 "asm.y"
                   { (yyval.atom) = new_op(0x46, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2388 "asm.tab.c"
    break;

  case 154: /* op: LSR am_zpx  */
#line 361 "asm.y"
                   { (yyval.atom) = new_op(0x56, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2394 "asm.tab.c"
    break;

  case 155: /* op: LSR am_a  */
#line 362 "asm.y"
                   { (yyval.atom) = new_op(0x4e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2400 "asm.tab.c"
    break;

  case 156: /* op: LSR am_ax  */
#line 363 "asm.y"
                   { (yyval.atom) = new_op(0x5e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2406 "asm.tab.c"
    break;

  case 157: /* op: ASL  */
#line 365 "asm.y"
                   { (yyval.atom) = new_op0(0x0a); }
#line 2412 "asm.tab.c"
    break;

  case 158: /* op: ASL am_zp  */
#line 366 "asm.y"
                   { (yyval.atom) = new_op(0x06, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2418 "asm.tab.c"
    break;

  case 159: /* op: ASL am_zpx  */
#line 367 "asm.y"
                   { (yyval.atom) = new_op(0x16, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2424 "asm.tab.c"
    break;

  case 160: /* op: ASL am_a  */
#line 368 "asm.y"
                   { (yyval.atom) = new_op(0x0e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2430 "asm.tab.c"
    break;

  case 161: /* op: ASL am_ax  */
#line 369 "asm.y"
                   { (yyval.atom) = new_op(0x1e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2436 "asm.tab.c"
    break;

  case 162: /* op: ROR  */
#line 371 "asm.y"
                   { (yyval.atom) = new_op0(0x6a); }
#line 2442 "asm.tab.c"
    break;

  case 163: /* op: ROR am_zp  */
#line 372 "asm.y"
                   { (yyval.atom) = new_op(0x66, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2448 "asm.tab.c"
    break;

  case 164: /* op: ROR am_zpx  */
#line 373 "asm.y"
                   { (yyval.atom) = new_op(0x76, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2454 "asm.tab.c"
    break;

  case 165: /* op: ROR am_a  */
#line 374 "asm.y"
                   { (yyval.atom) = new_op(0x6e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2460 "asm.tab.c"
    break;

  case 166: /* op: ROR am_ax  */
#line 375 "asm.y"
                   { (yyval.atom) = new_op(0x7e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2466 "asm.tab.c"
    break;

  case 167: /* op: ROL  */
#line 377 "asm.y"
                   { (yyval.atom) = new_op0(0x2a); }
#line 2472 "asm.tab.c"
    break;

  case 168: /* op: ROL am_zp  */
#line 378 "asm.y"
                   { (yyval.atom) = new_op(0x26, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2478 "asm.tab.c"
    break;

  case 169: /* op: ROL am_zpx  */
#line 379 "asm.y"
                   { (yyval.atom) = new_op(0x36, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2484 "asm.tab.c"
    break;

  case 170: /* op: ROL am_a  */
#line 380 "asm.y"
                   { (yyval.atom) = new_op(0x2e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2490 "asm.tab.c"
    break;

  case 171: /* op: ROL am_ax  */
#line 381 "asm.y"
                   { (yyval.atom) = new_op(0x3e, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2496 "asm.tab.c"
    break;

  case 172: /* op: BIT am_zp  */
#line 383 "asm.y"
                   { (yyval.atom) = new_op(0x24, ATOM_TYPE_OP_ARG_U8, (yyvsp[0].expr)); }
#line 2502 "asm.tab.c"
    break;

  case 173: /* op: BIT am_a  */
#line 384 "asm.y"
                   { (yyval.atom) = new_op(0x2c, ATOM_TYPE_OP_ARG_U16, (yyvsp[0].expr)); }
#line 2508 "asm.tab.c"
    break;

  case 174: /* am_im: HASH expr  */
#line 386 "asm.y"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 2514 "asm.tab.c"
    break;

  case 175: /* am_a: expr  */
#line 387 "asm.y"
             { (yyval.expr) = (yyvsp[0].expr); }
#line 2520 "asm.tab.c"
    break;

  case 176: /* am_ax: expr COMMA X  */
#line 388 "asm.y"
                     { (yyval.expr) = (yyvsp[-2].expr); }
#line 2526 "asm.tab.c"
    break;

  case 177: /* am_ay: expr COMMA Y  */
#line 389 "asm.y"
                     { (yyval.expr) = (yyvsp[-2].expr); }
#line 2532 "asm.tab.c"
    break;

  case 178: /* am_zp: LT expr  */
#line 390 "asm.y"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 2538 "asm.tab.c"
    break;

  case 179: /* am_zpx: LT expr COMMA X  */
#line 391 "asm.y"
                        { (yyval.expr) = (yyvsp[-2].expr); }
#line 2544 "asm.tab.c"
    break;

  case 180: /* am_zpy: LT expr COMMA Y  */
#line 392 "asm.y"
                        { (yyval.expr) = (yyvsp[-2].expr); }
#line 2550 "asm.tab.c"
    break;

  case 181: /* am_ix: LPAREN expr COMMA X RPAREN  */
#line 393 "asm.y"
                                   { (yyval.expr) = (yyvsp[-3].expr); }
#line 2556 "asm.tab.c"
    break;

  case 182: /* am_iy: LPAREN expr RPAREN COMMA Y  */
#line 394 "asm.y"
                                   { (yyval.expr) = (yyvsp[-3].expr); }
#line 2562 "asm.tab.c"
    break;

  case 183: /* expr: expr PLUS expr  */
#line 396 "asm.y"
                              { (yyval.expr) = new_expr_op2(PLUS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2568 "asm.tab.c"
    break;

  case 184: /* expr: expr MINUS expr  */
#line 397 "asm.y"
                              { (yyval.expr) = new_expr_op2(MINUS, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2574 "asm.tab.c"
    break;

  case 185: /* expr: expr MULT expr  */
#line 398 "asm.y"
                              { (yyval.expr) = new_expr_op2(MULT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2580 "asm.tab.c"
    break;

  case 186: /* expr: expr DIV expr  */
#line 399 "asm.y"
                              { (yyval.expr) = new_expr_op2(DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2586 "asm.tab.c"
    break;

  case 187: /* expr: expr MOD expr  */
#line 400 "asm.y"
                              { (yyval.expr) = new_expr_op2(MOD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2592 "asm.tab.c"
    break;

  case 188: /* expr: MINUS expr  */
#line 401 "asm.y"
                              { (yyval.expr) = new_expr_op1(vNEG, (yyvsp[0].expr)); }
#line 2598 "asm.tab.c"
    break;

  case 189: /* expr: LPAREN expr RPAREN  */
#line 402 "asm.y"
                           { (yyval.expr) = (yyvsp[-1].expr); }
#line 2604 "asm.tab.c"
    break;

  case 190: /* expr: INCLEN LPAREN STRING RPAREN  */
#line 403 "asm.y"
                                    { (yyval.expr) = new_expr_inclen((yyvsp[-1].str)); }
#line 2610 "asm.tab.c"
    break;

  case 191: /* expr: INCWORD LPAREN STRING COMMA expr RPAREN  */
#line 404 "asm.y"
                                                {
            (yyval.expr) = new_expr_incword((yyvsp[-3].str), (yyvsp[-1].expr)); }
#line 2617 "asm.tab.c"
    break;

  case 192: /* expr: NUMBER  */
#line 406 "asm.y"
               { (yyval.expr) = new_expr_number((yyvsp[0].num)); }
#line 2623 "asm.tab.c"
    break;

  case 193: /* expr: SYMBOL  */
#line 407 "asm.y"
               { (yyval.expr) = new_expr_symref((yyvsp[0].str)); }
#line 2629 "asm.tab.c"
    break;

  case 194: /* lexpr: lexpr LOR lexpr  */
#line 409 "asm.y"
                            { (yyval.expr) = new_expr_op2(LOR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2635 "asm.tab.c"
    break;

  case 195: /* lexpr: lexpr LAND lexpr  */
#line 410 "asm.y"
                            { (yyval.expr) = new_expr_op2(LAND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2641 "asm.tab.c"
    break;

  case 196: /* lexpr: LNOT lexpr  */
#line 411 "asm.y"
                            { (yyval.expr) = new_expr_op1(LNOT, (yyvsp[0].expr)); }
#line 2647 "asm.tab.c"
    break;

  case 197: /* lexpr: LPAREN lexpr RPAREN  */
#line 412 "asm.y"
                            { (yyval.expr) = (yyvsp[-1].expr); }
#line 2653 "asm.tab.c"
    break;

  case 198: /* lexpr: expr LT expr  */
#line 413 "asm.y"
                            { (yyval.expr) = new_expr_op2(LT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2659 "asm.tab.c"
    break;

  case 199: /* lexpr: expr GT expr  */
#line 414 "asm.y"
                            { (yyval.expr) = new_expr_op2(GT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2665 "asm.tab.c"
    break;

  case 200: /* lexpr: expr EQ expr  */
#line 415 "asm.y"
                            { (yyval.expr) = new_expr_op2(EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2671 "asm.tab.c"
    break;

  case 201: /* lexpr: expr NEQ expr  */
#line 416 "asm.y"
                            { (yyval.expr) = new_expr_op2(NEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2677 "asm.tab.c"
    break;

  case 202: /* lexpr: DEFINED LPAREN SYMBOL RPAREN  */
#line 418 "asm.y"
                                            { (yyval.expr) = new_is_defined((yyvsp[-1].str)); }
#line 2683 "asm.tab.c"
    break;


#line 2687 "asm.tab.c"

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
      yyerror (YY_("syntax error"));
    }

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
                      yytoken, &yylval);
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


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 420 "asm.y"


void yyerror (const char *s)
{
    fprintf (stderr, "line %d, %s\n", num_lines, s);
}

void asm_set_source(struct buf *buffer);

int assembleSinglePass(struct buf *source, struct buf *dest)
{
    int val;

    yydebug = 0;
    asm_src_buffer_push(source);
    vec_init(asm_atoms, sizeof(struct atom*));
    val = yyparse();
    if(val == 0)
    {
        output_atoms(dest, asm_atoms);
    }
    vec_free(asm_atoms, NULL);
    return val;
}
