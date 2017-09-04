%{

#include <stdlib.h>
#include <stdio.h>

%}
 
%union { int i; char *s; struct tree *t;}
 
%token<i> INT
%token<s> ID
%token<s> VARIABLE
%token<s> OP_ADD
%token<s> OP_SUB
%token<s> OP_PROD
%token<s> OP_DIV
%token<s> OP_PORC
%token<s> OP_MINOR
%token<s> OP_MAJOR
%token<s> OP_EQUAL
%token<s> OP_AND
%token<s> OP_OR
%token<s> TRUE
%token<s> FALSE

%type<t> expr
%type<i> isVar
 
%left '+' 
%left '*'
 
%%

%%