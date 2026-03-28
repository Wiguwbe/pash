
.SUFFIXES:

pash: main.o parser.gen.o ast.o pash.o printer.o
	$(CC) -o $@ $^

packcc: packcc.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $<

# TODO pass it through `pegof` to optimize
parser.gen.c parser.gen.h: parser.peg defs.peg packcc pash_internal.h
	./packcc -o parser.gen $<

printer.o: printer.c printer.h ast.h
pash.o: pash.c pash.h ast.h pash_internal.h
main.o: main.c ast.h printer.h pash.h
ast.o: ast.c ast.h
packcc.o: packcc.c
parser.gen.o: parser.gen.c parser.gen.h ast.h pash_internal.h

# TODO rules to make pash so/library

.PHONY: clean
clean:
	rm -f *.o *.gen.*

