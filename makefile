all : bshell

bshell : lex.yy.o y.tab.o commandtb.o
	gcc lex.yy.o y.tab.o commandtb.o -o bshell -lreadline

commandtb.o : commandtb.c
	gcc -c commandtb.c  -I/usr/include/readline -lreadline -lncurses
	
lex.yy.o :	lex.yy.c
	gcc -c lex.yy.c -ll  -I/usr/include/readline -lreadline
	
y.tab.o :	y.tab.c
	gcc -c y.tab.c -ly -I/usr/include/readline -lreadline

y.tab.c : shell.y
	yacc -d -v shell.y  

lex.yy.c: lexer.l
	lex lexer.l 

clean : 
	rm -rf *.o
