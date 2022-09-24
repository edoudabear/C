#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int triplets(int n);
int triplets_ref(int n);

int main(int argc,char *argv[]) {
	//int a=0,b=0;
	if (strcmp(argv[0],"help")==0 || argc!=1+1) {
		printf("When calling the program, you must pass a and b as arguments.\nThis will return PGCD(a,b) and E(sqrt(a))");
		return 0;
	}
	//printf("Entrez les entiers a et b :\n");
	//scanf("%d %d",&a,&b);
	int n = atoi(argv[1]);
	printf("Nombre de triplets Pythagoriciens pour n=%d : %d\n",n,triplets(n));
	printf("Nombre de tiplets Pythagoriciens (d'après la fonction de référence) : %d\n",triplets_ref(n));
	return 0;
}

int triplets (int n) {
	int acc=0;
	for (int c=1;c<=n;c++) {
		for (int b=n-c,a=0;a<=b;a++,b--) {
			//printf("C%d A%d B%d N%d\n",c,a,b,a+b+c);
			if (a*a+b*b==c*c) {
				//printf("true\n");
				acc++;
			}
		}
	}
	return acc;
}

int triplets_ref(int n) {
	int count=0;
	for (int a=0,b=n/2;a<=b;a++) {
		int test=a*a+b*b-(n-a-b)*(n-a-b);
		if (test==0) {
			count++;
		} if (test>=0) {
			b--;
		}
	}
	return count;
}
