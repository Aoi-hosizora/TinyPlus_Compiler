#include "globals.h"
#include "util/symtblutil.h"
#include "util/codeutil.h"
#include "generate.h"

/* tmpOffset is the memory offset for temps  
It is decremented each time a temp is  
stored, and incremeted when loaded again  
*/
static int tmpOffset = 0;

static void cGen(TreeNode *tree);

static void genStmt(TreeNode *tree) {
	TreeNode *p1, *p2, *p3, *p4;
	int savedLoc1, savedLoc2, currentLoc;
	int loc;

	switch (tree->kind.stmt) {
		case IfK:
			emitComment("If Start");
			p1 = tree->child[0];
			p2 = tree->child[1];
			p3 = tree->child[2];

			/* generate code for test expression */
			cGen(p1);
			savedLoc1 = emitSkip(1);
			/* recurse on then part */
			cGen(p2);
			savedLoc2 = emitSkip(1);
			currentLoc = emitSkip(0);
			emitBackup(savedLoc1);
			emitRM_Abs("JEQ", ac, currentLoc);
			emitRestore();
			/* recurse on else part */
			cGen(p3);
			currentLoc = emitSkip(0);
			emitBackup(savedLoc2);
			emitRM_Abs("LDA", pc, currentLoc);
			emitRestore();
			emitComment("If End");
		break;
		case RepeatK:
			emitComment("Repeat Start");
			p1 = tree->child[0];
			p2 = tree->child[1];
			savedLoc1 = emitSkip(0);
			/* generate code for body */
			cGen(p1);
			/* generate code for test */
			cGen(p2);
			emitComment("Repeat End");
		break;
		case WhileK: // new
			emitComment("While Start");
			p1 = tree->child[0];
			p2 = tree->child[1];

			savedLoc1 = emitSkip(1);
			cGen(p2);

			currentLoc = emitSkip(0);
			emitBackup(savedLoc1);
			emitRM("JMP", currentLoc, 0, 0);
			emitRestore();

			cGen(p1);
			emitComment("While End");
		break;
		case ForK: // new
			emitComment("For Start");
			p1 = tree->child[0]->child[0]; // To Var.
			p2 = tree->child[0]->child[1]; // To Cons.
			p3 = tree->child[0]->child[2]; // To Cons.
			p4 = tree->child[1]; // Code
			int IsTo = !strcmp(tree->child[0]->attr.name, "TO");
			TreeNode *cmp = newExpNode(OpK);
			cmp->child[0] = p1; // i
			cmp->child[1] = p3; // 3
			cmp->attr.op = EQ; // i = 3

			// for i:= 1 to 3 do

			cGen(p2); // 1
			loc = st_lookup_loc(p1->attr.name); // i
			emitRM("ST", ac, loc, gp); // i := 1

			savedLoc1 = emitSkip(0);
			cGen(p4); // repeat code

			loc = st_lookup_loc(p1->attr.name); // i
			if (IsTo) 
				emitRO("INC", loc, 0, 0); // i + 1
			else
				emitRO("DEC", loc, 0, 0); // i - 1

			cGen(cmp);
			// emitRM("JEQ", ac, 2, pc);
			emitComment("For End");
		break;
		case AssignK:
			emitComment("Assign Start");
			/* generate code for rhs */
			cGen(tree->child[0]);
			/* now store value */
			loc = st_lookup_loc(tree->attr.name);
			emitRM("ST", ac, loc, gp);
			emitComment("Assign End");
		break;
		case ReadK:
			emitComment("Read Start");
			emitRO("IN", ac, 0, 0);
			loc = st_lookup_loc(tree->attr.name);
			emitRM("ST", ac, loc, gp);
			emitComment("Read End");
		break;
		case WriteK:
			emitComment("Write Start");
			/* generate code for expression to write */
			cGen(tree->child[0]);
			/* now output it */
			emitRO("OUT", ac, 0, 0);
			emitComment("Write End");
		break;
		default:
		break;
	}
}

/* Procedure genExp generates code at an expression node */
static void genExp(TreeNode *tree) {
	int loc;
	TreeNode *p1, *p2;
	switch (tree->kind.exp) {
		case ConstK:
			/* gen code to load integer constant using LDC */
			emitRM("LDC", ac, tree->attr.val, 0);
		break;

		case IdK:
			loc = st_lookup_loc(tree->attr.name);
			emitRM("LD", ac, loc, gp);
		break;

		case OpK:
			p1 = tree->child[0];
			p2 = tree->child[1];
			/* gen code for ac = left arg */
			cGen(p1);
			/* gen code to push left operand */
			emitRM("ST", ac, tmpOffset--, mp);
			/* gen code for ac = right operand */
			cGen(p2);
			/* now load left operand */
			emitRM("LD", ac1, ++tmpOffset, mp);
			switch (tree->attr.op) {
				case PLUS:
					emitRO("ADD", ac, ac1, ac);
				break;
				case MINUS:
					emitRO("SUB", ac, ac1, ac);
				break;
				case TIMES:
					emitRO("MUL", ac, ac1, ac);
				break;
				case OVER:
					emitRO("DIV", ac, ac1, ac);
				break;
				case MOD: // new
					emitRO("MOD", ac, ac1, ac);
				break;
				case LT:
					emitRO("SUB", ac, ac1, ac);
					emitRM("JLT", ac, 2, pc);
					emitRM("LDC", ac, 0, ac);
					emitRM("LDA", pc, 1, pc);
					emitRM("LDC", ac, 1, ac);
				break;
				case EQ:
					emitRO("SUB", ac, ac1, ac);
					emitRM("JEQ", ac, 2, pc);
					emitRM("LDC", ac, 0, ac);
					emitRM("LDA", pc, 1, pc);
					emitRM("LDC", ac, 1, ac);
				break;
				default:
				break;
			}
		break;
		default:
		break;
	}
}

/* Procedure cGen recursively generates code by  
* tree traversal  
*/
static void cGen(TreeNode *tree) {
	if (tree != NULL) {
		switch (tree->nodekind) {
			case StmtK:
				genStmt(tree);
			break;
			case ExpK:
				genExp(tree);
			break;
			default:
			break;
		}
		cGen(tree->sibling);
	}
}

/**********************************************/
/* the primary function of the code generator */
/**********************************************/
/* Procedure codeGen generates code to a code  
* file by traversal of the syntax tree. The  
* second parameter (codefile) is the file name  
* of the code file, and is used to print the  
* file name as a comment in the code file  
*/
void codeGen(TreeNode *syntaxTree) {
	/* generate standard prelude */
	emitComment("Code Start");

	emitRM("LD", mp, 0, ac);
	emitRM("ST", ac, 0, ac);
	/* generate code for TINY program */
	cGen(syntaxTree);
	/* finish */
	emitRO("HALT", 0, 0, 0);
	emitComment("Code End");
}