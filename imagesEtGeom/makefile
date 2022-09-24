CFLAGS=-Wall -pedantic -std=c99

.PHONY: all run clean mrproper

basic_ppm.o: basic_ppm.c basic_ppm.h
	gcc $(CFLAGS) -c basic_ppm.c

geometrie.o: geometrie.c basic_ppm.h
	gcc $(CFLAGS) -c geometrie.c

geometrie: geometrie.o basic_ppm.o
	gcc -o geometrie geometrie.o basic_ppm.o -lm

all: geometrie

run: all
	./geometrie

clean:
	rm -f *.o core

mrproper: clean
	rm -f geometrie


