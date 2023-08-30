#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int *a;
    if (argc<2) {
	printf("NO ARGUMENT PROVIDED\n");
	return 1;
    }
    int length=atoi(argv[1]);
    a=malloc(sizeof(int) * length);
    printf("[");
    for (int i=0;i<length;i++) {
        a[i]=i+1;
        printf("%d",a[i]);
        if (i<length-1) {
            printf(", ");
        }
    }
    printf("]\n");
    free(a);
}
