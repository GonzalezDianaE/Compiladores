%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "types.c"
#include "ptds-sintaxis.tab.h"

%}
%x comment
%option noyywrap
%option yylineno

letter [a-zA-z]
digit [0-9]

%%

\{                            { BEGIN(comment);}
<comment>[^}\n]*              { printf("%s",yytext);}
<comment>\}                   { BEGIN(INITIAL);
                                printf("fin comentario\n");}

program                       { printf("Program\n");
                                yylval.i = yylineno;
                                return PROGRAM;}

begin                         { printf("Begin\n");
                                yylval.i = yylineno;
                                return BEGINN;}

end                           { printf("End\n");
                                yylval.i = yylineno;
                                return END;}

return                        { printf("Return\n");
                                yylval.i = yylineno;
                                return RETURN;}

bool                          { printf("Bool Type\n");
                                yylval.i = yylineno;
                                return BOOL;}

integer                       { printf("Integer Type\n");
                                yylval.i = yylineno;
                                return INTEGER;}

void                          { printf("Void\n");
                                yylval.i = yylineno;
                                return VOID;}

while                         { printf("While\n");
                                yylval.i = yylineno;
                                return WHILE;}

if                            { printf("If\n");
                                yylval.i = yylineno;
                                return IF;}

then                          { printf("Then\n");
                                yylval.i = yylineno;
                                return THEN;}

else                          { printf("Else\n");
                                yylval.i = yylineno;
                                return ELSE;}

true                          { printf("True\n");
                                yylval.i = yylineno;
                                return TRUE;}

false                         { printf("False\n");
                                yylval.i = yylineno;
                                return FALSE;}

\=                            { printf("Assignment\n");
                                yylval.i = yylineno;
                                return OP_ASS;}

\+                            { printf("Addition\n");
                                yylval.i = yylineno;
                                return OP_ADD;}

\-                            { printf("Subtraction\n");
                                yylval.i = yylineno;
                                return OP_SUB;}

\*                            { printf("Product\n");
                                yylval.i = yylineno;
                                return OP_PROD;}

\/                            { printf("Division\n");
                                yylval.i = yylineno;
                                return OP_DIV;}

\%                            { printf("Module\n");
                                yylval.i = yylineno;
                                return OP_MOD;}

\<                            { printf("Minor\n");
                                yylval.i = yylineno;
                                return OP_MINOR;}

\>                            { printf("Major\n");
                                yylval.i = yylineno;
                                return OP_MAJOR;}

\=\=                          { printf("Equal\n");
                                yylval.i = yylineno;
                                return OP_EQUAL;}

\&\&                          { printf("And\n");
                                yylval.i = yylineno;
                                return OP_AND;}

\|\|                          { printf("Or\n");
                                yylval.i = yylineno;
                                return OP_OR;}

\!                            { printf("Not\n");
                                yylval.i = yylineno;
                                return OP_NOT;}

\(                            { printf ("Left Parenthesis\n");
                                yylval.i = yylineno;
                                return PAR_LEFT;}

\)                            { printf ("Rigth Parenthesis\n");
                                yylval.i = yylineno;
                                return PAR_RIGHT;}

\,                            { printf ("Comma\n");
                                yylval.i = yylineno;
                                return COMMA;}

\;                            { printf ("Semicolon\n");
                                yylval.i = yylineno;
                                return SEMICOLON;}

{digit}+                      { //yylval.i = atoi(yytext);
                                yylval.tv = (struct tokensVal *) malloc(sizeof(struct tokensVal));
                                yylval.tv->value = atoi(yytext);
                                yylval.tv->line = yylineno;
                                if(yylval.i!=0 || strcmp(yytext,"0")==0){
                                  printf("INT : %d\n",atoi(yytext));
                                  return INT;
                                }
                              }

{letter}({letter}|{digit})*   { //yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                yylval.ts = (tokenStr *) malloc(sizeof(tokenStr));
                                strcpy(yylval.ts->value,yytext);
                                yylval.ts->line = yylineno;
                                //strcpy(yylval.s,yytext);
                                printf("ID : %s\n",yytext);
                                return ID;}

(\n)+                         { printf("Line break\n");}

[ \t]                         { printf("Spaces or Tabs\n");}

"//".*\n                      { printf("Short comment\n");}

.                             { printf("Error\n");};

%%

void yyerror(){
	printf("%s%d\n","-> SINTAX ERROR - line: ",yylineno);
    printf("%s\n",yytext);
}

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;

	yyparse();

}
