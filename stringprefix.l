%{ 
	#include <stdio.h> 
	#include "y.tab.h"
	extern YYSTYPE yylval;
%} 

%% 

[0-9]+	{strcpy(yylval.prefix,yytext);return NUM;}
[+*()\n]  {strcpy(yylval.prefix,yytext);return *yytext;}
[ \t] ;
. {printf("Unknown character\n");exit(1);}

%% 

int yywrap()
{
	return 1;
}
      
