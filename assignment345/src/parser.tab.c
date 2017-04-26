/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

/************************************************************************/
/*                                                                      */
/*      parser.y                                                        */
/*      This is the parser for the 488S course project.                 */
/************************************************************************/


/************************************************************************/
/*      Definitions common to all modules in the compiler               */
/************************************************************************/

#include "common.h"

/************************************************************************/
/*      Global variables                                                */
/************************************************************************/

#include "globalvars.h"

extern int yylex();             /* lexical analysis     */

/************************************************************************/
/*     Functions introduced in this file                                */
/************************************************************************/

void yyerror(char* message);   /* how to report errors */



#line 97 "parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENT = 258,
    TEXTCONST = 259,
    INTCONST = 260,
    BOOL = 261,
    DO = 262,
    ELSE = 263,
    EXIT = 264,
    FI = 265,
    GET = 266,
    IF = 267,
    INT = 268,
    OD = 269,
    PROC = 270,
    PUT = 271,
    SKIP = 272,
    RESULT = 273,
    RETURN = 274,
    THEN = 275,
    TFALSE = 276,
    TTRUE = 277,
    UNTIL = 278,
    WHILE = 279,
    FOR = 280,
    TO = 281,
    BY = 282,
    ROF = 283,
    FORWARD = 284,
    ASSERT = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 31 "parser.y" /* yacc.c:355  */

        int number;
        char *str;
        tokentype token;
        LitObjP litP;
        ScopeP scopeP;
        StmtP stmtP;
        FparmP fparmP;
        DeclP declP;
        ExpnP expnP;
        ArgsP argsP;
        ObjectP objectP; 
        

#line 183 "parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 200 "parser.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   336

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  160

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,     2,    44,     2,     2,    42,     2,
      36,    37,    48,    47,    40,    35,     2,    49,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    34,     2,
      46,    33,    45,    50,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    38,     2,    39,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,    41,    32,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    69,    69,    73,    75,    79,    81,    85,    89,    92,
      95,    97,    99,   101,   103,   105,   107,   109,   111,   113,
     115,   117,   119,   123,   125,   128,   131,   133,   137,   139,
     143,   145,   147,   149,   151,   155,   157,   159,   161,   165,
     169,   171,   175,   177,   181,   183,   187,   189,   191,   195,
     197,   201,   203,   207,   211,   213,   217,   219,   223,   225,
     229,   231,   233,   235,   237,   239,   241,   245,   247,   249,
     253,   255,   257,   261,   263,   267,   269,   271,   273,   275,
     279,   283,   285,   287,   291,   293
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENT", "TEXTCONST", "INTCONST", "BOOL",
  "DO", "ELSE", "EXIT", "FI", "GET", "IF", "INT", "OD", "PROC", "PUT",
  "SKIP", "RESULT", "RETURN", "THEN", "TFALSE", "TTRUE", "UNTIL", "WHILE",
  "FOR", "TO", "BY", "ROF", "FORWARD", "ASSERT", "'{'", "'}'", "'='",
  "':'", "'-'", "'('", "')'", "'['", "']'", "','", "'|'", "'&'", "'!'",
  "'#'", "'>'", "'<'", "'+'", "'*'", "'/'", "'?'", "$accept", "program",
  "statements", "declarations", "scope", "optDeclarations", "statement",
  "byPart", "procStatement", "ifstatement", "declaration", "routineBody",
  "typeAndIdent", "parameters", "arguments", "type", "output", "outputs",
  "inputs", "input", "expression", "conjunction", "negation",
  "compareExpr", "arithExpr", "term", "factor", "primary", "functionCall",
  "constant", "variable", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   123,   125,    61,    58,    45,    40,    41,    91,    93,
      44,   124,    38,    33,    35,    62,    60,    43,    42,    47,
      63
};
# endif

#define YYPACT_NINF -47

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-47)))

