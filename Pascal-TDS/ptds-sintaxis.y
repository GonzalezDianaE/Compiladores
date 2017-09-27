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
int yyparse();
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

prog:  PROGRAM BEGINN var_decls SEMICOLON method_decls END            {   printf("programa var_decl ; method_decl ; \n");}
      | PROGRAM BEGINN method_decls END                               {   printf("programa method_decl\n");}
      | PROGRAM BEGINN var_decls SEMICOLON END                        {   printf("programa var_decl\n");}
      | PROGRAM BEGINN END                                            {   printf("programa BEGINN END\n");}
    ;

var_decl : type ID                                                    {   insertTable ($2->value,0,VAR,$1);}
      | var_decl COMMA ID                                             {    insertTable ($3->value,0,VAR,typeLastVar());}
    ;

var_decls : var_decl                                                  {   printf("var_decl\n");}
      | var_decls SEMICOLON var_decl                                  {   printf("var_decls var_decl\n");}
    ;

method_decl : type ID PAR_LEFT param PAR_RIGHT block                   {  insertFunction($2->value, 0, FUNCTION, $1, $4,$6);}
      | VOID ID PAR_LEFT param PAR_RIGHT block                         {  insertFunction($2->value, 0, FUNCTION, $1, $4,$6);}
      | type ID PAR_LEFT PAR_RIGHT block                               {  paramsList *params = (paramsList *) malloc(sizeof(paramsList));
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
                                                                          $$ = params;
                                                                        }
      | param COMMA type ID                                             {
                                                                          addParamList($1,$3,$4->value);
                                                                          $$=$1;
                                                                        }
    ;

block: {openLevel();} blockAux                                         {closeLevel();
                                                                        node *call = insertTree("BLOCK",0,BLOCK,INDETERMINATE,yylineno);
                                                                        concatLeft (call,$2);
                                                                        }
    ;


blockAux: BEGINN var_decls SEMICOLON statements END                    {$$=$4;}
      | BEGINN statements END                                          {$$=$2;}
      | BEGINN var_decls SEMICOLON END                                 {printf("bloque var_decl\n");}
      | BEGINN END                                                     {printf("bloque BEGINN END\n");}
    ;

type : INTEGER                                                         {$$ = INTEGERAUX;}
      | BOOL                                                           {$$ = BOOLAUX;}
    ;

statement : ID OP_ASS expr SEMICOLON                                  { node *call = insertTree($1->value,0,ASSIGN,($3->content)->ret,yylineno);
                                                                        concatLeft (call,$3);
                                                                        $$=call;
                                                                      }
      | method_call SEMICOLON                                         { $$=$1;}
      | IF PAR_LEFT expr PAR_RIGHT THEN block ELSE block              {
                                                                        /*Chequeo de tipos, en caso de ser una funcion debe retornar un booleano
                                                                        en caso contrario, debe ser de tipo bool, op_log, op_rel */
                                                                        int t1 = $3->content->ret;
                                                                        if(!(t1 == BOOLAUX)){
                                                                            fprintf(stderr, "Error: invalid expression.\n");
                                                                            exit(EXIT_FAILURE);

                                                                        }else{
                                                                          node *call = insertTree("IFAUX",0,IFAUX,INDETERMINATE,yylineno);
                                                                          concatLeft(call,$3);//call->left = $3;
                                                                          concatMiddle(call,$6);//call->middle = $6;
                                                                          concatRight(call,$8);//call->right = $8;
                                                                          $$ = call;
                                                                        }
                                                                      }
      | IF PAR_LEFT expr PAR_RIGHT THEN block                         {
                                                                      /*Chequeo de tipos, en caso de ser una funcion debe retornar un booleano
                                                                        en caso contrario, debe ser de tipo bool, op_log, op_rel */
                                                                        int t1 = $3->content->ret;
                                                                        if(!(t1 == BOOLAUX)){
                                                                            fprintf(stderr, "Error: invalid expression.\n");
                                                                            exit(EXIT_FAILURE);
                                                                        }else{
                                                                          node *call = insertTree("IF_ELSE",0,IF_ELSE,INDETERMINATE,yylineno);
                                                                          concatLeft(call,$3);//call->left = $3;
                                                                          concatRight(call,$6);//call->right = $6;
                                                                          $$ = call;
                                                                        }
                                                                      }
      | WHILE expr block                                              {
                                                                      /*Chequeo de tipos, en caso de ser una funcion debe retornar un booleano
                                                                        en caso contrario, debe ser de tipo bool, op_log, op_rel */
                                                                        int t1 = $2->content->ret;
                                                                        if(!(t1 == BOOLAUX)){
                                                                          fprintf(stderr, "Error: invalid expression.\n");
                                                                          exit(EXIT_FAILURE);
                                                                        }else{
                                                                        node *call = insertTree("WHILEAUX",0,WHILEAUX,INDETERMINATE,yylineno);
                                                                        concatLeft(call,$2);//call->left = $2;
                                                                        concatRight(call,$3);//call->right = $3;
                                                                        $$ = call;
                                                                        }
                                                                      }
      | RETURN expr SEMICOLON                                         {node *call = insertTree("RETURN_EXPR",0,RETURN_EXPR,($2->content)->ret,yylineno);
                                                                        concatLeft(call,$2);//call->left = $2;
                                                                        $$ = call;
                                                                      }
      | RETURN SEMICOLON                                              {node *call = insertTree("RETURNAUX",0,RETURNAUX,VOIDAUX,yylineno);
                                                                        $$ = call;
                                                                      }
      | SEMICOLON                                                     {}
      | block                                                         {$$=$1;}
    ;

