#ifndef SumTblUtil_H
#define SumTblUtil_H

#include "../globals.h"

#define NOTFOUND NULL

symbolTblNode *st_lookup(char *name);
int st_lookup_loc(char *name);

void st_insert(char *name, int lineno, int loc);

void printSymbolTbl();

#endif // SumTblUtil_H