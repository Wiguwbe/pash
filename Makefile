
.SUFFIXES:

CFLAGS := -Wall -Wextra -O2 -fPIC
LDFLAGS :=

CORE_OBJS := ast.o parser.gen.o pash.o
PRINTER_OBJS := printer.o printer-pretty.o printer-json.o

.PHONY: all
all: pash libpash.so libpash.a libpash.c libpash.h

pash: main.o $(CORE_OBJS) $(PRINTER_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

libpash.so: $(CORE_OBJS)
	$(CC) -shared -o $@ $^

libpash.a: $(CORE_OBJS)
	$(AR) rcs $@ $^

libpash.c: ast.c parser.gen.c pash.c ast.h ast_internal.h pash.h pash_internal.h parser.gen.h
	python3 ./amalgamate.py -p libpash -o $@ ast.c pash.c parser.gen.c

libpash.h: ast.h pash.h
	python3 ./amalgamate.py -p libpash -o $@ $^

packcc: packcc.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# TODO pass it through `pegof` to optimize
parser.gen.c parser.gen.h: parser.peg defs.peg packcc pash_internal.h
	./packcc -o parser.gen $<

printer.o: printer.c printer.h ast.h
printer-pretty.o: printer-pretty.c ast.h
printer-json.o: printer-json.c ast.h
pash.o: pash.c pash.h ast.h pash_internal.h
main.o: main.c ast.h printer.h pash.h
ast.o: ast.c ast.h
packcc.o: packcc.c
parser.gen.o: parser.gen.c parser.gen.h ast.h pash_internal.h ast_internal.h

# TODO rules to make pash so/library

.PHONY: clean
clean:
	rm -f *.o *.gen.*

.PHONY: tests
tests: pash
	./tests/run.sh
