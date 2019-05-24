#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"

#include "treeutil.h"
#include "tokenutil.h"

TreeNode* newStmtNode(StmtKind kind) { 

	TreeNode *t = (TreeNode *) malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		printf("Out of memory error at line %d\n",lineno);
	else {
		for (i=0; i<MAXCHILDREN; i++) 
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
	}
	return t;
}

TreeNode* newExpNode(ExpKind kind) {
	
	TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		printf("Out of memory error at line %d\n",lineno);
	else {
		for (i=0; i<MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->type = Void;
	}
	return t;
}

char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    printf("Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

static int indentno = 0;

#define INDENT indentno += 4
#define UNINDENT indentno -= 4

static void printSpaces(void) { 
	int i;
	for (i=0;i<indentno;i++)
		printf(" ");
}

void printTree(TreeNode *tree) { 
	int i;
	INDENT;
	while (tree != NULL) {
		printSpaces();
		if (tree->nodekind == StmtK) {
			switch (tree->kind.stmt) {
			case IfK:
				printf("If\n");
				break;
			case RepeatK:
				printf("Repeat\n");
				break;
			case WhileK: // new
				printf("While\n");
				break;
			case ForK: // new
				printf("For\n");
				break;
			case ToK: // new
				printf("%s\n", tree->attr.name);
				break;
			case AssignK:
				printf("Assign to: %s\n",tree->attr.name);
				break;
			case ReadK:
				printf("Read: %s\n",tree->attr.name);
				break;
			case WriteK:
				printf("Write\n");
				break;
			default:
				printf("Unknown ExpNode kind\n");
				break;
			}
		}
		else if (tree->nodekind == ExpK) {
			switch (tree->kind.exp) {
			case OpK:
				printf("Op: ");
				printToken(tree->attr.op,"\0");
				break;
			case ConstK:
				printf("Const: %d\n",tree->attr.val);
				break;
			case IdK:
				printf("Id: %s\n",tree->attr.name);
				break;
			default:
				printf("Unknown ExpNode kind\n");
				break;
			}
		}
		else
			printf("Unknown node kind\n");
		for (i=0; i<MAXCHILDREN; i++)
			printTree(tree->child[i]);
		tree = tree->sibling;
	}
	UNINDENT;
}
