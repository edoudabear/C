#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ruban.h"

//-------------------------------------------------

// Etat de la machine (global)
int current_state = 0;

// Programme
int prog_new_state[253][11];
int prog_new_value[253][11];
bool prog_move_right[253][11];

//-------------------------------------------------

void clear_program(void) {
    for (int state=0; state<253; ++state) {
        for (int value=0; value<11; ++value) {
            prog_new_state[state][value] = ERROR;
            prog_new_value[state][value] = 0;
            prog_move_right[state][value] = true;
        }
    }
}

void set_instruction(int state, int value, int new_state, int new_value, bool move_right) {
    prog_new_state[state][value]=new_state;
    prog_new_value[state][value]=new_value;
    prog_move_right[state][value]=move_right;
}

void show(void) {
    show_tape(-4, 20);
    printf("Etat : ");
    if (current_state==ACCEPT) {
        printf("[ACCEPT]\n\n");
    } else if (current_state==REJECT) {
        printf("[REJECT]\n\n");
    } else if (current_state==ERROR) {
        printf("[ERROR]\n\n");
    } else {
        printf("<%d>\n\n", current_state);
    }
}

void execute(void) {
    show();

    while (current_state<252 /* <- à modifier */) {
        int value=read();
        write(prog_new_value[current_state][value]);
        if (prog_move_right[current_state][value]) {
            right();
        } else {
            left();
        }
        current_state=prog_new_state[current_state][value];
        show();
    }
}

// Programmes pour la machine

void load_program_to_1() {
    set_instruction(0, SPACE, 0, SPACE, true);
    for (int v=0; v<=9; ++v) {
        set_instruction(0, v, 1, 1, true);
        set_instruction(1, v, 1, 1, true);
    }
    set_instruction(1, SPACE, ACCEPT, SPACE, true);
}

void load_program_move_start() {
    for (int i=0;i<255;++i) {
        if (i!=42) {
            set_instruction(0, SPACE, 0, SPACE, true);
        } else {
            set_instruction(42, SPACE, 253, SPACE, true);
        }
    }
    for (int v=0; v<=9; ++v) {
        set_instruction(0, v, 42, v, false);
        set_instruction(1, v, 42, v, false);
    }
}

void load_program_move_end() {
    for (int i=0;i<255;++i) {
        if (i!=42) {
            set_instruction(0, SPACE, 0, SPACE, true);
        } else {
            set_instruction(42, SPACE, 253, SPACE, false);
        }
    }
    for (int v=0; v<=9; ++v) {
        set_instruction(0, v, 42, v, true);
        set_instruction(42, v, 42, v, true);
    }
}

void load_program_delete() {
    for (int i=0;i<255;++i) {
        if (i!=42) {
            set_instruction(0, SPACE, 0, SPACE, true);
        } else {
            set_instruction(42, SPACE, 253, 1, false);
        }
    }
    for (int v=0; v<=9; ++v) {
        set_instruction(0, v, 42, SPACE, true);
        set_instruction(42, v, 42, SPACE, true);
    }
}

void load_program_append_1() {
    for (int i=0;i<255;++i) {
        if (i!=42) {
            set_instruction(0, SPACE, 0, SPACE, true);
        } else {
            set_instruction(42, SPACE, 253, 1, false);
        }
    }
    for (int v=0; v<=9; ++v) {
        set_instruction(0, v, 42, v, true);
        set_instruction(42, v, 42, v, true);
    }
}

void load_program_swap() {
    for (int i=0;i<255;++i) {
        if (i!=42) {
            set_instruction(0, SPACE, 0, SPACE, true);
        } else {
            set_instruction(42, SPACE, 253, SPACE, false);
        }
    }
    set_instruction(0, 1, 42, 0, true);
    set_instruction(42, 1, 42, 0, true);

    set_instruction(0, 0, 42, 1, true);
    set_instruction(42, 0, 42, 1, true);
}

void load_program_shift_left() { // Incomplet
    set_instruction(20, SPACE, 20, SPACE, true);
    for (int i=0;i<10;++i) {
        set_instruction(i, SPACE, 20, i, true);
    }
    for (int v=0; v<=9; ++v) {
        set_instruction(20, v, v, v, false);
        set_instruction(42, v, v, SPACE, true);
    }
}

void load_program_concatenate() {
    // à completer
}

void load_program_is_length_even() {
    for (int i=0;i<255;++i) {
        if (i!=42 && i!=43) {
            set_instruction(0, SPACE, 0, SPACE, true);
        } else if (i==42) {
            set_instruction(42, SPACE, 253, SPACE, false);
        } else {
            set_instruction(43, SPACE, 254, SPACE, false);
        }
    }
    for (int v=0; v<=9; ++v) {
        set_instruction(0, v, 43, v, true);
        set_instruction(42, v, 43, v, true);
        set_instruction(43, v, 42, v, true);
    }
}

void load_program_is_palindrome() {
    // à completer
}

void load_program_collatz() {
    // à completer
}

// Programme principal

int main(int argc, char* argv[]) {
    clear_program();
    //load_program_to_1();
    //load_program_move_start();
    //load_program_move_end();
    //load_program_append_1();
    //load_program_delete();
    //load_program_swap();
    //load_program_is_length_even();
    load_program_shift_left();

    int data[] = { SPACE, SPACE, 0, 1, 0, 1, 1, SPACE, 2, 3, 1, SPACE };
    init(data, sizeof(data)/sizeof(int));

    current_state=0;
    execute();

    return 0;
}
