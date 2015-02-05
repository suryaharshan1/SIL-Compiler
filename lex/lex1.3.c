%option noyywrap
%{
	#include <stdlib.h>
	#include <stdio.h>
	
	int deccount=0,intcount=0;

%}

digit [0-9];
lower [a-z];
upper [A-Z];
alphabet [a-zA-Z];
 

%%
[0-9]+"."[0-9]+	{deccount++;}
[0-9]+	{intcount++;}
[\n] ;
%%

int main(int argc,char* argv[]){
	if(argc>1)
	{
		FILE *fp = fopen(argv[1],"r");
		if(fp)
			yyin = fp;
	}
	yylex();
	printf("intcount = %d\n",intcount);
	printf("deccount = %d\n",deccount);
	return 1;
}
