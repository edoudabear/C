#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
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
    nb_primes=0;
}

void add_prime(int p) {
    primes[nb_primes]=p;
    nb_primes++;
}

void gen_basic(void) {
    init_primes();
    for (int i=2;i<65536;i++) {
        if (test_if_prime_basic(i)) {
            add_prime(i);
        }
    }
}

bool test_if_prime_better(int k) {
    for (int i=0;(primes[i]<k && i<nb_primes);i++) {
        if (k%primes[i]==0) {
            return false;
        }
    }
    return true;
}

void gen_better(void) {
    init_primes();
    for (int i=2;i<65536;i++) {
        if (test_if_prime_better(i)) {
            add_prime(i);
            is_prime[i]=1;
        }
        else {
            is_prime[i]=0;
        }
    }
}

void print_decomp(int k) {
    int nb_termes=0,div=k;
    int termes[6542];
    termes[0]=1;
    for (int i=2;i<=div;i++) {
        if (test_if_prime_better(i) && div%i==0) {
            termes[nb_termes]=i;
            div=div/i;
            nb_termes++;
            if (1==div) {
                break;
            }
            i=1;
        }
    }
    printf("%d=",k);
    for (int i=0;(i<nb_termes || i<1);i++) {
        printf("%d",termes[i]);
        if (i<nb_termes-1) {
            printf("*");
        }
    }
    printf("\n");
}

void gen_erathostene(void) {
    is_prime[0]=is_prime[1]=false;
    for (int i=2;i<65535;i++) {
        is_prime[i]=true;
    }
    for (int i=2;i<65535;i++) {
        if (is_prime[i]) {
            for (int j=2*i;j<65535;j+=i) {
                is_prime[j]=false;
            }
        }
    }
}

void gen_erathostene_2(void) {
    is_prime[0]=is_prime[1]=false;
    for (int i=2;i<65535;i++) {
        is_prime[i]=true;
    }
    for (int i=2;i<=65535/i;i++) {
        if (is_prime[i]) {
            if (i==2) {
                for (int j=2*i;j<65535;j+=i) {
                    is_prime[j]=false;
                }
            }
            else if (i<=INT_MAX/i) {
                for (int j=i*i;j<65535;j+=2*i) {
                    is_prime[j]=false;
                }
            }
            else {
                printf("USING BADLY OPTIMIZED PART\n");
                for (int j=2*i;j<65535;j+=i) {
                    is_prime[j]=false;
                }
            }
        }
    }
}

int pow_mod(int a, int b, int k) {
    if (b==0) {
        return 1;
    } if (b%2==0) {
        return pow(pow_mod(a,b/2,k),2);
    } else {
        return a*pow(pow_mod(a,b/2,k),2);
    }
}

bool test_if_prime_fermat(int k, int a) {
    return pow_mod(a,k-1,k)==1;
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

void test_basic () {
    // test_if_prime_basic(999789210);
    test_if_prime_basic(999999863);
    // test_if_prime_basic(999999873);
    // test_if_prime_basic(999999883);
}

void test_better () {
    // test_if_prime_better(999789210);
    test_if_prime_better(999999863);
    // test_if_prime_better(999999873);
    // test_if_prime_better(999999883);
}

// Programme principal --------------------------------------------

int main(int argc, char* argv[]) {
    /*init_primes();
    check_primes();
    print_decomp(37);
    print_decomp(54);
    print_decomp(1);
    check_primes();

    printf("gen_basic = %lf\n",time_it(gen_basic));
    printf("gen_better = %lf\n",time_it(gen_better));
    printf("gen_erathostene = %lf\n",time_it(gen_erathostene));
    printf("gen_erathostene_2 = %lf\n",time_it(gen_erathostene_2));
    */

    printf("test_if_prime_basic 999999863 = %lf\n",time_it(test_basic));
    printf("test_if_prime_better 999999863 = %lf\n",time_it(test_better));
    print_decomp(999789210);
    print_decomp(999999863);
    print_decomp(999999873);
    print_decomp(999999883);
    return 0;
}

