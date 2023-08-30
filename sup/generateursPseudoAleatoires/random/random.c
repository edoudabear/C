#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "basic_ppm.h"

typedef void (*funcgen)(uint32_t, uint32_t*, int);

//*****************************************************************************
// Random generators

// gen_knuth : KNUTH, a=1664525, c=1013904223, m=2^32

void gen_randu(uint32_t seed, uint32_t tab[], int n) {
    uint32_t acc=seed;
    for (int i=0; i<n; ++i) {
        acc= (((acc << 16)+3*acc)*2)/2;
        tab[i] = acc ;
    }
}

// gen_minstd : STANDARD MINIMAL, a = 16807, c=0, m=2^31-1

void gen_minstd(uint32_t seed, uint32_t tab[], int n) {
    uint64_t acc=seed;
    for (int i=0; i<n; ++i) {
        acc=(acc*16807)%((1<<31) -1);
        tab[i] = acc;
    }
}

// gen_randu : RANDU, a = 65539, c=0, m=2^31

void gen_knuth(uint32_t seed, uint32_t tab[], int n) {
    int acc=seed;
    for (int i=0; i<n; ++i) {
        acc=(acc*1664525+1013904223);
        tab[i] = acc;
    }
}

// gen_tmt : TinyMT32

void gen_tmt(uint32_t seed, uint32_t tab[], int n) {
    uint32_t a = 0xDEADBEEF, b = 0xDEADFACE, c = 0xABADCAFE, d = seed;
    for (int i=0; i<n; ++i) {
        uint32_t x = (a & 0x7FFFFFFF) ^ b ^ c;
        x ^= (x << 1);
        a = b;
        b = c;
        d ^= x ^ (d >> 1);
        c = x ^ (d << 10);
        if (d & 0x1) { b = b ^ 0x8F7011EE; c = c ^ 0xFC78FF1F; }
        uint32_t t1 = a + (c >> 8);
        uint32_t t0 = d ^ t1;
        if (t1 & 0x1) { t0 ^= 0x3793FDFF; }
        tab[i] = t0;
    }
}

//*****************************************************************************
// Auxilliary random functions

void gen_float(funcgen gen, uint32_t seed, double tab[], int n) {
    uint32_t* aux=malloc(sizeof(uint32_t)*n);
    gen(seed,aux,n);
    for (int i=0; i<n; ++i) {
        double a=aux[i];
        tab[i] = a/(1u<<31);
    }
    free(aux);
}

uint32_t extrait(uint32_t n, int i, int p) {
    uint32_t res=((n>>i)<<(32-p))>>(32-p);
    return res; // A modifier
}

//*****************************************************************************
// Test functions

// Display first N values of a given generator

void disp(funcgen gen, uint32_t seed, int n) {
    uint32_t* tabi = (uint32_t*)malloc(n * sizeof(uint32_t));
    gen(seed, tabi, n);
    printf("%d premiers entier pour la graine %u :\n", n, seed);
    for (int i=0; i<n; ++i) {
        printf("%u\n", tabi[i]);
    }
}

// Build statistics of lowest byte
void printList(double *tab,int n) {
    for (int i=0;i<n;i++) {
    printf("%lf, ",tab[i]);
    }
}

void stats(funcgen gen, uint32_t seed, int n) {
    uint32_t vals[256]={0};
    gen(seed,vals,256);
    int count[256] = { 0 };
    for (int i=0;i<256;++i) {
        count[extrait(vals[i],23,8)]++;
    }
    // Tracé de l'histogramme
    int max = 0;
    for (int i=0; i<256; ++i) {
        if (count[i]>max) { max = count[i]; }
    }
    init(120, 256, black);
    for (int i=0; i<256; ++i) {
        int h = count[i]*100/max;
        for (int j=0; j<h; ++j) {
            set_pixel_color(119-j, i, blue);
        }
    }
    write("stats.ppm");
}

// Test consecutive (float) values

void draw2D(funcgen gen, uint32_t seed, int n) {
    double *tab=malloc(sizeof(double)*2*n);
    gen_float(gen,seed,tab,2*n);
    init(500, 500, black);
    for (int i=0; i<n; ++i) {
        int col = tab[2*i]*500; // A modifier
        int lgn = tab[2*i+1]*500; // A modifier
        set_pixel_color(lgn, col, yellow);
    }
    write("plot2D.ppm");
}

// Test consecutive (float) values, 3 by 3

void draw3D(funcgen gen, uint32_t seed, int n) {
    double *tab=malloc(sizeof(double)*3*n);
    gen_float(gen,seed,tab,3*n);
    init(500, 500, black);
    init(500, 500, black);
    for (int i=0; i<n; ++i) {
        int lgn = (tab[3*i]*0.6+(1-tab[3*i+1])*0.4)*500; // A modifier
        int col = tab[3*i+2]*500; // A modifier
        set_pixel_color(lgn, col, yellow);
    }
    write("plot3D.ppm");
}

//*****************************************************************************
// DieHard tests

// OPSO

void test_OPSO(funcgen gen, uint32_t seed) {
    int nb = 0x200001;;
    uint32_t* tab = (uint32_t*)malloc(nb * sizeof(uint32_t));
    gen(seed, tab, nb);

    // A completer
}

// Parking

void test_parking(funcgen gen, uint32_t seed) {
    int nb = 528000;;
    double* tab = (double*)malloc(nb * sizeof(double));
    gen_float(gen, seed, tab, nb);

    // A completer
}

// Rangs

int rang(uint32_t matr[]) {
    return 0; // A modifier
}

void test_rang(funcgen gen, uint32_t seed) {
    int nb = 1240000;;
    uint32_t* tab = (uint32_t*)malloc(nb * sizeof(uint32_t));
    gen(seed, tab, nb);

    // A completer
}

//*****************************************************************************
// Main program

int main(int argc, char* argv[]) {
    disp(gen_randu, 42, 10);
    disp(gen_minstd, 42, 10);
    disp(gen_knuth, 42, 10);
    stats(gen_knuth,17,1000);
    //double tab[542]={0};
    //gen_float(gen_randu,42,tab,542);
    //printList(tab,542);
    draw2D(gen_minstd,42,20000);
    draw3D(gen_randu,42,20000);
    return 0;
}

