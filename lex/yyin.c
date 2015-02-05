%{
	#include <stdlib.h>
	#include <stdio.h>
%}
%%
	
	
[0-9]+  {
	int k; 
	k = atoi(yytext);
	if(k%7 == 0)
		printf("%d",k+3);
	else
		printf("%d",k);
	}
%%
int yywrap(){
	return 1;
}

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
