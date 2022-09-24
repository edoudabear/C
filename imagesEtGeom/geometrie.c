#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "basic_ppm.h"

// Fonctions de tracé à compléter

void hline(int i, int j1, int j2, color c) {
    if (j1>j2) {
	int copy=j1;
	j1=j2;
	j2=copy;
    }
    for (int index=j1;index<=j2;index++) {
	set_pixel_color(i,index,c);
    }
}

void vline(int i1, int i2, int j, color c) {
    if (i1>i2) {
        int copy=i1;
        i1=i2;
        i2=copy;
    }
    for (int index=i1;index<=i2;index++) {
        set_pixel_color(index,j,c);
    }
}

void box(int i1, int j1, int i2, int j2, color c) {
    if (i1==i2 || j1==j2) {
        return;
    }
    int copie;
    if (i1>i2) {
        copie=i1;
        i1=i2;
        i2=copie;
    } if (j1>j2) {
        copie=j1;
        j1=j2;
        j2=copie;
    }
    hline(i1,j1,j2,c);
    hline(i2,j1,j2,c);
    vline(i1,i2,j1,c);
    vline(i1,i2,j2,c);
}

void boxf(int i1, int j1, int i2, int j2, color c) {
    if (i1==i2 || j1==j2) {
	return;
    }
    int copie;
    if (i1>i2) {
        copie=i1;
        i1=i2;
        i2=copie;
    } if (j1>j2) {
        copie=j1;
        j1=j2;
        j2=copie;
    }
    for (int i=i1;i<=i2;i++) {
        for (int j=j1;j<=j2;j++) {
            set_pixel_color(i,j,c);
        }
    }
}

void line_case1_basic(int i1, int j1, int i2, int j2, color c) {
    if (i1==i2) {
        hline(i1,j1,j2,c);
        return;
    } else if (j1==j2) {
        vline(i1,i2,j1,c);
        return;
    }
    float k=(i2-i1)/(j2-j1);
    for (int j=j1; j<=j2;j++) {
        set_pixel_color(i1+k*(j-j1),j,c);
    }
}

void line_case1(int i1, int j1, int i2, int j2, color c) {
    //printf("CASE 1 : %d %d %d %d\n",i1,j1,i2,j2);
    if (i1==i2) {
        hline(i1,j1,j2,c);
        return;
    } else if (j1==j2) {
        vline(i1,i2,j1,c);
        return;
    }
    int reliquat=0.;
    for (int i=i1; i<=i2;) {
        for (int j=j1;j<=j2;) {
            set_pixel_color(i,j,c);
            i++;
            reliquat+=(j2-j1);
            if (reliquat>(i2-i1)/2) {
                j++;
                reliquat-=(i2-i1);
            }
        }
    }
}

void line_case2(int i1, int j1, int i2, int j2, color c) {
    //printf("CASE 2 : %d %d %d %d\n",i1,j1,i2,j2);
    if (i1==i2) {
        hline(i1,j1,j2,c);
        return;
    } else if (j1==j2) {
        vline(i1,i2,j1,c);
        return;
    }
    int reliquat=0.;
    for (int i=i1; i<=i2;) {
        for (int j=j1;j>=j2;) {
            set_pixel_color(i,j,c);
            i++;
            reliquat+=abs(j2-j1);
            if (reliquat>abs((i2-i1)/2)) {
                j--;
                reliquat-=abs(i2-i1);
            }
        }
    }
}

void line_case3(int i1, int j1, int i2, int j2, color c) {
    //printf("CASE 3 : %d %d %d %d\n",i1,j1,i2,j2);
    if (i1==i2) {
        hline(i1,j1,j2,c);
        return;
    } else if (j1==j2) {
        vline(i1,i2,j1,c);
        return;
    }
    int reliquat=0.;
    for (int j=j1; j<=j2;) {
        for (int i=i1;i<=i2;) {
            set_pixel_color(i,j,c);
            j++;
            reliquat+=(i2-i1);
            if (reliquat>(j2-j1)/2) {
                i++;
                reliquat-=(j2-j1);
            }
        }
    }
}

void line_case4(int i1, int j1, int i2, int j2, color c) {
    //printf("CASE 4 : %d %d %d %d\n",i1,j1,i2,j2);
    if (i1==i2) {
        hline(i1,j1,j2,c);
        return;
    } else if (j1==j2) {
        vline(i1,i2,j1,c);
        return;
    }
    int reliquat=0.;
    for (int j=j1; j>=j2;) {
        for (int i=i1;i<=i2;) {
            set_pixel_color(i,j,c);
            j--;
            reliquat+=abs(i2-i1);
            if (reliquat>abs((j2-j1)/2)) {
                i++;
                reliquat-=abs(j2-j1);
            }
        }
    }
}

