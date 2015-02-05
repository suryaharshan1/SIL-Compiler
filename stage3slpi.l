%option noyywrap
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "y.tab.h"
%}
%%

[a-z] {yylval.aname = yytext[0];return ID;}
[0-9]+ {yylval.ival = atoi(yytext);return NUM;}
[=+*();\n-/] {return *yytext;}
read {return READ;}
write {return WRITE;}
%%