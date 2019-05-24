#include "analyseutil.h"

#include "../globals.h"
#include "symtblutil.h"

#include <stdio.h>
#include <string.h>

typedef void (*traverseFunc) (TreeNode *);

static int tbllocation = 0;

//遍历函数
static void traverse(TreeNode *t, traverseFunc preProc, traverseFunc postProc) {
	int i;
	if (t != NULL) {
		preProc(t);
		for (i=0; i< MAXCHILDREN; i++)
			traverse(t->child[i], preProc, postProc);
		postProc(t);
		traverse(t->sibling, preProc, postProc);
	}
}

//空遍历函数
static void nullProc(TreeNode* t) {
	return;
}

//根据节点类型，建立符号表
static void insertNode(TreeNode* t) {
	switch (t->nodekind) {
		case StmtK:
			switch (t->kind.stmt) {
				case AssignK:
				case ReadK:
					if (st_lookup(t->attr.name)==NOTFOUND)
						st_insert(t->attr.name, t->lineno, tbllocation++);
					else
						st_insert(t->attr.name, t->lineno, 0); 
				break;
				default:
				break;
			}
		break;
		case ExpK:
			switch (t->kind.exp) {
				case IdK:
					if (st_lookup(t->attr.name)==NOTFOUND)
						st_insert(t->attr.name, t->lineno, tbllocation++);
					else
						st_insert(t->attr.name, t->lineno, 0);
				break;
				default:
				break;
			}
		break;
		default:
		break;
	}
}

static void typeError(TreeNode* t, char* message) {
	printf("Type error at line %d: %s\n", t->lineno - 1, message);
	Error = TRUE;
}

//根据节点类型进行类型检查
static void checkNode(TreeNode* t) {
	switch (t->nodekind) {
		case StmtK:
			switch(t->kind.stmt) {
				case IfK: // if: Boolean
					if (t->child[0]->type==Integer)
						typeError(t->child[0], "if test is not Boolean");
				break;
				case AssignK: // :=: Integer
					if (t->child[0]->type!=Integer)
						typeError(t->child[0], "assignment of non-integer value");
				break;
				// Read: x
				case WriteK: // Write: Integer
					if (t->child[0]->type!=Integer)
						typeError(t->child[0], "write of non-integer value");
				break;
				case RepeatK: // repeat: Boolean
					if(t->child[1]->type==Integer)
						typeError(t->child[1], "repeat test is not Boolean");
				break;
				case WhileK: // while: Boolean
					if(t->child[0]->type==Integer)
						typeError(t->child[0], "while test is not Boolean");
				break;
				case ToK:
					if (t->child[1]->type!=Integer)
						typeError(t->child[1], "for-to test is not Boolean");
					if (t->child[2]->type!=Integer)
						typeError(t->child[2], "for-to test is not Boolean");
				break;
				default:
				break;
			}
		break;
		case ExpK:
			switch(t->kind.exp) {
				case OpK:
					if ((t->child[0]->type!=Integer) || (t->child[1]->type!=Integer))
						typeError(t, "Op applied to non-integer");
					if ((t->attr.op==EQ)||(t->attr.op==LT))
						t->type=Boolean;
					else
						t->type=Integer;
				break;
				case ConstK:
				case IdK:
					t->type = Integer;
				break;
				default:
                break;
			}
		break;
		default:
		break;
	}
}

//遍历语法树，创建符号表
void buildSymtab(TreeNode* syntaxTree) {
	typeCheck(syntaxTree);
	if (Error != TRUE) 
		traverse(syntaxTree, insertNode, nullProc);
}

//遍历语法树，进行类型检查
void typeCheck(TreeNode* syntaxTree) {
	traverse(syntaxTree, nullProc, checkNode);
}
