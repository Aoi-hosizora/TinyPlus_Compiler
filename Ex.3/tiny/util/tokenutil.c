#include "tokenutil.h"
#include <stdio.h>
#include "../globals.h"
 
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
			printf("\t%-20s %s\n", "reversed word: ", tokenString);
			break;
		case ID:
			printf("\t%-20s %s\n","identifier: ",  tokenString);
			break;
		case NUM:
			printf("\t%-20s %s\n","number: ",  tokenString);
			break;
		//////////////////////////////////
		case MOD:
			printf("%%\n");
			break;
		case ASSIGN:
			printf(":=\n");
			break;
		case EQ:
			printf("=\n");
			break;
		case LT:
			printf("<\n");
			break;
		case PLUS:
			printf("+\n");
			break;
		case MINUS:
			printf("-\n");
			break;
		case TIMES:
			printf("*\n");
			break;
		case OVER:
			printf("/\n");
			break;
		case LPAREN:
			printf("(\n");
			break;
		case RPAREN:
			printf(")\n");
			break;
		case SEMI:
			printf(";\n");
			break;
		case ERROR:
			printf("ERROR");
	}
}