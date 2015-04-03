/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    NUM = 259,
    ASGN = 260,
    DELIM = 261,
    READ = 262,
    WRITE = 263,
    IF = 264,
    THEN = 265,
    ENDIF = 266,
    ELSE = 267,
    WHILE = 268,
    DO = 269,
    ENDWHILE = 270,
    PLUS = 271,
    MUL = 272,
    MINUS = 273,
    DIV = 274,
    MOD = 275,
    GT = 276,
    LT = 277,
    EQ = 278,
    NL = 279,
    AND = 280,
    OR = 281,
    ENDOFFILE = 282,
    COMMA = 283,
    INT = 284,
    BOOL = 285,
    TTRUE = 286,
    TFALSE = 287,
    DECL = 288,
    ENDDECL = 289,
    GE = 290,
    LE = 291,
    NE = 292,
    MAIN = 293,
    RETURN = 294,
    BEGINC = 295,
    END = 296
  };
#endif
/* Tokens.  */
#define ID 258
#define NUM 259
#define ASGN 260
#define DELIM 261
#define READ 262
#define WRITE 263
#define IF 264
#define THEN 265
#define ENDIF 266
#define ELSE 267
#define WHILE 268
#define DO 269
#define ENDWHILE 270
#define PLUS 271
#define MUL 272
#define MINUS 273
#define DIV 274
#define MOD 275
#define GT 276
#define LT 277
#define EQ 278
#define NL 279
#define AND 280
#define OR 281
#define ENDOFFILE 282
#define COMMA 283
#define INT 284
#define BOOL 285
#define TTRUE 286
#define TFALSE 287
#define DECL 288
#define ENDDECL 289
#define GE 290
#define LE 291
#define NE 292
#define MAIN 293
#define RETURN 294
#define BEGINC 295
#define END 296

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 17 "stage9.y" /* yacc.c:1909  */

	int ival;
	struct Tnode * nptr;
	struct ArgStruct * arg;
	char c;

#line 143 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
