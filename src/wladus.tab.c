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
#line 5 "wladus.y" /* yacc.c:339  */

#include <stdio.h>
#include "uthash.h"
#include "utlist.h"
#include "utstring.h"
#include "utstack.h"

int yylex();
int yyerror(const char *s);
extern int yylex_destroy(void);

typedef struct simple_symbol_node {
  char *name;
  char dtype;
  char symbol_type;
  int scope_level;
  char *temp_name;
  struct simple_symbol_node *next;
} simple_symbol_node;

typedef struct scope {
  char *scope_name;
  int scope_level;
  struct scope *next;
} scope;

typedef struct symbol_node {
  char *name;                     // key field
  char type;                      // 'i'nt | 'f'loat | 's'tring | 'v'oid
  char symbol_type;               // 'V' (variable) | 'F' (function) | 'P' (parameter)
  int scope_level;                // 0 for global variables and functions, 1..n inside functions
  UT_hash_handle hh;              // makes this structure hashable
  struct {
    struct ast_node *func_body;   // function body
    simple_symbol_node *symbols;  // params and variables inside function scope
  } func_fields;
} symbol_node;

struct ast_node {
  int node_type;
  char dtype;
  char *addr;
  int string_size;
  struct ast_node *left;
  struct ast_node *right;
  union {
    char *string;
    char *func_name;
  };
};

typedef struct code_line {
  UT_string *code;
  struct code_line *prev;
  struct code_line *next;
} code_line;

typedef struct code_label {
  char *name;
  struct code_label *next;
} code_label;

typedef struct string_target {
  char *temp_name;
  struct string_target *next;
} string_target;


char * new_param();
char * new_temp();
void gen3(char * operation, char * rd, char * rs, char * rt);
void gen2(char * operation, char * rd, char * rs);
void gen1(char * operation, char * rd);
void gen0(char * operation);

void gen_table_symbol(char * type, char * name);

void gen_label(char * label);
char * new_label();

struct ast_node* add_ast_node(int node_type, struct ast_node *left, struct ast_node *right);

void add_symbol(char *name, char *type, char symbol_type, char * temp_name);
symbol_node* find_symbol(char *name);
simple_symbol_node* find_simple_symbol(char *name);
simple_symbol_node* create_simple_symbol_node(char *name, char dtype, char symbol_type, int scope_level, char * temp_name);
void create_internal_scope();
void remove_scope();

void error_not_declared(char *symbol_type, char *name);
void error_redeclaration(char *symbol_type, char *name);
void error_type_mismatch(char left_dtype, char right_dtype);

int mismatch(char left_dtype, char right_dtype);

char type_to_dtype(char *type);
char * dtype_to_type(char dtype);
int is_array(char * temp_name);
char * i_to_str(int integer);
char * array_string(char * array, char * pos);
char * reference(char * temp_name);
char * extract_pos(char * array_string);

struct symbol_node *symbol_table = NULL;
struct ast_node* syntax_tree = NULL;
struct scope* scope_stack = NULL;
struct string_target* string_target_stack = NULL;
struct code_label* label_stack = NULL;
code_line *tac_code = NULL;

int params_stacked = 0;
int params_generated = 0;
int temps_generated = 0;
int labels_generated = 0;
int has_return = 0;

extern int has_error;

#line 185 "wladus.tab.c" /* yacc.c:339  */

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
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "wladus.tab.h".  */
#ifndef YY_YY_WLADUS_TAB_H_INCLUDED
# define YY_YY_WLADUS_TAB_H_INCLUDED
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
    ID = 258,
    TYPE = 259,
    NUM = 260,
    DEC = 261,
    STR = 262,
    WHILE = 263,
    IF = 264,
    ELSE = 265,
    RETURN = 266,
    WRITE = 267,
    READ = 268,
    QUOTES = 269,
    ITP_START = 270,
    ITP_END = 271,
    EQ = 272,
    CEQ = 273,
    CNE = 274,
    CLT = 275,
    CLE = 276,
    CGT = 277,
    CGE = 278
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 124 "wladus.y" /* yacc.c:355  */

  char *id;
  char *type;
  char *op;

  char *str;

  struct ast_node *ast;

#line 259 "wladus.tab.c" /* yacc.c:355  */
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

#endif /* !YY_YY_WLADUS_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 290 "wladus.tab.c" /* yacc.c:358  */

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
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
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   105

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  70
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  121

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   278

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    34,     2,     2,
      29,    30,    24,    26,    31,    27,     2,    25,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    28,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    32,     2,    33,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   157,   157,   161,   162,   166,   167,   171,   181,   191,
     181,   220,   221,   222,   226,   244,   248,   249,   253,   265,
     266,   270,   271,   272,   273,   277,   278,   282,   292,   308,
     292,   322,   332,   345,   332,   366,   374,   385,   385,   402,
     406,   421,   430,   439,   448,   457,   466,   475,   479,   486,
     493,   500,   507,   514,   518,   519,   520,   525,   526,   527,
     527,   537,   551,   578,   591,   592,   596,   601,   609,   648,
     716
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "TYPE", "NUM", "DEC", "STR",
  "WHILE", "IF", "ELSE", "RETURN", "WRITE", "READ", "QUOTES", "ITP_START",
  "ITP_END", "EQ", "CEQ", "CNE", "CLT", "CLE", "CGT", "CGE", "'*'", "'/'",
  "'+'", "'-'", "';'", "'('", "')'", "','", "'{'", "'}'", "'%'", "$accept",
  "prog", "declarations", "declaration", "var_declaration",
  "func_declaration", "$@1", "$@2", "params", "param",
  "compound_statement", "local_declarations", "local_var_declaration",
  "statement_list", "statement", "expression_statement",
  "conditional_statement", "$@3", "$@4", "startIf", "iteration_statement",
  "$@5", "$@6", "return_statement", "expression", "$@7", "var",
  "simple_expression", "op_expression", "term", "$@8", "call", "args",
  "arg_list", "string", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,    42,    47,    43,    45,    59,    40,
      41,    44,   123,   125,    37
};
# endif

#define YYPACT_NINF -101

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-101)))

