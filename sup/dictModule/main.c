#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct listCell {
    char* key;
    char* value;
    struct listCell* next;
};

struct dict {
    int size;
    int nbKeys;
    struct listCell** arr;
    int max_size;
};

typedef struct listCell ListCell;
typedef struct dict Dict;

int max(int a,int b) {
    if (a>b) {
        return a;
    }
    return b;
}

/*char* strdup(char* str) {
    char* res=malloc(sizeof(char)*(strlen(str)+1));
    memcpy(str,res,strlen(str)+1);
    return str;
}*/

void stats(Dict* d) {
    printf("{ number of keys : %d ; max bucket size : %d ; arr size : %d }\n",d->nbKeys,d->max_size,d->size);
    return;
}

Dict* new_dict(int size) {
    ListCell** dic_arr=malloc(max(size,1)*sizeof(ListCell*));
    for (int i=0;i<max(size,1);++i) {
        dic_arr[i]=NULL;
    }
    Dict* res=malloc(sizeof(Dict));
    res->size=max(size,1);
    res->nbKeys=0;
    res->arr=dic_arr;
    res->max_size=0;
    return res;
}

int hash(char* key) {
    int res=0;
    for (int i=0;key[i]!=0;++i) {
        res=(res+(2<<5)*res)^key[i];
    }
    return res;
}

char* search(Dict* dict, char* key) { // dict is READ ONLY
    int index=abs(hash(key)%dict->size);
    ListCell* l=dict->arr[index];
    while (l!=NULL && strcmp(l->key,key)!=0) {
        l=l->next;
    }
    if (l!=NULL) {
        return strdup(l->value);
    }
    return NULL;
}

int exists(Dict* dict, char* key) { // dict is READ ONLY
    int index=abs(hash(key)%dict->size);
    ListCell* l=dict->arr[index];
    while (l!=NULL && strcmp(l->key,key)!=0) {
        l=l->next;
    }
    if (l!=NULL) {
        return 1;
    }
    return 0;
}

void resize(Dict* dict,int size) {
    ListCell** n_arr=malloc(sizeof(ListCell*)*size);
    dict->max_size=0;
    int* n_occs=malloc(sizeof(int)*size);
    for (int i=0;i<size;++i) {
        n_arr[i]=NULL;
        n_occs[i]=0;
    }
    for (int i=0;i<dict->size;++i) {
        while (dict->arr[i]!=NULL) { // On déplace les clés dans l'ordre inverse de celui selon lequel on les avait inséré du tableau initial vers le nv.
            n_occs[i]++;
            ListCell* pair=dict->arr[i];
            int h=abs(hash(pair->key)%size);
            dict->arr[i]=pair->next;
            pair->next=n_arr[i];
            n_arr[i]=pair; // Il faut traîter la question du max_size (résolu à la va vite)
        }
        dict->max_size=max(dict->max_size,n_occs[i]);
    }
    free(dict->arr);
    free(n_occs);
    dict->size=size;
    dict->arr=n_arr;
    /*for (int i=0;i<dict->size;++i) {
        if (dict->arr[i]==NULL) {
            printf("|NULL\n");
        } else
            printf("|%p\n",&dict->arr[i]);
    }*/
}

int pow_i (int v,int k) {
    if (k==0) {
        return 1;
    }
    return v*pow_i(v,k-1);
}

void add(Dict* dict,char* key,char* value) {
    int index=abs(hash(key)%dict->size), count=0;
    char hasBeenAdded=0;
    ListCell* pairs=dict->arr[index];
    ListCell* iterator=pairs;
    while (iterator!=NULL) {
        count++;

        if (strcmp(key,iterator->key)==0) {
            iterator->value=strdup(value);
            hasBeenAdded=1;
        }
        iterator=iterator->next;
    }
    if (!hasBeenAdded) {
        ListCell* new_pair=malloc(sizeof(ListCell));
        new_pair->key=strdup(key);
        new_pair->value=strdup(value);
        new_pair->next=pairs;
        dict->arr[index]=new_pair;
        count++;
        dict->nbKeys=dict->nbKeys+1;
    }
    dict->max_size=max(dict->max_size,count);
    if (dict->nbKeys*10>=7*dict->size) {
        resize(dict,dict->size*2-1);
        stats(dict);
    }
}

void delete(Dict* dict,char * key) {
    dict->nbKeys--;
    int index=abs(hash(key)%dict->size);
    ListCell* l=dict->arr[index];
    if (strcmp(l->key,key)==0) {
        dict->arr[index]=l->next;
        free(l->value);
        free(l->key);
        free(l);
        return;
    }
    while (l->next!=NULL && strcmp(key,l->key)!=0) {
        l=l->next;
    }
    if (l->next==NULL) {
        printf("Could not remove specified key : not found.\n");
        return;
    } else {
        ListCell* toRemove=l->next;
        l->next=l->next->next;
        free(toRemove->value);
        free(toRemove->key);
        free(toRemove);
    }
}

void empty(Dict* d) {
    for (int i=0;i<d->size;++i) {
        ListCell* l=d->arr[i];
        if (l!=NULL) {
            while (l->next!=NULL) {
                ListCell* toRemove=l->next;
                l->next=l->next->next;
                free(toRemove);
            }
            free(l);
        }
    }
    free(d->arr);
    free(d);
    printf("Dictionnary freed.\n");
}

int main(int* argc,char** argv) {
    Dict* d=new_dict(42);
    add(d,"Hello","World");
    add(d,"Hello","World2");
    delete(d,"Hello");
    char* v=search(d,"Hello");
    if (v!=NULL) {
        printf("%s\n",v);
    }
    stats(d);
    char* k=malloc(sizeof(char)*6);
    k[0]='e';
    k[5]=0;
    for (int i=0;i<10000000;++i) {
        for (int j=1 ;j<9;j++) {
            k[j]=(i/pow_i(26,j-1)%26==0) ? 97 : abs(i/pow_i(26,j-1)%26)+64;
        }
        add(d,k,k);
        //printf("'%s'\n",search(d,k));
    }
    stats(d);
    empty(d);
    return EXIT_SUCCESS;
}