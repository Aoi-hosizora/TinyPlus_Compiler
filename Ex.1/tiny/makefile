scanner.exe: main.o lex.yy.o util.o
	gcc main.o lex.yy.o util.o -o scanner.exe
main.o: main.c
	gcc main.c -c
util.o: util.c
	gcc util.c -c
lex.yy.o: tiny.l
	flex tiny.l
	gcc lex.yy.c -c
.PHONY: clean
clean:
	rm main.o lex.yy.o lex.yy.c util.o