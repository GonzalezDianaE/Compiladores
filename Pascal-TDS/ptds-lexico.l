%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ptds-sintaxis.tab.h"

%}

%option noyywrap
%option yylineno

letter [a-zA-z]
digit [0-9]

%%

program                       { printf("Program\n");
                                return PROGRAM;}

begin                         { printf("Begin\n");
                                return BEGINN;}

end                           { printf("End\n");
                                return END;}

return                        { printf("Return\n");
                                return RETURN;}

bool                          { printf("Bool Type\n");
                                return BOOL;}

integer                       { printf("Integer Type\n");
                                return INTEGER;}

void                          { printf("Void\n");
                                return VOID;}

while                         { printf("While\n");
                                return WHILE;}

if                            { printf("If\n");
                                return IF;}

then                          { printf("Then\n");
                                return THEN;}

else                          { printf("Else\n");
                                return ELSE;}

true                          { printf("True\n");
                                return TRUE;}

false                         { printf("False\n");
                                return FALSE;}

\+                            { printf("Addition\n");
                                return OP_ADD;}

\-                            { printf("Subtraction\n");
                                return OP_SUB;}

\*                            { printf("Product\n");
                                return OP_PROD;}

\/                            { printf("Division\n");
                                return OP_DIV;}

\%                            { printf("Module\n");
                                return OP_MOD;}

\<                            { printf("Minor\n");
                                return OP_MINOR;}

\>                            { printf("Major\n");
                                return OP_MAJOR;}

\=\=                          { printf("Equal\n");
                                return OP_EQUAL;}

\&\&                          { printf("And\n");
                                return OP_AND;}

\|\|                          { printf("Or\n");
                                return OP_OR;}

\!                            { printf("Not\n");
                                return OP_NOT;}

{digit}+                      { yylval.i = atoi(yytext);
                                if(yylval.i!=0 || strcmp(yytext,"0")==0){
                                  printf("INT : %d\n",atoi(yytext));
                                  return INT;
                                }
                              }

{letter}({letter}|{digit})*   { yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("ID : %s\n",yytext);
                                return ID;}

[;()]                         { printf("%s\n",yytext);
                                return *yytext;}


(\n|\b|\t)+                   { }

"//".*\n|\{.*\}               { }

.                             { };

%%

void yyerror(){
	printf("%s%d\n","-> SINTAX ERROR - line: ",yylineno);
}

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;

	yyparse();

}
