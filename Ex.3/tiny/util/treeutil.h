#ifndef TREEUTIL_H
#define TREEUTIL_H
 
#include "../globals.h"
 
TreeNode *newStmtNode(StmtKind);
TreeNode *newExpNode(ExpKind);

char *copyString(char *s);
void printTree(TreeNode *);
 
#endif // TREEUTIL_H