%{
#include "parser.tab.h"
#include "driver.h"

extern int yylex();
extern int yyerror(char *s);
%}

%option noyywrap
%option always-interactive

ws ([ \t])+
alphabet [a-zA-Z_][0-9a-zA-Z_]*
number [-]?([0-9])+

%%

"ADD" { return ADD;}
"SUBTRACT" {return SUB;}
"MULTIPLY" {return MUL;}
"DIVIDE" {return DIV;}
"LEFT" { return LEFT;}
"RIGHT" {return RIGHT;}
"UP" {return UP;}
"DOWN" {return DOWN;}
"ASSIGN"    {return ASSIGN;}
"TO"    {return TO;}
"VAR"   {return VAR;}
"IS"    {return IS;}
"VALUE" {return VALUE;}
"IN"    {return IN;}
"exit"  {return EXIT;}
"."     {return DOT;}
"\n"     {return NEWLINE;}
","     {return COMMA;}
{ws}      ;
{number}     {yylval.num = atoi(yytext); return NUM; }
{alphabet}      {yylval.str = strdup(yytext); return ID; }
.          yyerror("Unknown character");

%%