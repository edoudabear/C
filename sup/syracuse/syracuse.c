#include <stdio.h>
#define UZERO 177

int next(const int u) {
    if (u%2 == 0) {
        return u/2;
    } else {
        return 3*u+1;
    }
}

int main(void) {
    int u=2,i=2;
    while (u!=404) {
        u=next(u);
        if (u==1) {
            u=(i++);
        }
    }
    printf("le plus petit u(0) ayant un terme égal à 404 est u(0)=%d\n",i);
}
