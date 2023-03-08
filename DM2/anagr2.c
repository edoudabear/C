#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h> // Pour effectuer des tests de rapidité

struct { int n; char** s; } words;
void find_aux(int nOccs,int k,int index,char* acc, int *occs);

// Pour afficher un booléen dans la console
void printBool(bool val) {
    switch (val) {
        case true : 
            printf("true\n");
            break;
        case false : 
            printf("false\n");
            break;
    }
}

void printList(int *lst,int n) {
    for (int i=0;i<n;++i) {
        printf("|%d|",lst[i]);
    }
    printf("\n");
}

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

char *nodes=NULL; // Ne sert que si on veut ne pas imprimer à la volée (dans ce cas, décommenter deux lignes dans la fonction find et penser à décommenter l'affichage "à la volée")
int *adj=NULL;
int *links=NULL;

/* Simple fonction ayant pour but de tester si la structure est valide (au moins en testant sur certains mots)*/
bool exists(char* str) {
    int index=str[0]-'a',l=strlen(str);
    for (int i=1;i<l;++i) {
        int iter=adj[2*index],max=adj[2*index+1];
        for (;iter<max && nodes[links[iter]]!=str[i];iter++) {
           // printf("%c,%c\n",nodes[links[iter]],str[i]);
        }
        if (nodes[links[iter]]!=str[i]) {
            return false;
        }
        index=links[iter];
    }
    return links[adj[2*index]]==-1;
}

void find(char* req) {
    // Affiche les mots que l'on peut construire à partir des lettres de req
    int occs[27]={0}, req_size=strlen(req);
    for (int i=0;i<req_size;++i) {
        if (req[i]=='?') {
            occs[26]++;
        } else if (req[i]>='a' && req[i]<='z') {
            occs[req[i]-'a']++;
        } else {
            printf("Invalid query : check your input.\n");
            return;
        }
    }
    if (req_size>15) {
        return;
    }
    char *acc=malloc(16*sizeof(char));
    acc[0]=0;
    find_aux(req_size,0,-1,acc,occs);
}

void find_aux(int nOccs,int k,int index,char* acc, int *occs) {
    if (nOccs==0) { // Suppose que strlen(req)>=i
        if (links[adj[2*index]]==-1) {
            for (int i=0;i<k;++i) {
                printf("%c",acc[i]);
            }
            printf("\n");
        } else {
            return;
        }
    }
    int occs_save[27];
    for (int i=0;i<27;++i) {
        occs_save[i]=occs[i]; // On restaure occs à son état initial, qui est potentiellement modifié à chaque itération
    }
    int iter,max;
    if(index==-1) {
        iter=0;
        for (;iter<26;iter++) {
            if (occs[iter]>0) {
                occs[iter]--;
                acc[k]='a'+iter;
                find_aux(nOccs-1,k+1,iter,acc,occs);
                for (int i=0;i<27;++i) {
                    occs[i]=occs_save[i]; // On restaure occs à son état initial, qui est potentiellement modifié à chaque itération
                }
            } else if (occs[26]>0) {
                occs[26]--;
                acc[k]='a'+iter;
                find_aux(nOccs-1,k+1,iter,acc,occs);
                for (int i=0;i<27;++i) {
                    occs[i]=occs_save[i]; // On restaure occs à son état initial, qui est potentiellement modifié à chaque itération
                }
            }
        }
    } else {
        iter=adj[2*index];
        max=adj[2*index+1]; // iter=min au départ
        for (;iter<max;iter++) {
            /*for (int i=0;i<k;++i) {
                printf("%c",acc[i]);
            }
            printf(" %c,%d\n",acc[k-1],iter);*/
            if (links[iter]!=-1) {
                if (occs[nodes[links[iter]]-'a']>0) {
                    occs[nodes[links[iter]]-'a']--;
                    acc[k]=nodes[links[iter]];
                    find_aux(nOccs-1,k+1,links[iter],acc,occs);
                    for (int i=0;i<27;++i) {
                        occs[i]=occs_save[i]; // On restaure occs à son état initial, qui est potentiellement modifié à chaque itération
                    }
                } else if (occs[26]>0) { // Il nous reste des jokers ?
                    occs[26]--;
                    acc[k]=nodes[links[iter]];
                    find_aux(nOccs-1,k+1,links[iter],acc,occs);
                    for (int i=0;i<27;++i) {
                        occs[i]=occs_save[i]; // On restaure occs à son état initial, qui est potentiellement modifié à chaque itération
                    }
                }
            }
        }
    }
}

bool sameNode(char* str1,char* str2,int j) {
    for (int i=0;i<j;i++) {
        if (str1[i]=='\0' || str2[i]=='\0') { return false; }
        else if (str1[i]!=str2[i]) { return false; }
    }
    return true;
}

