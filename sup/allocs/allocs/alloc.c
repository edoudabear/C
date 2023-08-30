#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>


//----------------------------------------------------------------
// Structures de données manipulées

// Structure pour les blocs d'allocation mémoire

struct block {
    int* start;           // L'adresse du premier de [size] entiers consécutifs
    int size;             //
    struct block* next;   // L'adresse du prochain bloc dans la liste (ou NULL)
};

// Structure pour gérer l'allocation
// soit deux listes chaînées, une pour les blocs disponibles,
// une pour les blocs présentement utilisés

struct pool {
    struct block* avail;  // Une liste chaînée de blocs disponibles
    struct block* used;   // Une liste chaînée de blocs présentement alloués
};


//----------------------------------------------------------------
// Création d'une structure pour gerer une zone de mémoire

struct pool new_pool(int size) {
    // Création d'une structure avec deux listes vides
    struct pool pool = { .avail = NULL, .used = NULL };

    // Allocation d'une zone mémoire
    int* memory = (int*)malloc(size*sizeof(int));

    // Si l'allocation s'est bien passée
    if (memory != NULL) {
        // On crée un seul bloc de mémoire disponible
        pool.avail = (struct block*)malloc(sizeof(struct block));

        // Son adresse de début est celle obtenue avec malloc
        (*pool.avail).start = memory;

        // Sa taille est la taille totale allouée
        (*pool.avail).size = size;

        // Pas d'autre bloc dans la liste
        (*pool.avail).next = NULL;
    }

    // On retourne la structure créée
    // (elle peut ne contenir aucune mémoire si l'appel à malloc a échoué)
    return pool;
}


//----------------------------------------------------------------
// Recherche d'un bloc adéquat dans une liste
//
// Les fonctions suivantes prennent une liste de blocs
// (= l'adresse du premier bloc de la liste chaînée)
// et une taille souhaitée
//
// Elles retournent l'adresse d'un bloc de taille
// supérieure ou égal à la taille requise s'il en existe
// et NULL sinon
//
// AUCUNE MODIFICATION N'EST FAITE SUR LA LISTE


// Stratégie (1) : premier bloc de taille suffisante

struct block* get_first(struct block* lst, int size) {
    struct block* res=lst;
    for (;res!=NULL && res->size<size;res=res->next) {}
    return res;
}

// Stratégie (2) : plus grand bloc de taille suffisante

struct block* get_largest(struct block* lst, int size) {
    struct block* res=lst, *max=NULL;
    for (;res!=NULL;res=res->next) {
        if (res->size>=size) {
            max=res;
            size=res->size;
        }
    }
    return max;
}

// Stratégie (3) : plus petit bloc de taille suffisante

struct block* get_smallest(struct block* lst, int size) {
    struct block* res=lst, *bst=NULL;
    int min=-1;
    for (;res!=NULL;res=res->next) {
        if (res->size>=size && (res->size<=min || min<0)) {
            bst=res;
            min=res->size;
        }
    }
    return bst;
}


//----------------------------------------------------------------
// Recherche d'un bloc dans une liste correspondant à une adresse
//
// La fonction get_from_ptr prend en argument une liste chaînée
// de blocs (alloués) et un pointeur (int*) retournée lors
// d'un appel à ialloc, et retourne un pointeur vers le bloc
// dont l'adresse correspond au pointeur en question
//
// Cette recherche ne devrait jamais échouer !

struct block* get_from_ptr(struct block* lst, int* ptr) {
    assert(lst!=NULL);
    if (ptr==lst->start) {
        return lst;
    }
    return get_from_ptr(lst->next,ptr);
}


//----------------------------------------------------------------
// La fonction remove_block_from_list prend en argument
// un pointeur vers une variable contenant une liste de blocs
// (donc un pointeur vers un pointeur vers un bloc)
// et retire ce bloc de la liste en préservant la liste chaînée

void remove_block_from_list(struct block** pp_lst, struct block* block) {
    assert(*pp_lst != NULL);
    struct block* to_remove=*pp_lst;
    if (to_remove==block) { // si le premier est le bloc à supprimer
        *pp_lst=(*pp_lst)->next;
        return;
    }
    // Sinon, on cherche le suivant égal 
    remove_block_from_list(&((*pp_lst)->next),block);
}


//----------------------------------------------------------------
// La fonction add_block_to_list prend en argument
// un pointeur vers une variable contenant une liste de blocs
// (donc un pointeur vers un pointeur vers un bloc)
// et ajoute ce bloc en tête de la liste en préservant la
// structure de la liste chaînée

void add_block_to_list(struct block** pp_lst, struct block* p_block) {
    assert(pp_lst != NULL);
    p_block->next=(*pp_lst);
    *pp_lst=p_block;
}


