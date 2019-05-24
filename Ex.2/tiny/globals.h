#ifndef GLOBALS_H
#define GLOBALS_H
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef YYPARSER
#include "tiny.tab.h"
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

// token
typedef int TokenType; 

// global
extern int lineno;
extern FILE* in;
extern int Error; 

// tree
typedef enum {StmtK,ExpK} NodeKind;
typedef enum {OpK,ConstK,IdK} ExpKind;
typedef enum {Void,Integer,Boolean} ExpType;

typedef enum {
	IfK,
	RepeatK,
	WhileK,
	ForK,
	ToK,
	AssignK,
	ReadK,
	WriteK
} StmtKind;

#define MAXTOKENLEN 50
#define MAXCHILDREN 3

typedef struct treeNode { 
	struct treeNode * child[MAXCHILDREN];
    struct treeNode * sibling;
    int lineno;
    NodeKind nodekind;

    union {
		StmtKind stmt;
		ExpKind exp;
	} kind;
    union {
		TokenType op;
		int val;
		char * name;
	} attr;
	
	ExpType type; /* for type checking of exps */
} TreeNode;

// scan
extern char tokenString[MAXTOKENLEN+1];
TokenType getToken(void);

// parse
TreeNode *parse(void);
// void printTree(TreeNode *);

#endif