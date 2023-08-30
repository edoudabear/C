#include <stdlib.h>
#include <stdio.h>

typedef struct liste { int valeur_i ; struct liste *suivant; } liste ;

liste* cons(int a, liste* list) {
    liste* res=malloc(sizeof(liste));
    res->valeur_i=a;
    res->suivant=list;
    return res;
}

liste* supprime(int occ,liste* list) {
    liste* ori=list;
    while (ori!=NULL && ori->valeur_i==occ) {
        liste* cell=ori;
        ori=ori->suivant;
        free(cell);
    }
    if (ori==NULL) {
        return NULL;
    }
    liste* ptr=ori;
    while (ptr->suivant!=NULL) {
        if (ptr->suivant->valeur_i==occ) {
            liste* cell=ptr->suivant;
            ptr->suivant=ptr->suivant->suivant;
            free(cell);
        } else {
            ptr=ptr->suivant;
        }
    }
    return ori;
}

liste* lrange(int a, int b, int step) {
    if (a>=b) {
        return NULL;
    }
    liste *res=malloc(sizeof(liste));
    res->valeur_i=a;
    res->suivant=NULL;
    liste *ptr=res;
    a+=step;
    while (a<b) {
        liste *cellule=malloc(sizeof(liste));
        cellule->valeur_i=a;
        cellule->suivant=NULL;
        ptr->suivant=cellule;
        ptr=cellule;
        a+=step;
    }
    return res;
}

void print_list(liste *list) {
    printf("[");
    liste* ptr=list;
    while (ptr!=NULL) {
        printf("%d",ptr->valeur_i);
        if (ptr->suivant!=NULL) {
            printf(",");
        }
        ptr=ptr->suivant;
    }
    printf("]\n");
}

int main (int argc,char const *argv[]) {
    printf("Hello\n");
    liste *ma_liste= malloc(sizeof(liste));
    ma_liste->suivant=NULL;
    ma_liste->valeur_i=42;
    ma_liste=cons(5,ma_liste);
    ma_liste=cons(1,ma_liste);
    ma_liste=cons(3,ma_liste);
    ma_liste=cons(2,ma_liste);
    ma_liste=cons(1,ma_liste);
    ma_liste=supprime(1,ma_liste);
    print_list(ma_liste);
    free(ma_liste);
    liste *ma_liste2=lrange(10,24,2);
    print_list(ma_liste2);
    free(ma_liste2);
    return EXIT_SUCCESS;
}