#define YYTABLE_NINF -29

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       2,     8,    26,     2,  -101,  -101,  -101,    -1,  -101,  -101,
    -101,     1,    24,    29,   -28,  -101,  -101,  -101,    24,     7,
    -101,  -101,  -101,    30,    33,  -101,     4,    15,    27,    31,
    -101,  -101,  -101,  -101,    35,    32,    36,  -101,    39,  -101,
    -101,  -101,  -101,    37,  -101,  -101,    28,    42,  -101,    56,
    -101,  -101,  -101,  -101,    39,    40,  -101,  -101,    34,    39,
      67,  -101,    41,    39,  -101,  -101,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39,  -101,    66,    68,
      39,  -101,    70,  -101,    71,    43,  -101,    72,    39,    -3,
      -3,    -3,    -3,    -3,    -3,  -101,  -101,  -101,  -101,  -101,
    -101,    39,    73,  -101,  -101,  -101,  -101,    39,     7,  -101,
    -101,  -101,    81,    62,     7,  -101,    88,  -101,  -101,     7,
    -101
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     4,     5,     6,     8,     1,     3,
       7,     0,    13,     0,     0,    12,    14,     9,     0,     0,
      11,    17,    10,    20,     0,    16,     0,     0,     0,    40,
      57,    58,    32,    31,     0,     0,     0,    59,     0,    15,
      19,    21,    22,     0,    23,    24,     0,    55,    39,    47,
      53,    56,    18,    26,    65,     0,    36,    55,     0,     0,
       0,    70,     0,     0,    25,    37,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,    64,
       0,    35,     0,    40,     0,     0,    54,     0,     0,    41,
      42,    43,    44,    45,    46,    50,    51,    48,    49,    52,
      61,     0,     0,    62,    63,    68,    60,     0,     0,    38,
      66,    33,     0,    27,     0,    69,     0,    34,    29,     0,
      30
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -101,  -101,  -101,   101,  -101,  -101,  -101,  -101,  -101,    87,
    -100,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,
    -101,  -101,  -101,  -101,  -101,  -101,   -25,   -34,    18,    19,
    -101,  -101,  -101,  -101,  -101
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,     4,     5,     6,    11,    19,    14,    15,
      22,    23,    25,    26,    40,    41,    42,   116,   119,    43,
      44,    55,   114,    45,    46,    88,    57,    48,    49,    50,
      61,    51,    78,    79,    85
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      58,    47,    17,    18,    62,    28,     1,    29,   113,    30,
      31,     7,    32,    33,   117,    34,    35,    36,    37,   120,
      77,    72,    73,    74,    75,    82,     8,    10,    13,    87,
      12,    76,    16,    38,    24,    84,    27,    39,    29,    21,
      30,    31,    29,    52,    30,    31,   102,    35,    36,    37,
     105,    35,    36,    37,   109,    53,    64,   106,   107,    65,
      54,    59,    81,    56,    38,    60,    63,   110,    38,    80,
      83,    86,   -28,   112,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    89,    90,    91,    92,    93,    94,
      76,    95,    96,    97,    98,    99,   100,   115,   118,   101,
     103,   104,   108,   111,     9,    20
};

static const yytype_uint8 yycheck[] =
{
      34,    26,    30,    31,    38,     1,     4,     3,   108,     5,
       6,     3,     8,     9,   114,    11,    12,    13,    14,   119,
      54,    24,    25,    26,    27,    59,     0,    28,     4,    63,
      29,    34,     3,    29,     4,    60,     3,    33,     3,    32,
       5,     6,     3,    28,     5,     6,    80,    12,    13,    14,
       7,    12,    13,    14,    88,    28,    28,    14,    15,    17,
      29,    29,    28,    28,    29,    29,    29,   101,    29,    29,
       3,    30,    10,   107,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    66,    67,    68,    69,    70,    71,
      34,    72,    73,    74,    75,    76,    30,    16,    10,    31,
      30,    30,    30,    30,     3,    18
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    36,    37,    38,    39,    40,     3,     0,    38,
      28,    41,    29,     4,    43,    44,     3,    30,    31,    42,
      44,    32,    45,    46,     4,    47,    48,     3,     1,     3,
       5,     6,     8,     9,    11,    12,    13,    14,    29,    33,
      49,    50,    51,    54,    55,    58,    59,    61,    62,    63,
      64,    66,    28,    28,    29,    56,    28,    61,    62,    29,
      29,    65,    62,    29,    28,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    34,    62,    67,    68,
      29,    28,    62,     3,    61,    69,    30,    62,    60,    63,
      63,    63,    63,    63,    63,    64,    64,    64,    64,    64,
      30,    31,    62,    30,    30,     7,    14,    15,    30,    62,
      62,    30,    62,    45,    57,    16,    52,    45,    10,    53,
      45
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    35,    36,    37,    37,    38,    38,    39,    41,    42,
      40,    43,    43,    43,    44,    45,    46,    46,    47,    48,
      48,    49,    49,    49,    49,    50,    50,    51,    52,    53,
      51,    54,    56,    57,    55,    58,    58,    60,    59,    59,
      61,    62,    62,    62,    62,    62,    62,    62,    63,    63,
      63,    63,    63,    63,    64,    64,    64,    64,    64,    65,
      64,    66,    66,    66,    67,    67,    68,    68,    69,    69,
      69
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     0,     0,
       8,     3,     1,     0,     2,     4,     2,     0,     3,     2,
       0,     1,     1,     1,     1,     2,     2,     5,     0,     0,
       9,     1,     0,     0,     7,     3,     2,     0,     4,     1,
       1,     3,     3,     3,     3,     3,     3,     1,     3,     3,
       3,     3,     3,     1,     3,     1,     1,     1,     1,     0,
       4,     4,     4,     4,     1,     0,     3,     1,     2,     4,
       0
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
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

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 157 "wladus.y" /* yacc.c:1646  */
    { syntax_tree = (yyvsp[0].ast); }
#line 1548 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 161 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = add_ast_node('A', (yyvsp[-1].ast), (yyvsp[0].ast)); }
#line 1554 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 162 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 1560 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 166 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 1566 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 167 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 1572 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 171 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = NULL;
                                                  add_symbol((yyvsp[-1].id), (yyvsp[-2].type), 'V', NULL);
                                                  gen_table_symbol((yyvsp[-2].type), (yyvsp[-1].id));
                                                  free((yyvsp[-2].type));
                                                  free((yyvsp[-1].id));
                                                }
