CFLAGS=-Wall -pedantic -std=c99

.PHONY: all run clean mrproper

solitaire.c: 
	gcc $(CFLAGS) -c -o solitaire solitaire.c

all: solitaire

run: all
	./solitaire

clean:
	rm -f *.o core

mrproper: clean
	rm -f solitaire


