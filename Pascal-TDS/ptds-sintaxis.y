%{

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "types.c"
#include "assemblyCode.c"
extern int yylineno;

%}

%{
int yylex();
void yyerror(const char *s);
int yyparse();
int getFlag();
int getOS();
int returnCount = 0;
int flag;
int os;
int functionVariables = 0;
%}


%union { int i; char *s; struct tokensVal *tv; struct tokensStr *ts; struct tree *t; struct symbolTable *pl; struct paramsCalls *pc;}

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
%token<i> PRINT
%token<i> COMENT
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
program:
  { flag = getFlag();
    os = getOS();
    if(flag==1){
      printf("Comiezo chequeo Léxico/Sintáctico\n");
    }
    openLevel();
  }
  prog
  { if(flag != 5 && flag!=6){
      if(flag==1){
        printf("Comiezo generación Código Assembler\n");
      }
      generateFile(flag);
      generateAssembly(getIntermediateCode(),flag, os);
      if(flag==1){
        printf("  Código Assembler generado correctamente \n\n");
      }
    }
    printf("\n                            Compilación Exitosa                             \n\n");
  }
;

prog:   PROGRAM BEGINN var_decls SEMICOLON method_decls END              { if(flag==1){
                                                                            printf("  Chequeo Léxico/Sintáctico correcto \n\n");
                                                                          }
                                                                          if(flag != 5){
                                                                            if(flag == 1){
                                                                              printf("Comienzo chequeo Semántico\n");
                                                                            }
                                                                            checks(levels[0],flag);
                                                                            goOver(levels[0]);
                                                                            if(flag == 1){
                                                                              printf("  Chequeo Semántico correcto\n\n");
                                                                              printf("Comienzo generación Código Intermedio\n");
                                                                            }
                                                                            initListThreeDir(flag);
                                                                            symbol *aux = levels[0];
                                                                            while (aux->next!=NULL){
                                                                              aux=aux->next;
                                                                              if(aux->content->type != VAR){
                                                                                generate(aux->content);
                                                                              }
                                                                            }
                                                                            if(flag==1){
                                                                              printf("  Código Intermedio generado correctamente\n\n");
                                                                            }
                                                                            if(flag == 2 || flag==6){
                                                                              showOperation();
                                                                            }
                                                                          }
                                                                        }

      | PROGRAM BEGINN method_decls END                                 { if(flag==1){
                                                                            printf("  Chequeo Léxico/Sintáctico correcto \n\n");
                                                                          }
                                                                          if(flag != 5){
                                                                            if(flag == 1){
                                                                              printf("Comienzo chequeo Semántico\n");
                                                                            }
                                                                            checks(levels[0],flag);
                                                                            goOver(levels[0]);
                                                                            if(flag == 1){
                                                                              printf("  Chequeo Semántico correcto\n\n");
                                                                              printf("Comienzo generación Código Intermedio\n");
                                                                            }
                                                                            symbol *aux = levels[0];
                                                                            initListThreeDir(flag);
                                                                            while (aux->next!=NULL){
                                                                              aux=aux->next;
                                                                              generate(aux->content);
                                                                            }
                                                                            if(flag==1){
                                                                              printf("  Código Intermedio generado correctamente\n\n");
                                                                            }
                                                                            if(flag == 2 || flag==6){
                                                                              showOperation();
                                                                            }
                                                                          }
                                                                        }

      | PROGRAM BEGINN var_decls SEMICOLON END                          {}

      | PROGRAM BEGINN END                                              {}
    ;

var_decl : type ID                                                      { functionVariables++; insertTable ($2->value,0,VAR,$1,true,functionVariables,flag);}
      
      | var_decl COMMA ID                                               { functionVariables++; insertTable ($3->value,0,VAR,typeLastVar(),true,functionVariables,flag);}
    ;

var_decls : var_decl                                                    {}

			| var_decls SEMICOLON var_decl                                    {}
		;

