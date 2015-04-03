%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	int yylex(void);
	extern FILE *yyin;
	//extern int yylineno;
	#include "abstracttrees6.h"
	#include "symboltable.h"
	#include "symboltable.c"
	#include "abstracttrees6.c"	
	#include "codegen.c"
	struct Tnode *root;
	int decl_type;
%}

%union{
	int ival;
	struct Tnode * nptr;
	struct ArgStruct * arg;
	char c;
};
 
%token ID NUM ASGN DELIM READ WRITE IF THEN ENDIF ELSE WHILE DO ENDWHILE PLUS MUL MINUS DIV MOD GT LT EQ NL AND OR ENDOFFILE COMMA INT BOOL TTRUE TFALSE DECL ENDDECL GE LE NE MAIN RETURN BEGINC END
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
%type <nptr> MAIN
%type <nptr> RETURN
%type <nptr> slist
%type <nptr> stmt
%type <nptr> expr
%type <nptr> FdefList
%type <nptr> Fdef
%type <nptr> Mainblock
%type <arg> ArgList
%type <arg> ArgListHead
%type <arg> ArgListTail
%type <arg> ArgIDlist
%type <nptr> Body
%type <nptr> retstmt
%type <nptr> exprlist
%type <nptr> exprListHead
%type <nptr> GE
%type <nptr> LE
%type <nptr> NE
%type <nptr> AND
%type <nptr> OR
%nonassoc GT LT EQ GE LE NE 
%left PLUS MINUS
%left MUL DIV MOD
%right AND OR
%%

start : declarations FdefList Mainblock ENDOFFILE	{
														//root = $3;
														printf("Evaluating the tree\n");
														//Evaluate($3);
														exit(0);
													}
		;

declarations : DECL Gdeclist ENDDECL {}
			 ;

Gdeclist : Gdeclist Gdec   {}
	| 					   {/*global declaration can be null*/}	
	;

type : INT 	 { decl_type = STYPE_INT; }
	| BOOL 	 { decl_type = STYPE_BOOLEAN; }
	;

Gdec : type Gidlist DELIM		{}
	;

Gidlist : Gidlist COMMA ID 	 		{Ginstall($3->NAME,decl_type,1,NULL);}
	| Gidlist COMMA ID '[' NUM ']'	{
										switch(decl_type){
											case STYPE_INT:
												Ginstall($3->NAME,STYPE_ARR_INT,$5->VALUE,NULL);			
												break;
											case STYPE_BOOLEAN:
												Ginstall($3->NAME,STYPE_ARR_BOOLEAN,$5->VALUE,NULL);
												break;					
										}
									}
	| Gidlist COMMA ID '(' ArgList ')'	{
											switch(decl_type){
												case STYPE_INT:
													Ginstall($3->NAME,STYPE_FUNCTION_INT,1,$5);
													break;
												case STYPE_BOOLEAN:
													Ginstall($3->NAME,STYPE_FUNCTION_BOOLEAN,1,$5);
													break;
											}
											ReinitialiseLocalVariables();
										}

	| ID '(' ArgList ')'	{	
								switch(decl_type){
									case STYPE_INT:
										Ginstall($1->NAME,STYPE_FUNCTION_INT,1,$3);
										break;
									case STYPE_BOOLEAN:
										Ginstall($1->NAME,STYPE_FUNCTION_BOOLEAN,1,$3);
										break;
								}
								ReinitialiseLocalVariables();
						 	}

	| ID 				{Ginstall($1->NAME,decl_type,1,NULL);}
	
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
	| stmt      		{$$ = $1;}
	;

ArgList : ArgListHead	{$$ = $1;}
	|	{$$ = NULL;}	
	;

ArgListHead :	type ArgIDlist ArgListTail	{
                                                Argtemp = $2; 
                                                while(Argtemp->next != NULL){
                                                    Argtemp = Argtemp->next;
                                                }
                                                Argtemp->next = $3;
                                                $$ = $2;
                                            }
	;

ArgListTail : DELIM ArgListHead 	{$$ = $2;}
	| 	{$$ = NULL;}
	;

ArgIDlist : ArgIDlist COMMA ID  {Argtemp = Arginstall($3->NAME,decl_type,PTYPE_VAL); Argtemp->next = $1; $$ = Argtemp;}
	| ArgIDlist COMMA '&' ID    {Argtemp = Arginstall($4->NAME,decl_type,PTYPE_REF); Argtemp->next = $1; $$ = Argtemp;}
	| ID 	                    {$$ = Arginstall($1->NAME,decl_type,PTYPE_VAL);}
	| '&' ID 	                {$$ = Arginstall($2->NAME,decl_type,PTYPE_REF);} 
	;

FdefList : FdefList Fdef {}
		| 				 {}
		;

