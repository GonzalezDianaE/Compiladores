%{

#include <stdlib.h>
#include <stdio.h>
#include "list.c"

%}
 
%union { int i; char *s; struct tree *t;}
 
%token<i> INT
%token<s> ID
%token<s> VARIABLE

%type<t> expr
%type<i> isVar
 
%left '+' 
%left '*'
 
%%
 
prog:  isVar ';' expr ';'           { //printf("Variable y expresion. Resultado %d\n",$3);
                                      show();
                                      printf("Tree: \n");
                                      showTree($3);
                                      printf("\n");
                                      printf("Result: %d\n",eval($3));
                                    }
      | expr ';'                    { //printf("%s%d\n", "Resultado: ",$1);
                                      printf("Tree: \n");
                                      showTree($1);
                                    }
    ;

isVar : VARIABLE ID '=' INT             { insert($2,$4,VAR);
                                          printf("%s = %d \n",$2,$4);
                                        }
      | isVar ';' VARIABLE ID '=' INT   { insert($4,$6,VAR);
                                          printf("%s = %d \n",$4,$6);
                                        }

expr: INT               { //$$ = $1;
                          $$ = insertTree("",$1,CONSTANT);
                          printf("%s%d\n","Constant:",$1);
                        }
    | expr '+' expr     { //$$ = $1 + $3;
                          head = insertTree("+",0,OPER);
                          concatLeft(head,$1);
                          concatRight(head,$3);
                          $$ = head;
                          //printf("%s,%d,%d,%d\n","Operador Suma",$1,$3,$1+$3);
                        }
    | expr '*' expr     { //$$ = $1 * $3;
                          head = insertTree("*",0,OPER);
                          concatLeft(head,$1);
                          concatRight(head,$3);
                          $$ = head;
                          //printf("%s,%d,%d,%d\n","Operador Producto",$1,$3,$1*$3);  
                        }
    | '(' expr ')'      { //$$ =  $2;
                          head = insertTree("()",0,OPER);
                          concatLeft(head,$2);
                          $$ = head;
                        }

    | ID                { //$$ = 1;
                          $$ = insertTree($1,0,VAR);
                          printf("%s %s\n","Variable: ",$1);
                        }
    ;
%%


