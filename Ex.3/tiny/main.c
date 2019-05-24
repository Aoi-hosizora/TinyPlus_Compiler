#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "globals.h"

#include "util/analyseutil.h"
#include "generate.h"
#include "util/treeutil.h"
#include "util/codeutil.h"
 
#define ERROREXIT exit(0)

int lineno = 1;
int Error = FALSE;
FILE *in;
int IsTraceCode = FALSE;


void printHelp(const char *);
void HandleArgv(char *[5]);
 
int main(int argc, char* argv[]) {

	if (argc == 1 || (argc == 2 && argv[1] != "-i") || argc == 3)
		printHelp(argv[0]);

	short IsT = FALSE, IsP = FALSE, IsS = FALSE, IsC = FALSE, IsH = FALSE;

	int o;
	TreeNode *syntaxTree;

	while ((o = getopt(argc, argv, "i:tpsch")) != -1) {
		switch (o) {
			case 'i':
				in = fopen(optarg, "r");
				if (in == NULL) {
					printf("ERROR: file %s not found", optarg);
					ERROREXIT;
				}
			break;
			case 'p': IsP = TRUE; break;
			case 's': IsS = TRUE; break;
			case 't': IsT = TRUE; break;
			case 'c': IsC = TRUE; break;
			case 'h':
			default:  IsH = TRUE; break;
		}
	}

	if (IsT) {
		IsTraceCode = TRUE;
	}
	
	syntaxTree = parse();
	buildSymtab(syntaxTree);
	codeGen(syntaxTree);

	if (IsP) {
		printf("\nSyntax Tree: \n\n");
		if (Error != TRUE)
			printTree(syntaxTree);
	}
	if (IsS) {
		printf("\nSymbol Table: \n\n");
		if (Error != TRUE)
			printSymbolTbl(syntaxTree);
	}
	if (IsC) {
		printf("\nTAC: \n\n");
		if (Error != TRUE)
			printTAC();
	}
	if (IsH) {
		printHelp(argv[0]);
	}

	fclose(in);
}

void printHelp(const char *arg) {
	printf("\nUSAGE: %s -i <filename> [pscth]...\n", arg);
	printf("-i Tiny+ Code\n");
	printf("-p Print Syntax Tree\n");
	printf("-s Print Symbol Table\n");
	printf("-t Show Commit Tips\n");
	printf("-c Print TAC\n");
	printf("-h Usage\n");
	ERROREXIT;
}