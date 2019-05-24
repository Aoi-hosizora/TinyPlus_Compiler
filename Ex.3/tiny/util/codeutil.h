#ifndef _CODE_H_ 
#define _CODE_H_ 
 
/* pc = program counter  */ 
#define  pc 7 

/* mp = "memory pointer" points 
 * to top of memory (for temp storage) 
 */ 
#define  mp 6 

/* gp = "global pointer" points 
 * to bottom of memory for (global) 
 * variable storage 
 */ 
#define gp 5

/* accumulator */ 
#define ac 0

/* 2nd accumulator */ 
#define ac1 1

void emitComment(char *c);

void emitRO(char *op, int r, int s, int t);

void emitRM(char *op, int r, int d, int s);

int emitSkip(int howMany);

void emitBackup(int loc);

void emitRestore();

void emitRM_Abs(char *op, int r, int a);

void printTAC();

#endif // _CODE_H_