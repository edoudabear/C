#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic_ppm.h"

// Image data
static unsigned char* buf_pixels = NULL;
static int buf_nbl, buf_nbc;

static unsigned char comp_r[] = { 0, 255, 0, 255, 0, 255, 0, 255, 128 };
static unsigned char comp_g[] = { 0, 0, 255, 255, 0, 0, 255, 255, 128 };
static unsigned char comp_b[] = { 0, 0, 0, 0, 255, 255, 255, 255, 128 };

int init_image(int nbl, int nbc, color c) {
    // Free previous buffer
    if (buf_pixels)
        free(buf_pixels);
    buf_pixels = NULL;

    // Try to allocate image
    if (nbl<=0 || nbc<=0) {
        fprintf(stderr, "Invalid dimensions in init (%d, %d)\n", nbl, nbc);
        return -1;
    } else {
        buf_pixels = malloc(nbl*nbc*3*sizeof(char));
    }

    if (!buf_pixels) {
        return -2;
    }

    // If successful, fill it with color
    if (c<black || c>gray) {
        fprintf(stderr, "Invalid color in init (%d)\n", c);
        return -3;
    }

    for (int k=0; k<3*nbl*nbc;) {
        buf_pixels[k++] = comp_r[c];
        buf_pixels[k++] = comp_g[c];
        buf_pixels[k++] = comp_b[c];
    }

    buf_nbl = nbl;
    buf_nbc = nbc;

    return 0;
}

int nbl(void) {
    if (buf_pixels == NULL) {
        fprintf(stderr, "Image buffer not initialized\n");
        return -1;
    }
    return buf_nbl;
}

int nbc(void) {
    if (buf_pixels == NULL) {
        fprintf(stderr, "Image buffer not initialized\n");
        return -1;
    }
    return buf_nbc;
}

void set_pixel_color(int i, int j, color c) {
    if (c<black || c>gray) {
        fprintf(stderr, "Invalid color (%d)\n", c);
        return;
    }
    set_pixel_rgb(i, j, comp_r[c], comp_g[c], comp_b[c]);
}

void set_pixel_rgb(int i, int j, int r, int g, int b) {
    if (buf_pixels == NULL) {
        fprintf(stderr, "Image buffer not initialized\n");
        return;
    }

    if (i<0 || i>=buf_nbl || j<0 || j>=buf_nbc) {
        fprintf(stderr, "Coordinates (i=%d, j=%d) out of image bounds\n", i, j);
        return;
    }

    if (r<0 || r>=256 || g<0 || g>=256 || b<0 || b>=256) {
        fprintf(stderr, "Invalid color (r=%d, g=%d, b=%d)\n", r, g, b);
        return;
    }

    buf_pixels[(i*buf_nbc+j)*3] = r;
    buf_pixels[(i*buf_nbc+j)*3+1] = g;
    buf_pixels[(i*buf_nbc+j)*3+2] = b;
}

int write_image(char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) {  // Failure to open file
        fprintf(stderr, "Impossible to open file for writing\n");
        return -1;
    }

    fprintf(f, "P6\n%d %d\n255\n", buf_nbc, buf_nbl);

    for (int i = 0; i < buf_nbl*buf_nbc*3; ++i)
        fprintf(f, "%c", (char)(buf_pixels[i]));

    fclose(f);
    return 0;
}
