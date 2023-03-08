#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encode(char* message,char *cle) {
  int tour=0, n=strlen(message),l_cle=strlen(cle);
  for (int i=0;i<n;i++) {
    if (message[i]<='Z' && message[i]>='A') {
       message[i]=(message[i]+cle[tour])%('Z'-'A'+1)+'A';
       message[i]=(message[i]=='Z'+1) ? 'A' : message[i];
    }
    tour=(tour+1)%l_cle;
  }
}

void inverse(char *cle) {
   int l_cle=strlen(cle);
   for (int i=0;i<l_cle;++i) {
      cle[i]=('Z'-(cle[i])+'A'+1);
   }
}

int main(int argc, char** argv) {
        char str[24],str2[5];
        strcpy(str,"ALLONS VOIR SI LA ROSE");
        strcpy(str2,"BLEU");
        encode(str,str2);
        printf("%s\n",str);
        inverse(str2);
        encode(str,str2);
	printf("%s\n",str);
	return 0;
}
