#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int *a;
    int length;
    printf("Enter int value : ");
    scanf("%d",&length);
    a=malloc(sizeof(int) * length);
    printf("[");
    for (int i=0;i<length;i++) {
        a[i]=i+1;
        printf("%d",a[i]);
        if (i<length-1) {
            printf(", ");
        }
    }
    printf("]");
    free(a);
}