void line(int i1, int j1, int i2, int j2, color c) {
    // Si i2>i1, on inverse les deux valeurs
    if (i1>i2) {
        int copy=i2;
        i2=i1;
        i1=copy;
        copy=j2;
        j2=j1;
        j1=copy;
    }
    if (i2==i1) {
	hline(i1,j1,j2,c);
        return;
    } if (j1==j2) {
        vline(i1,i2,j1,c);
	return;
    }
    // Disjonction cas traits :
    if ((j1<=j2) && (j2<=(j1+(i2-i1)))) {
        line_case1(i1,j1,i2,j2,c);
    } else if (((j1-(i2-i1))<=j2) && (j2<=j1)) {
	line_case2(i1,j1,i2,j2,c);
    } else if (j2 > (j1 + (i2-i1))) {
	line_case3(i1,j1,i2,j2,c);
    } else if (j2 <= (j1-(i2-i1))) {
	line_case4(i1,j1,i2,j2,c);
    } else {
	//printf("Invalid coordinates\n");
    }
}

void circle(int ic, int jc, int r, color c) {
    int dj=0,di=-r;
    while (dj>di) {
        dj--;
        if (abs(r*r-dj*dj-(di+1)*(di+1))<abs(r*r-dj*dj-di*di)) {
            di++;
        }
        set_pixel_color(ic+di,jc+dj,c);
    }
    while (dj>-r) {
        di++;
        if (abs(r*r-di*di-(dj-1)*(dj-1))<abs(r*r-di*di-dj*dj)) {
            dj--;
        }
        set_pixel_color(ic+di,jc+dj,c);
    }
    while (abs(dj)>(abs(di)-r)) {
        di++;
        if (abs(r*r-di*di-(dj+1)*(dj+1))<abs(r*r-di*di-dj*dj)) {
            dj++;
        }
        set_pixel_color(ic+di,jc+dj,c);
    }
    /*while (dj<0) {
        dj++;
        if (abs(r*r-dj*dj-(di+1)*(di+1))<abs(r*r-dj*dj-di*di)) {
            di++;
        }
        set_pixel_color(ic+di,jc+dj,c);
    }*/
    /*while (dj<di-r) {
        dj++;
        if (abs(r*r-dj*dj-(di-1)*(di-1))<abs(r*r-dj*dj-di*di)) {
            di--;
        }
        set_pixel_color(ic+di,jc+dj,c);
    }
    while (di>r) {
        di--;
        if (abs(r*r-(dj+1)*(dj+1)-di*di)<abs(r*r-dj*dj-di*di)) {
            dj++;
        }
        set_pixel_color(ic+di,jc+dj,c);
    }
    while (di>r) {
        di--;
        if (abs(r*r-(dj+1)*(dj+1)-di*di)<abs(r*r-dj*dj-di*di)) {
            dj++;
        }
        set_pixel_color(ic+di,jc+dj,c);
    }*/
}

void circlef(int ic, int jc, int r, color c) {
    // A compléter
}

void trianglef(int i1, int j1, int i2, int j2, int i3, int j3, color c) {
    // A compléter
}

void trianglec(int i1, int j1, int i2, int j2, int i3, int j3) {
    // A compléter
}

void Mondrian(void) {
    boxf(0, 0, nbl()-1, nbc()-1, gray);
    hline(10, 0, nbc()-1,black);
    hline(nbl()-20,0,nbc()-1,black);
    vline(10,10,nbl()-1,black);
    vline(nbc()-10,10,nbl()-1,black);
    boxf(11,11,300,300,red);
    boxf(11,313,75,100,yellow);
    boxf(nbl()-5,400,225,110,blue);
}

// Tests

