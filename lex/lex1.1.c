%option noyywrap
%{
	#include <stdlib.h>
	#include <stdio.h>
	
	int count=0;

%}

digit [0-9];
lower [a-z];
upper [A-Z];
alphabet [a-zA-Z];
 

%%
[\n] count++;
[^\n] ;
%%

int main(int argc,char* argv[]){
	if(argc>1)
	{
		FILE *fp = fopen(argv[1],"r");
		if(fp)
			yyin = fp;
	}
	yylex();
	printf("%d\n",count);
	return 1;
}