#line 1584 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 181 "wladus.y" /* yacc.c:1646  */
    {
                                                  temps_generated = 0;
                                                  params_generated = 0;
                                                  add_symbol((yyvsp[0].id), (yyvsp[-1].type), 'F', NULL);
                                                  gen_label((yyvsp[0].id));
                                                  scope *new_scope = (scope *)malloc(sizeof *new_scope);
                                                  new_scope->scope_name = (char *) strdup((yyvsp[0].id));
                                                  new_scope->scope_level = 1;
                                                  STACK_PUSH(scope_stack, new_scope);
                                                }
#line 1599 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 191 "wladus.y" /* yacc.c:1646  */
    { ; }
#line 1605 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 192 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('F', NULL, (yyvsp[0].ast));
                                                  (yyval.ast)->dtype = type_to_dtype((yyvsp[-7].type));
                                                  (yyval.ast)->func_name = (char *) strdup((yyvsp[-6].id));

                                                  symbol_node *s = find_symbol((yyvsp[-6].id));
                                                  s->func_fields.func_body = (yyvsp[0].ast);

                                                  remove_scope();
                                                  scope_stack = NULL;
                                                  if(!has_return && strcmp((yyvsp[-6].id), "main") != 0){
                                                    if((yyval.ast)->dtype == 'i'){
                                                      gen1("return", "0");
                                                    }
                                                    else if((yyval.ast)->dtype == 'f'){
                                                      gen1("return", "0.0");
                                                    }
                                                    else {
                                                      gen0("return");
                                                    }
                                                  }

                                                  free((yyvsp[-7].type));
                                                  free((yyvsp[-6].id));
                                                }
#line 1635 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 220 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast)); }
#line 1641 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 221 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 1647 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 222 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = NULL; }
#line 1653 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 226 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('P', NULL, NULL);
                                                  (yyval.ast)->dtype = type_to_dtype((yyvsp[-1].type));
                                                  scope *top = STACK_TOP(scope_stack);
                                                  if(strcmp(top->scope_name, "main") == 0){
                                                    (yyval.ast)->addr = new_temp();
                                                    if ((yyval.ast)->dtype == 'i'){ gen2("mov", (yyval.ast)->addr, "0"); }
                                                    else if ((yyval.ast)->dtype == 'f'){ gen2("mov", (yyval.ast)->addr, "0.0"); }
                                                  }
                                                  else {
                                                    (yyval.ast)->addr = new_param();
                                                  }
                                                  add_symbol((yyvsp[0].id), (yyvsp[-1].type), 'P', (yyval.ast)->addr);
                                                  free((yyvsp[-1].type));
                                                  free((yyvsp[0].id));
                                                }
#line 1674 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 244 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[-1].ast)); }
#line 1680 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 248 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = add_ast_node('A', (yyvsp[-1].ast), (yyvsp[0].ast)); }
#line 1686 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 249 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = NULL; }
#line 1692 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 253 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('V', NULL, NULL);
                                                  (yyval.ast)->dtype = type_to_dtype((yyvsp[-2].type));
                                                  (yyval.ast)->addr = new_temp();
                                                  if ((yyval.ast)->dtype == 'i'){ gen2("mov", (yyval.ast)->addr, "0"); }
                                                  else if ((yyval.ast)->dtype == 'f'){ gen2("mov", (yyval.ast)->addr, "0.0"); }
                                                  add_symbol((yyvsp[-1].id), (yyvsp[-2].type), 'V', (yyval.ast)->addr);
                                                  free((yyvsp[-2].type));
                                                  free((yyvsp[-1].id));
                                                }
#line 1707 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 265 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = add_ast_node('A', (yyvsp[-1].ast), (yyvsp[0].ast)); }
#line 1713 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 266 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = NULL; }
#line 1719 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 270 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 1725 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 271 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 1731 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 272 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 1737 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 273 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 1743 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 277 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[-1].ast); }
#line 1749 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 278 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = NULL; }
#line 1755 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 282 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('C', add_ast_node('c', (yyvsp[-2].ast), (yyvsp[0].ast)), NULL);
                                                  remove_scope();

                                                  code_label *after_if_label;
                                                  STACK_POP(label_stack, after_if_label);
                                                  gen_label(after_if_label->name);
                                                  free(after_if_label->name);
                                                  free(after_if_label);
                                                }
#line 1770 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 292 "wladus.y" /* yacc.c:1646  */
    {
                                                  remove_scope();

                                                  char * after_else_label = new_label();
                                                  gen1("jump", after_else_label);

                                                  code_label *after_if_label;
                                                  STACK_POP(label_stack, after_if_label);
                                                  gen_label(after_if_label->name);
                                                  free(after_if_label->name);
                                                  free(after_if_label);

                                                  code_label *new_after_else_label = (code_label *)malloc(sizeof *new_after_else_label);
                                                  new_after_else_label->name = after_else_label;
                                                  STACK_PUSH(label_stack, new_after_else_label);
                                                }
#line 1791 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 308 "wladus.y" /* yacc.c:1646  */
    { create_internal_scope(); }
#line 1797 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 309 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('C', add_ast_node('c', (yyvsp[-6].ast), (yyvsp[-4].ast)), (yyvsp[0].ast));
                                                  remove_scope();

                                                  code_label *after_else_label;
                                                  STACK_POP(label_stack, after_else_label);
                                                  gen_label(after_else_label->name);
                                                  free(after_else_label->name);
                                                  free(after_else_label);
                                                }
#line 1812 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 322 "wladus.y" /* yacc.c:1646  */
    {
                                                  create_internal_scope();
                                                  char * after_if_label = new_label();
                                                  code_label *new_after_if_label = (code_label *)malloc(sizeof *new_after_if_label);
                                                  new_after_if_label->name = after_if_label;
                                                  STACK_PUSH(label_stack, new_after_if_label);
                                                }
