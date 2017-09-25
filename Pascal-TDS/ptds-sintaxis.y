%{

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.c"
#include "structures.c"
extern int yylineno;

%}

%{
int yylex();
void yyerror(const char *s);
%}


%union { int i; char *s; struct tokensVal *tv; struct tokensStr *ts; struct tree *t; int p[10];}

%token<tv> INT
%token<ts> ID
%token<i> OP_ADD
%token<i> OP_SUB
%token<i> OP_PROD
%token<i> OP_DIV
%token<i> OP_MOD
%token<i> OP_MINOR
%token<i> OP_MAJOR
%token<i> OP_EQUAL
%token<i> OP_AND
%token<i> OP_OR
%token<i> OP_ASS
%token<i> TRUE
%token<i> FALSE
%token<i> OP_NOT
%token<i> WHILE
%token<i> BEGINN
%token<i> RETURN
%token<i> END
%token<i> BOOL
%token<i> PROGRAM
%token<i> ELSE
%token<i> THEN
%token<i> IF
%token<i> INTEGER
%token<i> VOID
%token<i> PAR_LEFT
%token<i> PAR_RIGHT
%token<i> SEMICOLON
%token<i> COMMA


%nonassoc OP_ASS
%left OP_AND OP_OR
%nonassoc OP_EQUAL OP_MAJOR OP_MINOR
%left OP_ADD OP_SUB
%left OP_PROD OP_DIV OP_MOD
%right NEG

%type<i> type
%type<p> param
%type<t> expr
%type<t> literal
%type<i> bool_literal
%type<i> integer_literal



%%
program: {openLevel();} prog
;

prog:  PROGRAM BEGINN var_decls SEMICOLON method_decls END            {printf("programa var_decl ; method_decl ; \n");}
      | PROGRAM BEGINN method_decls END                               {printf("programa method_decl\n");}
      | PROGRAM BEGINN var_decls SEMICOLON END                        {printf("programa var_decl\n");}
      | PROGRAM BEGINN END                                            {printf("programa BEGINN END\n");}
    ;

var_decl : type ID                                                    {insertTable ($2->value,0,$1);}
      | var_decl COMMA ID                                             {insertTree ($3->value,0,typeLastVar());}
    ;

var_decls : var_decl                                                  {printf("var_decl\n");}
      | var_decls SEMICOLON var_decl                                  {printf("var_decls var_decl\n");}
    ;

method_decl : type ID PAR_LEFT param PAR_RIGHT block                   {printf("metodo decl type ID (var_decl) block\n");}
      | VOID ID PAR_LEFT param PAR_RIGHT block                         {printf("metodo decl VOID ID (var_decl) block\n");}
      | type ID PAR_LEFT PAR_RIGHT block                               {printf("metodo decl type ID () block\n");}
      | VOID ID PAR_LEFT PAR_RIGHT block                               {printf("metodo decl VOID ID () block\n");}
     ;

method_decls : method_decl                                             {printf("method_decl\n");}
      | method_decls method_decl                                       {printf("method_decls recursive\n");}
     ;

param : type ID                                                         {printf("param\n");}
      | param COMMA type ID                                             {printf("param recursive\n");}
    ;

block: {openLevel();} blockAux                                         {closeLevel();}
    ;


blockAux: BEGINN var_decls SEMICOLON statements END                    {printf("bloque var_decl statement\n");}
      | BEGINN statements END                                          {printf("bloque statement\n");}
      | BEGINN var_decls SEMICOLON END                                 {printf("bloque var_decl\n");}
      | BEGINN END                                                     {printf("bloque BEGINN END\n");}
    ;

type : INTEGER                                                         {$$ = INTEGERAUX;}
      | BOOL                                                           {$$ = BOOLAUX;}
    ;

statement : ID OP_ASS expr SEMICOLON                                  {printf("statement ID\n");}
      | method_call SEMICOLON                                         {printf("statement method_call\n");}
      | IF PAR_LEFT expr PAR_RIGHT THEN block ELSE block              {printf("statement IF expr THEN block ELSE block\n");}
      | IF PAR_LEFT expr PAR_RIGHT THEN block                         {printf("statement IF expr THEN block\n");}
      | WHILE expr block                                              {printf("statement WHILE expr block\n");}
      | RETURN expr SEMICOLON                                         {printf("statement RETURN expr\n");}
      | RETURN SEMICOLON                                              {printf("statement RETURN\n");}
      | SEMICOLON                                                     {printf("statement ;\n");}
      | block                                                         {printf("statement block\n");}
    ;

statements: statement                                                 {printf("statements \n");}
      | statements statement                                          {printf("statements recursive\n");}
    ;

