%{ 

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
//#include "lex.yy.c"
int yylex();
extern FILE *yyin;
%} 
%union{
	char prefix[100];
};

%token NUM
%type <prefix> NUM;
%type <prefix> expr; 
%left '+'
%left '*'

%% 
start : expr '\n'	{printf("\nComplete , ans is %s\n",$1);exit(1);}
	; 

expr:  expr '+' expr		{
							strcpy($$,"+ ");
							strcat($$,strcat($1,$3));	
							} 
	| expr '*' expr		{
						strcpy($$,"* ");
						strcat($$,strcat($1,$3));} 
	| '(' expr ')'	 	{strcpy($$,$2);}
	| NUM		 	{strcpy($$,strcat($1," "));}
	; 


%% 

yyerror() 
{ 
	printf("Error"); 
} 

int main(int argc,char* argv[]) 
{ 
	if(argc>1)
	{
		FILE *fp = fopen(argv[1],"r");
		if(fp)
			yyin = fp;
	}	
	yyparse(); 
	return 1; 
} 

