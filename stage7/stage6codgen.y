%{

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	int yylex(void);
	extern FILE *yyin;
	extern int yylineno;
	#include "abstracttrees6.h"
	#include "symboltables7.h"
	#include "symboltables7.c"
	#include "abstracttrees6.c"
	#include "codegens7.c"
	struct Tnode *root;
	int decl_type;
%}

%union{
	int ival;
	struct Tnode * nptr;
	char c;
};
 
%token ID NUM ASGN DELIM READ WRITE IF THEN ENDIF ELSE WHILE DO ENDWHILE PLUS MUL GT LT EQ NL ENDOFFILE COMMA INT BOOL TTRUE TFALSE DECL ENDDECL GE LE NE
%type <nptr> ID
%type <nptr> NUM
%type <nptr> READ
%type <nptr> WRITE
%type <nptr> IF
%type <nptr> THEN
%type <nptr> ENDIF
%type <nptr> ELSE
%type <nptr> WHILE
%type <nptr> DO
%type <nptr> ENDWHILE
%type <nptr> PLUS
%type <nptr> MUL
%type <nptr> GT
%type <nptr> LT
%type <nptr> EQ
%type <nptr> NL
%type <nptr> ENDOFFILE
%type <nptr> COMMA
%type <nptr> INT
%type <nptr> BOOL
%type <nptr> TTRUE
%type <nptr> TFALSE
%type <nptr> DECL
%type <nptr> ENDDECL
%type <nptr> slist
%type <nptr> stmt
%type <nptr> expr
%type <nptr> GE
%type <nptr> LE
%type <nptr> NE
%nonassoc GT LT EQ GE LE NE
%left PLUS
%left MUL

%%

start : declarations slist ENDOFFILE	{
											root = $2;
											printf("Evaluating the tree\n");
											//Evaluate($2);
											generate_code($2);
											exit(0);
										}
	;

declarations : DECL declist ENDDECL {}
	;

declist : declist dec   {}
	| dec   {}
	;

type : INT 	 { decl_type = STYPE_INT; }
	| BOOL 	 { decl_type = STYPE_BOOLEAN; }

dec : type idlist DELIM		{}
	;

idlist : idlist COMMA ID 	 {Ginstall($3->NAME,decl_type,1,NULL);}
	| idlist COMMA ID '[' NUM ']'	{
										switch(decl_type){
											case STYPE_INT:
												Ginstall($3->NAME,STYPE_ARR_INT,$5->VALUE,NULL);			
												break;
											case STYPE_BOOLEAN:
												Ginstall($3->NAME,STYPE_ARR_BOOLEAN,$5->VALUE,NULL);
												break;					
										}
									}
	| ID 	{Ginstall($1->NAME,decl_type,1,NULL);}
	| ID '[' NUM ']' 	{
							switch(decl_type){
								case STYPE_INT:
									Ginstall($1->NAME,STYPE_ARR_INT,$3->VALUE,NULL);			
									break;
								case STYPE_BOOLEAN:
									Ginstall($1->NAME,STYPE_ARR_BOOLEAN,$3->VALUE,NULL);
									break;					
							}
						}
	;

slist : slist stmt  	{$$ = TreeCreate(TYPE_VOID,NODETYPE_NONE,0,NULL,NULL,$1,$2,NULL);}
	| stmt      	{$$ = $1;}
	;

