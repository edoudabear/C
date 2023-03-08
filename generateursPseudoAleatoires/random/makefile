CFLAGS=-Wall -pedantic -std=c99
LDFLAGS=

.PHONY: all run clean mrproper

basic_ppm.o: basic_ppm.c basic_ppm.h
	gcc $(CFLAGS) -c basic_ppm.c

random.o: random.c basic_ppm.h
	gcc $(CFLAGS) -c random.c

random: random.o basic_ppm.o
	gcc $(LDFLAGS) -o random random.o basic_ppm.o

all: random

run: all
	./random

clean:
	rm -f *.o core

mrproper: clean
	rm -f random