method_decl : 
      type ID PAR_LEFT param PAR_RIGHT block                            { if (returnCount==0) {
                                                                            fprintf(stderr, "Error: Missing return statement for %s function.\n", $2->value);
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                          returnCount = 0;
                                                                          insertFunction($2->value, 0, FUNCTION, $1, $4,$6,functionVariables,flag);
                                                                          functionVariables = 0;
                                                                          closeLevel();
                                                                        }

      | VOID ID PAR_LEFT param PAR_RIGHT block                          { returnCount = 0;
                                                                          insertFunction($2->value, 0, FUNCTION, VOIDAUX, $4,$6,functionVariables,flag);
                                                                          functionVariables = 0;
                                                                          closeLevel();
                                                                        }

      | type ID PAR_LEFT PAR_RIGHT block                                { if (returnCount==0) {
                                                                            fprintf(stderr, "Error: Missing return statement for %s function.\n", $2->value);
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                          returnCount = 0;
                                                                          symbol *head = initParamCall();
                                                                          insertFunction($2->value, 0, FUNCTION, $1, head,$5,functionVariables,flag);
                                                                          functionVariables = 0;
                                                                        }

      | VOID ID PAR_LEFT PAR_RIGHT block                                { returnCount = 0;
                                                                          symbol *head = initParamCall();
                                                                          insertFunction($2->value, 0, FUNCTION, VOIDAUX, head,$5,functionVariables,flag);
                                                                          functionVariables = 0;
                                                                        }
     ;

method_decls : method_decl                                              {}

			| method_decls method_decl                                        {}
		 ;

param : type ID                                                         { openLevel();
                                                                          insertTable($2->value,0,PARAMETER,$1,false,functionVariables,flag);
                                                                          $$ = levels[top];
                                                                        }

      | param COMMA type ID                                             { insertTable($4->value,0,PARAMETER,$3,false,functionVariables,flag);
                                                                          $$ = $1;
                                                                        }
    ;

block: {openLevel();} blockAux                                          { closeLevel();
                                                                          node *call = insertTree("BLOCK",0,BLOCK,INDETERMINATE,yylineno,flag);
                                                                          concatLeft (call,$2);
                                                                          $$=call;
                                                                        }
    ;

blockAux: BEGINN var_decls SEMICOLON statements END                     { $$=$4;}

			| BEGINN statements END                                           { $$=$2;}

			| BEGINN var_decls SEMICOLON END                                  { node *aux = insertVoidNode(yylineno);
																																					$$ = aux;
																																				}

			| BEGINN END                                                      { node *aux = insertVoidNode(yylineno);
																																					$$ = aux;
																																				}
		;

type : INTEGER                                                          { $$ = INTEGERAUX;}

			| BOOL                                                            { $$ = BOOLAUX;}
		;


statement : ID OP_ASS expr SEMICOLON                                    { node *var = insertTree ($1->value,0,VAR,0,yylineno,flag);
                                                                          node *call = insertTree($1->value,0,ASSIGN,($3->content)->ret,yylineno,flag);
                                                                          concatLeft (call,$3);
                                                                          concatRight (call,var);
                                                                          $$=call;
                                                                        }

      | method_call SEMICOLON                                           { $$=$1;}

      | IF PAR_LEFT expr PAR_RIGHT THEN block ELSE block                { node *call = insertTree("IF_ELSE",0,IF_ELSE,INDETERMINATE,yylineno,flag);
                                                                          concatLeft(call,$3);
                                                                          concatMiddle(call,$6);
                                                                          concatRight(call,$8);
                                                                          $$ = call;
                                                                        }

      | IF PAR_LEFT expr PAR_RIGHT THEN block                           { node *call = insertTree("IFAUX",0,IFAUX,INDETERMINATE,yylineno,flag);
                                                                          concatLeft(call,$3);
                                                                          concatRight(call,$6);
                                                                          $$ = call;                                                                        
                                                                        }

      | WHILE expr block                                                { node *call = insertTree("WHILEAUX",0,WHILEAUX,INDETERMINATE,yylineno,flag);
                                                                          concatLeft(call,$2);
                                                                          concatRight(call,$3);
                                                                          $$ = call;                                                                       
                                                                        }

      | RETURN expr SEMICOLON                                           { returnCount++;
                                                                          node *call = insertTree("RETURN_EXPR",0,RETURN_EXPR,($2->content)->ret,yylineno,flag);
                                                                          concatLeft(call,$2);
                                                                          $$ = call;
                                                                        }

      | RETURN SEMICOLON                                                { node *call = insertTree("RETURNAUX",0,RETURNAUX,VOIDAUX,yylineno,flag);
                                                                          $$ = call;
                                                                        }

      | PRINT expr SEMICOLON                                           {  node *call = insertTree("PRINTS",0,PRINTAUX,($2->content)->ret,yylineno,flag);
                                                                          concatLeft(call,$2);
                                                                          $$ = call;}

      | SEMICOLON                                                       { node *aux = insertVoidNode(yylineno);
                                                                          $$ = aux;
                                                                        }

      | block                                                           { $$=$1;}
    ;

statements : statement                                                  { 
                                                                          node *call = insertTree("STATEMENTS",0,STATEMENTS,INDETERMINATE,yylineno,flag);
                                                                          concatLeft(call,$1);
                                                                          $$ = call;
                                                                        }

      | statements statement                                            { node *call = insertTree("STATEMENTS",0,STATEMENTS,INDETERMINATE,yylineno,flag);
                                                                          concatLeft(call,$1);
                                                                          concatRight(call,$2);
                                                                          $$ = call;
                                                                        }
    ;

method_call : ID PAR_LEFT params_call PAR_RIGHT                         { item *func = findFunction($1->value,flag);
                                                                          paramsCall *pc = ($3);
                                                                          if (func == NULL){
                                                                            fprintf(stderr, "Error: no encontro funcion\n");
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                          node *call;
                                                                          call=insertTree($1->value,0,FUNCTION_CALL_P,func->ret,yylineno,flag);
                                                                          call->content->function= func->function;
                                                                          call->content->paramsCall = pc;
                                                                          $$=call;
                                                                        }

      | ID PAR_LEFT PAR_RIGHT                                           { item *func = findFunction($1->value,flag);
                                                                          if (func == NULL){
                                                                            fprintf(stderr, "Error: no encontro funcion\n");
                                                                            exit(EXIT_FAILURE);
                                                                          }
                                                                          node *call;
                                                                          paramsCall *pc = (paramsCall *) malloc(sizeof(paramsCall));
                                                                          pc->next=NULL;
                                                                          call=insertTree($1->value,0,FUNCTION_CALL_P,func->ret,yylineno,flag);
                                                                          call->content->function= func->function;
                                                                          call->content->paramsCall = pc;
                                                                          $$=call;
                                                                        }
    ;

params_call : expr                                                      { paramsCall *head = (paramsCall *) malloc(sizeof(paramsCall));
                                                                          head->next=NULL;
                                                                          addParamCall(head,$1,flag);
                                                                          $$ = head;
                                                                        }

      | params_call COMMA expr                                          { addParamCall($1,$3,flag);
                                                                          $$ = $1;
                                                                        }
    ;

expr : ID                                                 { $$ = insertTree ($1->value,0,VAR,0,yylineno,flag);}


      | method_call                                       { $$ = $1;}

      | literal                                           { $$ = $1;}

      | expr OP_ADD expr                                  { node *father;
                                                            father = insertTree ("OP_ADD",0,OPER_AR,INTEGERAUX,yylineno,flag);
                                                            concatLeft(father,$1);
                                                            concatRight(father,$3);
                                                            $$ = father;
                                                          }

      | expr OP_SUB expr                                  { node *father;
                                                            father = insertTree ("OP_SUB",0,OPER_AR,INTEGERAUX,yylineno,flag);
                                                            concatLeft(father,$1);
                                                            concatRight(father,$3);
                                                            $$ = father;
                                                          }

      | expr OP_PROD expr                                 { node *father;
                                                            father = insertTree ("OP_PROD",0,OPER_AR,INTEGERAUX,yylineno,flag);
                                                            concatLeft(father,$1);
                                                            concatRight(father,$3);
                                                            $$ = father;                                                        
                                                          }

      | expr OP_DIV expr                                  { node *father;
                                                            father = insertTree ("OP_DIV",0,OPER_AR,INTEGERAUX,yylineno,flag);
                                                            concatLeft(father,$1);
                                                            concatRight(father,$3);
                                                            $$ = father;
                                                          }

      | expr OP_MOD expr                                  { node *father;
                                                            father = insertTree ("OP_MOD",0,OPER_AR,INTEGERAUX,yylineno,flag);
                                                            concatLeft(father,$1);
                                                            concatRight(father,$3);
                                                            $$ = father;                                                            
                                                          }

      | expr OP_MINOR expr                                { node *father;
                                                            father = insertTree ("OP_MINOR",0,OPER_REL,BOOLAUX,yylineno,flag);
                                                            concatLeft(father,$1);
                                                            concatRight(father,$3);
                                                            $$ = father;
                                                          }
      | expr OP_MAJOR expr                                { node *father;
                                                            father = insertTree ("OP_MAJOR",0,OPER_REL,BOOLAUX,yylineno,flag);
                                                            concatLeft(father,$1);
                                                            concatRight(father,$3);
                                                            $$ = father;
                                                          }

      | expr OP_EQUAL expr                                { node *father;
                                                            father = insertTree ("OP_EQUAL",0,OPER_EQUAL,BOOLAUX,yylineno,flag);
                                                            concatLeft(father,$1);
                                                            concatRight(father,$3);
                                                            $$ = father;
                                                          }

      | expr OP_AND expr                                  { node *father;
                                                            father = insertTree ("OP_AND",0,OPER_LOG,BOOLAUX,yylineno,flag);
                                                            concatLeft(father,$1);
                                                            concatRight(father,$3);
                                                            $$ = father;
                                                          }

      | expr OP_OR expr                                   { node *father;
                                                            father = insertTree ("OP_OR",0,OPER_LOG,BOOLAUX,yylineno,flag);
                                                            concatLeft(father,$1);
                                                            concatRight(father,$3);
                                                            $$ = father;
                                                          }

      | OP_SUB expr %prec NEG                             { node *father;
                                                            father = insertTree ("OP_SUB",0,OPER_AR_UN,INTEGERAUX,yylineno,flag);
                                                            concatLeft(father,$2);
                                                            $$ = father;
                                                          }

      | OP_NOT expr %prec NEG                             { node *father;
                                                            father = insertTree ("OP_NOT",0,OPER_LOG_UN,BOOLAUX,yylineno,flag);
                                                            concatLeft(father,$2);
                                                            $$ = father;
                                                          }

      | PAR_LEFT expr PAR_RIGHT                           { $$ = $2;}
    ;

literal : integer_literal                                 { $$ = insertTree("int_lit",$1,CONSTANT,INTEGERAUX,yylineno,flag);}
      
      | bool_literal                                      { $$ = insertTree("bool_lit",$1,CONSTANT,BOOLAUX,yylineno,flag);}
    ;

integer_literal : INT                                     { $$ = $1->value;}
		;

bool_literal : TRUE                                       { $$ = 1;}

			| FALSE                                             { $$ = 0;}
		;

%%
