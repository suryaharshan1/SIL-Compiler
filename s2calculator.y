%{ 

	#include <stdio.h> 
	#include <stdlib.h>
	#include "exprtree.h"
	int yylex(void);
	extern FILE *yyin;

struct Node * root;

%} 

%union{
	int ival;
	struct Node * nptr;
};

%token NUM;
%type <ival> NUM;
%type <nptr> expr;
%left '+' '-'
%left '*' '/'

%% 
start : expr '\n'		{root=$1;printf("\nComplete , ans is %d\n",evaluate($1));exit(1);} 
	; 

expr:  expr '+' expr		{$$ = mkOperatorNode('+',$1,$3);printf("+ ");} 
	| expr '*' expr		{$$ = mkOperatorNode('*',$1,$3);printf("* ");} 
	| expr '-' expr		{$$ = mkOperatorNode('-',$1,$3);printf("- ");}
	| expr '/' expr		{$$ = mkOperatorNode('/',$1,$3);printf("/ ");}
	| '(' expr ')'	 	{$$ = $2; }
	| NUM		 	{$$ = mkLeafNode($1);printf("%d  ",yylval.ival);}
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

