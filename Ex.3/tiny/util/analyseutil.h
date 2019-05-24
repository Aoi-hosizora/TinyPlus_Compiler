#ifndef AnalyseUtil_H
#define AnalyseUtil_H

#include "../globals.h"
#include "symtblutil.h"

typedef void (*traverseFunc) (TreeNode *);
//遍历函数
static void traverse(TreeNode *t,traverseFunc preProc,traverseFunc postProc);

//空遍历函数
static void nullProc(TreeNode* t);

//根据节点类型，建立符号表
static void insertNode(TreeNode* t);

static void typeError(TreeNode* t, char* message);

//根据节点类型进行类型检查
static void checkNode(TreeNode* t);

//遍历语法树，创建符号表
void buildSymtab(TreeNode* syntaxTree);

//遍历语法树，进行类型检查
void typeCheck(TreeNode* syntaxTree);

#endif // AnalyseUtil_H