//----------------------------------------------------------------
// La fonction insert_block_into_list prend en argument
// un pointeur vers une variable contenant une liste de blocs
// (donc un pointeur vers un pointeur vers un bloc)
// dont les adresses (champs start) sont rangées par ordre
// croissant et ajoute ce bloc en tête de la liste en préservant
// la structure de la liste chaînée et la condition sur l'ordre

void insert_block_into_list(struct block** pp_lst, struct block* p_block) {
    // !! Peut être faut il plutôt ajouter p_block à pp_lst quand pp_lst = NULL..
    assert(pp_lst != NULL);
    if ((*pp_lst)->start>p_block->start) { // Cas tout tout début
        p_block->next=*pp_lst;
        *pp_lst=p_block;  
        return;   
    }
    insert_block_into_list(&((*pp_lst)->next),p_block);
}


//----------------------------------------------------------------
// La fonction insert_block_into_list prend en argument
// deux pointeurs vers deux blocs et retourne un booléen
// indiquant si la fin du premier bloc coïncide avec le
// début du second dans la mémoire

bool consecutive(struct block* p1, struct block* p2) {
    return p2->start-p1->size==p1->start;
}


//----------------------------------------------------------------
// La fonction clean_list prend en argument une liste
// chaînée de blocs dont les adresses (champ start)
// sont rangées par ordre croissant, et fusionne tant
// que c'est possible les blocs consécutifs en mémoire

void clean_list(struct block* p_lst) {
    if ((p_lst == NULL) || ((*p_lst).next == NULL)) return;

    struct block* p_next = (*p_lst).next;

    if (consecutive(p_lst, p_next)) {
        struct block *cpy=p_next;
        p_lst->size=p_lst->size+p_next->size;
        p_lst->next=p_lst->next->next;
        free(cpy);
        clean_list(p_lst);
    } else {
        clean_list(p_lst->next);
    }
}


//----------------------------------------------------------------
// La fonction ialloc prend en argument un pointeur vers une
// structure de gestion de l'allocation mémoire et une taille
// requise
//
// Elle retourne un pointeur int* en cas de succès
// et NULL si l'allocation est impossible

int* ialloc(struct pool* p_pool, int size) {
    assert(size>0);

    // Choix de la stratégie
    typedef enum { ST_FIRST, ST_LARGEST, ST_SMALLEST } strategy_type;
    strategy_type strategy = ST_SMALLEST;

    // On cherche un bloc suffisamment grand disponible
    struct block* p_block = NULL;
    if (strategy == ST_FIRST) { p_block = get_first((*p_pool).avail, size); }
    else if (strategy == ST_LARGEST) { p_block = get_largest((*p_pool).avail, size); }
    else if (strategy == ST_SMALLEST) { p_block = get_smallest((*p_pool).avail, size); }

    // S'il n'y en a pas, l'allocation ne peut aboutir
    if (p_block == NULL) {
        return NULL;
    }
    if (p_block->size>size) {
        struct block* reliquat=malloc(sizeof(struct block));
        reliquat->size=(p_block->size)-size;
        reliquat->start=p_block->start+size;
        reliquat->next=p_block->next;
        p_block->next=reliquat;
        p_block->size=size;
    }
    remove_block_from_list(&(p_pool->avail),p_block);
    add_block_to_list(&(p_pool->used),p_block);
    return p_block->start;
}


//----------------------------------------------------------------
// La fonction ifree prend en argument un pointeur vers une
// structure de gestion de l'allocation mémoire et une
// adresse (int*) précédemment retournée par ialloc
//
// Elle rend disponible le bloc en question
//
// Pour des raisons de simplicité, si l'adresse fournie est NULL,
// la fonction ne fait rien (on peut ainsi appeler la fonction
// sur l'adresse retournée par ialloc même si elle a échoué)

void ifree(struct pool* p_pool, int* ptr) {
    if (ptr==NULL) return; // On ne fait rien si le pointeur est NULL
    struct block *to_free=get_from_ptr((p_pool->used),ptr);
    remove_block_from_list(&(p_pool->used),to_free);
    insert_block_into_list(&(p_pool->avail),to_free);
    clean_list(p_pool->avail);
}


//----------------------------------------------------------------
// La fonction print_bloc_list prend en argument une liste
// de blocs et affiche dans la sortie standard les blocs
// contenus dans la liste

void print_block_list(struct block* lst) {
    if (lst==NULL) {
        printf("<end of list>\n");
        fflush(stdout);
        return;
    }

    printf("At %p, size %5d      [Struct block at %p, next at %p]\n",
           (void*)(*lst).start, (*lst).size, (void*)lst, (void*)(*lst).next);
    fflush(stdout);
    print_block_list((*lst).next);
}

