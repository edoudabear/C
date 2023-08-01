CFLAGS=-Wall -pedantic -std=c99
LDFLAGS=

.PHONY: all run clean mrproper

heapq.o: heapq.c
	gcc $(CFLAGS) -c heapq.c

bus.o: bus.c
	gcc $(CFLAGS) -c bus.c

bus: bus.o heapq.o
	gcc $(LDFLAGS) -o bus heapq.o bus.o

all: bus

run: all
	./bus

clean:
	rm -f *.o core

mrproper: clean
	rm -f bus
