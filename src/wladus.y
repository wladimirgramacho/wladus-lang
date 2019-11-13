%error-verbose
%debug
%locations

%{
#include <stdio.h>
#include "uthash.h"
#include "utstack.h"

int yylex();
int yyerror(const char *s);

typedef struct param {
  char *name;
  char *type;
  struct param *next;
} param;

typedef struct simple_symbol_node {
  char *name;
  char *type;
  struct simple_symbol_node *next;
} simple_symbol_node;

typedef struct scope {
  char *scope_name;
  struct scope *next;
} scope;

struct ast_node* add_ast_node(int node_type, struct ast_node *left, struct ast_node *right);
struct ast_node* add_ast_func_node(char *func_name, param *params, struct ast_node *func_body);
void add_symbol(char *name, char *type, char symbol_type, param *param);
param* add_param(char *type, char *name, param *next);
simple_symbol_node* create_simple_symbol_node(char *name, char *type);

void error_redeclaration(char *symbol_type, char *name);

struct ast_node {
  int node_type;
  struct ast_node *left;
  struct ast_node *right;
  union {
    int integer;
    float decimal;
    char *string;
    char *operator;
    char *func_name;
  };
};

struct ast_func_node { // function declarations
  int node_type;
  char *func_name;
  param *params;
  struct ast_node *func_body;
};

struct symbol_node {
  char *name;                     // key field
  char *type;                     // int | float | string | void
  char symbol_type;               // 'V' (variable) | 'F' (function) | 'P' (parameter)
  UT_hash_handle hh;              // makes this structure hashable
  struct {
    struct ast_node *func_body;   // function body
    param *param_list;
    simple_symbol_node *symbols;
  } func_fields;
};

struct symbol_node *symbol_table = NULL;
struct ast_node* syntax_tree = NULL;
struct scope* scope_stack = NULL;
%}

%union {
  char *id;
  char *type;
  char *op;

  int num;
  double dec;
  char *str;

  struct ast_node *ast;
  struct param *param;
}

 
%token <id> ID
%token <type> TYPE
%token <num> NUM
%token <dec> DEC
%token <str> STR
%token WHILE IF ELSE RETURN WRITE READ
%token <op> EQ CEQ CNE CLT CLE CGT CGE
%token <op> PLUS MINUS MULT DIV
%token QUOTES
%token ITP_START ITP_END
%right EQ
%left '+' '-'
%left '*' '/'

%type <ast> prog declarations declaration var_declaration func_declaration
%type <ast> local_declarations statement_list compound_statement statement local_var_declaration
%type <ast> expression_statement conditional_statement iteration_statement return_statement
%type <ast> expression var simple_expression op_expression term call args arg_list string

%type <param> params

%start prog
%%

prog:
  declarations                                  { syntax_tree = $1; }
;

declarations:
  declarations declaration                      { $$ = add_ast_node('A', $1, $2); }
| declaration                                   { $$ = $1; }
;

declaration:
  var_declaration                               { $$ = $1; }
| func_declaration                              { $$ = $1; }
;

var_declaration:
  TYPE ID ';'                                   { $$ = NULL; add_symbol($2, $1, 'V', NULL); }
;

func_declaration:
  TYPE ID '(' params ')'                        {
                                                  scope *new_scope = (scope *)malloc(sizeof *new_scope);
                                                  new_scope->scope_name = (char *) strdup($2);
                                                  STACK_PUSH(scope_stack, new_scope);
                                                  add_symbol($2, $1, 'F', $4);
                                                }
  compound_statement                            {
                                                  $$ = add_ast_func_node($2, $4, $7);
                                                  scope *old_scope;
                                                  STACK_POP(scope_stack, old_scope);
                                                  free(old_scope->scope_name);
                                                  free(old_scope);
                                                  scope_stack = NULL;
                                                }
;

