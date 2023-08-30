CFLAGS=-Wall -pedantic -std=c99
LDFLAGS=

.PHONY: all run clean mrproper

redblacks.o: redblacks.c
	gcc $(CFLAGS) -c redblacks.c

redblacks: redblacks.o
	gcc $(LDFLAGS) -o redblacks redblacks.o

all: redblacks

run: all
	./redblacks

clean:
	rm -f *.o core

mrproper: clean
	rm -f redblacks


