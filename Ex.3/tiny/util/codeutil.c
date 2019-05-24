#include "../globals.h"   
#include "codeutil.h"   
#include <stdarg.h>

#define TACCodeLen 20000
#define CodeLen 50

static char *stringstream = NULL;

 /* TM location number for current instruction emission */   
static int emitLoc = 0;  

/* Highest TM location emitted so far  
   For use in conjunction with emitSkip,  
   emitBackup, and emitRestore */   
static int highEmitLoc = 0;

void printTAC() {
	printf("%s", stringstream);
}

void WriteTAC(const char* format, ...) {

	if (stringstream == NULL) {
		stringstream = (char *)malloc(sizeof(char) * TACCodeLen);
		stringstream++;
	}

	char *code = (char *)malloc(sizeof(char) * CodeLen);

    va_list vl;
    va_start(vl, format);
    vsprintf(code, format, vl);
    va_end(vl);

	sprintf(stringstream, "%s%s", stringstream, code);
}

void emitComment(char *c) { 
	if (IsTraceCode)
		WriteTAC("@%s\n", c);
}    

/* Procedure emitRO emits a register-only  
 * TM instruction  
 * op = the opcode  
 * r = target register  
 * s = 1st source register  
 * t = 2nd source register
 */   
void emitRO(char *op, int r, int s, int t) {
	WriteTAC("%3d:  %5s  %d,%d,%d\n", emitLoc++, op, r, s, t);
	if (highEmitLoc < emitLoc)
		highEmitLoc = emitLoc;   
}

/* Procedure emitRM emits a register-to-memory  
 * TM instruction  
 * op = the opcode  
 * r = target register  
 * d = the offset  
 * s = the base register 
 */   
void emitRM(char *op, int r, int d, int s) {
	WriteTAC("%3d:  %5s  %d,%d(%d)\n", emitLoc++, op, r, d, s);
	if (highEmitLoc < emitLoc)
		highEmitLoc = emitLoc;   
}
   
/* Procedure emitRM_Abs converts an absolute reference   
 * to a pc-relative reference when emitting a  
 * register-to-memory TM instruction  
 * op = the opcode  
 * r = target register  
 * a = the absolute location in memory
 */   
void emitRM_Abs(char *op, int r, int a) {
	WriteTAC("%3d:  %5s  %d,%d(%d)\n", emitLoc, op, r, a-(emitLoc+1), pc);
	++emitLoc;
	if (highEmitLoc < emitLoc)
		highEmitLoc = emitLoc;   
}

/* Function emitSkip skips "howMany" code  
 * locations for later backpatch. It also  
 * returns the current code position  
 */   
int emitSkip(int howMany) {
	int i = emitLoc;
	emitLoc += howMany;
	if (highEmitLoc < emitLoc)
		highEmitLoc = emitLoc;   
   return i;   
} 

/* Procedure emitBackup backs up to   
 * loc = a previously skipped location  
 */   
void emitBackup(int loc) {
	emitLoc = loc;   
}

/* Procedure emitRestore restores the current   
 * code position to the highest previously  
 * unemitted position  
 */   
void emitRestore() {
	emitLoc = highEmitLoc;
}   

char *codecat(char *code) {
	if (stringstream == NULL)
		stringstream = (char *)malloc(sizeof(char) * TACCodeLen);
	return stringstream = strcat(stringstream, code);
}