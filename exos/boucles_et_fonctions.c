#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int stein(int a,int b);
int e_sqrt(int a);

int main(int argc,char *argv[]) {
	//int a=0,b=0;
	if (strcmp(argv[0],"help")==0 || argc!=2+1) {
		printf("When calling the program, you must pass a and b as arguments.\nThis will return PGCD(a,b) and E(sqrt(a))");
		return 0;
	}
	//printf("Entrez les entiers a et b :\n");
	//scanf("%d %d",&a,&b);
	int a = atoi(argv[1]),  b = atoi(argv[2]);
	printf("PGCD(%d,%d)=%d\n",a,b,stein(a,b));
	printf("E(sqrt(%d))=%d\n",a,e_sqrt(a));
	return 0;
}

int e_sqrt(int p) {
	int b=p+1;
	for (int a=p;a<b;) {
		b=a;
		a=(a*a+p)/(2*a);
	}
	return b;
}

int stein (int a, int b) {
	printf("%dˆ%d\n",a,b);
	if (a==0 || b==0)
		return a+b;;
	if (a%2==0 && b%2==0)
		return 2*stein(a/2,b/2);
	if (a%2==0 && b%2==1)
		return stein(a/2,b);
	if (a%2==1 && b%2==0)
		return stein(a,b/2);
	if (a<b)
		return stein(b%a,a);
	return stein(a%b,b);
}