void print_pool(struct pool* p_pool) {
    printf("\nMemory state:\n\n");
    printf("List of available blocks:       [First struct block at %p]\n", (void*)(*p_pool).avail);
    fflush(stdout);
    print_block_list((*p_pool).avail);
    printf("\n");
    fflush(stdout);
    printf("List of allocated blocks:       [First struct block at %p]\n", (void*)(*p_pool).used);
    fflush(stdout);
    print_block_list((*p_pool).used);
    printf("\n");
    fflush(stdout);
}

//----------------------------------------------------------------
// La fonction test prend en argument un pointeur vers
// une structure de gestion de la mémoire, une séquence
// d'opérations et sa taille, et effectue une série de tests
// en affichant les résultats

void test(struct pool* p_pool, int seq[], int n, int debug) {
    // On réserve un tableau de pointeurs pour mémoriser les allocations
    // qui ont été faites, afin de pouvoir les libérer plus tard
    int* ptr[100] = { NULL };

    char* sres[] = { "FAILURE", "SUCCESS" };

    // On parcourt la séquence d'allocations / libérations
    for (int i=0; i<n; i+=2) {
        int var = seq[i];       // La "variable" qui recevra l'allocation
        int sze = seq[i+1];     // La taille demandée (0 = libération)

        assert((var>=0)&&(var<100)&&(sze>=0));

        if (debug) {
            print_pool(p_pool);
            printf("\033[30;101m");
        }

        if (sze>0) {
            // Allocation de mémoire via ialloc
            assert(ptr[var] == NULL);
            printf("Allocating (%02d)  size = %4d", var, sze);
            if (debug) { printf("\033[37;40m"); }
            fflush(stdout);
            ptr[var] = ialloc(p_pool, sze);
            printf("  <%s, addr = %p>", sres[ptr[var]!=NULL], (void*)ptr[var]);
        } else {
            // Libération de mémoire via ifree
            printf("Deallocating (%02d, addr = %p)", var, (void*)ptr[var]);
            if (debug) { printf("\033[37;40m"); }
            fflush(stdout);
            if (ptr[var] != NULL) {
                ifree(p_pool, ptr[var]);
                ptr[var] = NULL;
                printf("  <done>");
            } else {
                printf("  <skipped, not allocated>");
            }
        }
        printf("\n");
        fflush(stdout);
    }
}

//----------------------------------------------------------------
// Quelques séquences de tests (vous pouvez créer les vôtres)

// Première séquence pour vérifier le bon fonctionnement

int seq1[] = { 0, 1536,  // Doit échouer si la ressource totale est 1024
               1, 256,
               2, 512,
               3, 128,
               4, 512,   // Doit échouer si la ressource totale est 1024
               5, 64,
               6, 64,
               7, 64 };  // Doit échouer  si la ressource totale est 1024

// Seconde séquence pour tester les stratégies d'allocation

int seq2[] = { 0, 256,   // On alloue 256, 512, 128 puis 64
               1, 512,
               2, 128,
               3, 64,
               0, 0,     // On libère tout
               1, 0,
               2, 0,
               3, 0,
               0, 64,    // On alloue 64, 128, 256 puis 512
               1, 128,
               2, 256,
               3, 512,   // Peut échouer selon la stratégie
               0, 0,     // On libère tout
               1, 0,
               2, 0,
               3, 0,
               0, 512,
               1, 394,
                }; // Va échouer s'il n'y a pas de consolidation


int seq3[] = {
    0,64,
    1,64,
    2,512,
    3,64,
    4,64,
    5,128,
    3,0,
    4,0,
    0,0,
    1,0,
    0,256
};

//----------------------------------------------------------------
// Programme principal

int main(int argc, char* argv[]) {
    // On réserve une zone de mémoire pour les allocation
    struct pool pool = new_pool(1024);

    // Activation du mode debug avec l'option en ligne de commande --debug
    bool debug = false;
    if (argc>1) {
        if ((argc>2) || (strcmp(argv[1], "--debug"))) {
            fprintf(stderr, "Usage : %s [--debug]\n", argv[0]);
        } else {
            debug=true;
        }
    }

    // Choix du protocole de test (décommenter une seule ligne)
    // test(&pool, seq1, sizeof(seq1)/sizeof(int), debug);      // Séquence 1
    //test(&pool, seq2, sizeof(seq2)/sizeof(int), debug);       // Séquence 2
    test(&pool, seq3, sizeof(seq3)/sizeof(int), debug);        // Séquence 3

    // Affichage de l'état de la mémoire à l'issue du programme
    print_pool(&pool);
    return 0;
}