#line 1824 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 332 "wladus.y" /* yacc.c:1646  */
    {
                                                  create_internal_scope();
                                                  char * while_label = new_label();
                                                  gen_label(while_label);
                                                  code_label *new_while_label = (code_label *)malloc(sizeof *new_while_label);
                                                  new_while_label->name = while_label;
                                                  STACK_PUSH(label_stack, new_while_label);

                                                  char * after_while_label = new_label();
                                                  code_label *new_after_while_label = (code_label *)malloc(sizeof *new_after_while_label);
                                                  new_after_while_label->name = after_while_label;
                                                  STACK_PUSH(label_stack, new_after_while_label);
                                                }
#line 1842 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 345 "wladus.y" /* yacc.c:1646  */
    {;}
#line 1848 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 346 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('W', (yyvsp[-3].ast), (yyvsp[0].ast));
                                                  remove_scope();

                                                  code_label *after_while_label;
                                                  STACK_POP(label_stack, after_while_label);
                                                  code_label *while_label;
                                                  STACK_POP(label_stack, while_label);

                                                  gen1("jump", while_label->name);
                                                  gen_label(after_while_label->name);

                                                  free(after_while_label->name);
                                                  free(after_while_label);
                                                  free(while_label->name);
                                                  free(while_label);
                                                }
#line 1870 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 366 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = (yyvsp[-1].ast);
                                                  scope *top = STACK_TOP(scope_stack);
                                                  if(strcmp(top->scope_name, "main") != 0){
                                                    has_return = 1;
                                                    gen1("return", (yyvsp[-1].ast)->addr);
                                                  }
                                                }
#line 1883 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 374 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = NULL;
                                                  scope *top = STACK_TOP(scope_stack);
                                                  if(strcmp(top->scope_name, "main") != 0){
                                                    has_return = 1;
                                                    gen0("return");
                                                  }
                                                }
#line 1896 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 385 "wladus.y" /* yacc.c:1646  */
    {
                                                  if((yyvsp[-1].ast)->dtype == 's'){
                                                    string_target *new_string_target = (string_target *)malloc(sizeof *new_string_target);
                                                    new_string_target->temp_name = (yyvsp[-1].ast)->addr;
                                                    STACK_PUSH(string_target_stack, new_string_target);
                                                  }
                                                }
#line 1908 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 392 "wladus.y" /* yacc.c:1646  */
    {
                                                  if(mismatch((yyvsp[-3].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-3].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else {
                                                    (yyval.ast) = add_ast_node('A', (yyvsp[-3].ast), (yyvsp[0].ast));
                                                    (yyval.ast)->dtype = (yyvsp[-3].ast)->dtype;
                                                    if((yyval.ast)->dtype != 's'){
                                                      gen2("mov", (yyvsp[-3].ast)->addr, (yyvsp[0].ast)->addr);
                                                    }
                                                  }
                                                }
#line 1923 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 402 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 1929 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 406 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('V', NULL, NULL);
                                                  symbol_node *s = find_symbol((yyvsp[0].id));
                                                  if(s == NULL){ error_not_declared("variable", (yyvsp[0].id)); }
                                                  else {
                                                    (yyval.ast)->dtype = s->type;
                                                    HASH_FIND_STR(symbol_table, (yyvsp[0].id), s);
                                                    if(s != NULL && s->symbol_type != 'F'){ (yyval.ast)->addr = (char *) strdup((yyvsp[0].id)); } // global variable
                                                    else { (yyval.ast)->addr = find_simple_symbol((yyvsp[0].id))->temp_name; }
                                                  }
                                                  free((yyvsp[0].id));
                                                }
#line 1946 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 421 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("seq", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                  gen2("brz", top_label->name, (yyval.ast)->addr);
                                                }
#line 1960 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 430 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("seq", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                  gen2("brnz", top_label->name, (yyval.ast)->addr);
                                                }
#line 1974 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 439 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("slt", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                  gen2("brz", top_label->name, (yyval.ast)->addr);
                                                }
#line 1988 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 448 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("sleq", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                  gen2("brz", top_label->name, (yyval.ast)->addr);
                                                }
#line 2002 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 457 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("sleq", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                  gen2("brnz", top_label->name, (yyval.ast)->addr);
                                                }
#line 2016 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 466 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("slt", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                  gen2("brnz", top_label->name, (yyval.ast)->addr);
                                                }
#line 2030 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 475 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 2036 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 479 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  gen3("add", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                }
#line 2048 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 486 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  gen3("sub", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                }
#line 2060 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 493 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  gen3("mul", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                }
#line 2072 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 500 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  gen3("div", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                }
#line 2084 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 507 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  if(mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype)){ error_type_mismatch((yyvsp[-2].ast)->dtype, (yyvsp[0].ast)->dtype); }
                                                  else { (yyval.ast)->dtype = (yyvsp[-2].ast)->dtype; }
                                                  (yyval.ast)->addr = new_temp();
                                                  gen3("mod", (yyval.ast)->addr, (yyvsp[-2].ast)->addr, (yyvsp[0].ast)->addr);
                                                }
#line 2096 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 514 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 2102 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 518 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[-1].ast); }
#line 2108 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 519 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 2114 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 520 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = (yyvsp[0].ast);
                                                  symbol_node *s = find_symbol((yyvsp[0].ast)->func_name);
                                                  if(s != NULL){ (yyval.ast)->dtype = s->type; }
                                                }
#line 2124 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 525 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = add_ast_node('I', NULL, NULL); (yyval.ast)->addr = (yyvsp[0].str); (yyval.ast)->dtype = 'i'; }
#line 2130 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 526 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = add_ast_node('D', NULL, NULL); (yyval.ast)->addr = (yyvsp[0].str); (yyval.ast)->dtype = 'f'; }
#line 2136 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 527 "wladus.y" /* yacc.c:1646  */
    {
                                                  
                                                }
#line 2144 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 530 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = (yyvsp[-1].ast);
                                                  (yyval.ast)->dtype = 's';
                                                }
