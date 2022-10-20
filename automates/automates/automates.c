#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "basic_ppm.h"

void display(bool arr[], int n) {
    for (int i=0;i<n;++i) {
        if (!arr[i]) {
            printf(".");
        } else {
            printf("#");
        }
    }
    printf("\n");
}

void rule_A(bool src[], bool dst[], int n) {
    for (int i=0;i<n;i++) {
        if (src[i]==true) {
            dst[i]=false;
        } else if ((i<n-1 && src[i+1]==true) || (i>0 && src[i-1]==true)) {
            dst[i]=true;
        } else {
            dst[i]=false;
        }
    }
}

void rule_W(bool src[], bool dst[], int n, bool rules[]) {
    for (int i=0;i<n;++i) {
        bool left=src[n-1],right=src[0],center=src[i];
        if (i>0) {
            left=src[i-1];
        } if (i<n-1) {
            right=src[i+1];
        }
        // Disjonction de cas :
        if (left && center && right) {
            dst[i]=rules[0];
        }
        if (left && center && !right) {
            dst[i]=rules[1];
        }
        if (left && !center && right) {
            dst[i]=rules[2];
        }
        if (left && !center && !right) {
            dst[i]=rules[3];
        }
        if (!left && center && right) {
            dst[i]=rules[4];
        }
        if (!left && center && !right) {
            dst[i]=rules[5];
        }
        if (!left && !center && right) {
            dst[i]=rules[6];
        }
        if (!left && !center && !right) {
            dst[i]=rules[7];
        }
    }
}

void drawList(bool arr[], int n, int l) {
    for (int i=0;i<n;++i) {
        if (arr[i]) {
            set_pixel_color(i,l,white);
        }
    }
}

void exec(bool arr_init[], int n, int nb_steps) {
    bool* arr1 = malloc(n*sizeof(bool)); // à modifier
    bool* arr2 = malloc(n*sizeof(bool)); // à modifier
    //bool auto18[8] = {0,0,0,1,0,0,1,0};
    //bool auto54[8] = {0,0,1,1,0,1,1,0};
    bool autotest[8] = {0,0,0,1,1,1,1,0}; 

    for (int i=0;i<n;++i) {
        arr1[i]=arr_init[i];
    }

    for (int i=0;i<nb_steps;++i) { // à modifier
        display(arr1,n);
        drawList(arr1,n,i);
        rule_W(arr1,arr2,n,autotest);
        bool* cpy=arr1;
        arr1=arr2;
        arr2=cpy;
    }
    free(arr1);
}

int main(int argc, char* argv[]) {
    bool arr_test[901] = { false };
    arr_test[450]=true;

    init_image(901,450,black);
    exec(arr_test, 901, 450);
    write_image("output.ppm");
    return 0;
}
