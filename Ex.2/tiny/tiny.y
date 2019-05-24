%{
#define YYPARSER

#include "globals.h"
#include "treeutil.h"
#include "tokenutil.h"

#define YYSTYPE TreeNode *

static char *savedName;
static int savedLineNo;
static TreeNode *savedTree;
%}


// book-keeping tokens
%token ENDFILE ERROR
// reserved words
%token IF THEN ELSE END REPEAT UNTIL READ WRITE
// NEW
%token WHILE DO TO DOWNTO FOR ENDDO ENDWHILE MOD
// multicharacter
%token ID NUM
// operator
%token ASSIGN EQ LT PLUS MINUS TIMES OVER LPAREN RPAREN SEMI

%%

program     : stmt_seq { savedTree = $1;} 
            ;
stmt_seq    : stmt_seq SEMI stmt
                 { YYSTYPE t = $1;
                   if (t != NULL)
                   { while (t->sibling != NULL)
                        t = t->sibling;
                     t->sibling = $3;
                     $$ = $1; }
                     else $$ = $3;
                 }
            | stmt  { $$ = $1; }
            ;
stmt        : if_stmt { $$ = $1; }
            | repeat_stmt { $$ = $1; }
            | while_stmt { $$ = $1; }
            | assign_stmt { $$ = $1; }
            | for_stmt { $$ = $1; }
            | read_stmt { $$ = $1; }
            | write_stmt { $$ = $1; }
            | error  { $$ = NULL; }
            ;
if_stmt     : IF exp THEN stmt_seq END
                 { $$ = newStmtNode(IfK);
                   $$->child[0] = $2;
                   $$->child[1] = $4;
                 }
            | IF exp THEN stmt_seq ELSE stmt_seq END
                 { $$ = newStmtNode(IfK);
                   $$->child[0] = $2;
                   $$->child[1] = $4;
                   $$->child[2] = $6;
                 }
            ;
repeat_stmt : REPEAT stmt_seq UNTIL exp
                 { $$ = newStmtNode(RepeatK);
                   $$->child[0] = $2;
                   $$->child[1] = $4;
                 }
            ;
/////////////
while_stmt  : WHILE exp stmt_seq ENDWHILE // new
                 { $$ = newStmtNode(WhileK);
                   $$->child[0] = $2;
                   $$->child[1] = $3;
                 }
            ;
for_stmt    : FOR to_stmt DO stmt_seq ENDDO // new
                 { $$ = newStmtNode(ForK);
                   $$->child[0] = $2;
                   $$->child[1] = $4;
                 }
            ;
to_stmt     : idf ASSIGN factor TO factor // new
                 {
                   $$ = newStmtNode(ToK);
                   
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->child[2] = $5;
                   $$->attr.name = "To";
                 }
            | idf ASSIGN factor DOWNTO factor // new
                 {
                   $$ = newStmtNode(ToK);
                   
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->child[2] = $5;
                   $$->attr.name = "DownTo";
                 }
            ;
/////////////     
assign_stmt : ID { savedName = copyString(tokenString);
                   savedLineNo = lineno; }
              ASSIGN exp
                 { $$ = newStmtNode(AssignK);
                   $$->child[0] = $4;
                   $$->attr.name = savedName;
                   $$->lineno = savedLineNo;
                 }
            ;
read_stmt   : READ ID
                 { $$ = newStmtNode(ReadK);
                   $$->attr.name =
                     copyString(tokenString);
                 }
            ;
write_stmt  : WRITE exp
                 { $$ = newStmtNode(WriteK);
                   $$->child[0] = $2;
                 }
            ;
exp         : simple_exp LT simple_exp 
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = LT;
                 }
            | simple_exp EQ simple_exp
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = EQ;
                 }
            | simple_exp { $$ = $1; }
            ;
simple_exp  : simple_exp PLUS term 
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = PLUS;
                 }
            | simple_exp MINUS term
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = MINUS;
                 } 
            | term { $$ = $1; }
            ;
term        : term TIMES factor 
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = TIMES;
                 }
            | term OVER factor
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = OVER;
                 }
            | term MOD factor // new
                 { $$ = newExpNode(OpK);
                   $$->child[0] = $1;
                   $$->child[1] = $3;
                   $$->attr.op = MOD;
                 }
            | factor { $$ = $1; }
            ;
factor      : LPAREN exp RPAREN
                 { $$ = $2; }
            | NUM
                 { $$ = newExpNode(ConstK);
                   $$->attr.val = atoi(tokenString);
                 }
            | idf
            | error { $$ = NULL; }
            ;
idf         : ID { $$ = newExpNode(IdK);
                   $$->attr.name = copyString(tokenString);
                 }

%%

int yyerror(char *message) { 
	printf("Syntax error at line %d: %s\n",lineno,message);
	printf("Current token: ");
	printToken(yychar,tokenString);
	Error = TRUE;
	return 0;
}


TreeNode *parse(void) { 
	yyparse();
	return savedTree;
}