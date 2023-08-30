#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


// Fonctions auxilliaires fournies

void print_arr(int arr[], int size) {
    // Affiche l'état du jeu décrit par le tableau tab de taille n
    for (int i=0; i<size; ++i) {
        printf("|%2d", arr[i]);
    }
    printf("|");
}

bool ident(int arr1[], int arr2[], int size) {
    // Retourne un booléen indiquant si le contenu des deux tableaux est identique
    for (int i=0; i<size; ++i) {
        if (arr1[i] != arr2[i]) { return false; }
    }
    return true;
}


// Fonctions auxiliaires ajoutées

bool is_finished(int* arr, int size) { // on suppose que les deux pointeurs renvoient vers des segments de mémoire de longueur supérieure ou égale à size
     for (int i=0;i<size;i++) {
	if (arr[i]!=0)
	   return false;
     }
     return true;
}

void arrCpy(int src[], int dst[], int size) {
   for (int i=0;i<size;i++) {
	dst[i]=src[i];
   }
}

// Fonctions à compléter

int count(int arr[], int size) {
    int sum=0; // Définition de l'accumulateur somme (contiendra à terme le nombre total de billes)
    for (int i=0;i<size;i++) {
	sum+=arr[i]; // Invariant de Boucle (IB) : Le nombre de billes dans le sous-tableau arr[0..i] est de sum
    }
    return sum; // On retourne le résultat obtenu
}

bool is_valid(int arr[], int k) {
    return arr[k]==k+1; // Le coup est bon ssi la case k contient antant de billes que de cases à sa gauche plus 1 bille, soit k+1
}

int* next(int arr[], int size, int k) {
    if (!is_valid(arr,k)) { // on vérifie que le coup demandé est possible (sinon on retourne NULL)
	    return NULL;
    }
    int *nvJeu=malloc(sizeof(int)*size);
    if (nvJeu==NULL) { // Si malloc échoue
        return NULL;  // On retourne NULL
    }
    for (int i=0;i<size;i++) { // copie du jeu original dans nvJeu
	nvJeu[i]=arr[i];
	if (i==k) { // On en profite pour mettre à jour le contenu de la case k, qui vaut 0 au coup suivant
	   nvJeu[i]=0;
	}
    }
    if (nvJeu==NULL) { // Création d'un nouveau tableau accessible via le pointeur nvJeu et vérification du succès de l'opération d'allocation de mémoire pour le tableau
	    return NULL;
    }
    for (int i=0;i<k;i++) {
	nvJeu[i]+=1;
    }
    return nvJeu; // On retourne le pointeur du nouveau tableau
}

bool possible(int arr[], int size) {
    for (int i=0;i<size;i++) { // On vérifie si le jeu n'est pas déjà fini en itérant toutes les cases pour voir si elles sont vides
        if (arr[i]!=0) { // une bille dans une case --> le jeu n'est pas terminé (simulation des coups suivants nécessaire)
            break;
        } if (i==size-1) { // S'il n'y a pas de billes dans les cases 0 à size-1, c'est qu'il n'y en a plus du tout. Le jeu est terminable
            return true;
        }
    }
    bool som_comp=false; // On crée une sorte d'accumulateur de comparaisons, qui va permettre de savoir si au moins l'un des appels récursif de possible retournera vrai
    for (int i=0;i<size;i++) { // On reparcoure les cases du tableau pour voir s'il reste des coups possibles
        if (is_valid(arr,i)) {
            int *coupTest=next(arr,size,i); // On crée un pointeur contenant le jeu valide
            som_comp |= possible(coupTest,size); // On retient vrai si la fonction ou som_comp est vrai et faux sinon (synctatic sugar pour som_comp=som_comp || possible(coupTest,size);)
            free(coupTest); // On libère la mémoire utilisée par le tableau contenant le jeu testé
        }
    }
    return som_comp; // On retourne le résultat de la série de comparaisons de résultats (OR comparison)
}