params:
  params ',' TYPE ID                            { $$ = add_param($3, $4, $1); add_symbol($4, $3, 'P', NULL); }
| TYPE ID                                       { $$ = add_param($1, $2, NULL); add_symbol($2, $1, 'P', NULL); }
|                                               { $$ = NULL; }
;

compound_statement:
  '{' local_declarations statement_list '}'     { $$ = add_ast_node('A', $2, $3); }
;

local_declarations:
  local_declarations local_var_declaration      { $$ = add_ast_node('A', $1, $2); }
|                                               { $$ = NULL; }
;

local_var_declaration:
  TYPE ID ';'                                   { $$ = NULL; add_symbol($2, $1, 'V', NULL); } // TODO: ADD SCOPE

statement_list:
  statement_list statement                      { $$ = add_ast_node('A', $1, $2); }
|                                               { $$ = NULL; }
;

statement:
  expression_statement                          { $$ = $1; }
| conditional_statement                         { $$ = $1; }
| iteration_statement                           { $$ = $1; }
| return_statement                              { $$ = $1; }
;

expression_statement:
  expression ';'                                { $$ = $1; }
;

conditional_statement:
  IF '(' expression ')' compound_statement      { $$ = add_ast_node('C', $3, $5); }
| IF '(' expression ')' compound_statement ELSE  compound_statement { $$ = add_ast_node('C', $3, add_ast_node('c', $5, $7)); }
;

iteration_statement:
  WHILE '(' expression ')' compound_statement   { $$ = add_ast_node('W', $3, $5); }
;

return_statement:
  RETURN expression ';'                         { $$ = $2; }
| RETURN ';'                                    { $$ = NULL; }
;

