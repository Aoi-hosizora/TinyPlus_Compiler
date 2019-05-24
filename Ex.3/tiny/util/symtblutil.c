#include "symtblutil.h"
#include "../globals.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static symbolTblNode *symtblhead = NULL;
static symbolTblNode *symtbltail = NULL;

// 查找符号表，不存在->NOTFOUND，存在->node
symbolTblNode *st_lookup(char *name) {
	if (symtblhead == NULL) 
		return NOTFOUND;

	symbolTblNode *curr = symtblhead;

	while (curr != NULL) {
		if (strcmp(curr->symbol, name)) {
			curr = curr->next;
		}
		else 
			return curr;
	}
	return NOTFOUND;
}

int st_lookup_loc(char *name) {
	if (symtblhead == NULL)
		return -1;

	symbolTblNode *curr = symtblhead;

	while (curr != NULL) {
		if (strcmp(curr->symbol, name))
			curr = curr->next;
		else
			return curr->loc;
	}
	return -1;
}

void st_insert(char *name, int lineno, int loc) {
	// Tbl 节点
	symbolTblNode *ret = st_lookup(name);
	
	// lineno 节点
	symbolLneNode *linenolist = (symbolLneNode *)malloc(sizeof(symbolLneNode));
	linenolist->lineno = lineno - 1;
	linenolist->next = NULL;

	if (ret == NOTFOUND) { // 新节点
		symbolTblNode *newnode = (symbolTblNode *)malloc(sizeof(symbolTblNode));
		newnode->symbol = name;
		newnode->linenolist = linenolist;
		newnode->loc = loc;
		newnode->next = NULL;

		if (symtblhead == NULL) // 头节点
			symtbltail = symtblhead = newnode;
		else {
			symtbltail -> next = newnode;
			symtbltail = symtbltail -> next;
		}
	}
	else {
		symbolLneNode *last = ret->linenolist; // 尾节点
		while (last->next != NULL)
			last = last -> next;
		last -> next = linenolist;
	}
}

void printSymbolTbl() {
	symbolTblNode *curr = symtblhead;

	printf("%-10s%-10s%s\n", "Symbol", "Location", "LineNo");
	while (curr != NULL) {
		printf("%-10s%-10d", curr->symbol, curr->loc);
		symbolLneNode *linenolist = curr->linenolist;
		while (linenolist != NULL) {
			printf("%-4d", linenolist->lineno);
			linenolist = linenolist->next;
		}
		printf("\n");
		curr = curr -> next;
	}
}