#line 2153 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 537 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('L', NULL, (yyvsp[-1].ast));
                                                  (yyval.ast)->func_name = (char *) strdup((yyvsp[-3].id));
                                                  symbol_node *s = find_symbol((yyvsp[-3].id));
                                                  if(s == NULL) error_not_declared("function", (yyvsp[-3].id));
                                                  if(params_stacked) gen2("call", (yyvsp[-3].id), i_to_str(params_stacked));
                                                  else gen1("call", (yyvsp[-3].id));
                                                  params_stacked = 0;
                                                  (yyval.ast)->addr = new_temp();
                                                  if(s->type != 'v'){
                                                    gen1("pop", (yyval.ast)->addr);
                                                  }
                                                  free((yyvsp[-3].id));
                                                }
#line 2172 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 551 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('L', NULL, (yyvsp[-1].ast));
                                                  (yyval.ast)->func_name = (char *) strdup("write");
                                                  if((yyvsp[-1].ast)->dtype == 's'){
                                                    char *pos = new_temp();
                                                    gen2("mov", pos, "0");

                                                    char * while_label = new_label();
                                                    gen_label(while_label);
                                                    char * after_while_label = new_label();

                                                    char *value = new_temp();
                                                    gen2("mov", value, array_string((yyvsp[-1].ast)->addr, pos));
                                                    char *aux = new_temp();
                                                    gen3("seq", aux, value, "0");
                                                    gen2("brnz", after_while_label, aux);
                                                    gen2("inttoch", value, value);
                                                    gen1("print", value);
                                                    gen3("add", pos, pos, "1");
                                                    gen1("jump", while_label);
                                                    gen_label(after_while_label);
                                                    gen1("println", "");
                                                  }
                                                  else {
                                                    gen1("println", (yyvsp[-1].ast)->addr);
                                                  }
                                                }
#line 2204 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 578 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('L', NULL, (yyvsp[-1].ast));
                                                  (yyval.ast)->func_name = (char *) strdup("read");
                                                  if((yyvsp[-1].ast)->dtype == 'i'){
                                                    gen1("scani", (yyvsp[-1].ast)->addr);
                                                  }
                                                  else if((yyvsp[-1].ast)->dtype == 'f') {
                                                    gen1("scanf", (yyvsp[-1].ast)->addr);
                                                  }
                                                }
#line 2219 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 591 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = (yyvsp[0].ast); }
#line 2225 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 592 "wladus.y" /* yacc.c:1646  */
    { (yyval.ast) = NULL; }
#line 2231 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 596 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('A', (yyvsp[-2].ast), (yyvsp[0].ast));
                                                  params_stacked++;
                                                  gen1("param", (yyvsp[0].ast)->addr);
                                                }
#line 2241 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 601 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = (yyvsp[0].ast);
                                                  params_stacked++;
                                                  gen1("param", (yyvsp[0].ast)->addr);
                                                }
#line 2251 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 609 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('S', NULL, (yyvsp[-1].ast));
                                                  (yyval.ast)->string = (char *) strdup((yyvsp[0].str));

                                                  int str_len = strlen((yyval.ast)->string);
                                                  (yyval.ast)->addr = new_temp();
                                                  (yyvsp[-1].ast)->addr = (yyval.ast)->addr;
                                                  (yyvsp[-1].ast)->string_size = str_len;
                                                  int i = 0;
                                                  gen2("mema", (yyval.ast)->addr, i_to_str(str_len + 1));
                                                  for (; i < str_len; ++i){
                                                    gen2("mov", array_string((yyval.ast)->addr, i_to_str(i)), i_to_str((yyval.ast)->string[i]));
                                                  }
                                                  char * string_end = array_string((yyval.ast)->addr, i_to_str(i));
                                                  gen2("mov", string_end, "0");

                                                  string_target *s;
                                                  STACK_POP(string_target_stack, s);
                                                  if(is_array(s->temp_name)){
                                                    char * aux = new_temp();
                                                    for (i = 0; i < str_len; ++i){
                                                      gen2("mov", aux, array_string((yyval.ast)->addr, i_to_str(i)));
                                                      gen2("mov", s->temp_name, aux);
                                                      gen3("add", extract_pos(s->temp_name), extract_pos(s->temp_name), "1");
                                                    }
                                                    gen2("mov", aux, array_string((yyval.ast)->addr, i_to_str(i)));
                                                    gen2("mov", s->temp_name, aux);
                                                    gen3("add", extract_pos(s->temp_name), extract_pos(s->temp_name), "1");
                                                  }
                                                  else {
                                                    gen2("mov", s->temp_name, (yyval.ast)->addr);
                                                  }

                                                  string_target *new_string_target = (string_target *)malloc(sizeof *new_string_target);
                                                  new_string_target->temp_name = string_end;
                                                  STACK_PUSH(string_target_stack, new_string_target);

                                                  free((yyvsp[0].str));
                                                }
