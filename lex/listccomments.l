%option noyywrap
%{
	#include <stdlib.h>
	#include <stdio.h>
	
%}

digit [0-9]
lower [a-z]
upper [A-Z]
alphabet [a-zA-Z]
%x IN_COMMENT 

%%
<INITIAL>{
	"/*" BEGIN(IN_COMMENT);
	"//"[^\n]*	printf("%s",yytext + 2);
	.    ;
}
<IN_COMMENT>{
	"*/" BEGIN(INITIAL);
	[^*\n]+ printf("%s",yytext);
	"*"	printf("*");
}
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
