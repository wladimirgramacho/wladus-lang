%error-verbose
%debug
%locations

%{
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
%}

%union {
  char *id;
  char *type;
  char *op;

  char *str;

  struct ast_node *ast;
}
%type <ast> prog declarations declaration var_declaration func_declaration params param
%type <ast> local_declarations statement_list compound_statement statement local_var_declaration
%type <ast> expression_statement conditional_statement iteration_statement return_statement
%type <ast> expression var simple_expression op_expression term call args arg_list string

%token <id> ID
%token <type> TYPE
%token <str> NUM
%token <str> DEC
%token <str> STR
%token WHILE IF ELSE RETURN WRITE READ
%token QUOTES
%token ITP_START ITP_END

%right EQ
%left CEQ CNE CLT CLE CGT CGE
%left '*' '/'
%left '+' '-'


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
  TYPE ID ';'                                   {
                                                  $$ = NULL;
                                                  add_symbol($2, $1, 'V', NULL);
                                                  gen_table_symbol($1, $2);
                                                  free($1);
                                                  free($2);
                                                }
;

func_declaration:
  TYPE ID                                       {
                                                  temps_generated = 0;
                                                  params_generated = 0;
                                                  add_symbol($2, $1, 'F', NULL);
                                                  gen_label($2);
                                                  scope *new_scope = (scope *)malloc(sizeof *new_scope);
                                                  new_scope->scope_name = (char *) strdup($2);
                                                  new_scope->scope_level = 1;
                                                  STACK_PUSH(scope_stack, new_scope);
                                                }
  '(' params ')'                                { ; }
  compound_statement                            {
                                                  $$ = add_ast_node('F', NULL, $8);
                                                  $$->dtype = type_to_dtype($1);
                                                  $$->func_name = (char *) strdup($2);

                                                  symbol_node *s = find_symbol($2);
                                                  s->func_fields.func_body = $8;

                                                  remove_scope();
                                                  scope_stack = NULL;
                                                  if(!has_return && strcmp($2, "main") != 0){
                                                    if($$->dtype == 'i'){
                                                      gen1("return", "0");
                                                    }
                                                    else if($$->dtype == 'f'){
                                                      gen1("return", "0.0");
                                                    }
                                                    else {
                                                      gen0("return");
                                                    }
                                                  }

                                                  free($1);
                                                  free($2);
                                                }
;

params:
  params ',' param                              { $$ = add_ast_node('A', $1, $3); }
| param                                         { $$ = $1; }
|                                               { $$ = NULL; }
;

param:
  TYPE ID                                       {
                                                  $$ = add_ast_node('P', NULL, NULL);
                                                  $$->dtype = type_to_dtype($1);
                                                  scope *top = STACK_TOP(scope_stack);
                                                  if(strcmp(top->scope_name, "main") == 0){
                                                    $$->addr = new_temp();
                                                    if ($$->dtype == 'i'){ gen2("mov", $$->addr, "0"); }
                                                    else if ($$->dtype == 'f'){ gen2("mov", $$->addr, "0.0"); }
                                                  }
                                                  else {
                                                    $$->addr = new_param();
                                                  }
                                                  add_symbol($2, $1, 'P', $$->addr);
                                                  free($1);
                                                  free($2);
                                                }

compound_statement:
  '{' local_declarations statement_list '}'     { $$ = add_ast_node('A', $2, $3); }
;

local_declarations:
  local_declarations local_var_declaration      { $$ = add_ast_node('A', $1, $2); }
|                                               { $$ = NULL; }
;

local_var_declaration:
  TYPE ID ';'                                   {
                                                  $$ = add_ast_node('V', NULL, NULL);
                                                  $$->dtype = type_to_dtype($1);
                                                  $$->addr = new_temp();
                                                  if ($$->dtype == 'i'){ gen2("mov", $$->addr, "0"); }
                                                  else if ($$->dtype == 'f'){ gen2("mov", $$->addr, "0.0"); }
                                                  add_symbol($2, $1, 'V', $$->addr);
                                                  free($1);
                                                  free($2);
                                                }

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
| error ';'                                     { $$ = NULL; }
;

conditional_statement:
  startIf '(' simple_expression ')'compound_statement {
                                                  $$ = add_ast_node('C', add_ast_node('c', $3, $5), NULL);
                                                  remove_scope();

                                                  code_label *after_if_label;
                                                  STACK_POP(label_stack, after_if_label);
                                                  gen_label(after_if_label->name);
                                                  free(after_if_label->name);
                                                  free(after_if_label);
                                                }
