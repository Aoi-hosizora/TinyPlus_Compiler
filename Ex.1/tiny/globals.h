#ifndef GLOBALS_H
#define GLOBALS_H
 
#include <stdio.h>
typedef enum 
{
	// book-keeping tokens
	ENDFILE, ERROR,
	// reserved words
	IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
	// NEW
	WHILE, DO, TO, DOWNTO, FOR, ENDDO, ENDWHILE, MOD,
	// multicharacter
	ID, NUM,
	// operator
	ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI

} TokenType;
extern int lineno;
 
/* The max size of identifier of reserved word */
#define MAXTOKENLEN 50
 
#endif