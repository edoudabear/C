CFLAGS=-Wall -pedantic -std=c99
LDFLAGS=

.PHONY: all run clean mrproper

basic_ppm.o: basic_ppm.c basic_ppm.h
	gcc $(CFLAGS) -c basic_ppm.c

time_it.o: time_it.c time_it.h
	gcc $(CFLAGS) -c time_it.c

premiers.o: premiers.c basic_ppm.h time_it.h
	gcc $(CFLAGS) -c premiers.c

premiers: premiers.o basic_ppm.o time_it.o
	gcc $(LDFLAGS) -o premiers premiers.o basic_ppm.o time_it.o -lm

all: premiers

run: all
	./premiers

clean:
	rm -f *.o core

mrproper: clean
	rm -f premiers