expression:
  var EQ expression                             { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| simple_expression                             { $$ = $1; }
;

var:
  ID                                            { $$ = NULL; } // TODO: check if identifier exists and is variable
;

simple_expression:
  op_expression CEQ op_expression               { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| op_expression CNE op_expression               { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| op_expression CLT op_expression               { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| op_expression CLE op_expression               { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| op_expression CGT op_expression               { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| op_expression CGE op_expression               { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| op_expression                                 { $$ = $1; }
;

op_expression:
  op_expression PLUS term                       { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| op_expression MINUS term                      { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| op_expression MULT term                       { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| op_expression DIV term                        { $$ = add_ast_node('O', $1, $3); $$->operator = (char *) strdup($2); }
| term                                          { $$ = $1; }
;

term:
  '(' simple_expression ')'                     { $$ = $2; }
| var                                           { $$ = $1; }
| call                                          { $$ = $1; }
| NUM                                           { $$ = add_ast_node('I', NULL, NULL); $$->integer = $1; }
| DEC                                           { $$ = add_ast_node('D', NULL, NULL); $$->decimal = $1; }
| QUOTES string QUOTES                          { $$ = $2; }
;

call:
  ID '(' args ')'                               { $$ = add_ast_node('L', NULL, $3); $$->func_name = (char *) strdup($1); }
| WRITE '(' simple_expression ')'               { $$ = add_ast_node('L', NULL, $3); $$->func_name = (char *) strdup("write"); }
| READ '(' var ')'                              { $$ = add_ast_node('L', NULL, $3); $$->func_name = (char *) strdup("read"); }
;

args:
  arg_list                                      { $$ = $1; }
|                                               { $$ = NULL; }
;

arg_list:
  arg_list ',' simple_expression                { $$ = add_ast_node('A', $1, $3); }
| simple_expression                             { $$ = $1; }
;

string:
  string STR                                    {
                                                  $$ = add_ast_node('S', NULL, $1);
                                                  // $$->string = (char *) strdup($2);
                                                }
| string ITP_START simple_expression ITP_END    {
                                                  $$ = add_ast_node('T', $1, $3);
                                                  // $$->string = strdup("");
                                                }
|                                               { $$ = NULL; }
;

%%

struct ast_node* add_ast_node(int node_type, struct ast_node *left, struct ast_node *right){
  struct ast_node* ast_node = (struct ast_node*)malloc(sizeof(struct ast_node));

  ast_node->node_type = node_type;
  ast_node->left = left;
  ast_node->right = right;

  return ast_node;
}

struct ast_node* add_ast_func_node(char *func_name, param *params, struct ast_node *func_body){
  struct ast_func_node* ast_node = (struct ast_func_node*)malloc(sizeof(struct ast_func_node));

  ast_node->node_type = 'F';
  ast_node->func_name = (char *) strdup(func_name);
  ast_node->params = params;
  ast_node->func_body = func_body;

  return (struct ast_node *) ast_node;
}

void print_params(param *param, int depth){
  if(param == NULL) return;

  printf("%*s", depth, "");
  printf("%s", param->type);
  printf("%s", param->name);
  print_params(param->next, depth);
}

void print_ast_node(struct ast_node *s, int depth) {
  if(s == NULL) return;

  printf("%*s", depth, "");

  switch (s->node_type){
    case 'A':
    case 'T':
      printf("\n");
      print_ast_node(s->left, depth + 1);
      print_ast_node(s->right, depth + 1);
      break;
    case 'F':
      {
        struct ast_func_node *node = (struct ast_func_node *) s;
        printf(" (%s)\n", node->func_name);
        if(node->params) print_params(node->params, depth+1);
        print_ast_node(node->func_body, depth+1);
      }
      break;
    case 'O':
      {
        printf(" (%s)\n", s->operator);
        print_ast_node(s->left, depth + 1);
        print_ast_node(s->right, depth + 1);
      }
      break;
    case 'C':
      {
        printf("\n");
        print_ast_node(s->left, depth + 1);
        print_ast_node(s->right, depth + 1);
      }
      break;
    case 'W':
      {
        printf("\n");

        printf("%*s", depth, "");
        printf("-- condition --\n");
        print_ast_node(s->left, depth + 1);

        printf("%*s", depth, "");
        printf("-- while body --\n");
        print_ast_node(s->right, depth + 1);
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
        printf(" (%d)\n", s->integer);
      }
      break;
    case 'D':
      {
        printf(" (%lf)\n", s->decimal);
      }
      break;
    case 'S':
      {
        printf(" (%s)\n", s->string);
        print_ast_node(s->left, depth + 1);
        print_ast_node(s->right, depth + 1);
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

void free_params(param *param){
  if(param == NULL) return;
  free(param->type);
  free(param->name);
  free_params(param->next);
  free(param);
}

void free_syntax_tree(struct ast_node *s){
  if(s == NULL) return;

  switch (s->node_type){
    case 'A':
    case 'C':
    case 'c':
    case 'W':
      if(s->left) free_syntax_tree(s->left);
      if(s->right) free_syntax_tree(s->right);
      free(s);
      break;
    case 'F':
      {
        struct ast_func_node *node = (struct ast_func_node *) s;
        if(node->params) free_params(node->params);
        free(node->func_name);
        free_syntax_tree(node->func_body);
        free(node);
      }
      break;
    case 'O':
      {
        free(s->operator);
        if(s->left) free_syntax_tree(s->left);
        if(s->right) free_syntax_tree(s->right);
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

struct symbol_node* build_symbol(char *name, char *type, char symbol_type, param *param){
  struct symbol_node *s = (struct symbol_node *)malloc(sizeof *s);

  s->name = (char *) strdup(name);
  s->type = (char *) strdup(type);
  s->symbol_type = symbol_type;
  if(symbol_type == 'F'){
    // s->func_fields.func_body = ast_node;
    s->func_fields.param_list = param;
    s->func_fields.symbols = NULL;
  }

  return s;
}

void add_symbol(char *name, char *type, char symbol_type, param *param){
  struct symbol_node *s;

  if(symbol_type == 'F') {
    HASH_FIND_STR(symbol_table, name, s);
    if(s == NULL){ // function not declared -> add to symbol table
      s = build_symbol(name, type, symbol_type, param);
      HASH_ADD_STR(symbol_table, name, s);
    }
    else { // function already declared -> error
      error_redeclaration("function", name);
      return;
    }
  }
  else {
    if(STACK_TOP(scope_stack) == NULL){ // is not inside function
      HASH_FIND_STR(symbol_table, name, s);
      if(s == NULL){ // global variable not declared -> add to symbol table
        s = build_symbol(name, type, symbol_type, param);
        HASH_ADD_STR(symbol_table, name, s);
      }
      else { // global variable already declared -> error
        error_redeclaration("variable", name);
        return;
      }
    }
    else { // is inside function
      HASH_FIND_STR(symbol_table, name, s);
      if(s != NULL){ // local variable is declared as global variable -> error
        error_redeclaration("variable", name);
        return;
      }

      scope * top = STACK_TOP(scope_stack);
      HASH_FIND_STR(symbol_table, top->scope_name, s);

      simple_symbol_node *tmp, *new_node;

      new_node = create_simple_symbol_node(name, type);

      if(s->func_fields.symbols == NULL){
        s->func_fields.symbols = new_node;
        return;
      }

      for (tmp = s->func_fields.symbols; tmp != NULL; tmp = tmp->next){
        if(strcmp(tmp->name, name) == 0){ // local variable is already declared in function -> error
          error_redeclaration("variable", name);
          free(new_node->name);
          free(new_node->type);
          free(new_node);
          return;
        }
      }
      tmp = new_node;
    }
  }
}

simple_symbol_node* create_simple_symbol_node(char *name, char *type){
  simple_symbol_node *new_node = (simple_symbol_node *)malloc(sizeof *new_node);
  new_node->name = (char *) strdup(name);
  new_node->type = (char *) strdup(type);
  new_node->next = NULL;
  return new_node;
}

void error_redeclaration(char *symbol_type, char *name){
  char * error_message = (char *)malloc(50 * sizeof(char));
  sprintf(error_message, "semantic error, %s '%s' was already declared", symbol_type, name);
  yyerror(error_message);
  free(error_message);
}

param* add_param(char *type, char *name, param *next){
  param *p;

  p = (param *)malloc(sizeof *p);

  p->type = (char *) strdup(type);
  p->name = (char *) strdup(name);
  p->next = next;
  return p;
}

void print_symbol_table() {
  struct symbol_node *s;

  printf("===============  SYMBOL TABLE ===============\n");
  printf("NAME\t\tTYPE\t\tSYMBOL_TYPE\n");
  for(s=symbol_table; s != NULL; s=s->hh.next) {
    printf("%s\t\t%s\t\t%c\n", s->name, s->type, s->symbol_type);
  }
}

void free_simple_symbol_node(simple_symbol_node * node){
  if(node == NULL) return;
  free_simple_symbol_node(node->next);
  free(node->name);
  free(node->type);
  free(node);
}

void free_symbol_table(){
  struct symbol_node *s, *tmp;

  HASH_ITER(hh, symbol_table, s, tmp) {
    HASH_DEL(symbol_table, s);
    free(s->name);
    free(s->type);
    s->func_fields.func_body = NULL;
    free_simple_symbol_node(s->func_fields.symbols);
    free(s);
  }
}

void main (int argc, char **argv){
  int print_table = 0;
  int print_tree = 0;

  if(argc > 1 && !strcmp(argv[1], "-t")){
    print_table = 1;
  }

  if(argc > 1 && !strcmp(argv[1], "-tt")){
    print_table = 1;
    print_tree = 1;
  }

  yyparse();

  if(print_table) print_symbol_table();
  if(print_tree) print_syntax_tree();
  free_symbol_table();
  free_syntax_tree(syntax_tree);
}