#define YYTABLE_NINF -85

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -19,   107,    21,   -47,   -47,   -47,    37,   107,   305,   -47,
     114,    66,   -47,   125,   -47,   130,   305,   -47,    78,    63,
      10,    63,   -47,    63,    78,    63,   153,   -47,   -47,   -47,
     -47,    49,    63,   144,    16,    82,   -47,   -47,   -47,    63,
      97,   204,    55,    57,   -47,   -47,   138,   -47,   -47,   -47,
      97,    63,    63,    22,    70,   -47,   -47,    29,     3,   -47,
     -47,   -47,   -47,   -47,   -47,   -47,   -47,    71,    76,    76,
       2,    95,    76,   -47,   -47,    63,    76,   -47,   -47,   -47,
      54,   102,    91,    76,    69,    63,    78,    63,   -47,    64,
     -47,   305,    63,    63,    97,    97,    97,    14,   122,    97,
      97,    97,    10,   305,   109,    76,   111,    16,   -47,   -47,
      63,   -47,    76,   -47,    98,   -47,    63,   179,    70,   -47,
      23,     3,    23,    97,    23,    97,    23,     3,   -47,   -47,
     -47,   229,    63,   150,   -47,    76,   -47,    31,   305,   -47,
      23,    23,   -47,    -3,   -47,   -47,    63,   254,    63,    30,
     -47,    -2,   -47,     6,   305,   -47,   149,   279,   -47,   -47
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     9,     0,     2,    45,    44,     0,     8,     0,     5,
      30,     0,     1,     0,     6,    26,     0,    14,     0,     0,
       0,     0,    15,     0,     0,     0,     0,    20,     3,    19,
      11,     0,     0,     0,     0,     0,    33,    39,    34,     0,
       0,     0,    84,    18,    51,    53,    84,    81,    83,    82,
       0,     0,     0,     0,    54,    56,    58,    60,    67,    70,
      73,    78,    79,    77,    47,    48,    49,    17,    46,    16,
       0,     0,    22,     7,     4,     0,    31,    37,    38,    40,
       0,     0,     0,    42,     0,     0,     0,     0,    74,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    10,     0,     0,    32,    27,
       0,    85,    13,    52,     0,    75,     0,     0,    55,    57,
      61,    69,    62,     0,    63,     0,    65,    68,    71,    72,
      50,     0,     0,     0,    41,    43,    80,     0,     0,    29,
      64,    66,    12,     0,    36,    35,     0,     0,     0,     0,
      28,    25,    76,     0,     0,    23,     0,     0,    24,    21
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -47,   -47,   -14,   -47,     1,   -47,   -21,   -47,   -47,   -47,
     160,   157,   -28,   -47,    93,   -47,    90,   -47,   -47,   108,
       5,   101,   -45,   -47,   -37,     8,   -46,   -47,   -47,   -47,
      -8
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    26,     7,    27,     8,    28,   154,    29,    30,
       9,    36,    10,    80,    82,    11,    66,    67,    43,    44,
      68,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      31,     3,    41,    84,    88,    74,    79,    90,    31,   103,
      45,   155,     1,    46,    64,    47,    71,    46,    31,    47,
      74,    12,     4,   148,    53,   153,    69,    65,    70,     5,
      72,    48,    49,    31,    78,    48,    49,    76,    92,    92,
      13,   156,    91,    92,    83,    50,    51,   123,   119,    50,
      51,   100,   101,    52,   128,   129,    89,   120,    95,   122,
     124,   126,    94,    92,    95,   146,    46,   152,    47,    37,
      99,    92,    92,    96,    97,    98,    99,   117,    45,   134,
     105,    42,    75,    31,    48,    49,   140,    81,   141,   131,
     112,   106,    83,    40,   107,    31,    74,    86,    50,    51,
      46,   115,    47,   121,    95,    92,    52,   127,   111,    31,
      74,   102,    93,     4,   116,   135,    99,    92,    48,    49,
       5,   137,     6,    31,   147,    46,    74,    47,   109,   104,
      31,   110,    50,    51,   145,   136,    74,   143,   110,    31,
     157,   108,   132,    48,    49,   133,    31,    32,    33,    31,
      34,   149,    35,   151,   158,   125,    15,    50,    51,    33,
      16,    34,    17,   -84,    18,    19,    39,    14,    40,    20,
      38,    21,    22,    77,    87,     1,    40,    23,    24,   144,
     114,     1,    15,    25,     1,    73,    16,   138,    17,   139,
      18,    19,   130,   118,   113,    20,     0,    21,    22,     0,
       0,     0,     0,    23,    24,     0,     0,    15,     0,    25,
       1,    16,     0,    17,     0,    18,    19,     0,     0,     0,
      20,     0,    21,    22,     0,     0,     0,    85,    23,    24,
       0,     0,    15,     0,    25,     1,    16,     0,    17,     0,
      18,    19,     0,   142,     0,    20,     0,    21,    22,     0,
       0,     0,     0,    23,    24,     0,     0,    15,     0,    25,
       1,    16,     0,    17,   150,    18,    19,     0,     0,     0,
      20,     0,    21,    22,     0,     0,     0,     0,    23,    24,
       0,     0,    15,     0,    25,     1,    16,     0,    17,     0,
      18,    19,     0,     0,     0,    20,     0,    21,    22,     0,
       0,     0,     0,    23,    24,     0,     0,   159,    15,    25,
       1,     0,    16,     0,    17,     0,    18,    19,     0,     0,
       0,    20,     0,    21,    22,     0,     0,     0,     0,    23,
      24,     0,     0,     0,     0,    25,     1
};

