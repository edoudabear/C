CFLAGS=-Wall -pedantic -std=c99
LDFLAGS=

.PHONY: all run clean mrproper

alloc.o: alloc.c
	gcc $(CFLAGS) -c alloc.c

alloc: alloc.o
	gcc $(LDFLAGS) -o alloc alloc.o

all: alloc

run: all
	./alloc

debug: all
	./alloc --debug

clean:
	rm -f *.o core

mrproper: clean
	rm -f alloc