void setup_array_2(void) {
    int n=words.n;
    nodes=malloc(n*16*sizeof(char)); // Noeuds de l'arbre (plutôt un graphe d'ailleurs..)
    adj=malloc(2*n*sizeof(int)*16); // Matrice pointant vers les liens d'un élément nodes[i]. Taille multipliée par deux pour stocker source et dest
    links=malloc(n*sizeof(int)*15); // Matrice des liens
    adj[0]=0;

    int nodeCounter=0; // Compteur du nombre de noeuds
    int linksPointer=0;

    for (int j=0;j<15;++j) {
        int k=0;
        int arity=0; 
        // char currentLetter='a'; // pour du débuggage
        for (;strlen(words.s[k])<j;k++) {}
        for (int i=k;i<n;i++) {

            /* pour du débuggage
            if (i==k && j>0 && nodeCounter<350) { 
                printf("New letter : %c\n",currentLetter);
            }
            */

            // Gestion de la matrice d'adjacence et des noeuds
            if ((arity==0 && sameNode(words.s[i],words.s[i],j+1)) ||
             (i>0 && (sameNode(words.s[i-1],words.s[i],j+1) && !sameNode(words.s[i-1],words.s[i],j+2)))) {
                arity++; // Si sameNode retourne true, c'est que words.s[i] contient au moins j+1 caractères si il est mis deux fois en argument
                         // sinon, c'est que les deux mots ont le même ancêtre à la profondeur j+2
            } else if (arity>0 && !sameNode(words.s[i-1],words.s[i],j+1)) { // Si deux éléments n'ont pas le même ancêtre à la pronfondeur j
                nodes[nodeCounter]=words.s[i-1][j]; // On ajoute le caractère précédent à l'arbre-graphe
                if (nodeCounter>0) { // pour éviter de faire adj[-1]..(adj[0]=0)
                    adj[2*nodeCounter]=adj[2*nodeCounter-1];
                }
                adj[2*nodeCounter+1]=adj[2*nodeCounter]+arity;
                
                /* pour du débuggage
                if (nodeCounter<350) {
                    printf("[%d] %c | %d (%d) -> %d    |[%d] %d \n",nodeCounter,nodes[nodeCounter],adj[2*nodeCounter],adj[2*nodeCounter]+26,adj[2*nodeCounter+1],nodeCounter,links[nodeCounter]);
                    if (j>0 && currentLetter!=words.s[i][j-1]) {
                        currentLetter=words.s[i][j-1];
                        printf("New letter : %c\n",currentLetter);
                    }
                }
                */

                nodeCounter++;
                if (sameNode(words.s[i],words.s[i],j+1)) { // Si le nouveau mot est de pronfondeur supérieure à j on le compte, sinon, non.
                    arity=1;
                } else {
                    arity=0;
                }
            }
            if (i==n-1 && sameNode(words.s[i],words.s[i],j+1)) { // Cas particulier de i=n-1
                nodes[nodeCounter]=words.s[i][j];
                if (nodeCounter>0) {
                    adj[2*nodeCounter]=adj[2*nodeCounter-1];
                }
                adj[2*nodeCounter+1]=adj[2*nodeCounter]+arity;
                nodeCounter++; 
            }
            // Gestion des liens
            if (j>0) {
                int l=strlen(words.s[i]);
                if (l==j) { // Si la longueur de la chaîne est de j, on met une sentinelle pour indiquer la fin du mot (-1) dans links
                    links[linksPointer]=-1;
                    linksPointer++;
                } else if (l>j) { // Sinon, on ajoute le lien et on incrémente notre pointeur
                    if ((i>k && !sameNode(words.s[i-1],words.s[i],j+1) && (linksPointer==0 || links[linksPointer-1]!=nodeCounter))
                        || i==k) {
                            links[linksPointer]=nodeCounter;
                            linksPointer++;
            }}}
        }
    }
    /* ^pour du débuggage
    for (int i=0;i<350;++i) {
        printf("[%d] %c | %d -> %d    |[%d] %d \n",i,nodes[i],adj[2*i],adj[2*i+1],i,links[i]);
    }
    */
    return;
}

int main(void) {
    read_data();
    clock_t t_init;
    t_init = clock();
    setup_array_2();
    t_init = clock() - t_init;
    double time_taken_init = ((double)t_init)/CLOCKS_PER_SEC; // in seconds
 
    printf("find took %f seconds to initialize \n", time_taken_init);
    //printBool(exists("adjacent"));
    /*clock_t t;
    t = clock();
    find("x?y?z?");
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
 
    printf("find took %f seconds to execute \n", time_taken);
    */
    while (true) {
        char req[20];
        printf("Lettres : ");
        scanf("%19s", req);
        if (strcmp(req,"!")==0) { return 0; }
        clock_t t_query=clock();
        find(req);
        t_query = clock() - t_query;
        double query_time =  ((double)t_query)/CLOCKS_PER_SEC ; // in seconds
        printf("(Query took %f seconds to execute)\n", query_time);
    }
    free(nodes);
    free(adj);
    free(links);
}
