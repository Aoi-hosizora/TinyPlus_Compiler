%option noyywrap
%{
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "util.h"

char tokenString[MAXTOKENLEN + 1];
%}

digit		[0-9]
number		{digit}+
letter		[a-zA-Z]
identifier	{letter}[a-zA-Z0-9]*
newline		[\n]
newlineR	[\r]
whitespace	[ \t]

%%

"if"			{return IF;}  // reserved words
"then"			{return THEN;}
"else"			{return ELSE;}
"end"			{return END;}
"repeat"		{return REPEAT;}
"until"			{return UNTIL;}
"read"			{return READ;}
"write"			{return WRITE;}

"while"			{return WHILE;} // NEW
"do"			{return DO;}
"to"			{return TO;}
"downto"		{return DOWNTO;}
"for"			{return FOR;}
"enddo"			{return ENDDO;}
"endwhile"		{return ENDWHILE;}
"mod"			{return MOD;}

":="			{return ASSIGN;} // operator
"="			{return EQ;}
"<"				{return LT;}
"+"				{return PLUS;}
"-"				{return MINUS;}
"*"				{return TIMES;}
"/"				{return OVER;}
"("				{return LPAREN;}
")"				{return RPAREN;}
";"				{return SEMI;}

{number}		{return NUM;}
{identifier}	{return ID;}
{newline}		{lineno++;}
{whitespace}	{}
{newlineR}		{}

"{"				{char c;
			  	int count = 1;
			  	do {
					c = input();
				  	if (c == EOF) break;
				  	else if (c == '\n') lineno++;
				  	else if (c == '{') count++;
				  	else if (c == '}') count--;
			  	} while (count != 0);}

.				{return ERROR;}

%%

TokenType getToken(void)
{
	// https://www.ibm.com/developerworks/cn/linux/sdk/lex/
	// yytext	匹配模式的文本存储在这一变量中（char*）。
	// yywrap()	这一函数在文件（或输入）的末尾调用。 如果函数的返回值是1，就停止解析。 

	TokenType currentToken = yylex();

	strncpy(tokenString, yytext, MAXTOKENLEN);
	if (currentToken != ENDFILE)
		printf("%d: ", lineno);
	printToken(currentToken, tokenString);
	return currentToken;
}