int* solution(int arr[], int size) { // Structure de la donnée renvoyée par la fonction : pointeur qui pointe vers un segment de mémoire contenant à la première case le nombre de coups de la solution puis les coups successifs. Ex : |2|1|0| correspond à une solution de 2 coups et dont les coups successifs sont la case 1 puis la case 0 (on compte les cases de 0 à n-1)
    int *jeu=malloc(sizeof(int)*size); // On crée une copie du jeu accessible via un pointeur
    if (jeu==NULL) {
        exit(1); // Si l'allocation n'a pas fonctionné, on stoppe le processus
    }
    arrCpy(arr,jeu,size);
    int nbCoups=count(jeu,size);
    int *coupsSolution=malloc(sizeof(int)*nbCoups); // On crée un pointeur dont le segment de mémoire associé renvoie : 1. En position 0 le nombre de coups
    if (coupsSolution==NULL) {
        exit(1); // Si l'allocation n'a pas fonctionné, on stoppe le processus
    }
    if (!possible(arr,size)) { free(jeu); free(coupsSolution); return NULL; } // On quitte la fonction si le jeu n'a pas de solution
    if (is_finished(jeu,size)) { // Si le jeu est déjà fini, on renvoie solution -> | 0 | (signifie qu'on a une solution constituée de 0 coups)
        free(jeu); // On libère la mémoire..
        return coupsSolution; // ..et on retourne le pointeur pointant vers la solution
    }
    int compteurCoups=0; // Ce compteur de coups nous permettra de savoir dans quelles cases placer les de la solution
    while (!is_finished(jeu,size)) { // On itère la boucle suivante tant que le jeu n'est pas terminé (ie. ne contient pas des cases toutes nulles)
        for (int i=0;i<size;i++) { // On itère les cases du jeu
            if (!is_valid(jeu,i)) { // On regarde si la case itérée est un coup valide..
                continue; // ..sinon on ignore la case
            }
            int *nvJeu=next(jeu,size,i);  // si c'est un coup valide, on détermine le nouveau jeu associé
            if (possible(nvJeu,size)) {   // pour ensuite voir si le jeu est toujours solvable une fois ce coup joué
                free(jeu); // Si c'est bon, on remplace l'état du jeu par le nouveau..
                jeu=nvJeu;
                coupsSolution[compteurCoups]=i; // enfin, on ajoute le coup valide et faisant partie de la solution à déterminer
                compteurCoups++;
                break;
            } else { // si la case ne vérifiait pas la condition précédente mais que c'était un coup valide, alors on pense à libérer la mémoire par nvJeu
                free(nvJeu);
            }
        }
    } // Une fois cette boucle finie, on peut :
    free(jeu);  // Se débarasser du modèle du jeu (il le faut même !)
    return coupsSolution; // retourner la solution
}

// Lorsqu'on ne lit pas bien l'énoncé comme moi, on se prend la tête pour rien../////**/
/**/    int* solution_bonus(int arr[], int size) { //Structure de la donnée         /**/
/**/    int *jeu=malloc(sizeof(int)*size);//pointeur->|taille_solution|coup1|coup2../**/
/**/    if (jeu==NULL) { // EX : |2|1|0| est une solution de 2 coups dont les coups /**/
/**/        exit(1);     // sont 1 puis 0                                           /**/
/**/    }                                                                           /**/
/**/    arrCpy(arr,jeu,size); //pointe vers un segment de mémoire contenant à la    /**/
/**/    int *solution=malloc(sizeof(int)*1); // première case le nombre de coups de /**/
/**/    if (solution==NULL) {                                                       /**/
/**/        exit(1); // Si l'allocation n'a pas fonctionné, on stoppe le processus  /**/
/**/    }                                                                           /**/
/**/    solution[0]=0;                                                              /**/
/**/    if (!possible(arr,size)) { exit(1); }                                       /**/
/**/    if (is_finished(jeu,size)) {                                                /**/
/**/        free(jeu);                                                              /**/
/**/        return solution;                                                        /**/
/**/    }                                                                           /**/
/**/    while (!is_finished(jeu,size)) {                                            /**/
/**/        for (int i=0;i<size;i++) {                                              /**/
/**/            if (!is_valid(jeu,i)) {                                             /**/
/**/                continue;                                                       /**/
/**/            }                                                                   /**/
/**/            int *nvJeu=next(jeu,size,i);                                        /**/
/**/            if (possible(nvJeu,size)) {                                         /**/
/**/                free(jeu);                                                      /**/
/**/                jeu=nvJeu;                                                      /**/
/**/                solution[0]++;                                                  /**/
/**/                solution=realloc(solution,sizeof(int)*(solution[0]+1));         /**/
/**/                if (solution==NULL) {                                           /**/
/**/                    return NULL;                                                /**/
/**/                }                                                               /**/
/**/                solution[solution[0]]=i;                                        /**/
/**/                break;                                                          /**/
/**/            } else {                                                            /**/
/**/                free(nvJeu);                                                    /**/
/**/            }                                                                   /**/
/**/        }                                                                       /**/
/**/    } // Une fois cette boucle finie, on peut :                                 /**/
/**/    free(jeu);  // Se débarasser du modèle du jeu (il le faut même !)           /**/
/**/    return solution; // retourner la solution                                   /**/
/**/}                                                                               /**/
////////////////////////////////////////////////////////////////////////////////////////

