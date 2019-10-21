%error-verbose
%debug
%locations

%{
#include <stdio.h>
#include "uthash.h"


int yylex();
int yyerror(const char *s);
struct ast_node* add_syntax_node(char *data);
void add_symbol(char *name, char *type, char *object_type);

struct ast_node {
  char *data;
  struct ast_node *left;
  struct ast_node *right;
};

struct symbol_node {
  char *name;                 // key field
  char *type;
  char *object_type;          // "var" or "func"
  struct ast_node *function;  // function body
  UT_hash_handle hh;          // makes this structure hashable
};

struct symbol_node *symbol_table = NULL;
struct ast_node* syntax_tree;
%}

%union {
  char *id;
  char *type;

  int num;
  double dec;
  char *str;

  struct ast_node *ast;
}

 
%token <id> ID
%token <type> TYPE
%token <num> NUM
%token <dec> DEC
%token <str> STR
%token WHILE IF ELSE RETURN WRITE READ
%token EQ CEQ CNE CLT CLE CGT CGE
%token PLUS MINUS MULT DIV
%token QUOTES
%token INTERPOL_START INTERPOL_END
%right EQ
%left '+' '-'
%left '*' '/'

%type <ast> prog declarations declaration var_declaration fun_declaration

%start prog
%%

prog:
  declarations                                  { syntax_tree = add_syntax_node("program"); }
;

declarations:
  declarations declaration
| declaration
;

declaration:
  var_declaration
| fun_declaration
;

var_declaration:
  TYPE ID ';'                                   { add_symbol($2, $1, "var"); }
| TYPE ID '[' NUM ']' ';'                       { add_symbol($2, $1, "var"); }
;

fun_declaration:
  TYPE ID '(' params ')' compound_statement     { add_symbol($2, $1, "func"); }
;

params:
| param_list
;

param_list:
  param_list ',' TYPE param
| TYPE param
;

param:
  ID
| ID '[' ']'
;

compound_statement:
  '{' local_declarations statement_list '}'
;

local_declarations:
  local_declarations var_declaration
|
;

statement_list:
| statement_list statement
;

statement:
  expression_statement
| conditional_statement
| iteration_statement
| RETURN return_statement
;

expression_statement:
  expression ';'
;

conditional_statement:
  IF '(' expression ')' compound_statement
| IF '(' expression ')' compound_statement ELSE  compound_statement
;

iteration_statement:
  WHILE '(' expression ')' compound_statement
;

return_statement:
  expression ';'
| ';'
;

expression:
  var EQ expression
| simple_expression
;

var:
  ID
| ID '[' expression ']'
;

simple_expression:
  op_expression cond_op op_expression
| op_expression
;

cond_op:
  CEQ
| CNE
| CLT
| CLE
| CGT
| CGE
;

op_expression:
  op_expression op term
| term
;

op:
  PLUS
| MINUS
| MULT
| DIV
;

term:
  '(' simple_expression ')'
| var
| call
| NUM
| DEC
| QUOTES string QUOTES
;

call:
  ID '(' args ')'
| WRITE '(' simple_expression ')'
| READ '(' var ')'
;

args:
| arg_list
;

arg_list:
  arg_list ',' simple_expression
| simple_expression
;

string:
| string STR
| string INTERPOL_START simple_expression INTERPOL_END
;

%%

struct ast_node* add_syntax_node(char *data){
  struct ast_node* ast_node = (struct ast_node*)malloc(sizeof(struct ast_node));

  ast_node->data = (char *) strdup(data);
  ast_node->left = NULL;
  ast_node->right = NULL;

  return ast_node;
}

void add_symbol(char *name, char *type, char *object_type){
  struct symbol_node *s;

  HASH_FIND_STR(symbol_table, name, s);
  if(s == NULL){
    s = (struct symbol_node *)malloc(sizeof *s);

    s->name = (char *) strdup(name);
    s->type = (char *) strdup(type);
    s->object_type = (char *) strdup(object_type);

    HASH_ADD_STR(symbol_table, name, s);
  }
}

void print_symbol_table() {
  struct symbol_node *s;

  printf("===============  SYMBOL TABLE ===============\n");
  printf("NAME\t\tTYPE\t\tOBJECT_TYPE\n");
  for(s=symbol_table; s != NULL; s=s->hh.next) {
    printf("%s\t\t%s\t\t%s\n", s->name, s->type, s->object_type);
  }
}

void print_syntax_node(struct ast_node *s) {
  if(s == NULL) return;
  printf("%s\n\t", s->data);
  print_syntax_node(s->left);
  print_syntax_node(s->right);
}

void print_syntax_tree() {
  struct ast_node *s = syntax_tree;

  printf("======  SYNTAX TREE ======\n");
  print_syntax_node(s);
  printf("\n");
}

void free_symbol_table(){
  struct symbol_node *s;
  for(s=symbol_table; s != NULL; s=s->hh.next) {
    HASH_DEL(symbol_table, s);
    free(s->name);
    free(s->type);
    free(s->object_type);
    free(s);
  }
}

void main (int argc, char **argv){
  int print_table = 0;

  if(argc > 1 && !strcmp(argv[1], "-t")){
    print_table = 1;
  }

  yyparse();

  if(print_table) print_symbol_table();
  print_syntax_tree();
  free_symbol_table();
}