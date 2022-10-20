CFLAGS=-Wall -pedantic -std=c99
LDFLAGS=

.PHONY: all run clean mrproper

ruban.o: ruban.h ruban.c
	gcc $(CFLAGS) -c ruban.c

turing.o: turing.c ruban.h
	gcc $(CFLAGS) -c turing.c

turing: turing.o ruban.o
	gcc $(LDFLAGS) -o turing turing.o ruban.o -lm

all: turing

run: all
	./turing

clean:
	rm -f *.o core

mrproper: clean
	rm -f turing


