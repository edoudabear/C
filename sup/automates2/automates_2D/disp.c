#include "disp.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void _disp(int tab[], int nbl, int nbc, char* ch[], int nb_states, int wait) {
    printf("\033[1;1H\033[2J");
    for(int i=0; i<nbl*nbc; ++i) {
        if (tab[i]<0 || tab[i] >= nb_states) {
            printf("\nIllegal state <%d> at (%d, %d)\n", tab[i], i/nbc, i%nbc);
            return;
        }
        printf("%s", ch[tab[i]]);
        if (i%nbc==nbc-1) {
            printf("\n");
        }
    }
    fflush(stdout);
    usleep(100000*wait);
}

char* _LangtonElemsCol[] = { "\033[37;40m ", "\033[30;104m1\033[37;40m", "\033[30;101m2\033[37;40m", "\033[30;102m3\033[37;40m", "\033[30;103m4\033[37;40m", "\033[30;105m5\033[37;40m", "\033[30;107m6\033[37;40m", 
"\033[30;106m7\033[37;40m" };
char* _LangtonElemsAsc[] = { " ", "@", "*", "+", "x", "o", "#", "%" };

char* _RotorElemsCol[] = { "\033[37;40m ", "\033[30;101m>\033[37;40m", "\033[30;102m^\033[37;40m", "\033[30;103m<\033[37;40m", "\033[30;104mv\033[37;40m" };
char* _RotorElemsAsc[] = { " ", ">", "^", "<", "v" };

char* _ConwayElemsCol[] = { "\033[37;40m ", "\033[30;103m@\033[37;40m", "\033[30;41m.\033[37;40m", "\033[30;44mO\033[37;40m" };
char* _ConwayElemsAsc[] = { " ", "@", ".", "O" };

char* _LoopElemsCol[] = { "\033[37;40m ", "\033[30;104m1\033[37;40m", "\033[30;101m2\033[37;40m", "\033[30;102m3\033[37;40m", "\033[30;103m4\033[37;40m", "\033[30;105m5\033[37;40m", "\033[30;107m6\033[37;40m", 
"\033[30;106m7\033[37;40m" };
char* _LoopElemsAsc[] = { " ", "1", "2", "3", "4", "5", "6", "7" };

bool _asc = false;

void disable_color(void) {
    _asc = true;
}

void disp_Langton(int tab[], int nbl, int nbc, int wait) {
    _disp(tab, nbl, nbc, _asc?_LangtonElemsAsc:_LangtonElemsCol, 8, wait);
}

void disp_rotorouteur(int tab[], int nbl, int nbc, int wait) {
    _disp(tab, nbl, nbc, _asc?_RotorElemsAsc:_RotorElemsCol, 5, wait);
}

void disp_Conway(int tab[], int nbl, int nbc, int wait) {
    _disp(tab, nbl, nbc, _asc?_ConwayElemsAsc:_ConwayElemsCol, 4, wait);
}

void disp_Loop(int tab[], int nbl, int nbc, int wait) {
    _disp(tab, nbl, nbc, _asc?_LoopElemsAsc:_LoopElemsCol, 8, wait);
}

