CFLAGS=-Wall -pedantic
LDFLAGS=
PYTHON=/cygdrive/c/Program\ Files/winPython64-3.6.7.0-Qt5/python-3.6.7.amd64/python

.PHONY: all run clean mrproper

basic_ppm.o: basic_ppm.c basic_ppm.h
	gcc $(CFLAGS) -c basic_ppm.c

disp.o: disp.c disp.h
	gcc $(CFLAGS) -c disp.c

automates_2D.o: automates_2D.c loop.h disp.h
	gcc $(CFLAGS) -c automates_2D.c

automates_2D: automates_2D.o disp.o basic_ppm.o
	gcc $(LDFLAGS) -o automates_2D automates_2D.o disp.o basic_ppm.o -lm

all: automates_2D

run: all
	./automates_2D

run_py: all
	./automates_2D | $(PYTHON) disp.py

clean:
	rm -f *.o core

mrproper: clean
	rm -f automates_2D


