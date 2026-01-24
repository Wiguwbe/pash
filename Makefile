
pash: main.o pash.tab.o lex.yy.o ast.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $<

%.tab.c %.tab.h: %.y
	bison --debug -H $<

lex.yy.c: pash.l pash.tab.h
	flex $<

main.o: main.c ast.h pash.tab.h
pash.tab.o: pash.tab.c pash.tab.h ast.h
lex.yy.o: lex.yy.c
pash.tab.c: pash.y
pash.tab.h: pash.y
ast.o: ast.c ast.h

