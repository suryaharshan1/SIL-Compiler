%{ 

#include <stdio.h> 
#include <stdlib.h>

%} 

%token DIGIT 
%left '+'
%left '*'

%% 
start : expr '\n'		{printf("\nComplete , ans is %d\n",$$);exit(1);} 
	; 

expr:  expr '+' expr		{$$ = $1 + $3;printf("+ ");} 
	| expr '*' expr		{$$ = $1 * $3;printf("* ");} 
	| '(' expr ')'	 	{$$ = $2; }
	| DIGIT			{$$ = $1;printf("%d ",$1);} 
	; 

%% 
yyerror() 
{ 
	printf("Error"); 
} 

yylex() 
{ 
	int c; 
	c = getchar(); 
	if(isdigit(c)) 
	{ 
		yylval = c - '0'; 
		return DIGIT; 
		 
	} 
	return c; 
} 

main() 
{ 
	yyparse(); 
	return 1; 
} 

