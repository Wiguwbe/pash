
.SUFFIXES:

pash: main.o pash.tab.o pash.yy.o ast.o pash.o printer.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $<

%.tab.c %.tab.h: %.y
	bison --debug -Wall -H $<

pash.yy.c pash.yy.h: pash.l pash.tab.h
	flex -o pash.yy.c --header-file=pash.yy.h $<

printer.o: printer.c printer.h ast.h
pash.o: pash.c pash.h pash.tab.h pash.yy.h ast.h
main.o: main.c ast.h printer.h pash.h pash.tab.h pash.yy.h
pash.tab.o: pash.tab.c pash.tab.h ast.h
pash.yy.o: pash.yy.c pash.yy.h
pash.tab.c: pash.y
pash.tab.h: pash.y
ast.o: ast.c ast.h

# TODO rules to make pash so/library

.PHONY: clean
clean:
	rm -f *.o *.tab.* *.yy.*

