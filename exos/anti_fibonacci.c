#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define NTERMS 150

int * anti_fibonacci (int count);

int main(void) {
	int *terms;
	int n;
	printf("ENTER INT VALUE : ");
	scanf("%d",&n);
	if (n<0) {
		printf("ERROR MUST BE POSITIVE\n");
		return 1;
	}
	terms=anti_fibonacci(n);
	printf("[");
	for (int i=0;i<n;i++) {
	   printf("%d",terms[i]);
	   if (i<n-1) {
		printf(",");
	   }
	}
	printf("]\n");
	return EXIT_SUCCESS;
}

int max(int a, int b) {
	if (a>b) {
		return a;
	}
	return b;
}

int * anti_fibonacci (int count) {
	if (!(count<50000000)) {
		printf("INDEX OUT OF RANGE\n");
		return 0;
	}
	static int terms[50000000]={0};
	int n1=1,n2=2,ind1Check=1,ind2Check=1;
	for (int i=1;i<count;i++) {
	   terms[i]=n1+n2; // On ajoute le terme calculé à l'itération précédente (où celle issue de l'init)
	   n1=n2+1;
           while (terms[ind1Check]<=n1 && terms[ind1Check]!=0) { // on détermine le premier nouvel entier naturel qui n'est pas dans la suite
	       if (terms[ind1Check]==n1) {
	       	   n1++;
	       }
	       ind1Check++;
	   }
	   ind2Check=max(ind1Check,ind2Check);
	   n2=n1+1;
	   while (terms[ind2Check]<=n2 && terms[ind2Check]!=0) { // On en déduit le second tout en vérifiant qu'il est bien absent des termes de la suite
	       if (terms[ind2Check]==n2) {
		   n2++;
	       }
	       ind2Check++;
	   }
	}
	return terms;
}