#line 2295 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 648 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('T', (yyvsp[-3].ast), (yyvsp[-1].ast));
                                                  (yyval.ast)->string = (char *) strdup("interpolated string");
                                                  (yyval.ast)->addr = new_temp();
                                                  gen2("mov", (yyval.ast)->addr, (yyvsp[-1].ast)->addr);
                                                  char * string_pointer = new_temp();
                                                  gen2("mema", string_pointer, "20");

                                                  char * pos = new_temp();
                                                  gen2("mov", pos, "0");

                                                  char *num = new_temp();
                                                  gen2("mov", num, (yyval.ast)->addr);

                                                  char *div = new_temp();
                                                  gen2("mov", div, "10");

                                                  char * find_char_label = new_label();
                                                  gen_label(find_char_label);

                                                  char * store_char_label = new_label();
                                                  char * div_label = new_label();
                                                  char *temp = new_temp();
                                                  gen3("slt", temp, num, "10");
                                                  gen2("brz", div_label, temp);
                                                  
                                                  char *aux = new_temp();
                                                  gen2("mov", aux, num);
                                                  gen1("jump", store_char_label);

                                                  gen_label(div_label);
                                                  gen3("div", aux, num, div);
                                                  gen3("slt", temp, aux, "10");
                                                  gen2("brnz", store_char_label, temp);
                                                  gen3("mul", div, div, "10");
                                                  gen1("jump", find_char_label);

                                                  gen_label(store_char_label);
                                                  char *int_to_char_aux = new_temp();
                                                  gen3("add", int_to_char_aux, aux, "48");
                                                  gen2("mov", array_string(string_pointer, pos), int_to_char_aux);
                                                  gen3("add", pos, pos, "1");

                                                  char *after_string_build_label = new_label();
                                                  gen3("slt", temp, num, "10");
                                                  gen2("brnz", after_string_build_label, temp);

                                                  gen3("mod", num, num, div);
                                                  gen2("mov", div, "10");
                                                  gen1("jump", find_char_label);

                                                  gen_label(after_string_build_label);
                                                  char * string_end = array_string(string_pointer, pos);
                                                  gen2("mov", string_end, "0");

                                                  string_target *s;
                                                  STACK_POP(string_target_stack, s);
                                                  if(is_array(s->temp_name)){
                                                    gen2("mov", s->temp_name, reference(string_pointer));
                                                  }
                                                  else {
                                                    gen2("mov", s->temp_name, (yyval.ast)->addr);
                                                  }

                                                  string_target *new_string_target = (string_target *)malloc(sizeof *new_string_target);
                                                  new_string_target->temp_name = string_end;
                                                  STACK_PUSH(string_target_stack, new_string_target);
                                                }
#line 2368 "wladus.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 716 "wladus.y" /* yacc.c:1646  */
    {
                                                  (yyval.ast) = add_ast_node('S', NULL, NULL);
                                                  (yyval.ast)->string = (char *) strdup("");
                                                }
#line 2377 "wladus.tab.c" /* yacc.c:1646  */
    break;


#line 2381 "wladus.tab.c" /* yacc.c:1646  */
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
  *++yylsp = yyloc;

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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
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
#line 722 "wladus.y" /* yacc.c:1906  */


char * new_param(){
  UT_string *tmp;
  utstring_new(tmp);
  utstring_printf(tmp, "#%d", params_generated);
  params_generated++;
  return utstring_body(tmp);
}

char * new_temp(){
  UT_string *tmp;
  utstring_new(tmp);
  utstring_printf(tmp, "$%d", temps_generated);
  temps_generated++;
  return utstring_body(tmp);
}

void gen3(char * operation, char * rd, char * rs, char * rt){
  code_line *new_line = (code_line *)malloc(sizeof *new_line);
  utstring_new(new_line->code);
  utstring_printf(new_line->code, "%s %s, %s, %s\n", operation, rd, rs, rt);
  DL_APPEND(tac_code, new_line);
}

void gen2(char * operation, char * rd, char * rs){
  code_line *new_line = (code_line *)malloc(sizeof *new_line);
  utstring_new(new_line->code);
  utstring_printf(new_line->code, "%s %s, %s\n", operation, rd, rs);
  DL_APPEND(tac_code, new_line);
}

void gen1(char * operation, char * rd){
  code_line *new_line = (code_line *)malloc(sizeof *new_line);
  utstring_new(new_line->code);
  utstring_printf(new_line->code, "%s %s\n", operation, rd);
  DL_APPEND(tac_code, new_line);
}

void gen0(char * operation){
  code_line *new_line = (code_line *)malloc(sizeof *new_line);
  utstring_new(new_line->code);
  utstring_printf(new_line->code, "%s\n", operation);
  DL_APPEND(tac_code, new_line);
}

void gen_table_symbol(char * type, char * name){
  code_line *new_line = (code_line *)malloc(sizeof *new_line);
  utstring_new(new_line->code);
  utstring_printf(new_line->code, "%s %s\n", type, name);
  DL_PREPEND(tac_code, new_line);
}

char * new_label(){
  UT_string *tmp;
  utstring_new(tmp);
  utstring_printf(tmp, "L%d", labels_generated);
  labels_generated++;
  return utstring_body(tmp);
}

void gen_label(char * label){
  code_line *new_line = (code_line *)malloc(sizeof *new_line);
  utstring_new(new_line->code);
  utstring_printf(new_line->code, "%s:\n", label);
  DL_APPEND(tac_code, new_line);
}

struct ast_node* add_ast_node(int node_type, struct ast_node *left, struct ast_node *right){
  struct ast_node* ast_node = (struct ast_node*)calloc(1, sizeof(struct ast_node));

  ast_node->node_type = node_type;
  ast_node->left = left;
  ast_node->right = right;
  ast_node->dtype = '0'; // empty dtype

  return ast_node;
}

void print_ast_node(struct ast_node *s, int depth) {
  if(s == NULL) return;

  if(s->node_type == 'A'){
    print_ast_node(s->left, depth);
    print_ast_node(s->right, depth);
    return;
  }

  printf("%*s", depth, "");

  switch (s->node_type){
    case 'T':
      print_ast_node(s->left, depth + 1);
      print_ast_node(s->right, depth + 1);
      break;
    case 'F':
      {
        printf("%s \t\t type = %s\n", s->func_name, dtype_to_type(s->dtype));

        print_ast_node(s->right, depth+1);
      }
      break;
    case 'c':
      {
        printf("if\n");

        printf("%*s", depth + 1, "");
        printf("-- condition --\n");
        print_ast_node(s->left, depth + 2);

        printf("%*s", depth + 1, "");
        printf("-- body --\n");
        print_ast_node(s->right, depth + 2);
      }
      break;
    case 'C':
      {
        print_ast_node(s->left, depth);

        if(s->right) {
          printf("%*s", depth + 1, "");
          printf("-- else --\n");
          print_ast_node(s->right, depth + 2);
        }
      }
      break;
    
    case 'W':
      {
        printf("while\n");

        printf("%*s", depth + 1, "");
        printf("-- condition --\n");
        print_ast_node(s->left, depth + 2);

        printf("%*s", depth + 1, "");
        printf("-- body --\n");
        print_ast_node(s->right, depth + 2);
      }
      break;
    case 'L':
      {
        printf(" (%s)\n", s->func_name);
        print_ast_node(s->left, depth + 1);
        print_ast_node(s->right, depth + 1);
      }
      break;
    case 'I':
      {
        printf(" (%s) \t\t type = %s\n", s->addr, dtype_to_type(s->dtype));
      }
      break;
    case 'D':
      {
        printf(" (%s) \t\t type = %s\n", s->addr, dtype_to_type(s->dtype));
      }
      break;
    case 'S':
      {
        printf(" (%s)\n", s->string);
        print_ast_node(s->left, depth + 1);
        print_ast_node(s->right, depth + 1);
      }
      break;
    case 'V':
      {
        printf(" (ID) \t\t type = %s\n", dtype_to_type(s->dtype));
      }
      break;
    case 'P':
      {
        printf(" (param) \t\t type = %s\n", dtype_to_type(s->dtype));
      }
      break;
  }  
}

