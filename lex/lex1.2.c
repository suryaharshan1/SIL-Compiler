%option noyywrap
%{
	#include <stdlib.h>
	#include <stdio.h>
	
	int count=0,words=0,chars=0;

%}

digit [0-9];
lower [a-z];
upper [A-Z];
alphabet [a-zA-Z];
 

%%
[A-za-z]+ {words++;chars+=yyleng;}
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
	printf("%d\n",words);
	printf("%d\n",chars);
	
	return 1;
}