statements: statement                                                 {$$=$1;}
      | statements statement                                          {node *call = insertTree("STATEMENTS",0,STATEMENTS,INDETERMINATE,yylineno);
                                                                        concatLeft(call,$1);//call->left= $1;
                                                                        concatRight(call,$2);//call->right = $2;
                                                                        $$ = call;
                                                                      }
    ;

method_call : ID PAR_LEFT {openLevel();} params_call PAR_RIGHT          {
                                                                        item *func = searchFunction($1->value);
                                                                        if (func == NULL){
                                                                          fprintf(stderr, "Error: no encontro funcion\n");
                                                                          exit(EXIT_FAILURE);
                                                                        }
                                                                        paramsList pl = (func->function)->params;
                                                                        paramsCall pc = *($4);
                                                                        if (checkParams (pl,pc)){
                                                                          node *call;
                                                                          call=insertTree($1->value,0,FUNCTION_CALL_P,func->ret,yylineno);
                                                                          call->content->function= func->function;
                                                                          call->content->paramsCall = &pc;
                                                                          //concatLeft(call,insertParams(pl,pc));
                                                                          //concatRight(call,(func->function)->tree);
                                                                          $$=call;
                                                                        }else{
                                                                          fprintf(stderr, "Error: parametros incorrectos\n");
                                                                           exit(EXIT_FAILURE);
                                                                        }
                                                                      }
      | ID PAR_LEFT PAR_RIGHT                                         {
                                                                        node *call;
                                                                        item *func = searchFunction($1->value);
                                                                        if(((func->function)->params).paramsNo==0){
                                                                          node *call;
                                                                          call=insertTree($1->value,0,FUNCTION_CALL_NP,func->ret,yylineno);
                                                                          concatLeft(call,(func->function)->tree);
                                                                          $$ = call;
                                                                        }else{
                                                                          //$$ = insertTree ("ERROR",0,ERROR);
                                                                          fprintf(stderr, "Error: parametros incorrectos\n");
                                                                          exit(EXIT_FAILURE);
                                                                        }
                                                                        closeLevel();
                                                                      }
    ;

params_call : expr                                        {
                                                            paramsCall *params = (paramsCall *) malloc(sizeof(paramsCall));;
                                                            addParamCall(params,$1);
                                                            $$ = params;
                                                          }
      | params_call COMMA expr                            {
                                                            addParamCall($1,$3);
                                                            $$ = $1;
                                                          }
    ;

