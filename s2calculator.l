%{ 
	#include <stdio.h> 
	#include "y.tab.h"
	#include <stdlib.h>
%} 

%% 

[0-9]+		{yylval.ival = atoi(yytext);return NUM;}	/*return the number*/
[-+*()\n/]  	{return *yytext;}	/*\n indicates newline , and set of operations , and parenthesis */	
[ \t] 		;	/*ignore whitespaces*/
. 		{printf("Unknown character");exit(1);}

%% 

int yywrap()
{
	return 1;
}
      
