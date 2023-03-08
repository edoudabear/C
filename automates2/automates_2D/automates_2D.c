#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "loop.h"
#include "disp.h"

void avance_Langton(int tab[], int nbl, int nbc, int* i, int* j, int* d) {
    if (tab[nbc * (*i) +( *j)]==0) {
	*d=(*d+1)%4;
	tab[nbc * (*i) + (*j)]=1;
    } else {
	*d=(*d-1+4)%4;
	tab[nbc * (*i) + (*j)]=0;
    }
    if (*d==0) {
	*j=(*j+1)%nbc;
    } else if (*d==1) {
	*i=(*i-1+nbl)%nbl;
    } else if (*d==2) {
	*j=(*j-1+nbc)%nbc;
    } else if (*d==3) {
	*i=(*i+1)%nbl;
    }
}

void avance_Langton_2(int tab[], int nbl, int nbc, int* i1, int* j1, int* d1, int* i2, int* j2,  int* d2,  int* i3, int* j3, int* d3) {
    if (tab[nbc * (*i1) +( *j1)]==0) {
        *d1=(*d1+1)%4;
        tab[nbc * (*i1) + (*j1)]=1;
    } else {
        *d1=(*d1-1+4)%4;
        tab[nbc * (*i1) + (*j1)]=0;
    }
    if (*d1==0) {
        *j1=(*j1+1)%nbc;
    } else if (*d1==1) {
        *i1=(*i1-1+nbl)%nbl;
    } else if (*d1==2) {
        *j1=(*j1-1+nbc)%nbc;
    } else if (*d1==3) {
        *i1=(*i1+1)%nbl;
    }
    if (tab[nbc * (*i2) +( *j2)]==0) {
        *d2=(*d2+1)%4;
        tab[nbc * (*i2) + (*j2)]=2;
    } else {
        *d2=(*d2-1+4)%4;
        tab[nbc * (*i2) + (*j2)]=0;
    }
    if (*d2==0) {
        *j2=(*j2+1)%nbc;
    } else if (*d2==1) {
        *i2=(*i2-1+nbl)%nbl;
    } else if (*d2==2) {
        *j2=(*j2-1+nbc)%nbc;
    } else if (*d2==3) {
        *i2=(*i2+1)%nbl;
    }
    if (tab[nbc * (*i3) +( *j3)]==0) {
        *d3=(*d3+1)%4;
        tab[nbc * (*i3) + (*j3)]=3;
    } else {
        *d3=(*d3-1+4)%4;
        tab[nbc * (*i3) + (*j3)]=0;
    }
    if (*d3==0) {
        *j3=(*j3+1)%nbc;
    } else if (*d3==1) {
        *i3=(*i3-1+nbl)%nbl;
    } else if (*d3==2) {
        *j3=(*j3-1+nbc)%nbc;
    } else if (*d3==3) {
        *i3=(*i3+1)%nbl;
    }
}

void Langton_2(int nbl, int nbc, int wait) {
    int* tab = malloc(nbl*nbc*sizeof(int));
    for (int i=0;i<nbl*nbc;i++) {
	tab[i]=0;
    }
    int *d1=malloc(sizeof(int)),*y1=malloc(sizeof(int)),*x1=malloc(sizeof(int)),
	*d2=malloc(sizeof(int)),*y2=malloc(sizeof(int)),*x2=malloc(sizeof(int)),
	*d3=malloc(sizeof(int)),*y3=malloc(sizeof(int)),*x3=malloc(sizeof(int));
    *d1=0;
    *x1=nbc/2;
    *y1=nbl/2;
    *d2=0;
    *x2=nbc/2-1;
    *y2=nbl/2;
    *d3=0;
    *x3=nbc/2+1;
    *y3=nbl/2;
    //int step=500;
    while (1) {
	avance_Langton_2(tab,nbl,nbc,y1,x1,d1,y2,x2,d2,y3,x3,d3);
	//step--;
	//if (step<0) {
      	   disp_Langton(tab,nbl,nbc,wait);
	   //step=500;
	//}
    }
}

void Langton(int nbl, int nbc, int wait) {
    int* tab = malloc(nbl*nbc*sizeof(int));
    for (int i=0;i<nbl*nbc;i++) {
        tab[i]=0;
    }
    int *d=malloc(sizeof(int)),*y=malloc(sizeof(int)),*x=malloc(sizeof(int));
    *d=0;
    *x=nbc/2;
    *y=nbl/2;
    //int step=500;
    while (1) {
        avance_Langton(tab,nbl,nbc,y,x,d);
        //step--;
        //if (step<0) {
           disp_Langton(tab,nbl,nbc,wait);
           //step=500;
        //}
    }
}

void avance_rotorouteur(int tab[], int nbl, int nbc, int* i, int* j) {
    if (tab[(*i)*nbc+(*j)]==0) {
	tab[(*i)*nbc+(*j)]=1;
	*i=nbl/2;
	*j=nbc/2;
    } else {
	tab[(*i)*nbc+(*j)]=tab[(*i)*nbc+(*j)]+1;
        if (tab[(*i)*nbc+(*j)]==5) {
		tab[(*i)*nbc+(*j)]=1;
	}
	int dir=tab[(*i)*nbc+(*j)];
	if (dir==1) {
	   *j+=1;
	} if (dir==2) {
	   *i-=1;
	} if (dir==3) {
	   *j-=1;
	} if (dir==4) {
	  *i+=1;
	}
    }
}

void rotorouteur(int nbl, int nbc, int wait) {
    int* tab = malloc(nbl*nbc*sizeof(int));
    for (int i=0;i<nbl*nbc;i++) {
        tab[i]=0;
    }
    int *y=malloc(sizeof(int)),*x=malloc(sizeof(int));
    *x=nbc/2;
    *y=nbl/2;
    int step=5000;
    while (1) {
        avance_rotorouteur(tab,nbl,nbc,y,x);
        step--;
        if (step<0) {
           disp_Langton(tab,nbl,nbc,wait);
           step=5000;
        }
    }
}

int* init_Conway(int nbl, int nbc) {
    int* ptr=malloc(nbl*nbc);
    for (int i=0;i<nbl*nbc;++i) {
	if (rand()>(80/100)*RAND_MAX) {
		ptr[i]=1;
	} else {
		ptr[i]=0;
	}
    }
    return ptr; // à modifier
}

void passe_1_Conway(int tab[], int nbl, int nbc) {
    // à compléter
}

void passe_2_Conway(int tab[], int nbl, int nbc) {
    // à compléter
}

void Conway(int nbl, int nbc, int wait) {
    int* tab = NULL; // à modifier
    // à compléter
}

void Loop(int nbl, int nbc) {
    int* tab = NULL; // à modifier
    // à compléter
}

int main(void) {
    //Langton_2(15,15,5);
    rotorouteur(62,62,5);
    return 0;
}
