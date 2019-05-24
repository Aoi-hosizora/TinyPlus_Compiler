OBJ = main.o tokenutil.o lex.yy.o treeutil.o tiny.tab.o symtblutil.o analyseutil.o codeutil.o generate.o
EXE = compiler.exe

LEX = flex
YACC = bison

$(EXE): $(OBJ)
	gcc $(OBJ) -o $(EXE)
	@echo

main.o:
	gcc main.c -c 

lex.yy.o:
	gcc lex.yy.c -c 

tiny.tab.o:
	gcc tiny.tab.c -c 

generate.o:
	gcc generate.c -c

# Util
tokenutil.o:
	gcc util/tokenutil.c -c 

treeutil.o:
	gcc util/treeutil.c -c

symtblutil.o:
	gcc util/symtblutil.c -c

analyseutil.o:
	gcc util/analyseutil.c -c

codeutil.o:
	gcc util/codeutil.c -c

.PHONY: lex
lex:
	$(LEX) tiny.l
	@echo

# make yacc 后修改 #define YYLEX yylex() 为 #define YYLEX getToken()
.PHONY: yacc
yacc:
	$(YACC) -d tiny.y
	sed -ie 's/#define YYLEX		yylex()/#define YYLEX getToken()/g' tiny.tab.c
	rm tiny.tab.ce
	@echo

.PHONY: clean
clean:
	rm $(OBJ)