| startIf '(' simple_expression ')' compound_statement {
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
  ELSE                                          { create_internal_scope(); }
  compound_statement                            {
                                                  $$ = add_ast_node('C', add_ast_node('c', $3, $5), $9);
                                                  remove_scope();

                                                  code_label *after_else_label;
                                                  STACK_POP(label_stack, after_else_label);
                                                  gen_label(after_else_label->name);
                                                  free(after_else_label->name);
                                                  free(after_else_label);
                                                }
;

startIf:
  IF                                            {
                                                  create_internal_scope();
                                                  char * after_if_label = new_label();
                                                  code_label *new_after_if_label = (code_label *)malloc(sizeof *new_after_if_label);
                                                  new_after_if_label->name = after_if_label;
                                                  STACK_PUSH(label_stack, new_after_if_label);
                                                }
;

iteration_statement:
  WHILE                                         {
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
  '(' simple_expression ')'                     {;}
  compound_statement                            {
                                                  $$ = add_ast_node('W', $4, $7);
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
;

return_statement:
  RETURN simple_expression ';'                  {
                                                  $$ = $2;
                                                  scope *top = STACK_TOP(scope_stack);
                                                  if(strcmp(top->scope_name, "main") != 0){
                                                    has_return = 1;
                                                    gen1("return", $2->addr);
                                                  }
                                                }
| RETURN ';'                                    {
                                                  $$ = NULL;
                                                  scope *top = STACK_TOP(scope_stack);
                                                  if(strcmp(top->scope_name, "main") != 0){
                                                    has_return = 1;
                                                    gen0("return");
                                                  }
                                                }
;

expression:
  var EQ                                        {
                                                  if($1->dtype == 's'){
                                                    string_target *new_string_target = (string_target *)malloc(sizeof *new_string_target);
                                                    new_string_target->temp_name = $1->addr;
                                                    STACK_PUSH(string_target_stack, new_string_target);
                                                  }
                                                }
  simple_expression                             {
                                                  if(mismatch($1->dtype, $4->dtype)){ error_type_mismatch($1->dtype, $4->dtype); }
                                                  else {
                                                    $$ = add_ast_node('A', $1, $4);
                                                    $$->dtype = $1->dtype;
                                                    if($$->dtype != 's'){
                                                      gen2("mov", $1->addr, $4->addr);
                                                    }
                                                  }
                                                }
| simple_expression                             { $$ = $1; }
;

var:
  ID                                            {
                                                  $$ = add_ast_node('V', NULL, NULL);
                                                  symbol_node *s = find_symbol($1);
                                                  if(s == NULL){ error_not_declared("variable", $1); }
                                                  else {
                                                    $$->dtype = s->type;
                                                    HASH_FIND_STR(symbol_table, $1, s);
                                                    if(s != NULL && s->symbol_type != 'F'){ $$->addr = (char *) strdup($1); } // global variable
                                                    else { $$->addr = find_simple_symbol($1)->temp_name; }
                                                  }
                                                  free($1);
                                                }
;

simple_expression:
  op_expression CEQ op_expression               {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){ error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("seq", $$->addr, $1->addr, $3->addr);
                                                  gen2("brz", top_label->name, $$->addr);
                                                }
| op_expression CNE op_expression               {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){ error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("seq", $$->addr, $1->addr, $3->addr);
                                                  gen2("brnz", top_label->name, $$->addr);
                                                }
| op_expression CLT op_expression               {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){ error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("slt", $$->addr, $1->addr, $3->addr);
                                                  gen2("brz", top_label->name, $$->addr);
                                                }
| op_expression CLE op_expression               {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){ error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("sleq", $$->addr, $1->addr, $3->addr);
                                                  gen2("brz", top_label->name, $$->addr);
                                                }
| op_expression CGT op_expression               {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){ error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("sleq", $$->addr, $1->addr, $3->addr);
                                                  gen2("brnz", top_label->name, $$->addr);
                                                }
| op_expression CGE op_expression               {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){ error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  code_label *top_label = STACK_TOP(label_stack);
                                                  gen3("slt", $$->addr, $1->addr, $3->addr);
                                                  gen2("brnz", top_label->name, $$->addr);
                                                }
| op_expression                                 { $$ = $1; }
;

op_expression:
  op_expression '+' term                        {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  gen3("add", $$->addr, $1->addr, $3->addr);
                                                }
| op_expression '-' term                        {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){ error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  gen3("sub", $$->addr, $1->addr, $3->addr);
                                                }
| op_expression '*' term                        {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){ error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  gen3("mul", $$->addr, $1->addr, $3->addr);
                                                }
| op_expression '/' term                        {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){ error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  gen3("div", $$->addr, $1->addr, $3->addr);
                                                }
| op_expression '%' term                        {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  if(mismatch($1->dtype, $3->dtype)){ error_type_mismatch($1->dtype, $3->dtype); }
                                                  else { $$->dtype = $1->dtype; }
                                                  $$->addr = new_temp();
                                                  gen3("mod", $$->addr, $1->addr, $3->addr);
                                                }
| term                                          { $$ = $1; }
;

term:
  '(' simple_expression ')'                     { $$ = $2; }
| var                                           { $$ = $1; }
| call                                          {
                                                  $$ = $1;
                                                  symbol_node *s = find_symbol($1->func_name);
                                                  if(s != NULL){ $$->dtype = s->type; }
                                                }
| NUM                                           { $$ = add_ast_node('I', NULL, NULL); $$->addr = $1; $$->dtype = 'i'; }
| DEC                                           { $$ = add_ast_node('D', NULL, NULL); $$->addr = $1; $$->dtype = 'f'; }
| QUOTES                                        {
                                                  
                                                }
  string QUOTES                                 {
                                                  $$ = $3;
                                                  $$->dtype = 's';
                                                }
;

call:
  ID '(' args ')'                               {
                                                  $$ = add_ast_node('L', NULL, $3);
                                                  $$->func_name = (char *) strdup($1);
                                                  symbol_node *s = find_symbol($1);
                                                  if(s == NULL) error_not_declared("function", $1);
                                                  if(params_stacked) gen2("call", $1, i_to_str(params_stacked));
                                                  else gen1("call", $1);
                                                  params_stacked = 0;
                                                  $$->addr = new_temp();
                                                  if(s->type != 'v'){
                                                    gen1("pop", $$->addr);
                                                  }
                                                  free($1);
                                                }
| WRITE '(' simple_expression ')'               {
                                                  $$ = add_ast_node('L', NULL, $3);
                                                  $$->func_name = (char *) strdup("write");
                                                  if($3->dtype == 's'){
                                                    char *pos = new_temp();
                                                    gen2("mov", pos, "0");

                                                    char * while_label = new_label();
                                                    gen_label(while_label);
                                                    char * after_while_label = new_label();

                                                    char *value = new_temp();
                                                    gen2("mov", value, array_string($3->addr, pos));
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
                                                    gen1("println", $3->addr);
                                                  }
                                                }
| READ '(' var ')'                              {
                                                  $$ = add_ast_node('L', NULL, $3);
                                                  $$->func_name = (char *) strdup("read");
                                                  if($3->dtype == 'i'){
                                                    gen1("scani", $3->addr);
                                                  }
                                                  else if($3->dtype == 'f') {
                                                    gen1("scanf", $3->addr);
                                                  }
                                                }
;

args:
  arg_list                                      { $$ = $1; }
|                                               { $$ = NULL; }
;

arg_list:
  arg_list ',' simple_expression                {
                                                  $$ = add_ast_node('A', $1, $3);
                                                  params_stacked++;
                                                  gen1("param", $3->addr);
                                                }
| simple_expression                             {
                                                  $$ = $1;
                                                  params_stacked++;
                                                  gen1("param", $1->addr);
                                                }
;

string:
  string STR                                    {
                                                  $$ = add_ast_node('S', NULL, $1);
                                                  $$->string = (char *) strdup($2);

                                                  int str_len = strlen($$->string);
                                                  $$->addr = new_temp();
                                                  $1->addr = $$->addr;
                                                  $1->string_size = str_len;
                                                  int i = 0;
                                                  gen2("mema", $$->addr, i_to_str(str_len + 1));
                                                  for (; i < str_len; ++i){
                                                    gen2("mov", array_string($$->addr, i_to_str(i)), i_to_str($$->string[i]));
                                                  }
                                                  char * string_end = array_string($$->addr, i_to_str(i));
                                                  gen2("mov", string_end, "0");

                                                  string_target *s;
                                                  STACK_POP(string_target_stack, s);
                                                  if(is_array(s->temp_name)){
                                                    char * aux = new_temp();
                                                    for (i = 0; i < str_len; ++i){
                                                      gen2("mov", aux, array_string($$->addr, i_to_str(i)));
                                                      gen2("mov", s->temp_name, aux);
                                                      gen3("add", extract_pos(s->temp_name), extract_pos(s->temp_name), "1");
                                                    }
                                                    gen2("mov", aux, array_string($$->addr, i_to_str(i)));
                                                    gen2("mov", s->temp_name, aux);
                                                    gen3("add", extract_pos(s->temp_name), extract_pos(s->temp_name), "1");
                                                  }
                                                  else {
                                                    gen2("mov", s->temp_name, $$->addr);
                                                  }

                                                  string_target *new_string_target = (string_target *)malloc(sizeof *new_string_target);
                                                  new_string_target->temp_name = string_end;
                                                  STACK_PUSH(string_target_stack, new_string_target);

                                                  free($2);
                                                }
| string ITP_START simple_expression ITP_END    {
                                                  $$ = add_ast_node('T', $1, $3);
                                                  $$->string = (char *) strdup("interpolated string");
                                                  $$->addr = new_temp();
                                                  gen2("mov", $$->addr, $3->addr);
                                                  char * string_pointer = new_temp();
                                                  gen2("mema", string_pointer, "20");

                                                  char * pos = new_temp();
                                                  gen2("mov", pos, "0");

                                                  char *num = new_temp();
                                                  gen2("mov", num, $$->addr);

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
                                                    gen2("mov", s->temp_name, $$->addr);
                                                  }

                                                  string_target *new_string_target = (string_target *)malloc(sizeof *new_string_target);
                                                  new_string_target->temp_name = string_end;
                                                  STACK_PUSH(string_target_stack, new_string_target);
                                                }
|                                               {
                                                  $$ = add_ast_node('S', NULL, NULL);
                                                  $$->string = (char *) strdup("");
                                                }
;

%%

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