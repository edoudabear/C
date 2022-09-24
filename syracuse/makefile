CFLAGS=-Wall -pedantic -std=c99

.PHONY: all run clean mrproper

syracuse.o: syracuse.c
	gcc $(CFLAGS) -c syracuse.c

syracuse: syracuse.o
	gcc -o syracuse syracuse.o

all: syracuse

run: all
	./syracuse

clean:
	rm -f *.o core

mrproper: clean
	rm -f syracuse