void print_syntax_tree() {
  struct ast_node *s = syntax_tree;

  printf("======  SYNTAX TREE ======\n");
  print_ast_node(s, 0);
  printf("\n");
}

void free_syntax_tree(struct ast_node *s){
  if(s == NULL) return;

  switch (s->node_type){
    case 'A':
    case 'C':
    case 'c':
    case 'W':
    case 'V':
    case 'P':
      if(s->left) free_syntax_tree(s->left);
      if(s->right) free_syntax_tree(s->right);
      free(s);
      break;
    case 'F':
      {
        free(s->func_name);
        free_syntax_tree(s->right);
        free(s);
      }
      break;
    case 'L':
      {
        free(s->func_name);
        if(s->left) free_syntax_tree(s->left);
        if(s->right) free_syntax_tree(s->right);
        free(s);
      }
      break;
    case 'I':
      {
        free(s);
      }
      break;
    case 'D':
      {
        free(s);
      }
      break;
    case 'S':
      {
        free(s->string);
        if(s->left) free_syntax_tree(s->left);
        if(s->right) free_syntax_tree(s->right);
        free(s);
      }
      break;
    case 'T':
      {
        free(s->string);
        if(s->left) free_syntax_tree(s->left);
        if(s->right) free_syntax_tree(s->right);
        free(s);
      }
      break;
  }
}

char type_to_dtype(char *type){
  if(strcmp(type, "int") == 0) { return 'i'; }
  else if(strcmp(type, "float") == 0) { return 'f'; }
  else if(strcmp(type, "string") == 0) { return 's'; }
  else if(strcmp(type, "void") == 0) { return 'v'; }
}

char * dtype_to_type(char dtype){
  if(dtype == 'i') { return "int"; }
  else if(dtype == 'f') { return "float"; }
  else if(dtype == 's') { return "string"; }
  else if(dtype == 'v') { return "void"; }
}

int is_array(char * temp_name){
  UT_string * aux;
  utstring_new(aux);
  utstring_printf(aux, "%s", temp_name);
  int found = utstring_find(aux, 0, "[", 1);
  return found != -1 ? 1 : 0;
}

char * i_to_str(int integer){
  UT_string * aux;
  utstring_new(aux);
  utstring_printf(aux, "%d", integer);
  return utstring_body(aux);
}

char * array_string(char * array, char * pos){
  UT_string * aux;
  utstring_new(aux);
  utstring_printf(aux, "%s[%s]", array, pos);
  return utstring_body(aux);
}

char * reference(char * temp_name){
  UT_string * aux;
  utstring_new(aux);
  utstring_printf(aux, "*%s", temp_name);
  return utstring_body(aux);
}

char * extract_pos(char * array_string){
  int i = 0;
  UT_string * aux;
  utstring_new(aux);
  utstring_printf(aux, "%s", array_string);
  int start = utstring_find(aux, 0, "[", 1);
  int end = utstring_find(aux, 0, "]", 1);
  int size = end - start;
  char pos[size];
  for (; i < size - 1; ++i){
    pos[i] = utstring_body(aux)[start + i + 1];
  }
  pos[i] = '\0';
  return strdup(pos);
}

void create_internal_scope(){
  scope *new_scope = (scope *)malloc(sizeof *new_scope);
  scope *top = STACK_TOP(scope_stack);
  new_scope->scope_name = (char *) strdup(top->scope_name);
  new_scope->scope_level = top->scope_level + 1;
  STACK_PUSH(scope_stack, new_scope);
}

void remove_scope(){
  scope *old_scope;
  STACK_POP(scope_stack, old_scope);
  free(old_scope->scope_name);
  free(old_scope);
}

symbol_node* build_symbol(char *name, char *type, char symbol_type, int scope_level){
  symbol_node *s = (symbol_node *)malloc(sizeof *s);

  s->name = (char *) strdup(name);
  s->type = type_to_dtype(type);
  s->symbol_type = symbol_type;
  s->scope_level = scope_level;
  if(symbol_type == 'F'){
    s->func_fields.symbols = NULL;
  }

  return s;
}

symbol_node* find_symbol(char *name){
  symbol_node *s;

  HASH_FIND_STR(symbol_table, name, s);
  if(s != NULL) return s;

  scope * top = STACK_TOP(scope_stack);
  HASH_FIND_STR(symbol_table, top->scope_name, s);

  simple_symbol_node *tmp;
  for (tmp = s->func_fields.symbols; tmp != NULL && (strcmp(tmp->name, name) != 0); tmp = tmp->next);

  return (symbol_node *) tmp;
}

simple_symbol_node* find_simple_symbol(char *name){
  symbol_node *s;

  scope * top = STACK_TOP(scope_stack);
  HASH_FIND_STR(symbol_table, top->scope_name, s);

  simple_symbol_node *tmp;
  for (tmp = s->func_fields.symbols; tmp != NULL && (strcmp(tmp->name, name) != 0); tmp = tmp->next);

  return tmp;
}

