%{
/*** Auxiliary declarations section ***/

#include<stdio.h>
#include<stdlib.h>

/* Custom function to print an operator*/
void op_print(char op);

/* Variable to keep track of the position of the number in the input */
int pos=0;

%}

 /*** YACC Declarations section ***/
%token DIGIT
%left '+'
%left '*'
%%

/*** Rules Section ***/
start : expr '\n'		{exit(1);} 
	; 

expr:  expr '+' expr	{op_print('+');} 
	| expr '*' expr	{op_print('*');} 
	| '(' expr ')'	
	| DIGIT		{printf("NUM%d ",pos);} 
	; 

%% 

/*** Auxiliary functions section ***/

void op_print(char op)
{
	if(op == '+')
		printf("PLUS ");
	else if(op == '*')
		printf("MUL ");
}
	
yyerror() 
{ 
	printf("error");	
	return; 
} 

yylex() 
{ 
	int c; 
	c = getchar(); 
	if(isdigit(c)) 
	{ 
		pos++;		 
		return DIGIT; 
	}
	else
		return c; 
}

main() 
{ 
	yyparse(); 
	return 1; 
}

