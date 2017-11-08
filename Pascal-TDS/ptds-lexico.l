%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "types.c"
#include "ptds-sintaxis.tab.h"

void error(const char *s);
void yyerror();
int getFlag();
int getOS();
int flag;
int os;

%}
%x comment
%option noyywrap
%option yylineno

letter [a-zA-z]
digit [0-9]

%%

\{                            { BEGIN(comment);
                                if(flag==4){
                                    printf("Found 'Begin comment' at line %d\n",yylineno);
                                }
                              }
<comment>[^}]*                { if(flag==4){
                                    printf("    %s",yytext);
                                }
                              }
<comment>\}                   { BEGIN(INITIAL);
                                if(flag==4){
                                    printf("Found 'End comment' at line %d\n",yylineno);
                                }  
                              }
<comment><<EOF>>              { error("Unexpected eof");
                                yyterminate();}

print                         { yylval.i = yylineno;
                                return PRINT;}

program                       { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'program' at line %d\n",yylineno);
                                }
                                return PROGRAM;}

begin                         { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'begin' at line %d\n",yylineno);
                                }
                                return BEGINN;}

end                           { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'end' at line %d\n",yylineno);
                                }
                                return END;}

return                        { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'return' at line %d\n",yylineno);
                                }
                                return RETURN;}

bool                          { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'bool' at line %d\n",yylineno);
                                }
                                return BOOL;}

integer                       { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'integer' at line %d\n",yylineno);
                                }
                                return INTEGER;}

void                          { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'void' at line %d\n",yylineno);
                                }
                                return VOID;}

while                         { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'while' at line %d\n",yylineno);
                                }
                                return WHILE;}

if                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'if' at line %d\n",yylineno);
                                }
                                return IF;}

then                          { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'then' at line %d\n",yylineno);
                                }
                                return THEN;}

else                          { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'else' at line %d\n",yylineno);
                                }
                                return ELSE;}

true                          { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'true' at line %d\n",yylineno);
                                }
                                return TRUE;}

false                         { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found 'false' at line %d\n",yylineno);
                                }
                                return FALSE;}

\=                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '=' at line %d\n",yylineno);
                                }
                                return OP_ASS;}

\+                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '+' at line %d\n",yylineno);
                                }
                                return OP_ADD;}

\-                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '-' at line %d\n",yylineno);
                                }
                                return OP_SUB;}

\*                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '*' at line %d\n",yylineno);
                                }
                                return OP_PROD;}

\/                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '/' at line %d\n",yylineno);
                                }
                                return OP_DIV;}

\%                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '%%' at line %d\n",yylineno);
                                }
                                return OP_MOD;}

\<                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '<' at line %d\n",yylineno);
                                }
                                return OP_MINOR;}

\>                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '>' at line %d\n",yylineno);
                                }
                                return OP_MAJOR;}

\=\=                          { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '==' at line %d\n",yylineno);
                                }
                                return OP_EQUAL;}

\&\&                          { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '&&' at line %d\n",yylineno);
                                }
                                return OP_AND;}

\|\|                          { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '||' at line %d\n",yylineno);
                                }
                                return OP_OR;}

\!                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '!' at line %d\n",yylineno);
                                }
                                return OP_NOT;}

\(                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found '(' at line %d\n",yylineno);
                                }
                                return PAR_LEFT;}

\)                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found ')' at line %d\n",yylineno);
                                }
                                return PAR_RIGHT;}

\,                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found ',' at line %d\n",yylineno);
                                }
                                return COMMA;}

\;                            { yylval.i = yylineno;
                                if(flag==4){
                                    printf("Found ';' at line %d\n",yylineno);
                                }
                                return SEMICOLON;}

{digit}+                      { yylval.tv = (tokenVal *) malloc(sizeof(tokenVal));
                                char* aux;
                                long auxLong;
                                auxLong = strtol(yytext,&aux,10);;
                                yylval.tv->line = yylineno;
                                auxLong = strtol(yytext,&aux,10);
                                if((auxLong>=INT_MIN) && (auxLong<=INT_MAX)){
                                  yylval.tv->value = (int) auxLong;
                                  if(flag==4){
                                    printf("Found 'Int: %d' at line %d\n",atoi(yytext),yylineno);
                                  }
                                  return INT;
                                } else {
                                  error("Out of range");
                                  yyterminate();
                                }
                              }

{letter}({letter}|{digit})*   { yylval.ts = (tokenStr *) malloc(sizeof(tokenStr));
                                strcpy(yylval.ts->value,yytext);
                                yylval.ts->line = yylineno;
                                if(flag==4){
                                    printf("Found 'Id: %s' at line %d\n",yytext,yylineno);
                                }
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

int getFlag(){
    return flag;
}

int getOS(){
    return os;
}

int main(int argc,char *argv[]){
    switch ((argv[1])[0]){
        case '0' :
            flag = 0;
            break;
        case '1' :
            flag = 1;
            break;
        case '2' :
            flag = 2;
            break;
        case '3' :
            flag = 3;
            break;
        case '4' :
            flag = 4;
            break;
        case '5' :
            flag = 5;
            break;
        case '6' :
            flag = 6;
            break;
        case '7' :
            flag = 7;
            break;
    }

    switch ((argv[2])[0]){
        case '0':
            os = 0;
            break;
        case '1':
            os = 1;
            break;
    }

	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;
    yyparse();

}
