CFLAGS=-Wall -pedantic -std=c99
LDFLAGS=

.PHONY: all run clean mrproper

basic_ppm.o: basic_ppm.c basic_ppm.h
	gcc $(CFLAGS) -c basic_ppm.c

automates.o: automates.c basic_ppm.h
	gcc $(CFLAGS) -c automates.c

automates: automates.o basic_ppm.o
	gcc $(LDFLAGS) -o automates automates.o basic_ppm.o -lm

all: automates

run: all
	./automates

clean:
	rm -f *.o core

mrproper: clean
	rm -f automates