int* problem(int n) { // Notons que la fonction est d'une efficacité très limitée
    int* jeu=malloc(n*sizeof(int)); // On alloue un segment de mémoire contenant autant cases mémoire que de billes
    if (jeu==NULL) { // Si l'allocation ne fonctionne pas, on retourne NULL
        return jeu;
    }
    int indexMax=0; // On initialise l'index maximal à 0 (permet de déterminer la plus grande case contenant au moins une bille)
    for (int i=1;i<n+1;i++) { // On itère la boucle tant que le nombr de billes incluses dans le jeu est inférieur à n
        if (i%2==0) { // si i (le nombre de boules à entrer) est pair
            indexMax++; // On incrémente l'index de la case maximale contenant au moins une bille
            for (int j=0;j<indexMax;j++) {
                jeu[j]-=1; // On diminue de 1 le nombre de billes stockées dans chaque case
                if (jeu[j]<0) { // Si une case n'avait déjà plus de billes :
                    jeu[j]=j+1; // On lui met alors j+1 billes (ce qui nous donne un total de i billes dans le jeu)
                    indexMax--; // On décrémente indexMax (car cela signifie qu'on n'a pas besoin de nouvelle case)
                    break; // On quitte la boucle. On a un jeu soluble constitué de i billes.
                }
            }
            jeu[indexMax]=indexMax+1; // une bille enlevée dans chaque case d'index 0 à indexMax-1 -> indexMax+1 billes à ajouter dans la case indexMax (ce qui par l'occasion nous assure que ce coup est valide)
        } else { // si i impair, alors la case 0 est incrémentée de 1 (donne nécessairement 1, car elle ne peut que valoir 0 ou 1 et elle a été décrémentée lorsque i était pair)
            jeu[0]=1;
        }
    }
    return jeu; // à modifier
}


// Quelques exemples de tests

void tests(void) {
    int arr1[8] = {2, 0, 1, 4, 2, 1, 0, 0};
    int arr2[8] = {3, 1, 2, 0, 2, 1, 0, 0};
    char* str_false = "Faux";
    char* str_true = "Vrai";

    // Test de la fonction count

    printf("Test de la fonction count\n\n");

    print_arr(arr1, sizeof(arr1)/sizeof(int));
    printf(" : attendu %d, obtenu %d\n", 10, count(arr1, sizeof(arr1)/sizeof(int)));
    print_arr(arr2, sizeof(arr2)/sizeof(int));
    printf(" : attendu %d, obtenu %d\n", 9, count(arr2, sizeof(arr2)/sizeof(int)));
    printf("\n");

    // Test de la fonction is_valid

    printf("Test de la fonction is_valid\n\n");

    print_arr(arr1, sizeof(arr1)/sizeof(int));
    printf(" k=%d : attendu %s, obtenu %s\n", 3, str_true, is_valid(arr1, 3)?str_true:str_false);
    print_arr(arr1, sizeof(arr1)/sizeof(int));
    printf(" k=%d : attendu %s, obtenu %s\n", 5, str_false, is_valid(arr1, 5)?str_true:str_false);
    printf("\n");

    // Test de la fonction next

    printf("Test de la fonction next\n\n");

    print_arr(arr1, sizeof(arr1)/sizeof(int));
    printf(" k=%d :\n", 3);
    int* arr_next = next(arr1, sizeof(arr1)/sizeof(int), 3);
    printf("attendu ");
    print_arr(arr2, sizeof(arr2)/sizeof(int));
    if (arr_next != NULL) {
        printf("  obtenu ");
        print_arr(arr_next, sizeof(arr1)/sizeof(int));
        if (ident(arr_next, arr2, sizeof(arr2)/sizeof(int))) {
            printf(" (identiques)\n");
        } else {
            printf(" (différents)\n");
        }
        free(arr_next);
    } else {
        printf("pas de résultat (NULL)\n");
    }
    printf("\n");

    // Test de la fonction possible

    printf("Test de la fonction possible\n\n");

    print_arr(arr1, sizeof(arr1)/sizeof(int));
    printf(" k=%d : attendu %s, obtenu %s\n", 3, str_true, is_valid(arr1, 3)?str_true:str_false);
    print_arr(arr1, sizeof(arr1)/sizeof(int));
    printf(" k=%d : attendu %s, obtenu %s\n", 5, str_false, is_valid(arr1, 5)?str_true:str_false);
    printf("\n");

    // test de la fonction solution
    printf("fonction solution\n");
    int arr3[] = {0,2,2,4,0,0,0,0};
    print_arr(arr3,5);
    printf("\n");
    int *solution_pb=solution(arr3,5);
    if (solution_pb==NULL) {
        printf("Pas de solution\n");
    } else {
        printf("Solution :\n");
        print_arr(solution_pb,count(arr3,5));
        printf("\n");
    }
    printf("fonction problem\n");
    int i=15;
        int *a=problem(i);
        int *sol_a=solution(a,i);
        printf("%d : ",i);
        print_arr(a,i);
        printf(" <|> Possible : %s",possible(a,i)?str_true:str_false);
        printf("\n Solution : ");
        print_arr(sol_a,i);
        printf("\n");
        free(a);
        free(sol_a);
}


// Fonction principale

int main(void) {
    // On exécute la série de tests
    tests();

    return 0;
}
