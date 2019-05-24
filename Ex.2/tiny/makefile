OBJ = main.o tokenutil.o treeutil.o lex.yy.o tiny.tab.o
EXE = parser.exe

LEX = flex
YACC = bison

$(EXE): $(OBJ)
	gcc $(OBJ) -o $(EXE)

main.o: main.c globals.h
	gcc main.c -c 

tokenutil.o: tokenutil.c tokenutil.h globals.h
	gcc tokenutil.c -c 

treeutil.o: treeutil.c treeutil.h globals.h
	gcc treeutil.c -c

lex.yy.o: lex.yy.c tokenutil.o
	gcc lex.yy.c -c 

tiny.tab.o: tiny.tab.c treeutil.o
	gcc tiny.tab.c -c 

.PHONY: lex
lex:
	$(LEX) tiny.l

# make yacc 后修改 #define YYLEX yylex() 为 #define YYLEX getToken()
.PHONY: yacc
yacc:
	$(YACC) -d tiny.y
	sed -ie 's/#define YYLEX		yylex()/#define YYLEX getToken()/g' tiny.tab.c
	rm tiny.tab.ce

.PHONY: clean
clean:
	rm $(OBJ)