void add_symbol(char *name, char *type, char symbol_type, char * temp_name){
  symbol_node *s;
  scope * top;

  if(symbol_type == 'F') {
    HASH_FIND_STR(symbol_table, name, s);
    if(s == NULL){ // function not declared -> add to symbol table
      s = build_symbol(name, type, symbol_type, 0);
      HASH_ADD_STR(symbol_table, name, s);
    }
    else { // function already declared -> error
      error_redeclaration("function", name);
      return;
    }
  }
  else {
    if(STACK_TOP(scope_stack) == NULL){ // is not inside scope
      HASH_FIND_STR(symbol_table, name, s);
      if(s == NULL){ // global variable not declared -> add to symbol table
        s = build_symbol(name, type, symbol_type, 0);
        HASH_ADD_STR(symbol_table, name, s);
      }
      else { // global variable already declared -> error
        error_redeclaration("variable", name);
        return;
      }
    }
    else { // is inside scope
      HASH_FIND_STR(symbol_table, name, s);
      top = STACK_TOP(scope_stack);
      if(s != NULL && top->scope_level == s->scope_level){ // local variable is declared as global variable -> error
        error_redeclaration("variable", name);
        return;
      }
      HASH_FIND_STR(symbol_table, top->scope_name, s);

      simple_symbol_node *tmp, *new_node;

      char dtype = type_to_dtype(type);
      new_node = create_simple_symbol_node(name, dtype, symbol_type, top->scope_level, temp_name);

      if(s->func_fields.symbols == NULL){
        s->func_fields.symbols = new_node;
        return;
      }

      for (tmp = s->func_fields.symbols; tmp->next != NULL; tmp = tmp->next){
        if(strcmp(tmp->name, name) == 0 && tmp->scope_level == top->scope_level){ // local variable is already declared in function -> error
          error_redeclaration("variable", name);
          free(new_node->name);
          free(new_node);
          return;
        }
      }
      if(strcmp(tmp->name, name) == 0 && tmp->scope_level == top->scope_level){ // local variable is already declared in function -> error
        error_redeclaration("variable", name);
        free(new_node->name);
        free(new_node);
        return;
      }
      tmp->next = new_node;
    }
  }
}

simple_symbol_node* create_simple_symbol_node(char *name, char dtype, char symbol_type, int scope_level, char * temp_name){
  simple_symbol_node *new_node = (simple_symbol_node *)malloc(sizeof *new_node);
  new_node->name = (char *) strdup(name);
  new_node->dtype = dtype;
  new_node->symbol_type = symbol_type;
  new_node->scope_level = scope_level;
  new_node->temp_name = (char *) strdup(temp_name);
  new_node->next = NULL;
  return new_node;
}

int mismatch(char left_dtype, char right_dtype){
  return left_dtype != '0' && right_dtype != '0' && left_dtype != right_dtype;
}

void error_not_declared(char *symbol_type, char *name){
  char * error_message = (char *)malloc(50 * sizeof(char));
  sprintf(error_message, "semantic error, %s '%s' was not declared", symbol_type, name);
  yyerror(error_message);
  free(error_message);
}

void error_redeclaration(char *symbol_type, char *name){
  char * error_message = (char *)malloc(50 * sizeof(char));
  sprintf(error_message, "semantic error, %s '%s' was already declared", symbol_type, name);
  yyerror(error_message);
  free(error_message);
}

void error_type_mismatch(char left_dtype, char right_dtype){
  char * error_message = (char *)malloc(50 * sizeof(char));
  char * left = (char *)malloc(7 * sizeof(char));
  char * right = (char *)malloc(7 * sizeof(char));
  strcpy(left, dtype_to_type(left_dtype));
  strcpy(right, dtype_to_type(right_dtype));
  sprintf(error_message, "semantic error, ‘%s‘ type mismatch with ‘%s‘", left, right);
  yyerror(error_message);
  free(error_message);
}

void print_symbol_table() {
  symbol_node *s;

  printf("===============  SYMBOL TABLE ===============\n");
  printf("NAME\t\tTYPE\t\tSYMBOL_TYPE\t\tSCOPE SYMBOLS\n");
  for(s=symbol_table; s != NULL; s=s->hh.next) {
    printf("%s\t\t%s\t\t%c", s->name, dtype_to_type(s->type), s->symbol_type);
    if(s->symbol_type == 'F'){
      simple_symbol_node *ss;
      printf("\t\t\t");
      for (ss = s->func_fields.symbols; ss != NULL; ss = ss->next){
        printf("(%c) %s %s", ss->symbol_type, dtype_to_type(ss->dtype), ss->name);
        if (ss->scope_level > 1) printf(" [%d]", ss->scope_level);
        printf(", ");
      }
    }
    printf("\n");
  }
}

void free_simple_symbol_node(simple_symbol_node * node){
  if(node == NULL) return;
  free_simple_symbol_node(node->next);
  free(node->name);
  free(node);
}

void free_symbol_table(){
  symbol_node *s, *tmp;

  HASH_ITER(hh, symbol_table, s, tmp) {
    HASH_DEL(symbol_table, s);
    free(s->name);
    s->func_fields.func_body = NULL;
    free_simple_symbol_node(s->func_fields.symbols);
    free(s);
  }
}

int main (int argc, char **argv){
  code_line *line;
  int print_table = 0;
  int print_tree = 0;

  if(argc > 1 && !strcmp(argv[1], "-t")){
    print_table = 1;
  }

  if(argc > 1 && !strcmp(argv[1], "-tt")){
    print_table = 1;
    print_tree = 1;
  }

  code_line *code_section = (code_line *)malloc(sizeof *code_section);
  utstring_new(code_section->code);
  utstring_printf(code_section->code, ".code\n");
  DL_APPEND(tac_code, code_section);
  yyparse();
  yylex_destroy();

  if(!has_error && print_table) print_symbol_table();
  if(!has_error && print_tree) print_syntax_tree();
  free_symbol_table();
  free_syntax_tree(syntax_tree);

  code_line *table_section = (code_line *)malloc(sizeof *table_section);
  utstring_new(table_section->code);
  utstring_printf(table_section->code, ".table\n");
  DL_PREPEND(tac_code, table_section);

  code_line *eof_line = (code_line *)malloc(sizeof *eof_line);
  utstring_new(eof_line->code);
  utstring_printf(eof_line->code, "nop\n");
  DL_APPEND(tac_code, eof_line);

  DL_FOREACH(tac_code, line) printf("%s", utstring_body(line->code));

  return 0;
}