static const yytype_int16 yycheck[] =
{
       8,     0,    16,    40,    50,    26,    34,    52,    16,     7,
      18,     5,    31,     3,     4,     5,    24,     3,    26,     5,
      41,     0,     6,    26,    19,    27,    21,    17,    23,    13,
      25,    21,    22,    41,    33,    21,    22,    32,    41,    41,
       3,    35,    20,    41,    39,    35,    36,    33,    93,    35,
      36,    48,    49,    43,   100,   101,    51,    94,    35,    96,
      97,    98,    33,    41,    35,    34,     3,    37,     5,     3,
      47,    41,    41,    44,    45,    46,    47,    91,    86,   107,
      75,     3,    33,    91,    21,    22,   123,     5,   125,   103,
      85,    37,    87,    38,    40,   103,   117,    40,    35,    36,
       3,    37,     5,    95,    35,    41,    43,    99,    39,   117,
     131,    40,    42,     6,    50,   110,    47,    41,    21,    22,
      13,   116,    15,   131,   138,     3,   147,     5,    37,    34,
     138,    40,    35,    36,   133,    37,   157,   132,    40,   147,
     154,    39,    33,    21,    22,    34,   154,    33,    34,   157,
      36,   146,    38,   148,     5,    33,     3,    35,    36,    34,
       7,    36,     9,    33,    11,    12,    36,     7,    38,    16,
      13,    18,    19,    29,    36,    31,    38,    24,    25,    29,
      87,    31,     3,    30,    31,    32,     7,     8,     9,    10,
      11,    12,   102,    92,    86,    16,    -1,    18,    19,    -1,
      -1,    -1,    -1,    24,    25,    -1,    -1,     3,    -1,    30,
      31,     7,    -1,     9,    -1,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    -1,    -1,    -1,    23,    24,    25,
      -1,    -1,     3,    -1,    30,    31,     7,    -1,     9,    -1,
      11,    12,    -1,    14,    -1,    16,    -1,    18,    19,    -1,
      -1,    -1,    -1,    24,    25,    -1,    -1,     3,    -1,    30,
      31,     7,    -1,     9,    10,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    -1,    -1,    -1,    -1,    24,    25,
      -1,    -1,     3,    -1,    30,    31,     7,    -1,     9,    -1,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    -1,
      -1,    -1,    -1,    24,    25,    -1,    -1,    28,     3,    30,
      31,    -1,     7,    -1,     9,    -1,    11,    12,    -1,    -1,
      -1,    16,    -1,    18,    19,    -1,    -1,    -1,    -1,    24,
      25,    -1,    -1,    -1,    -1,    30,    31
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    31,    52,    55,     6,    13,    15,    54,    56,    61,
      63,    66,     0,     3,    61,     3,     7,     9,    11,    12,
      16,    18,    19,    24,    25,    30,    53,    55,    57,    59,
      60,    81,    33,    34,    36,    38,    62,     3,    62,    36,
      38,    53,     3,    69,    70,    81,     3,     5,    21,    22,
      35,    36,    43,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,     4,    17,    67,    68,    71,    71,
      71,    81,    71,    32,    57,    33,    71,    29,    55,    63,
      64,     5,    65,    71,    75,    23,    40,    36,    77,    71,
      73,    20,    41,    42,    33,    35,    44,    45,    46,    47,
      48,    49,    40,     7,    34,    71,    37,    40,    39,    37,
      40,    39,    71,    70,    65,    37,    50,    53,    72,    73,
      75,    76,    75,    33,    75,    33,    75,    76,    77,    77,
      67,    53,    33,    34,    63,    71,    37,    71,     8,    10,
      75,    75,    14,    71,    29,    55,    34,    53,    26,    71,
      10,    71,    37,    27,    58,     5,    35,    53,     5,    28
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    53,    53,    54,    54,    55,    56,    56,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    58,    58,    58,    59,    59,    60,    60,
      61,    61,    61,    61,    61,    62,    62,    62,    62,    63,
      64,    64,    65,    65,    66,    66,    67,    67,    67,    68,
      68,    69,    69,    70,    71,    71,    72,    72,    73,    73,
      74,    74,    74,    74,    74,    74,    74,    75,    75,    75,
      76,    76,    76,    77,    77,    78,    78,    78,    78,    78,
      79,    80,    80,    80,    81,    81
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     2,     4,     1,     0,
       3,     1,     5,     4,     1,     1,     2,     2,     2,     1,
       1,    10,     2,     2,     3,     0,     1,     4,     7,     5,
       1,     3,     4,     2,     3,     5,     5,     2,     2,     2,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     1,     3,     1,     3,     1,     2,
       1,     3,     3,     3,     4,     3,     4,     1,     3,     3,
       1,     3,     3,     1,     2,     3,     7,     1,     1,     1,
       4,     1,     1,     1,     1,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
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
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
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
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
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
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
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
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
        case 2:
#line 70 "parser.y" /* yacc.c:1646  */
    { parseTree = mkProgram ((yyvsp[0].scopeP)); }
#line 1424 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 74 "parser.y" /* yacc.c:1646  */
    { (yyval.objectP) = mkStmt_list(NULL, (yyvsp[0].stmtP));}
#line 1430 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 76 "parser.y" /* yacc.c:1646  */
    { (yyval.objectP) = mkStmt_list((yyvsp[-1].objectP), (yyvsp[0].stmtP));}
#line 1436 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 80 "parser.y" /* yacc.c:1646  */
    { (yyval.objectP) = mkDecl_list(NULL, (yyvsp[0].declP)); }
#line 1442 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 82 "parser.y" /* yacc.c:1646  */
    { (yyval.objectP) = mkDecl_list((yyvsp[-1].objectP), (yyvsp[0].declP)); }
#line 1448 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 86 "parser.y" /* yacc.c:1646  */
    { (yyval.scopeP) = mkScope ((yyvsp[-2].objectP), (yyvsp[-1].objectP), line); }
#line 1454 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 90 "parser.y" /* yacc.c:1646  */
    { (yyval.objectP) = (yyvsp[0].objectP); }
#line 1460 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 92 "parser.y" /* yacc.c:1646  */
    { (yyval.objectP) = NULL; }
#line 1466 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 96 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_assign( (yyvsp[-2].expnP), (yyvsp[0].expnP), line); }
#line 1472 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 98 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = (yyvsp[0].stmtP); }
#line 1478 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 100 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_while( (yyvsp[-3].expnP), (yyvsp[-1].objectP), line); }
#line 1484 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 102 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_do( (yyvsp[-2].objectP), (yyvsp[0].expnP), line); }
#line 1490 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 104 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_exit(line); }
#line 1496 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 106 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_return(line); }
#line 1502 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 108 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_result((yyvsp[0].expnP), line); }
#line 1508 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 110 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_put((yyvsp[0].argsP), line); }
#line 1514 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 112 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_get((yyvsp[0].argsP), line); }
#line 1520 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 114 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = (yyvsp[0].stmtP); }
#line 1526 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 116 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_scope((yyvsp[0].scopeP), line); }
#line 1532 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 118 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_for( (yyvsp[-8].expnP), (yyvsp[-5].expnP), (yyvsp[-3].expnP), (yyvsp[-2].number), (yyvsp[-1].objectP), line ); }
#line 1538 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 120 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_assert((yyvsp[0].expnP), line); }
#line 1544 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 124 "parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[0].number); }
#line 1550 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 126 "parser.y" /* yacc.c:1646  */
    { (yyval.number) = -(yyvsp[0].number); }
#line 1556 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 128 "parser.y" /* yacc.c:1646  */
    { (yyval.number) = 1; }
#line 1562 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 132 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_proc((yyvsp[0].token), NULL, line); }
#line 1568 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 134 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_proc((yyvsp[-3].token), (yyvsp[-1].argsP), line); }
#line 1574 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 138 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_if ((yyvsp[-5].expnP), (yyvsp[-3].objectP), (yyvsp[-1].objectP), line); }
#line 1580 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 140 "parser.y" /* yacc.c:1646  */
    { (yyval.stmtP) = mkStmt_if ((yyvsp[-3].expnP), (yyvsp[-1].objectP), NULL, line); }
#line 1586 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 144 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_scalar((yyvsp[0].declP), NULL); }
#line 1592 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 146 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_scalar((yyvsp[-2].declP), (yyvsp[0].expnP)); }
#line 1598 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 148 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_arr((yyvsp[-3].declP), (yyvsp[-1].number)); }
#line 1604 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 150 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_func ((yyvsp[-1].declP), (yyvsp[0].declP)); }
#line 1610 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 152 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_proc ((yyvsp[-1].token), (yyvsp[0].declP)); }
#line 1616 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 156 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_body((yyvsp[-3].fparmP), (yyvsp[0].scopeP), line); }
#line 1622 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 158 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_body((yyvsp[-3].fparmP), NULL, line); }
#line 1628 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 160 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_body(NULL, NULL, line); }
#line 1634 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 162 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_body(NULL, (yyvsp[0].scopeP), line); }
#line 1640 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 166 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_var_ident((yyvsp[-1].declP), (yyvsp[0].token)); }
#line 1646 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 170 "parser.y" /* yacc.c:1646  */
    { (yyval.fparmP) = mkFparm_list(NULL, (yyvsp[0].declP)); }
#line 1652 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 172 "parser.y" /* yacc.c:1646  */
    { (yyval.fparmP) = mkFparm_list((yyvsp[-2].fparmP), (yyvsp[0].declP)); }
#line 1658 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 176 "parser.y" /* yacc.c:1646  */
    { (yyval.argsP) = mkArg_list_expn(NULL, (yyvsp[0].expnP)); }
#line 1664 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 178 "parser.y" /* yacc.c:1646  */
    { (yyval.argsP) = mkArg_list_expn((yyvsp[-2].argsP), (yyvsp[0].expnP)); }
#line 1670 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 182 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_var_type(Dint, line); }
#line 1676 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 184 "parser.y" /* yacc.c:1646  */
    { (yyval.declP) = mkDecl_var_type(Dbool, line); }
#line 1682 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 188 "parser.y" /* yacc.c:1646  */
    { (yyval.argsP) = mkArg_expn((yyvsp[0].expnP)); }
#line 1688 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 190 "parser.y" /* yacc.c:1646  */
    { (yyval.argsP) = mkArg_str((yyvsp[0].token), line); }
#line 1694 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 192 "parser.y" /* yacc.c:1646  */
    { (yyval.argsP) = mkArg_skip(line); }
#line 1700 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 196 "parser.y" /* yacc.c:1646  */
    { (yyval.argsP) = (yyvsp[0].argsP); }
#line 1706 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 198 "parser.y" /* yacc.c:1646  */
    { (yyval.argsP) = mkArg_list_args((yyvsp[-2].argsP), (yyvsp[0].argsP)); }
#line 1712 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 202 "parser.y" /* yacc.c:1646  */
    { (yyval.argsP) = (yyvsp[0].argsP); }
#line 1718 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 204 "parser.y" /* yacc.c:1646  */
    { (yyval.argsP) = mkArg_list_args((yyvsp[-2].argsP), (yyvsp[0].argsP)); }
#line 1724 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 208 "parser.y" /* yacc.c:1646  */
    { (yyval.argsP) = mkArg_expn((yyvsp[0].expnP)); }
#line 1730 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 212 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = (yyvsp[0].expnP); }
#line 1736 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 214 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-2].expnP), Oor, (yyvsp[0].expnP), line); }
#line 1742 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 218 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = (yyvsp[0].expnP); }
#line 1748 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 220 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-2].expnP), Oand, (yyvsp[0].expnP), line); }
#line 1754 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 224 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = (yyvsp[0].expnP); }
#line 1760 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 226 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper(NULL, Onot, (yyvsp[0].expnP), line); }
#line 1766 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 230 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = (yyvsp[0].expnP); }
#line 1772 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 232 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-2].expnP), Oeq, (yyvsp[0].expnP), line); }
#line 1778 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 234 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-2].expnP), Oneq, (yyvsp[0].expnP), line); }
#line 1784 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 236 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-2].expnP), Ogrtr, (yyvsp[0].expnP), line); }
#line 1790 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 238 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-3].expnP), Ogrtreq, (yyvsp[0].expnP), line); }
#line 1796 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 240 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-2].expnP), Oless, (yyvsp[0].expnP), line); }
#line 1802 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 242 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-3].expnP), Olesseq, (yyvsp[0].expnP), line); }
#line 1808 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 246 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = (yyvsp[0].expnP); }
#line 1814 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 248 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-2].expnP), Oadd, (yyvsp[0].expnP), line); }
#line 1820 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 250 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-2].expnP), Osub, (yyvsp[0].expnP), line); }
#line 1826 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 254 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = (yyvsp[0].expnP); }
#line 1832 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 256 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-2].expnP), Omul, (yyvsp[0].expnP), line); }
#line 1838 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 258 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper((yyvsp[-2].expnP), Odiv, (yyvsp[0].expnP), line); }
#line 1844 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 262 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = (yyvsp[0].expnP); }
#line 1850 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 264 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_oper(NULL, Ouminus, (yyvsp[0].expnP), line); }
#line 1856 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 268 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = (yyvsp[-1].expnP); }
#line 1862 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 270 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_cond((yyvsp[-5].expnP), (yyvsp[-3].expnP), (yyvsp[-1].expnP), line); }
#line 1868 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 272 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = (yyvsp[0].expnP); }
#line 1874 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 274 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = (yyvsp[0].expnP); }
#line 1880 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 276 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_const((yyvsp[0].litP), line); }
#line 1886 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 280 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_func((yyvsp[-3].token), (yyvsp[-1].argsP), line); }
#line 1892 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 284 "parser.y" /* yacc.c:1646  */
    { (yyval.litP) = mkLit(Dint, (yyvsp[0].number)); }
#line 1898 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 286 "parser.y" /* yacc.c:1646  */
    { (yyval.litP) = mkLit_bool(Dbool, 1); }
#line 1904 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 288 "parser.y" /* yacc.c:1646  */
    { (yyval.litP) = mkLit_bool(Dbool, 0); }
#line 1910 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 292 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_var((yyvsp[0].token), line); }
#line 1916 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 294 "parser.y" /* yacc.c:1646  */
    { (yyval.expnP) = mkExpn_var_arr((yyvsp[-3].token), (yyvsp[-1].expnP), line); }
#line 1922 "parser.tab.c" /* yacc.c:1646  */
    break;


#line 1926 "parser.tab.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

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
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 298 "parser.y" /* yacc.c:1906  */


/************************************************************************/
/*     Functions to print error messages                                */
/************************************************************************/

void yyerror(char* message) {
        fprintf(errorFile,"Parse error (line %d) - %s\n", line , message );
        errorOccurred = TRUE;
}
