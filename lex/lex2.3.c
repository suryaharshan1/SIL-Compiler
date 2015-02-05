%option noyywrap
%{
	#include<stdio.h>
%}

%%
//for single comments

\/\/.* ;

//for mutliple line comments
\/\*(.*\n)*.*\*\/ ;

%%

int main(int argc,char* argv[])
{
	if(argc>1)
	{
		fp = fopen(argv[1],"r");
		if(fp)
			{yyin = fp;
			yyout = fopen("out.txt","w");}
	}
	yylex();
	return 1;
}


