#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ruban.h"

struct {
    int* tape;
    int start;
    int length;
    int pos;
} _t_machine = { NULL, 0, 0, 0 };

void init(int tape[], int length) {
    if (_t_machine.tape != NULL) {
        free(_t_machine.tape);
        _t_machine.tape=NULL;
    }

    _t_machine.tape = malloc(2*length*sizeof(int));
    for(int i=0; i<2*length; ++i) {
        _t_machine.tape[i] = SPACE;
    }
    memcpy(_t_machine.tape, tape, length*sizeof(int));

    _t_machine.start = 0;
    _t_machine.length = length;

    _t_machine.pos = 0;
}

void left(void) {
    _t_machine.pos--;
}

void right(void) {
    _t_machine.pos++;
}

int _read(int index) {
    if (index<0 || index>=_t_machine.length) {
        return SPACE;
    }
    return _t_machine.tape[index];
}

int read(void) {
    return _read(_t_machine.pos-_t_machine.start);
}

void write(int data) {
    int index = _t_machine.pos-_t_machine.start;

    if (data<0 || data>10) {
        fprintf(stderr, "Invalid write on tape (data=%d)\n", data);
        return;
    }

    if (index < 0) {
        int* new_tape = malloc(2*_t_machine.length*sizeof(int));
        for(int i=0; i<_t_machine.length; ++i) {
            new_tape[i] = SPACE;
            new_tape[_t_machine.length+i] = _t_machine.tape[i];
        }
        free(_t_machine.tape);
        _t_machine.tape = new_tape;
        _t_machine.start -= _t_machine.length;
        _t_machine.length *= 2;
        write(data);
    } else if (index >= _t_machine.length) {
        int* new_tape = malloc(2*_t_machine.length*sizeof(int));
        for(int i=0; i<_t_machine.length; ++i) {
            new_tape[i] = _t_machine.tape[i];
            new_tape[_t_machine.length+i] = SPACE;
        }
        free(_t_machine.tape);
        _t_machine.tape = new_tape;
        _t_machine.length *= 2;
        write(data);
    } else {
        _t_machine.tape[index] = data;
    }
}

void show_tape(int imin, int imax) {
    printf("\n...");

    if (imin==0) {
        printf("[");
    } else if (imin==1) {
        printf("]");
    } else {
        printf("|");
    }

    for(int i=imin; i<=imax; ++i) {
        int value = _read(i);
        if (value == SPACE) {
            printf(" ");
        } else {
            printf("%d", value);
        }
        if (i==-1) {
            printf("[");
        } else if (i==0) {
            printf("]");
        } else {
            printf("|");
        }
    }

    printf("...\n    ");

    if (_t_machine.pos>=imin && _t_machine.pos<=imax) {
        for(int i=imin; i<_t_machine.pos; ++i) {
            printf("  ");
        }
        printf("^");
    }
    printf("\n");
}
