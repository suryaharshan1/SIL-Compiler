%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "y.tab.h"
	#include "abstracttrees6.h"
	//extern int yylval;
	int number;
%}


digit	[0-9]
letter 	[a-zA-Z]
whitespaces [ \t]
NL 	[\n]
plus	[+]
mul	[*]
asgn 	[=]
gt	[>]
lt 	[<]

	 
%%
<<EOF>>    	{return ENDOFFILE;} //returns end of file

{digit}+	{number=atoi(yytext);yylval.nptr = TreeCreate(TYPE_INT,NODETYPE_LEAF,number,NULL,NULL,NULL,NULL,NULL);return NUM;}
[;]   		 {return DELIM;}
"="    	 {return ASGN;}
"["    	{return *yytext;}
"]"    	{return *yytext;}
"decl"    	{return DECL;}
"enddecl"	{return ENDDECL;}
"read"  	{return READ;}
"write" 	{return WRITE;}
"if"    	{return IF;}
"then"  	{return THEN;}
"else"  	{return ELSE;}
"endif" 	{return ENDIF;}
"while" 	{return WHILE;}
"do"    	{return DO;}
"endwhile"  {return ENDWHILE;}
"integer"   {return INT;}
"boolean"   {return BOOL;}
"true"  	{yylval.nptr = TreeCreate(TYPE_BOOLEAN,NODETYPE_LEAF,1,NULL,NULL,NULL,NULL,NULL);return TTRUE;}
"false" 	{yylval.nptr = TreeCreate(TYPE_BOOLEAN,NODETYPE_LEAF,0,NULL,NULL,NULL,NULL,NULL);return TFALSE;}
{plus}  	{return PLUS;}
[a-zA-Z][a-zA-Z0-9]* {yylval.nptr = TreeCreate(TYPE_VOID,NODETYPE_ID,0,yytext,NULL,NULL,NULL,NULL);return ID;}
{mul}   	{return MUL;}
{gt}    	{return GT;}
{lt}    	{return LT;}
"=="   	{return EQ;}
{NL}    	;
{whitespaces} ;
[()]    	{return *yytext;}
","    	{return COMMA;}
.   		 {printf("INVALID CHARACTER");exit(0);}

%%

int yywrap(){
	return 1;
}