expr : ID                                                 { $$ = insertTree ($1->value,0,VAR,0,yylineno);}
      | method_call                                       { $$ = $1;}
      | literal                                           { $$ = $1;}

      | expr OP_ADD expr                                  { /*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int r1 = ($1->content)->ret;
                                                            int r2 = ($3->content)->ret;
                                                            if((r1 == INTEGERAUX) && (r2 == INTEGERAUX)){
                                                              node *father;
                                                              father = insertTree ("OP_ADD",0,OPER_AR,INTEGERAUX,yylineno);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                           }
                                                          }
      | expr OP_SUB expr                                  {/*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int r1 = ($1->content)->ret;
                                                            int r2 = ($3->content)->ret;
                                                            if((r1 == INTEGERAUX) && (r2 == INTEGERAUX)){
                                                              node *father;
                                                              father = insertTree ("OP_SUB",0,OPER_AR,INTEGERAUX,yylineno);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_PROD expr                                 { /*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int r1 = ($1->content)->ret;
                                                            int r2 = ($3->content)->ret;
                                                            if((r1 == INTEGERAUX) && (r2 == INTEGERAUX)){
                                                              node *father;
                                                              father = insertTree ("OP_PROD",0,OPER_AR,INTEGERAUX,yylineno);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_DIV expr                                  { /*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int r1 = ($1->content)->ret;
                                                            int r2 = ($3->content)->ret;
                                                            if((r1 == INTEGERAUX) && (r2 == INTEGERAUX)){
                                                              node *father;
                                                              father = insertTree ("OP_DIV",0,OPER_AR,INTEGERAUX,yylineno);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_MOD expr                                  {/*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int r1 = ($1->content)->ret;
                                                            int r2 = ($3->content)->ret;
                                                            if((r1 == INTEGERAUX) && (r2 == INTEGERAUX)){
                                                              node *father;
                                                              father = insertTree ("OP_MOD",0,OPER_AR,INTEGERAUX,yylineno);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_MINOR expr                                {/*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int r1 = ($1->content)->ret;
                                                            int r2 = ($3->content)->ret;
                                                            if((r1 == INTEGERAUX) && (r2 == INTEGERAUX)){
                                                              node *father;
                                                              father = insertTree ("OP_MINOR",0,OPER_REL,BOOLAUX,yylineno);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_MAJOR expr                                {/*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un operacion aritmetica o integer */
                                                            int r1 = ($1->content)->ret;
                                                            int r2 = ($3->content)->ret;
                                                            if((r1 == INTEGERAUX) && (r2 == INTEGERAUX)){
                                                              node *father;
                                                              father = insertTree ("OP_MAJOR",0,OPER_REL,BOOLAUX,yylineno);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_EQUAL expr                                {/*Chequeo de tipos, en caso de ser una funcion debe retornar integer o bool
                                                            en caso contrario, debe ser un operacion aritmetica, integer o bool */
                                                            int r1 = ($1->content)->ret;
                                                            int r2 = ($3->content)->ret;
                                                            int t1 = ($1->content)->type;
                                                            int t2 = ($3->content)->type;
                                                            bool intCont = ((r1 == INTEGERAUX && r2 == INTEGERAUX));
                                                            bool boolCont = ((r1 == BOOLAUX) && (r2 == BOOLAUX));
                                                            bool boolContType1 = (t1 == CONSTANT || t1 == VAR || t1==FUNCTION_CALL_NP || t1==FUNCTION_CALL_P);
                                                            bool boolContType2 = (t2 == CONSTANT || t2 == VAR || t2==FUNCTION_CALL_NP || t2==FUNCTION_CALL_P);
                                                            if( intCont || (boolCont && boolContType1 && boolContType2)){
                                                              node *father;
                                                              father = insertTree ("OP_EQUAL",0,OPER_REL,BOOLAUX,yylineno);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_AND expr                                  {/*Chequeo de tipos, en caso de ser una funcion debe retornar bool
                                                            en caso contrario, debe ser un oper_log, oper_rel o bool */
                                                            int r1 = ($1->content)->ret;
                                                            int r2 = ($3->content)->ret;
                                                            if((r1 == BOOLAUX) && (r2 == BOOLAUX)){
                                                              node *father;
                                                              father = insertTree ("OP_AND",0,OPER_LOG,BOOLAUX,yylineno);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | expr OP_OR expr                                   {/*Chequeo de tipos, en caso de ser una funcion debe retornar bool
                                                            en caso contrario, debe ser un oper_log, oper_rel o bool */
                                                            int r1 = ($1->content)->ret;
                                                            int r2 = ($3->content)->ret;
                                                            if((r1 == BOOLAUX) && (r2 == BOOLAUX)){
                                                              node *father;
                                                              father = insertTree ("OP_OR",0,OPER_LOG,BOOLAUX,yylineno);
                                                              concatLeft(father,$1);
                                                              concatRight(father,$3);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | OP_SUB expr %prec NEG                             { /*Chequeo de tipos, en caso de ser una funcion debe retornar integer
                                                            en caso contrario, debe ser un oper_ar o integer*/
                                                            int r1 = $2->content->ret;
                                                            if(r1 == INTEGERAUX){
                                                              node *father;
                                                              father = insertTree ("OP_SUB",0,OPER_AR,INTEGERAUX,yylineno);
                                                              concatLeft(father,$2);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | OP_NOT expr %prec NEG                             { /*Chequeo de tipos, en caso de ser una funcion debe retornar bool
                                                            en caso contrario, debe ser un oper_log, oper_rel o bool */
                                                            int r1 = $2->content->ret;
                                                            if(r1 == BOOLAUX){
                                                              node *father;
                                                              father = insertTree ("OP_NOT",0,OPER_LOG,BOOLAUX,yylineno);
                                                              concatLeft(father,$2);
                                                              $$ = father;
                                                            }else{
                                                              fprintf(stderr, "Error: no match type\n");
                                                              exit(EXIT_FAILURE);
                                                            }
                                                          }
      | PAR_LEFT expr PAR_RIGHT                           {$$ = $2;} /* Ignora parentesis */
    ;

literal : integer_literal                                 {$$ = insertTree("int_lit",$1,CONSTANT,INTEGERAUX,yylineno);}
      | bool_literal                                      {$$ = insertTree("bool_lit",$1,CONSTANT,BOOLAUX,yylineno);}
    ;

integer_literal : INT                                     {$$ = $1->value;}
    ;

bool_literal : TRUE                                       {$$ = TRUE;}
      | FALSE                                             {$$ = FALSE;}
    ;

%%
