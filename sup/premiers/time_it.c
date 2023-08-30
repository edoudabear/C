#include <time.h>

double time_it(void foo(void)) {
    double duration = 0.0;
    int nb_iter = 5;
    
    while(duration<1.0) {
        nb_iter*=10;
        clock_t start = clock();
        for (int i=0; i<nb_iter; ++i) {
            foo();
        }
        clock_t stop = clock();
        duration = (double)(stop-start)/CLOCKS_PER_SEC;
    }
    return duration/nb_iter*1000.0;
}
