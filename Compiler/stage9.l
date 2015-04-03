%option yylineno
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "y.tab.h"
	#include "abstracttrees6.h"
	int number;
%}


digit	[0-9]
letter 	[a-zA-Z]
whitespaces [ \t]
NL 	[\n]
plus	[+]
mul	[*]
minus [-]
asgn 	[=]
mod [%]
div [/]
gt	[>]
lt 	[<]

	 
%%
<<EOF>>  {return ENDOFFILE;} //returns end of file

{digit}+	{number=atoi(yytext);yylval.nptr = TreeCreate(TYPE_INT,NODETYPE_LEAF,number,NULL,NULL,NULL,NULL,NULL);return NUM;}
[;]   		 {return DELIM;}
"="    	 {return ASGN;}
"["    	{return *yytext;}
"]"    	{return *yytext;}
"{"		{return *yytext;}
"}"		{return *yytext;}
"&"		{return *yytext;}
"pair" 	{return PAIR;}
"main"	{return MAIN;}
"return"	{return RETURN;}
"begin"	{return BEGINC;}
"end"	{return END;}
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
{minus}		{return MINUS;}
{mod}		{return MOD;}
{div}		{return DIV;}
{gt}    	{return GT;}
{lt}    	{return LT;}
"=="   	{return EQ;}
"&&"	{return AND;}
"||"	{return OR;}
{NL}    	;
{whitespaces} ;
[()]    	{return *yytext;}
","    	{return COMMA;}
"."		{return DOT;}
">="	{return GE;}
"<="	{return LE;}
"!="	{return NE;}
.   		 {printf("INVALID CHARACTER");exit(0);}

%%

int yywrap(){
	return 1;
}
yyerror(char const *s)
{
	//printf("yyerror - line %d : character %d %s",yylineno,yychar,s);
	printf("yyerror - line %d : character %s",yylineno,s);
}



