#include "util.h"
#include <stdio.h>
#include "globals.h"
 
void printToken(TokenType token, char* tokenString)
{
	switch(token)
	{
		case IF:
		case THEN:
		case ELSE:
		case END:
		case REPEAT:
		case UNTIL:
		case READ:
		case WRITE:
		case WHILE:
		case ENDWHILE:
		case DO:
		case TO:
		case DOWNTO:
		case FOR:
		case ENDDO:
		case MOD:
			printf("\t%-20s %s\n", "reversed word: ", tokenString);
			break;
		case ID:
			printf("\t%-20s %s\n","identifier: ",  tokenString);
			break;
		case NUM:
			printf("\t%-20s %s\n","number: ",  tokenString);
			break;
		case ASSIGN:
		case EQ:
		case LT:
		case PLUS:
		case MINUS:
		case TIMES:
		case OVER:
		case LPAREN:
		case RPAREN:
		case SEMI:
			printf("\t%-20s %s\n","operator:",  tokenString);
			break;
		case ERROR:
			printf("ERROR");
	}
}