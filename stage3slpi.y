%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    int yylex();
    extern FILE *yyin;
    int* value[26];
%}

%union{
	int ival;
	char aname;
};

%token NUM ID READ WRITE
%type <ival> NUM;
%type <aname> ID;
%type <ival> expr;
%type <ival> slist;
%type <ival> stmt;
%type <ival> READ;
%type <ival> WRITE;
%left '+' '-'
%left '*' '/'

%%
start : slist '\n' {exit(1);}
    ;

slist : slist stmt    {}
    | stmt    {}
    ;

stmt : ID '=' expr ';' {
							if(value[$1 - 'a'] == NULL){
								value[$1 - 'a'] = malloc(sizeof(int*));
							}
							*value[$1 - 'a'] = $3;
						}
		| READ '(' ID ')' ';' {
								if(value[$3 - 'a'] == NULL){
									value[$3 - 'a'] = malloc(sizeof(int*));
								}
								scanf("%d",value[$3 - 'a' ]);
							}
		| WRITE '(' expr ')' ';' {printf("%d\n",$3);}
		;

expr:  expr '+' expr       {$$ = $1 + $3;}
    | expr '*' expr        {$$ = $1 * $3;}
    | expr '-' expr        {$$ = $1 - $3;}
    | expr '/' expr        {$$ = $1 / $3;}
    | '(' expr ')'         {$$ = $2;}
    | NUM             		{$$ = $1;}
    | ID             {    if(value[$1 - 'a'] == NULL){
                            printf("error\n");
                   }
                    else{
                        $$ = *value[$1 - 'a'];
                    }
               }
    ;

%%

yyerror(){
    printf("error\n");
}

int main(int argc,char* argv[]){
    if(argc > 1){
        FILE *fp = fopen(argv[1],"r");
        if(fp)
            yyin = fp;
	}
	yyparse();
	int i;
	for(i=0;i<26;i++){
		free(value[i]);
	}
	return 1;
}

