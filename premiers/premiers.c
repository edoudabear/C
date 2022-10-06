#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "time_it.h"
#include "basic_ppm.h"

// Variables globales ---------------------------------------------

bool is_prime[65536];

int primes[10000];
int nb_primes = 0;

// Test basique de primalité --------------------------------------

bool test_if_prime_basic(int n) {
    // retourne un booléen indiquant si n est premier
    // on suppose n>0
    
    if (n<2) {
        return false;
    } else if (n%2 == 0) {
        return n == 2;
    }
    // Dorénavant, n>=3 et n est impair
    for (int div=3; INT_MAX/div>=div && div*div <= n; div+=2) {
        if (n % div == 0) {
            return false;
        }
    }
    return true;
}

// Fonctions à compléter ------------------------------------------

void init_primes(void) {
    //
}

void add_prime(int p) {
    //
}

void gen_basic(void) {
    //
}

bool test_if_prime_better(int k) {
    //
    return false;
}

void gen_better(void) {
    //
}

void print_decomp(int k) {
    //
}

void gen_erathostene(void) {
    //
}

void gen_erathostene_2(void) {
    //
}

int pow_mod(int a, int b, int k) {
    //
    return 0;
}

bool test_if_prime_fermat(int k, int a) {
    //
    return false;
}

// Fonctions de test ----------------------------------------------

void check_primes(void) {
    printf("----------------------------------------------------\nPremiers obtenus : ");
    for (int i=0; i<nb_primes-1; ++i) {
        printf("%d, ", primes[i]);
    }
    printf("%d\n", primes[nb_primes-1]);    
    printf("----------------------------------------------------\n");
    printf("Nombre de premiers obtenus : %4d (attendus : 6542)\n", nb_primes);
    
    // Vérification de la concordance du nombre de premiers
    // trouvés dans primes et is_prime
    int compte = 0;
    for (int i=0; i<65536; ++i) {
        if (is_prime[i]) {
            compte++;
        }
    }
    if (compte != nb_primes) {
        printf("Erreur: il n'y a pas le même nombre de premiers dans primes et is_prime");
    }
    
    // Vérification que les premiers de primes sont marqués
    // premiers dans is_prime
    bool ok=true;
    for (int i=0; i<nb_primes; ++i) {
        if (!is_prime[primes[i]]) {
            ok = false;
        }
    }
    if (!ok) {
        printf("Erreur: les premiers dans primes et is_prime ne sont pas les mêmes");
    }
    
    // Calcul d'un CRC sur le tableau is_prime
    int hash = 0, acc;
    for (int i=0; i<65536;) {
        acc = 0;
        for (int j=0; j<8; ++j, ++i) {
            acc += is_prime[i];
            acc *= 2;
        }
        hash = (hash*31+acc)%0xFFFFFF;
    }
    if (hash!=8087583) {
        printf("Erreur : le tableau is_prime est incorrect\n");
    }
    printf("----------------------------------------------------\n");
}

// Programme principal --------------------------------------------

int main(int argc, char* argv[]) {
    init_primes();
    
    // A compléter
    
    return 0;
}
