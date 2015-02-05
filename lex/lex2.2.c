%option noyywrap
%{
	#include <stdlib.h>
	#include <stdio.h>
	int num,r,i,digit,count=0,pcount=0;
	char a[20];
%}

digit [0-9];
lower [a-z];
upper [A-Z];
alphabet [a-zA-Z];
 

%%

[0-9]+ {
	num = atoi(yytext);
	while(num!=0){
		r = num % 16;
		digit = '0' + r;
		if(digit>'9')
			{
			digit += 7;}
		a[count++] = digit;
		num = num /16;
		}
	 for(i=count-1;i>=pcount;--i)
                printf("%c",a[i]);
                pcount=count;
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
