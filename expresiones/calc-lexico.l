%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "calc-sintaxis.tab.h"

%}

%option noyywrap
%option yylineno

letra [a-zA-z]
digito [0-9]

%%

var 						{ yylval.s=(char *) malloc(3);
                                strcpy(yylval.s,yytext);
                                printf("VAR : %s\n",yytext);
                                return VAR;}

{digito}+                     { yylval.i = atoi(yytext);
                                printf("INT : %d\n",atoi(yytext)); 
                                return INT;}

{letra}({letra}|{digito})*    { yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("ID : %s\n",yytext);
                                return ID;}

[+*=;()]                   {  printf("%s\n",yytext);
                                  return *yytext;}

.|\n                          ; /* ignore all the rest */

%%

typedef struct items{
    String name;
    int value;
    int type;
} item;

typedef struct symbolTable{
    item *content;
    struct tablaDeSimbolos *next;
} symbol;

typedef struct tree{
    item *content;
    struct tree *left;
    struct tree *right;
} node;

void insert(String n, int v, int t){
    symbol *element;
    element = (symbol *) malloc(sizeof(symbol));
    item *content;
    content = (item *) malloc(sizeof(item));
    content->name = n;
    content->value = v;
    content->type = t;
    element->content = content;
    element->next = NULL;
    if(first==NULL){
        first = element;
        last = element;
    } else {
        last->next = element;
        last = element;
    }
}

void show(symbol first){
    symbol aux = first;
    if(aux!=NULL){
        printf("Showing list\n");
        while(aux !=  NULL){
            printf("Name: %s, value %d, type: %d\n",aux->name,aux->value,aux->type);
            aux = aux->next;
        }
    }else{
        printf("Empty list.\n");
    }
}

item * find(symbol first, string n, int t){
    symbol aux = first;
    if(aux!=NULL){
        while(aux !=  NULL && (aux->content)->name!=n && (aux->content)->type!=t){
            aux = aux->next;
        }
        if(aux == NULL){
            printf("Element not found");
            return NULL;
        } else {
            printf("Element found");
            return aux->content;
        }
    }else{
        printf("Empty list.\n");
    }
}


void yyerror(){
	printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;

	yyparse();

}
