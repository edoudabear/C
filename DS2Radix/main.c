#include <stdio.h>
#include <stdlib.h>

int digit (int p,int k);
int* count(int arr[],int n,int k);
void print_int_list(int arr[],int n);
int* positions(int arr[],int n,int k);
void sort_by_digits(int arr[],int n,int k);
void sort(int arr[],int n,int N);
void sort_auto(int arr[],int n);

int main(int argc, char const *argv[])
{
    printf("%d\n",digit(156,32563587));
    int arr[12]={49,42,37,1,29,22,11,17,9,4,27,14};
    int *res=count(arr,12,0);
    print_int_list(res,10);
    int *pos_s=positions(arr,12,0);
    print_int_list(pos_s,10);
    sort_auto(arr,12);
    print_int_list(arr,12);
    free(pos_s);
    free(res);
    return 0;
}

int digit (int p, int k) {
    for (int i=0;i<k;++i) {
        p/=10;
        if (p==0) {
            return 0;
        }
    }
    return p%10;
}

void print_int_list(int arr[],int n) {
    printf("{");
    for (int i=0;i<n;++i) {
        printf("%d",arr[i]);
        if (i<n-1) {
            printf(",");
        }
    }
    printf("}\n");
}

int* count(int arr[],int n,int k) {
    int *res=malloc(10*sizeof(int));
    for (int i=0;i<10;++i) {
        res[i]=0;
    }
    for (int i=0;i<n;++i) {
        res[digit(arr[i],k)]++;
    }
    return res;
}

int* positions(int arr[],int n,int k) {
    int *res=count(arr,n,k);
    int acc=0;
    for (int i=0;i<10;++i) {
        int cpy=res[i];
        res[i]=acc;
        acc+=cpy;
    }
    return res;
}

int *arrayCopy(int array[],int n) {
    int *res=malloc(sizeof(int)*n);
    for (int i=0;i<n;++i) {
        res[i]=array[i];
    }
    return res;
}

void sort_by_digits(int arr[],int n,int k) {
    int *pos_s=positions(arr,n,k);
    int *arrCpy=arrayCopy(arr,n);
    for (int i=0;i<n;++i) {
        int digi=digit(arrCpy[i],k);
        arr[pos_s[digi]]=arrCpy[i];
        pos_s[digi]++;
    }
    free(pos_s);
    free(arrCpy);
}

void sort(int arr[],int n,int N) {
    for (int i=0;i<N;++i) {
        sort_by_digits(arr,n,i);
    }
}

int max(int a, int b) {
    return (a>b) ? a : b;
}

int n_digits(int n) {
    int nb_chiffres=0;
    while (n!=0) {
        n/=10;
        nb_chiffres++;
    }
    return nb_chiffres;
}

void sort_auto(int arr[],int n) {
    int maxi=0;
    for (int i=0;i<n;++i) {
        maxi=max(n_digits(arr[i]),maxi);
    }
    printf("N=%d\n",maxi);
    sort(arr,n,maxi);
}