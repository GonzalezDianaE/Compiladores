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
%type<t> statement
%type<t> statements
%type<t> method_call
%type<t> block
%type<t> blockAux


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

block: {openLevel();} blockAux                                         {closeLevel(); $$=$2;}
    ;


blockAux: BEGINN var_decls SEMICOLON statements END                    {$$=$4;}
      | BEGINN statements END                                          {$$=$2;}
      | BEGINN var_decls SEMICOLON END                                 {printf("bloque var_decl\n");}
      | BEGINN END                                                     {printf("bloque BEGINN END\n");}
    ;

type : INTEGER                                                         {$$ = INTEGERAUX;}
      | BOOL                                                           {$$ = BOOLAUX;}
    ;

statement : ID OP_ASS expr SEMICOLON                                  { call *node = insertTree($1->value,0,ASSIGN);
                                                                        call->left = concatLeft ($3);
                                                                        $$=call;                                                       
                                                                      }
      | method_call SEMICOLON                                         { $$=$1;}
      | IF PAR_LEFT expr PAR_RIGHT THEN block ELSE block              { call *node = insertTree("IFAUX",0,IFAUX);
                                                                        /*Chequeo de tipos, en caso de ser una funcion debe retornar un booleano
                                                                        en caso contrario, debe ser de tipo bool, op_log, op_rel */
                                                                        int t1 = $3->content->type;
                                                                        if (t1 == FUNCTION_CALL){
                                                                          int t2 = ((call->content)->function)->ret;
                                                                          if (t2 != BOOLAUX){ 
                                                                           fprintf(stderr, "Error: no match type.\n");
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                        }else{ 
                                                                          if(!(t1 == BOOLAUX || t1 == OPER_LOG || t1 == OPER_REL)){
                                                                            fprintf(stderr, "Error: invalid expression.\n");
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                        }
                                                                        call->left = $3;
                                                                        call->middle = $6;
                                                                        call->right = $8;
                                                                        $$ = call;
                                                                      }
      | IF PAR_LEFT expr PAR_RIGHT THEN block                         {call *node = insertTree("IF_ELSE",0,IF_ELSE);
                                                                      /*Chequeo de tipos, en caso de ser una funcion debe retornar un booleano
                                                                        en caso contrario, debe ser de tipo bool, op_log, op_rel */
                                                                        int t1 = $3->content->type;
                                                                        if (t1 == FUNCTION_CALL){
                                                                          int t2 = ((call->content)->function)->ret;
                                                                          if (t2 != BOOLAUX){ 
                                                                           fprintf(stderr, "Error: no match type.\n");
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                        }else{ 
                                                                          if(!(t1 == BOOLAUX || t1 == OPER_LOG || t1 == OPER_REL)){
                                                                            fprintf(stderr, "Error: invalid expression.\n");
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                        }
                                                                        call->left = $3;
                                                                        call->right = $6;
                                                                        $$ = call;
                                                                      }
      | WHILE expr block                                              {call *node = insertTree("WHILEAUX",0,WHILEAUX);
                                                                      /*Chequeo de tipos, en caso de ser una funcion debe retornar un booleano
                                                                        en caso contrario, debe ser de tipo bool, op_log, op_rel */
                                                                        int t1 = $2->content->type;
                                                                        if (t1 == FUNCTION_CALL){
                                                                          int t2 = ((call->content)->function)->ret;
                                                                          if (t2 != BOOLAUX){ 
                                                                           fprintf(stderr, "Error: no match type.\n");
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                        }else{ 
                                                                          if(!(t1 == BOOLAUX || t1 == OPER_LOG || t1 == OPER_REL)){
                                                                            fprintf(stderr, "Error: invalid expression.\n");
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                        }
                                                                        call->left = $2;
                                                                        call->right = $3;
                                                                        $$ = call;
                                                                      }
      | RETURN expr SEMICOLON                                         {call *node = insertTree("RETURN_EXPR",0,RETURN_EXPR);
                                                                        call->left = $2;
                                                                        $$ = call;
                                                                      }
      | RETURN SEMICOLON                                              {call *node = insertTree("RETURNAUX",0,RETURNAUX);
                                                                        $$ = call;
                                                                      }
      | SEMICOLON                                                     {}                                                              
      | block                                                         {$$=$1}
    ;

statements: statement                                                 {$$=$1}
      | statements statement                                          {call *node = insertTree("STATEMENTS",0,STATEMENTS);
                                                                        call->left= $1;
                                                                        call->right = $2;
                                                                        $$ = call;
                                                                      }
    ;

method_call : ID PAR_LEFT {paramNo=0} paramscall PAR_RIGHT            { call *node;
                                                                        call = $4;
                                                                        (call->content)->function = searchFuntion($1->value);
                                                                        (call->content)->name = $1->value;
                                                                        size = paramNo;
                                                                        paramNo = 0;
                                                                        bool error = false;
                                                                        int t1;
                                                                        while (paramNo<=size || !error){
                                                                          t1 = call->callFunnc[paramNo]->type;
                                                                          if (((call->content)->function)->params[paramNo] == NULL){
                                                                            error = true;
                                                                        }
                                                                        if (t1 == FUNCTION_CALL && !error){
                                                                          t1 = ((call->content)->function)->ret;
                                                                        }
                                                                        if (((call->content)->function)->params[paramNo]=INTEGERAUX && !error)
                                                                        {                                                       
                                                                          error = (t1 == OPER_AR || t1 == INTEGERAUX);
                                                                        }else{
                                                                          error = (t1 == OPER_LOG || t1 == BOOLAUX || t1 == OPER_REL);
                                                                        }

                                                                        paramNo++;
                                                                        }
                                                                        if (((call->content)->function)->params[paramNo] != NULL){
                                                                          error = true;
                                                                        }
                                                                        if (!error){
                                                                          $$ = call;
                                                                        }
                                                                      }
      | ID PAR_LEFT PAR_RIGHT                                         {
                                                                        call *node;
                                                                        call=insertTree($1->value,0,FUNCTION_CALL);
                                                                        (call->content)->function = searchFuntion($1->value);
                                                                        /* if  ((call->content)->function->cantParams == 0){
                                                                          $$ == call;
                                                                          }else{
                                                                            $$ = insertTree ("ERROR",0,ERROR);
                                                                            fprintf(stderr, "Error: no match type\n");
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                        */
                                                                      }
    ;

paramscall : expr                                         {
                                                          call *node;
                                                          call=insertTree("CALLFUNCTION",0,FUNCTION_CALL);
                                                          call->callFunnc[paramNo]=$1;
                                                          paramNo++;
                                                          $$=call;
                                                          }
      | paramscall COMMA expr                             {                                                            
                                                          call = $1;
                                                          call->callFunnc[paramNo]=$3;
                                                          paramNo++;
                                                          $$=call;
                                                          }
    ;

expr : ID                                                 { $$ = insertTree ($1->value,0,VAR);}
      | method_call                                       { $$ = $1;}
      | literal                                           { $$ = $1;}
      | expr OP_ADD expr                                  { /*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = ((call->content)->function)->ret;
                                                            }
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              node *father;
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
      | expr OP_SUB expr                                  {/*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = ((call->content)->function)->ret;
                                                            }
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              node *father;
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
      | expr OP_PROD expr                                 { /*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = ((call->content)->function)->ret;
                                                            }
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              node *father;
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
      | expr OP_DIV expr                                  { /*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = ((call->content)->function)->ret;
                                                            }
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              node *father;
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
      | expr OP_MOD expr                                  {/*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = ((call->content)->function)->ret;
                                                            }
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              node *father;
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
      | expr OP_MINOR expr                                {/*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = ((call->content)->function)->ret;
                                                            }
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              node *father;
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
      | expr OP_MAJOR expr                                {/*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = ((call->content)->function)->ret;
                                                            }
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX) && (t2 == OPER_AR || t2 == INTEGERAUX)){
                                                              node *father;
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
      | expr OP_EQUAL expr                                {/*Chequeo de tipos, en caso de ser una funcion debe retornar integer o bool
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = ((call->content)->function)->ret;
                                                            }
                                                            if((t1 == OPER_AR || t1 == INTEGERAUX || t1 == BOOLAUX) && (t2 == OPER_AR || t2 == INTEGERAUX || t2 == BOOLAUX)){
                                                              node *father;
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
      | expr OP_AND expr                                  {/*Chequeo de tipos, en caso de ser una funcion debe retornar bool
                                                            en caso contrario, debe ser un oper_log, oper_rel o bool */
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = ((call->content)->function)->ret;
                                                            }
                                                            if((t1 == OPER_LOG || t1 == BOOLAUX || t1 == OPER_REL) && (t2 == OPER_LOG || t2 == BOOLAUX ||  t2 == OPER_REL)){
                                                              node *father;
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
      | expr OP_OR expr                                   {/*Chequeo de tipos, en caso de ser una funcion debe retornar bool
                                                            en caso contrario, debe ser un oper_log, oper_rel o bool */
                                                            int t1 = $1->content->type;
                                                            int t2 = $3->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = ((call->content)->function)->ret;
                                                            }
                                                            if((t1 == OPER_LOG || t1 == BOOLAUX || t1 == OPER_REL) && (t2 == OPER_LOG || t2 == BOOLAUX ||  t2 == OPER_REL)){
                                                              node *father;
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
      | OP_SUB expr %prec NEG                             { /*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un oper_ar o integer*/
                                                            int t1 = $2->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if(t1 == OPER_AR || t1 == INTEGERAUX){
                                                              node *father;
                                                              father = insertTree ("OP_SUB",0,OPER_AR);
                                                              concatLeft(father,$2);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | OP_NOT expr %prec NEG                             { /*Chequeo de tipos, en caso de ser una funcion debe retornar bool
                                                            en caso contrario, debe ser un oper_log, oper_rel o bool */
                                                            int t1 = $2->content->type;
                                                            if (t1 == FUNCTION_CALL){
                                                                t1 = ((call->content)->function)->ret;
                                                            }
                                                            if(t1 == OPER_LOG || t1 == BOOLAUX || t1 == OPER_REL){
                                                              node *father;
                                                              father = insertTree ("OP_NOT",0,OPER_LOG);
                                                              concatLeft(father,$2);
                                                              $$ = father;
                                                            }else{
                                                              //$$ = insertTree ("ERROR",0,ERROR);
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | PAR_LEFT expr PAR_RIGHT                           {$$ = $2;} /* Ignora parentesis */
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
