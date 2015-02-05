%option noyywrap
%{
	#include <stdlib.h>
	#include <stdio.h>
	int letter;
	FILE *fp;
%}

digit [0-9];
lower [a-z];
upper [A-Z];
alphabet [a-zA-Z];
 

%%
[a-z] 	{ 	
		printf("%c",yytext[0]-32);
		fprintf(yyout,"%c",toupper(yytext[0]));
      	}
[A-Z] 	{
		printf("%c",yytext[0]);		
		fprintf(yyout,"%c",yytext[0]);
      	
	}


%%
int main(int argc,char* argv[]){
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
