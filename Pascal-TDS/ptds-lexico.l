%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "types.c"
#include "ptds-sintaxis.tab.h"

void error(const char *s);
void yyerror();

%}
%x comment
%option noyywrap
%option yylineno

letter [a-zA-z]
digit [0-9]

%%

\{                            { BEGIN(comment);}
<comment>[^}\n]*              { }
<comment>\}                   { BEGIN(INITIAL);}
<comment><<EOF>>              { error("Unexpected eof");
                                yyterminate();}

program                       { yylval.i = yylineno;
                                return PROGRAM;}

begin                         { yylval.i = yylineno;
                                return BEGINN;}

end                           { yylval.i = yylineno;
                                return END;}

return                        { yylval.i = yylineno;
                                return RETURN;}

bool                          { yylval.i = yylineno;
                                return BOOL;}

integer                       { yylval.i = yylineno;
                                return INTEGER;}

void                          { yylval.i = yylineno;
                                return VOID;}

while                         { yylval.i = yylineno;
                                return WHILE;}

if                            { yylval.i = yylineno;
                                return IF;}

then                          { yylval.i = yylineno;
                                return THEN;}

else                          { yylval.i = yylineno;
                                return ELSE;}

true                          { yylval.i = yylineno;
                                return TRUE;}

false                         { yylval.i = yylineno;
                                return FALSE;}

\=                            { yylval.i = yylineno;
                                return OP_ASS;}

\+                            { yylval.i = yylineno;
                                return OP_ADD;}

\-                            { yylval.i = yylineno;
                                return OP_SUB;}

\*                            { yylval.i = yylineno;
                                return OP_PROD;}

\/                            { yylval.i = yylineno;
                                return OP_DIV;}

\%                            { yylval.i = yylineno;
                                return OP_MOD;}

\<                            { yylval.i = yylineno;
                                return OP_MINOR;}

\>                            { yylval.i = yylineno;
                                return OP_MAJOR;}

\=\=                          { yylval.i = yylineno;
                                return OP_EQUAL;}

\&\&                          { yylval.i = yylineno;
                                return OP_AND;}

\|\|                          { yylval.i = yylineno;
                                return OP_OR;}

\!                            { yylval.i = yylineno;
                                return OP_NOT;}

\(                            { yylval.i = yylineno;
                                return PAR_LEFT;}

\)                            { yylval.i = yylineno;
                                return PAR_RIGHT;}

\,                            { yylval.i = yylineno;
                                return COMMA;}

\;                            { yylval.i = yylineno;
                                return SEMICOLON;}

{digit}+                      { yylval.tv = (struct tokensVal *) malloc(sizeof(struct tokensVal));
                                char* aux;
                                yylval.tv->value=strtol(yytext,&aux,10);
                                //yylval.tv->value=atol(yytext);
                                yylval.tv->line = yylineno;
                                if(yylval.tv->value!=0 || strcmp(yytext,"0")==0){
                                  printf("%ld\n",yylval.tv->value);
                                  return INT;
                                } else {
                                  error("Out of range");
                                  yyterminate();
                                }
                              }

{letter}({letter}|{digit})*   { yylval.ts = (tokenStr *) malloc(sizeof(tokenStr));
                                strcpy(yylval.ts->value,yytext);
                                yylval.ts->line = yylineno;
                                return ID;}

(\n)+                         {}

[ \t]                         {}

"//".*\n                      {}

.                             { error("Unrecognized input");
                                yyterminate();};

%%

void error(const char *s){
  printf("Error: %s - Line: %d\n",s,yylineno);
}

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