Fdef : type ID '(' ArgList ')' '{' LDefblock Body '}'	{
                                                            validateFunction($2->NAME,decl_type,$4,$8);
                                                            $2->Lentry = Lhead;
															codegen_function($2->NAME,$8);
															ReinitialiseLocalVariables();
                                                        }
	 ;

Mainblock : INT MAIN '(' ')' '{' LDefblock Body '}'		{/*check all  functions are defined or not*/
															Ginstall("MAIN",STYPE_INT,1,NULL);
															$$ = TreeCreate(TYPE_INT,NODETYPE_MAIN,0,NULL,NULL,$7,NULL,NULL);
															gen_code_eval($$);
															ReinitialiseLocalVariables();
														}
		;

LDefblock : DECL Ldeclist ENDDECL 	{}
		  ;

Ldeclist : Ldeclist Ldec   {}
	|		{}
	;

Ldec : type Lidlist DELIM		{}
	;


Lidlist : Lidlist COMMA ID 	 		{Linstall($3->NAME,decl_type);}
	| ID 							{Linstall($1->NAME,decl_type);}
	;


Body : BEGINC slist retstmt END 	{$$ = TreeCreate(TYPE_VOID,NODETYPE_BODY,0,NULL,NULL,$2,$3,NULL);}
	;

stmt  : ID ASGN expr DELIM  {	Ltemp = Llookup($1->NAME);
								if(Ltemp == NULL){
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
									}
								}
								else{
									$1->Lentry = Ltemp;
									if(Ltemp->type == ARG_INT || Ltemp->type == STYPE_INT || Ltemp->type == ARG_REF_INT){
										$1->TYPE = TYPE_INT;								
									}
									else if(Ltemp->type == ARG_BOOLEAN || Ltemp->type == STYPE_BOOLEAN || Ltemp->type == ARG_REF_BOOLEAN){
										$1->TYPE = TYPE_BOOLEAN;	
									}
								}
								$$ = TreeCreate(TYPE_VOID,NODETYPE_ASGN,0,NULL,NULL,$1,$3,NULL);
							}
	| ID '[' expr ']' ASGN expr DELIM   {	Gtemp=Glookup($1->NAME);
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
											}
											$$ = TreeCreate(TYPE_VOID,NODETYPE_ARR_ASGN,0,NULL,NULL,$1,$3,$6);
										}
	| READ '(' ID ')' DELIM         	{	
											Ltemp = Llookup($3->NAME);
											if(Ltemp == NULL){
												Gtemp=Glookup($3->NAME);
												if(Gtemp==NULL){
													yyerror("undefined variable ");
													printf("%s",$3->NAME);
													exit(0);
												}
												else if(Gtemp->type == STYPE_ARR_INT || Gtemp->type == STYPE_ARR_BOOLEAN){
													yyerror("invalid reference to an array ");
													printf("%s",$3->NAME);
													exit(0);
												}
												else {
													$3->Gentry=Gtemp;
													if(Gtemp->type == STYPE_INT){
														$3->TYPE = TYPE_INT;
													}
													else if(Gtemp->type == STYPE_BOOLEAN){
														$3->TYPE = TYPE_BOOLEAN;	
													}
												}
											}
											else{
												$3->Lentry = Ltemp;
												if(Ltemp->type == ARG_INT || Ltemp->type == STYPE_INT || Ltemp->type == ARG_REF_INT){
													$3->TYPE = TYPE_INT;								
												}
												else if(Ltemp->type == ARG_BOOLEAN || Ltemp->type == STYPE_BOOLEAN || Ltemp->type == ARG_REF_BOOLEAN){
													$3->TYPE = TYPE_BOOLEAN;	
												}
											}
											$$ = TreeCreate(TYPE_VOID,NODETYPE_READ,0,NULL,NULL,$3,NULL,NULL);
										}
	| READ '(' ID '[' expr ']' ')' DELIM 	{								
												Gtemp=Glookup($3->NAME);
												if(Gtemp==NULL){
													printf("undefined variable ");
													printf("%s",$3->NAME);
													exit(0);
												}
												else if(Gtemp->type == STYPE_INT || Gtemp->type == STYPE_BOOLEAN){
													yyerror("invalid reference to a variable ");
													printf("%s",$3->NAME);
													exit(0);		
												}
												else { 
													$3->Gentry=Gtemp;
													if(Gtemp->type == STYPE_ARR_INT){
														$3->TYPE = TYPE_INT;
													}
													else if(Gtemp->type == STYPE_ARR_BOOLEAN){
														$3->TYPE = TYPE_BOOLEAN;	
													}
												}
												$$ = TreeCreate(TYPE_VOID,NODETYPE_ARR_READ,0,NULL,NULL,$3,$5,NULL);
											}

	| WRITE '(' expr ')' DELIM      			{$$ = TreeCreate(TYPE_VOID,NODETYPE_WRITE,0,NULL,NULL,$3,NULL,NULL);}

	| IF '(' expr ')' THEN slist ENDIF DELIM	{$$ = TreeCreate(TYPE_VOID,NODETYPE_IF,0,NULL,NULL,$3,$6,NULL);}
	
	| IF '(' expr ')' THEN slist ELSE slist ENDIF DELIM	{$$ = TreeCreate(TYPE_VOID,NODETYPE_IF,0,NULL,NULL,$3,$6,$8);}
	
	| WHILE '(' expr ')' DO slist ENDWHILE DELIM	{$$ = TreeCreate(TYPE_VOID,NODETYPE_WHILE,0,NULL,NULL,$3,$6,NULL);}
	;

