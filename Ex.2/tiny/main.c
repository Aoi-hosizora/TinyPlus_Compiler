#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "tokenutil.h"
 
#define ERROREXIT return 1

int lineno = 1;
int Error = FALSE;
FILE *in;
void printTree(TreeNode *);
 
int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("USAGE: %s <filename>", argv[0]);
		ERROREXIT;
	}
	const char *path = argv[1];
	in = fopen(path, "r");
	if (in == NULL) {
		printf("ERROR: %d not found", path);
		ERROREXIT;
	}

	TreeNode *syntaxTree;
	
	syntaxTree = parse();
	printTree(syntaxTree);

	fclose(in);
}