stmt  : ID ASGN expr DELIM  {
								Gtemp=Glookup($1->NAME);
								if(Gtemp==NULL){
									yyerror("undefined variable ");printf("%s",$1->NAME);exit(0);}	
								else if(Gtemp->type == STYPE_ARR_INT || Gtemp->type == STYPE_ARR_BOOLEAN){
									yyerror("invalid reference to an array ");
									printf("%s",$1->NAME);
									exit(0);
								}
								else $1->Gentry=Gtemp;
								$$ = TreeCreate(TYPE_VOID,NODETYPE_ASGN,0,NULL,NULL,$1,$3,NULL);
							}
	| ID '[' expr ']' ASGN expr DELIM   {	
											Gtemp=Glookup($1->NAME);
											if(Gtemp==NULL){
												yyerror("undefined variable ");printf("%s",$1->NAME);exit(0);}	
											else if(Gtemp->type == STYPE_INT || Gtemp->type == STYPE_BOOLEAN){
												yyerror("invalid reference to a variable ");
												printf("%s",$1->NAME);
												exit(0);		
											}
											else $1->Gentry=Gtemp;
											$$ = TreeCreate(TYPE_VOID,NODETYPE_ARR_ASGN,0,NULL,NULL,$1,$3,$6);
										}
	| READ '(' ID ')' DELIM         	{	
											Gtemp=Glookup($3->NAME);
											if(Gtemp==NULL){
												yyerror("undefined variable ");printf("%s",$3->NAME);exit(0);}	
											else if(Gtemp->type == STYPE_ARR_INT || Gtemp->type == STYPE_ARR_BOOLEAN){
												yyerror("invalid reference to an array ");
												printf("%s",$3->NAME);
												exit(0);
											}
											else 
												$3->Gentry=Gtemp;
											$$ = TreeCreate(TYPE_VOID,NODETYPE_READ,0,NULL,NULL,$3,NULL,NULL);
										}
	| READ '(' ID '[' expr ']' ')' DELIM 	{	
												Gtemp=Glookup($3->NAME);
												if(Gtemp==NULL){
													yyerror("undefined variable ");printf("%s",$3->NAME);exit(0);}	
												else if(Gtemp->type == STYPE_INT || Gtemp->type == STYPE_BOOLEAN){
													yyerror("invalid reference to a variable ");
													printf("%s",$3->NAME);
													exit(0);		
												}		
												else 
													$3->Gentry=Gtemp;
												$$ = TreeCreate(TYPE_VOID,NODETYPE_ARR_READ,0,NULL,NULL,$3,$5,NULL);
											}

	| WRITE '(' expr ')' DELIM      		{$$ = TreeCreate(TYPE_VOID,NODETYPE_WRITE,0,NULL,NULL,$3,NULL,NULL);}

	| IF '(' expr ')' THEN slist ENDIF DELIM	{$$ = TreeCreate(TYPE_VOID,NODETYPE_IF,0,NULL,NULL,$3,$6,NULL);}
	
	| IF '(' expr ')' THEN slist ELSE slist ENDIF DELIM	{$$ = TreeCreate(TYPE_VOID,NODETYPE_IF,0,NULL,NULL,$3,$6,$8);}
	
	| WHILE '(' expr ')' DO slist ENDWHILE DELIM	{$$ = TreeCreate(TYPE_VOID,NODETYPE_WHILE,0,NULL,NULL,$3,$6,NULL);}
	;

expr  : expr PLUS expr  	{$$ = TreeCreate(TYPE_INT,NODETYPE_PLUS,0,NULL,NULL,$1,$3,NULL);}
	| expr MUL expr 	{$$ = TreeCreate(TYPE_INT,NODETYPE_MUL,0,NULL,NULL,$1,$3,NULL);}
	| '(' expr ')'  	{$$ = $2;}
	| expr GT expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_GT,0,NULL,NULL,$1,$3,NULL);}
	| expr LT expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_LT,0,NULL,NULL,$1,$3,NULL);}
	| expr EQ expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_EQ,0,NULL,NULL,$1,$3,NULL);}
	| expr GE expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_GE,0,NULL,NULL,$1,$3,NULL);}
	| expr LE expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_LE,0,NULL,NULL,$1,$3,NULL);}
	| expr NE expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_NE,0,NULL,NULL,$1,$3,NULL);}
	| NUM           	{$$ = $1;}
	| ID            	{
							Gtemp=Glookup($1->NAME);
							if(Gtemp==NULL){
								yyerror("undefined variable ");
								printf("%s",$1->NAME);
								exit(0);
							}
							else if(Gtemp->type == STYPE_ARR_INT || Gtemp->type == STYPE_ARR_BOOLEAN){
								yyerror("invalid reference to an array ");
								printf("%s",$1->NAME);
								exit(0);
							}
							else {
								$1->Gentry=Gtemp;
								if(Gtemp->type == STYPE_INT){
									$1->TYPE = TYPE_INT;
								}
								else if(Gtemp->type == STYPE_BOOLEAN){
									$1->TYPE = TYPE_BOOLEAN;	
								}
								$$=$1;
							}
						}
	| ID '[' expr ']'   	{	
								Gtemp=Glookup($1->NAME);
								if(Gtemp==NULL){
									printf("undefined variable ");
									printf("%s",$1->NAME);
									exit(0);
								}
								else if(Gtemp->type == STYPE_INT || Gtemp->type == STYPE_BOOLEAN){
									yyerror("invalid reference to a variable ");
									printf("%s",$1->NAME);
									exit(0);		
								}
								else { 
									$1->Gentry=Gtemp;
									if(Gtemp->type == STYPE_ARR_INT){
										$1->TYPE = TYPE_INT;
									}
									else if(Gtemp->type == STYPE_ARR_BOOLEAN){
										$1->TYPE = TYPE_BOOLEAN;	
									}
									$$ = TreeCreate($1->TYPE,NODETYPE_ARR_ID,0,NULL,NULL,$1,$3,NULL);
								}
							}
	| TTRUE		{$$ = $1;}
	| TFALSE 	{$$ = $1;}
	;


%%

yyerror(char const *s)
{
	printf("yyerror - line %d : character %d %s",yylineno,yychar,s);
}

int main(int argc,char* argv[])
{
	if(argc>1)
	{
    	FILE *fp = fopen(argv[1],"r");
    	if(fp)
        	yyin = fp;
	}    
	yyparse();
	return 1;
}



