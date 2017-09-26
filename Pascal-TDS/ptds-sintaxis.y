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


%union { int i; char *s; struct tokensVal *tv; struct tokensStr *ts; struct tree *t; struct paramsLists *pl; struct paramsCalls *pc;}

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
%type<pl> param
%type<pc> params_call
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
      | var_decl COMMA ID                                             {insertTable ($3->value,0,typeLastVar());}
    ;

var_decls : var_decl                                                  {printf("var_decl\n");}
      | var_decls SEMICOLON var_decl                                  {printf("var_decls var_decl\n");}
    ;

method_decl : type ID PAR_LEFT param PAR_RIGHT block                   {insertFunction($2->value, 0, FUNCTION, $1, $4,$6);}
      | VOID ID PAR_LEFT param PAR_RIGHT block                         {insertFunction($2->value, 0, FUNCTION, $1, $4,$6);}
      | type ID PAR_LEFT PAR_RIGHT block                               {paramsList *params = (paramsList *) malloc(sizeof(paramsList));
                                                                        (params->paramsNo) = 0;
                                                                        insertFunction($2->value, 0, FUNCTION, $1, params,$5);}
      | VOID ID PAR_LEFT PAR_RIGHT block                               { paramsList *params = (paramsList *) malloc(sizeof(paramsList));
                                                                        (params->paramsNo) = 0;
                                                                        insertFunction($2->value, 0, FUNCTION, $1, params,$5);}
     ;

method_decls : method_decl                                             {printf("method_decl\n");}
      | method_decls method_decl                                       {printf("method_decls recursive\n");}
     ;

param : type ID                                                         {
                                                                          paramsList *params = (paramsList *) malloc(sizeof(paramsList));
                                                                          (params->paramsNo) = 0;
                                                                          addParamList(params,$1,$2->value);
                                                                          //insertTable();
                                                                          $$ = params;
                                                                        }
      | param COMMA type ID                                             {
                                                                          addParamList($1,$3,$4->value);
                                                                          $$=$1;
                                                                        }
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

statement : ID OP_ASS expr SEMICOLON                                  { node *call = insertTree($1->value,0,ASSIGN);
                                                                        concatLeft (call,$3);
                                                                        $$=call;
                                                                      }
      | method_call SEMICOLON                                         { $$=$1;}
      | IF PAR_LEFT expr PAR_RIGHT THEN block ELSE block              { node *call = insertTree("IFAUX",0,IFAUX);
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
      | IF PAR_LEFT expr PAR_RIGHT THEN block                         {node *call = insertTree("IF_ELSE",0,IF_ELSE);
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
      | WHILE expr block                                              {node *call = insertTree("WHILEAUX",0,WHILEAUX);
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
      | RETURN expr SEMICOLON                                         {node *call = insertTree("RETURN_EXPR",0,RETURN_EXPR);
                                                                        call->left = $2;
                                                                        $$ = call;
                                                                      }
      | RETURN SEMICOLON                                              {node *call = insertTree("RETURNAUX",0,RETURNAUX);
                                                                        $$ = call;
                                                                      }
      | SEMICOLON                                                     {}
      | block                                                         {$$=$1;}
    ;

statements: statement                                                 {$$=$1;}
      | statements statement                                          {node *call = insertTree("STATEMENTS",0,STATEMENTS);
                                                                        call->left= $1;
                                                                        call->right = $2;
                                                                        $$ = call;
                                                                      }
    ;

method_call : ID PAR_LEFT params_call PAR_RIGHT                       { node *call;
                                                                        call=insertTree($1->value,0,FUNCTION_CALL);
                                                                        paramsCall pc = *($3);
                                                                        itemFunc *func = searchFunction($1->value);
                                                                        int size = pc.paramsNo;
                                                                        int i = 0;
                                                                        bool error = false;
                                                                        int t1;
                                                                        if(size!=(func->params).paramsNo){
                                                                          //tirar error
                                                                        } else {
                                                                          while (i<size && !error){
                                                                            t1 = ((pc.params[i])->content)->type;
                                                                            if (t1 == FUNCTION_CALL){
                                                                              t1 = ((pc.params[i]->content)->function)->ret;
                                                                            }
                                                                            if (((func->params).params[i]).type == INTEGERAUX){
                                                                              error = (t1 == OPER_AR || t1 == INTEGERAUX);
                                                                            }else{
                                                                              error = (t1 == OPER_LOG || t1 == BOOLAUX || t1 == OPER_REL);
                                                                            }
                                                                            i++;
                                                                          }
                                                                          if (!error){
                                                                            (call->content)->params = pc;
                                                                            (call->content)->function = func;
                                                                            $$ = call;
                                                                          } else {
                                                                            // tirar error
                                                                          }
                                                                        }
                                                                      }
      | ID PAR_LEFT PAR_RIGHT                                         {
                                                                        node *call;
                                                                        call=insertTree($1->value,0,FUNCTION_CALL);
                                                                        itemFunc *func = searchFunction($1->value);
                                                                        if((func->params).paramsNo==0){
                                                                          (call->content)->function = func;
                                                                          $$ = call;
                                                                        }else{
                                                                          //$$ = insertTree ("ERROR",0,ERROR);
                                                                          //fprintf(stderr, "Error: no match type\n");
                                                                          //exit(EXIT_FAILURE);
                                                                        }
                                                                      }
    ;

params_call : expr                                        {
                                                            paramsCall *params = (paramsCall *) malloc(sizeof(paramsCall));;
                                                            addParamCall(params,$1);
                                                            //insertTable();
                                                            $$ = params;
                                                          }
      | params_call COMMA expr                            {
                                                            addParamCall($1,$3);
                                                            $$ = $1;
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
                                                                t1 = (($1->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = (($3->content)->function)->ret;
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
                                                                t1 = (($1->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = (($3->content)->function)->ret;
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
                                                                t1 = (($1->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = (($3->content)->function)->ret;
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
                                                                t1 = (($1->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = (($3->content)->function)->ret;
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
                                                                t1 = (($1->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = (($3->content)->function)->ret;
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
                                                                t1 = (($1->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = (($3->content)->function)->ret;
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
                                                                t1 = (($1->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = (($3->content)->function)->ret;
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
                                                                t1 = (($1->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = (($3->content)->function)->ret;
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
                                                                t1 = (($1->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = (($3->content)->function)->ret;
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
                                                                t1 = (($1->content)->function)->ret;
                                                            }
                                                            if (t2 == FUNCTION_CALL){
                                                                t2 = (($3->content)->function)->ret;
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
                                                                t1 = (($2->content)->function)->ret;
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
                                                                t1 = (($2->content)->function)->ret;
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