retstmt : RETURN expr DELIM		{$$ = TreeCreate($2->TYPE, NODETYPE_RET,0,NULL,NULL,$2,NULL,NULL);}
	;

expr  : expr PLUS expr  {$$ = TreeCreate(TYPE_INT,NODETYPE_PLUS,0,NULL,NULL,$1,$3,NULL);}
	| expr MINUS expr  {$$ = TreeCreate(TYPE_INT,NODETYPE_MINUS,0,NULL,NULL,$1,$3,NULL);}
	| expr MUL expr 	{$$ = TreeCreate(TYPE_INT,NODETYPE_MUL,0,NULL,NULL,$1,$3,NULL);}
	| expr MOD expr 	{$$ = TreeCreate(TYPE_INT,NODETYPE_MOD,0,NULL,NULL,$1,$3,NULL);}
	| expr DIV expr 	{$$ = TreeCreate(TYPE_INT,NODETYPE_DIV,0,NULL,NULL,$1,$3,NULL);}
	| '(' expr ')'  	{$$ = $2;}
	| expr GT expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_GT,0,NULL,NULL,$1,$3,NULL);}
	| expr LT expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_LT,0,NULL,NULL,$1,$3,NULL);}
	| expr EQ expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_EQ,0,NULL,NULL,$1,$3,NULL);}
	| expr GE expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_GE,0,NULL,NULL,$1,$3,NULL);}
	| expr LE expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_LE,0,NULL,NULL,$1,$3,NULL);}
	| expr NE expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_NE,0,NULL,NULL,$1,$3,NULL);}
	| expr AND expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_AND,0,NULL,NULL,$1,$3,NULL);}
	| expr OR expr  	{$$ = TreeCreate(TYPE_BOOLEAN,NODETYPE_OR,0,NULL,NULL,$1,$3,NULL);}
	| NUM           	{$$ = $1;}
	| ID            	{
							Ltemp = Llookup($1->NAME);
							if(Ltemp == NULL){
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
							else{
								$1->Lentry = Ltemp;
								if(Ltemp->type == ARG_INT || Ltemp->type == STYPE_INT || Ltemp->type == ARG_REF_INT){
									$1->TYPE = TYPE_INT;								
								}
								else if(Ltemp->type == ARG_BOOLEAN || Ltemp->type == STYPE_BOOLEAN || Ltemp->type == ARG_REF_BOOLEAN){
									$1->TYPE = TYPE_BOOLEAN;	
								}
								$$ = $1;
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
	| ID '(' exprlist ')'	{ 
	                            Gtemp=Glookup($1->NAME);
	                            if(Gtemp == NULL){
    	                            printf("undefined function ");
    								printf("%s",$1->NAME);
    								exit(1);
	                            }
	                            else if(Gtemp->type != STYPE_FUNCTION_INT && Gtemp->type != STYPE_FUNCTION_BOOLEAN){
	                                yyerror("Invalid reference to a variable ");
									printf("%s",$1->NAME);
									exit(1);
	                            }
    	                        else{
    	                            $1->Gentry=Gtemp;
    	                            if(Gtemp->type == STYPE_FUNCTION_INT){
										$1->TYPE = TYPE_INT;
									}
									else if(Gtemp->type == STYPE_FUNCTION_BOOLEAN){
										$1->TYPE = TYPE_BOOLEAN;	
									}
									$$ = TreeCreate($1->TYPE,NODETYPE_FUNCTION,0,$1->NAME,$3,NULL,NULL,NULL);
    	                        }
    	                    }
	;

exprlist : exprListHead {$$ = $1;}
    |   {$$ = NULL;}
    ;
    
exprListHead : exprListHead COMMA expr {$$ = TreeCreate(TYPE_VOID,NODETYPE_EXPRLIST,0,NULL,NULL,$1,$3,NULL);}
    | expr {$$ = TreeCreate(TYPE_VOID,NODETYPE_EXPRLIST,0,NULL,NULL,NULL,$1,NULL);} 
    ;
	
%%


int main(int argc,char* argv[])
{
	if(argc>1)
	{
    	FILE *file = fopen(argv[1],"r");
    	if(file)
        	yyin = file;
	}
	fp = fopen("intercode","w+");
	if(fp == NULL){
		printf("Error generating the code : FILE not created\n");
		exit(1);
	}
	yyparse();
	fclose(fp);    
	return 1;
}


