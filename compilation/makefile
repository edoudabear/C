CFLAGS=-Wall -pedantic -std=c99

.PHONY: all run clean mrproper

raytr.o: raytr.c vect.h material.h
	gcc $(CFLAGS) -c raytr.c

vect.o: vect.h vect.c
	gcc $(CFLAGS) -c vect.c

raytr: raytr.o vect.o
	gcc raytr.o vect.o -lm -o raytr

all: raytr

run: all
	./raytr

clean:
	rm -f *.o core

mrproper: clean
	rm -f raytr


