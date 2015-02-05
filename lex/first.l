%%
	int k;
[0-9]+  { 
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
