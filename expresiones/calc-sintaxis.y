%{

#include <stdlib.h>
#include <stdio.h>

%}
 
%union { int i; char *s; }
 
%token<i> INT
%token<s> ID
%token<s> VAR

%type<i> expr
%type<i> isVar
 
%left '+' 
%left '*'
 
%%
 
prog:  isVar ';' expr ';'          {printf("Variable y expresion. Resultado %d\n",$3);}
      | expr ';'          { printf("%s%d\n", "Resultado: ",$1); } 
    ;

isVar : VAR ID '=' INT            {printf("Solo una variable\n");}
      | isVar ';' VAR ID '=' INT  {printf("Varias variables\n");}

expr: INT               { $$ = $1; 
                           printf("%s%d\n","Constante entera:",$1);
                        }
    | expr '+' expr     { $$ = $1 + $3; 
                          printf("%s,%d,%d,%d\n","Operador Suma",$1,$3,$1+$3);
                        }
    | expr '*' expr     { $$ = $1 * $3; 
                          printf("%s,%d,%d,%d\n","Operador Producto",$1,$3,$1*$3);  
                        }
    | '(' expr ')'      { $$ =  $2; }

    | ID                { $$ = 1;
                          printf("%s %s\n","Variable",$1);
                        }
    ;
%%


