%{

#include <stdlib.h>
#include <stdio.h>
#include "structures.c"

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
 
prog:  isVar ';' expr ';'           { // Variable list, followed by an expression.
                                      showList();
                                      printf("Tree: \n");
                                      showTree($3);
                                      printf("\n");
                                      printf("Result: %d\n",evalTree($3));
                                    }
      | expr ';'                    { // Expression without variables.
                                      printf("Tree: \n");
                                      showTree($1);
                                      printf("Result: %d\n",evalTree($1));
                                    }
    ;

isVar : VARIABLE ID '=' INT             {  // Base case (unique variable).
                                          insertList($2,$4,VAR);
                                          printf("%s = %d \n",$2,$4);
                                        }
      | isVar ';' VARIABLE ID '=' INT   { // Recursive variable's list.
                                          insertList($4,$6,VAR);
                                          printf("%s = %d \n",$4,$6);
                                        }

expr: INT               { // Numeric expression (constants).
                          $$ = insertTree("",$1,CONSTANT);
                          printf("%s%d\n","Constant:",$1);
                        }
    | expr '+' expr     { // Addition expression.
                          head = insertTree("+",0,OPER);
                          concatLeft(head,$1);
                          concatRight(head,$3);
                          $$ = head;
                        }
    | expr '*' expr     { // Multiplication expression.
                          head = insertTree("*",0,OPER);
                          concatLeft(head,$1);
                          concatRight(head,$3);
                          $$ = head;
                        }
    | '(' expr ')'      { // Expression (any type) between parenthesis.
                          head = insertTree("()",0,OPER);
                          concatLeft(head,$2);
                          $$ = head;
                        }

    | ID                { // Variable expression (look up the symbol's table).
                          $$ = insertTree($1,0,VAR);
                          printf("%s %s\n","Variable: ",$1);
                        }
    ;
%%


