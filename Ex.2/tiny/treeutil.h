#ifndef TREEUTIL_H
#define TREEUTIL_H
 
#include "globals.h"
 
TreeNode *newStmtNode(StmtKind);
TreeNode *newExpNode(ExpKind);

void printTree(TreeNode *);
 
#endif // TREEUTIL_H