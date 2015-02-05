%option noyywrap
%{
	#include <stdlib.h>
	#include <stdio.h>
%}

digit [0-9];
lower [a-z];
upper [A-Z];
alphabet [a-zA-Z];
 

%%


%%
int main(int argc,char* argv[]){
	if(argc>1)
	{
		FILE *fp = fopen(argv[1],"r");
		if(fp)
			yyin = fp;
	}
	yylex();
	return 1;
}
