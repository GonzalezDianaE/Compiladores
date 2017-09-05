%{

#include <stdlib.h>
#include <stdio.h>

%}
 
%union { int i; char *s; struct tree *t;}
 
%token<i> INT
%token<s> ID
%token<s> OP_ADD
%token<s> OP_SUB
%token<s> OP_PROD
%token<s> OP_DIV
%token<s> OP_MOD
%token<s> OP_MINOR
%token<s> OP_MAJOR
%token<s> OP_EQUAL
%token<s> OP_AND
%token<s> OP_OR
%token<s> TRUE
%token<s> FALSE
%token<s> WHILE
%token<s> BEGINN
%token<s> RETURN
%token<s> END
%token<s> BOOL
%token<s> PROGRAM
%token<s> ELSE
%token<s> THEN
%token<s> IF
%token<s> INTEGER
%token<s> VOID

%left OP_ADD OP_SUB
%left OP_PROD
%left OP_DIV
%left OP_PER
%left OP_EQUAL
%left OP_MAYOR OP_MINOR

//%type<i> expr
//%type<i> var_decl
 

%%
 
prog:  PROGRAM BEGINN var_decl ';' method_decl ';' END    {printf("programa var_decl ; method_decl ; \n");}
      | PROGRAM BEGINN method_decl END                    {printf("programa method_decl \n");}
      | PROGRAM BEGINN var_decl END                       {printf("programa var_decl\n");}
      | PROGRAM BEGINN END                                {printf("programa BEGINN END\n");}
    ;

var_decl : type ID                                       {printf("declaracion de variable type ID\n");}
      | var_decl ',' type ID                             {printf("declaracion de variable var_decl , type ID\n");}
    ;

method_decl : type ID '(' var_decl ')' block             {printf("metodo decl type ID (var_decl) block\n");}
      | VOID ID '(' var_decl ')' block                   {printf("metodo decl VOID ID (var_decl) block\n");}
      | method_decl type ID '(' var_decl ')' block       {printf("metodo decl method_decl type ID (var_decl) block\n");}
      | method_decl VOID ID '(' var_decl ')' block       {printf("metodo decl method_decl VOID ID (var_decl) block\n");}
      | VOID ID '(' ')' block                            {printf("metodo decl VOID ID () block\n");}
      | method_decl type ID '(' ')' block                {printf("metodo decl method_decl type ID () block \n");}
      | method_decl VOID ID '(' ')' block                {printf("metodo decl method_decl VOID ID () block\n");}
    ;

block: BEGINN var_decl statement END                      {printf("bloque var_decl statement\n");}
      | BEGINN statement END                              {printf("bloque statement\n");}
      | BEGINN var_decl END                               {printf("bloque var_decl\n");}
      | BEGINN END                                        {printf("bloque BEGINN END\n");}
    ;

type : INTEGER                                           {printf("tipo entero\n");}
      | BOOL                                             {printf("tipo booleano\n");}
    ;

statement : ID '=' expr ';'                              {printf("statement ID\n");}
      | method_call ';'                                  {printf("statement method_call\n");}
      | IF '(' expr ')' THEN block ELSE block            {printf("statement IF expr THEN block ELSE block\n");}
      | IF '(' expr ')' THEN block                       {printf("statement IF expr THEN block\n");}
      | WHILE expr block                                 {printf("statement WHILE expr block\n");}
      | RETURN expr ';'                                  {printf("statement RETURN expr\n");}
      | RETURN ';'                                       {printf("statement RETURN\n");}
      | ';'                                              {printf("statement ;\n");}
      | block                                            {printf("statement block\n");}
    ;

method_call : ID '(' expr ')'                            {printf(" method_call ID (expr)\n");}
      | ID '(' ')'                                       {printf("method_call ID ()\n");}
    ;

expr : ID                                                {printf("expr ID\n");}
      | method_call                                      {printf("expr method_call\n");}
      | literal                                          {printf("expr literal\n");}
      | expr OP_ADD expr                                 {printf("expr bin_op expr\n");}
      | expr OP_SUB expr                                 {printf("expr bin_op expr\n");}
      | expr OP_PROD expr                                {printf("expr bin_op expr\n");}
      | expr OP_DIV expr                                 {printf("expr bin_op expr\n");}
      | expr OP_PER expr                                 {printf("expr bin_op expr\n");}
      | expr OP_MINOR expr                               {printf("expr bin_op expr\n");}
      | expr OP_MAJOR expr                               {printf("expr bin_op expr\n");}
      | expr OP_EQUAL expr                               {printf("expr bin_op expr\n");}
      | expr OP_AND expr                                 {printf("expr bin_op expr\n");}
      | expr OP_OR expr                                  {printf("expr bin_op expr\n");}
      | OP_SUB expr                                         {printf("expr -\n");}
      | OP_MOD expr                                         {printf("expr !\n");}
      | '(' expr ')'                                     {printf("expr (expr)\n");}
    ;

literal : integer_literal                                {printf("literal integer_literal\n");}
      | bool_literal                                     {printf("literal bool_literal\n");}
    ;

integer_literal : INT                                    {printf("integer_literal\n");}
    ;              

bool_literal : TRUE                                      {printf("bool_literal TRUE\n");}
      | FALSE                                            {printf("bool_literal FALSE\n");}
    ;

%%