method_call : ID PAR_LEFT exprs PAR_RIGHT                              {printf("method_call ID (expr)\n");}
      | ID PAR_LEFT PAR_RIGHT                                         {printf("method_call ID ()\n");}
    ;

exprs : expr
      | exprs COMMA expr
    ;

expr : ID                                                 { $$ = insertTree ($1->value,0,VAR);}
      | method_call                                       {//NO TENGO NI IDEAAAAAA
                                                            printf("expr method_call\n");}
      | literal                                           { $$ = $1;}
      | expr OP_ADD expr                                  { node *father;
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            printf("AAAAAAA\n");}

                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              father = insertTree ("OP_ADD",0,OPER_AR);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }

                                                          }
      | expr OP_SUB expr                                  { node *father;
                                                           //CHEQUEAR TIPOS DE EXPRESIONES
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              father = insertTree ("OP_SUB",0,OPER_AR);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_PROD expr                                 { node *father;
                                                           //CHEQUEAR TIPOS DE EXPRESIONES
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              father = insertTree ("OP_PROD",0,OPER_AR);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_DIV expr                                  { node *father;
                                                            //CHEQUEAR TIPOS DE EXPRESIONES
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              father = insertTree ("OP_DIV",0,OPER_AR);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_MOD expr                                  { node *father;
                                                           //CHEQUEAR TIPOS DE EXPRESIONES
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              father = insertTree ("OP_MOD",0,OPER_AR);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_MINOR expr                                { node *father;
                                                           //CHEQUEAR TIPOS DE EXPRESIONES
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              father = insertTree ("OP_MINOR",0,OPER_REL);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_MAJOR expr                                { node *father;
                                                           //CHEQUEAR TIPOS DE EXPRESIONES
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){

                                                              father = insertTree ("OP_MAJOR",0,OPER_REL);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_EQUAL expr                                { node *father;
                                                           //CHEQUEAR TIPOS DE EXPRESIONES
                                                            concatLeft(father,$1);
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            bool tipearit1 = (t1 == OPER_AR || t1 == INTEGERAUX);
                                                            bool tipearit2 = (t2 == OPER_AR || t2 == INTEGERAUX);
                                                            if(tipearit1 == tipearit2){
                                                              father = insertTree ("OP_EQUAL",0,OPER_REL);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_AND expr                                  { node *father;
                                                           //CHEQUEAR TIPOS DE EXPRESIONES
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if((t1 == OPER_LOG || t1 == BOOLAUX || t1 == OPER_REL) && (t2 == OPER_LOG || t2 == BOOLAUX ||  t2 == OPER_REL)){
                                                              father = insertTree ("OP_AND",0,OPER_LOG);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_OR expr                                   { node *father;
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if((t1 == OPER_LOG || t1 == BOOLAUX || t1 == OPER_REL) && (t2 == OPER_LOG || t2 == BOOLAUX ||  t2 == OPER_REL)){
                                                              father = insertTree ("OP_OR",0,OPER_LOG);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | OP_SUB expr %prec NEG                             { node *father;
                                                            father = insertTree ("OP_NEG",0,OPER_LOG);
                                                           //CHEQUEAR TIPOS DE EXPRESIONES
                                                            int t1 = $2->content->type;
                                                            if(t1 == OPER_LOG || t1 == BOOLAUX || t1 == OPER_REL){
                                                              father = insertTree ("OP_NOT",0,OPER_LOG);
                                                              concatLeft(father,$2);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | OP_NOT expr %prec NEG                             { node *father;
                                                            father = insertTree ("OP_NOT",0,OPER_LOG);
                                                            int t1 = $2->content->type;
                                                            if(t1 == OPER_AR || t1 == INTEGERAUX){
                                                              father = insertTree ("OP_NOT",0,OPER_LOG);
                                                              concatLeft(father,$2);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | PAR_LEFT expr PAR_RIGHT                           {//node *father;
                                                           //father = insertTree ("OP_PARATHESIS",0,INDETERMINATE);
                                                           //CHEQUEAR TIPOS DE EXPRESIONES
                                                           //concatLeft(father,$2);
                                                           $$ = $2;
                                                          }
    ;

literal : integer_literal                                 {$$ = insertTree("int_lit",$1,INTEGERAUX);}
      | bool_literal                                      {$$ = insertTree("bool_lit",$1,BOOLAUX);}
    ;

integer_literal : INT                                     {$$ = $1->value;}
    ;

bool_literal : TRUE                                       {$$ = TRUE;}
      | FALSE                                             {$$ = FALSE;}
    ;

%%