void test_question(int question) {
    if (question==1) {
        hline(140, 100, 200, blue);
        hline(160, 200, 100, red);
        hline(150, 150, 150, white);
    } else if (question==2) {
        hline(140, 100, 200, blue);
        hline(160, 200, 100, red);
        vline(100, 200, 140, yellow);
        vline(200, 100, 160, green);
        vline(150, 150, 150, white);
    } else if (question==3) {
        box(95, 95, 205, 205, yellow);
        box(100, 100, 200, 200, magenta);
        box(105, 105, 195, 195, cyan);
        box(150, 150, 150, 150, white);
        box(145, 150, 110, 150, blue);
        box(150, 155, 150, 175, red);
    } else if (question==4) {
        boxf(150, 150, 150, 150, white);
        boxf(150, 152, 150, 200, red);
        boxf(150, 148, 150, 100, green);
        boxf(152, 150, 200, 150, blue);
        boxf(148, 150, 100, 150, gray);
        boxf(152, 152, 200, 200, magenta);
        boxf(152, 148, 200, 100, cyan);
        boxf(148, 152, 100, 200, yellow);
        boxf(148, 148, 100, 100, white);
        box(98, 98, 202, 202, red);
    } else if (question==5) {
        Mondrian();
    } else if (question==6) {
        for (int k=0; k<3; ++k) {
            box(162, 150+4*k, 166, 154+4*k, red);
            box(242, 150+44*k, 246, 154+44*k, red);
            line_case1_basic(164, 152+4*k, 244, 152+44*k, blue);
        }
    } else if (question==7) {
        for (int k=0; k<3; ++k) {
            box(162, 150+4*k, 166, 154+4*k, red);
            box(242, 150+44*k, 246, 154+44*k, red);
            line_case1(164, 152+4*k, 244, 152+44*k, yellow);
        }
    } else if (question==8) {
        for (int k=0; k<3; ++k) {
            box(162, 150+4*k, 166, 154+4*k, red);
            box(242, 150+44*k, 246, 154+44*k, red);
            line_case1(164, 152+4*k, 244, 152+44*k, yellow);
        }
        for (int k=0; k<3; ++k) {
            box(162, 146-4*k, 166, 150-4*k, red);
            box(242, 146-44*k, 246, 150-44*k, red);
            line_case2(164, 148-4*k, 244, 148-44*k, green);
        }
    } else if (question==9) {
        for (int k=0; k<3; ++k) {
            box(162, 150+4*k, 166, 154+4*k, red);
            box(242, 150+44*k, 246, 154+44*k, red);
            line_case1(164, 152+4*k, 244, 152+44*k, yellow);
        }
        for (int k=0; k<3; ++k) {
            box(162, 146-4*k, 166, 150-4*k, red);
            box(242, 146-44*k, 246, 150-44*k, red);
            line_case2(164, 148-4*k, 244, 148-44*k, green);
        }
        for (int k=0; k<3; ++k) {
            box(150+4*k, 162, 154+4*k, 166, red);
            box(150+44*k, 242, 154+44*k, 246, red);
            line_case3(152+4*k, 164, 152+44*k, 244, magenta);
        }
        for (int k=0; k<3; ++k) {
            box(150+4*k, 138, 154+4*k, 134, red);
            box(150+44*k, 58, 154+44*k, 54, red);
            line_case4(152+4*k, 136, 152+44*k, 56, cyan);
        }
    } else if (question==10) {
        for (int i=0; i<24; i++) {
            int i1 = round(150+20*sin(3.14159*i/12));
            int j1 = round(150+20*cos(3.14159*i/12));
            int i2 = round(150+110*sin(3.14159*i/12));
            int j2 = round(150+110*cos(3.14159*i/12));
            //printf("%d %d %d %d\n",i1,j1,i2,j2);
	    //printf("BOX TO BLAME\n");
            box(i1-2, j1-2, i1+2, j1+2, red);
            box(i2-2, j2-2, i2+2, j2+2, red);
            //printf("LINE TO BLAME\n");
            line(i1, j1, i2, j2, yellow);
        }
    } else if (question==14) {
        box(58, 78, 242, 262, red);
        box(148, 78, 152, 82, red);
        line(150, 170, 212, 108, gray);
        circle(150, 170, 90, yellow);
    } else if (question==15) {
        box(58, 78, 242, 262, red);
        box(148, 78, 152, 82, red);
        circle(150, 170, 90, yellow);
        circle(150, 170, 88, green);
        circle(150, 170, 86, cyan);
        circle(150, 170, 84, magenta);
    } else if (question==16) {
        circlef(170, 120, 90, red);
        circlef(149, 129, 80, yellow);
        circlef(140, 150, 70, green);
        circlef(149, 171, 60, blue);
        circlef(170, 180, 50, magenta);
        box(67, 28, 262, 233, white);
    } else if (question==17) {
        circlef(20, 30, 90, red);
        circlef(270, 40, 80, yellow);
        circlef(30, 260, 70, green);
        circlef(240, 250, 60, blue);
        circlef(150, 150, 50, magenta);
    } else if (question==18) {
        for (int k=0; k<12; ++k) {
            int i1 = round(150+20*sin(3.14159*k/6));
            int j1 = round(150+20*cos(3.14159*k/6));
            int i2 = round(150+110*sin(3.14159*(k/6.0-0.05)));
            int j2 = round(150+110*cos(3.14159*(k/6.0-0.05)));
            int i3 = round(150+110*sin(3.14159*(k/6.0+0.05)));
            int j3 = round(150+110*cos(3.14159*(k/6.0+0.05)));
            trianglef(i1, j1, i2, j2, i3, j3, 1+k%6);
        }
    } else if (question==19) {
        for (int k=0; k<12; ++k) {
            int i1 = round(150+20*sin(3.14159*k/6));
            int j1 = round(150+20*cos(3.14159*k/6));
            int i2 = round(150+110*sin(3.14159*(k/6.0-0.05)));
            int j2 = round(150+110*cos(3.14159*(k/6.0-0.05)));
            int i3 = round(150+110*sin(3.14159*(k/6.0+0.05)));
            int j3 = round(150+110*cos(3.14159*(k/6.0+0.05)));
            trianglec(i1, j1, i2, j2, i3, j3);
        }
    } else if (question!=0) {
        printf("Pas de test pour la question %d\n", question);
    }
}

// Programme principal

int main(void) {
    // Création d'une image uniformément noire
    //printf("TEST QUESTION 10\n");
    init(300, 300, black);
    // Indiquer ci-dessous la question pour activer les tests
    test_question(14);

    // Enregistrement de l'image
    return write("resultat.ppm");
}
