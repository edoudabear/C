#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // Pour effectuer des tests de rapidité

struct { int n; char** s; } words;

void read_data() {
    char buffer[21];
    FILE* f=fopen("dict.dat", "r");
    // Lecture du nombre total de mots
    fscanf(f, "%d\n", &words.n);
    // Allocation du tableau pour recueillir les chaînes
    words.s = (char**)malloc(words.n * sizeof(char*));
    if (words.s == NULL) { words.n = 0; return; } // Echec d'allocation
    // Remplissage du tableau
    for (int i=0; i<words.n; ++i) {
        fscanf(f, "%20s\n", buffer);
        words.s[i] = strdup(buffer);
    }
    fclose(f);
}

char *words_c=NULL; // Tableau contenant la taille du i-ième mot à la case 18*i et le mot entre les cases 18*i+1 et 18*i+16
char *resBuffer=NULL; // Ne sert que si on veut ne pas imprimer à la volée (dans ce cas, décommenter deux lignes dans la fonction find et penser à décommenter l'affichage "à la volée")

void find(char* req) {
    int n=words.n;
    resBuffer[0]='\0';
    // Affiche les mots que l'on peut construire à partir des lettres de req
    int occs_save[27]={0},occs[27]={0}, req_size=strlen(req);
    for (int i=0;i<req_size;++i) {
        if (req[i]=='?') {
            occs_save[26]++;
        } else if (req[i]>='a' && req[i]<='z') {
            occs_save[req[i]-'a']++;
        } else {
            printf("Invalid query : check your input.\n");
            return;
        }
    }
    if (req_size>15) {
        return;
    }
    for (int i=0; i<n; ++i) {
        for (int i=0;i<27;++i) {
            occs[i]=occs_save[i]; // On restaure occs à son état initial, qui est potentiellement modifié à chaque itération
        }
        if (words_c[18*i]==req_size) {
            bool isValid=true;
            for (int j=0;j<req_size && isValid;++j) {
                if (occs[words_c[18*i+1+j]-'a']>0) { // On regarde si la lettre est dans la requête..
                    occs[words_c[18*i+1+j]-'a']--;
                } else if (occs[26]>0) { // Sinon on vérifie s'il reste des jokers..
                    occs[26]--;
                } else { // ..sans quoi c'est un mot invalide.
                    isValid=false;
                }
            }
            if (isValid) {
                printf("%s",&words_c[18*i+1]);
                //Les deux lignes de code mis en commentaire ci-dessous étaient un test pour voir si on gagne en efficacité en logant tout d'un coup. La différence n'est pas flagrante (parfois même défavorable par rapport à l'affichage "à la volée").
                //strcat(resBuffer,&words_c[18*i+1]);
            }
        }
    }
    //printf("%s",resBuffer); // (cf. commentaire ci dessus)
}

void setup_array(void) {
    int n=words.n;
    words_c=malloc(n*18*sizeof(char));
    resBuffer=malloc(words.n*sizeof(char)*15+1);
    for (int i=0;i<n;++i) {
        words_c[i]=0;
    }
    for (int i=0;i<n;++i) {
        words_c[i*18]=strlen(words.s[i]); // On stocke la taille du mot (inférieure à 128, tout va bien !)
        strcpy(&words_c[i*18+1],words.s[i]); // On stocke le mot ensuite
        strcat(&words_c[i*18+1],"\n");
    }
}

int main(void) {
    read_data();
    setup_array();
    /*for (int i=0;i<words.n;++i) {
        printf("%d, %s\n",words_c[18*i],&words_c[18*i+1]);
    }*/
    /* Cette partie de code permet de tester le temps d'éxécution de la fonction pour une requête entrée au préalable*/
    clock_t t;
    t = clock();
    find("x?y?z?");
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
 
    printf("find took %f seconds to execute \n", time_taken);
    while (true) {
        char req[20];
        printf("Lettres : ");
        scanf("%19s", req);
        if (strcmp(req,"!")==0) { return 0; }
        find(req);
    }
    free(words_c);
    free(resBuffer);
}
