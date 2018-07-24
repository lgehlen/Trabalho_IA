#ifndef GA__H_INCLUDED
#define GA__H_INCLUDED

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h> // if using C++, #include <cstdlib>; may not be necessary in C
#include <time.h>   // if using C++, #include <ctime>
/* All the headers include this file. */
//#include <_mingw.h>

extern struct gaopt {int maxpop;
                     int nger;
                     float pmut;
                     int nbits;
                     unsigned char *best;
                     int nbytes;
                     float *fit;
                     unsigned int *ind;
                     unsigned char *pop;
                     unsigned nshow;     };  // g = {100,300,30.,35,0,0,0,0,0,1};


struct bytbit {int nbyte;
               int nbit;};

int RandomIntInRange(int low, int high);
void plotabits(char num);
void showpop(struct gaopt g);
unsigned char byte_ij(struct gaopt g, int x, int y);
struct bytbit fquebra(int nbit);
struct gaopt ordena(struct gaopt g);

int fselect(struct gaopt g);
struct gaopt ga(struct gaopt g, float (*func)());
struct gaopt fcross(struct gaopt g);
struct gaopt ffit(struct gaopt g, float (*func)());
struct gaopt fmut(struct gaopt g);



unsigned char byte_ij(struct gaopt g, int x, int y){
return(*(g.pop + g.nbytes*x+y));
}

int RandomIntInRange(int low, int high) {
	return (int)(rand() / (RAND_MAX / (double)(high - low))) + low;
}

void plotabits(char num) {
int i;
for (i=0; i<8; i++)
    printf("%d ", (char)(1&&(1<<i)&num));
    printf("\n\n");
}

void showpop(struct gaopt g) {
int i, j;
for (i=0; i<g.nshow; i++) {
    printf("\n%3d %5.2f - ",*(g.ind+i), *(g.fit + *(g.ind+i)));
    for (j=0; j<g.nbytes; j++)
        printf("%d ",*(g.pop + *(g.ind+i)*g.nbytes + j));
}
}

struct bytbit fquebra(int nbit) {
struct bytbit mret;
mret.nbyte = (int) (nbit/8);
mret.nbit = nbit - 8*mret.nbyte;
return(mret);
}

struct gaopt ordena(struct gaopt g) {
int i,j,k,l;
float mor;
char *ordenado;
ordenado = malloc(g.maxpop*sizeof (int));
for (i=0; i<g.maxpop; *(ordenado+i)=0, i++);
for (i=0; i<g.maxpop; i++) {
	mor = 0.;
	for (j=0; j<g.maxpop; j++)
		if ((mor< *(g.fit+j)) && !*(ordenado+j)) {
			*(g.ind+i) = j;
			mor = *(g.fit+j);
		}
		*(ordenado + *(g.ind+i)) = 1;
}
free(ordenado);
return(g);
}

struct gaopt ga(struct gaopt g, float (*func)()) {
    int nbytes, i, j;
    long u;
    unsigned short v;
    float x;
    g.nbytes = (g.nbits/8)+ ((g.nbits%8)>0);
    g.pop = malloc(g.nbytes*g.maxpop);
    //Gera vetor para afitness
    g.fit = malloc(g.maxpop*sizeof(float));
    g.ind = malloc(g.maxpop*sizeof(int));
    //Gera popinic (1)
    srand((char) time(NULL));
    for (i=0; i<g.maxpop; i++) {
        for (j=0; j<g.nbytes; j++) {
            x = RandomIntInRange(0,256);
            *(g.pop + g.nbytes*i + j) = x;
        }
    }
    //Roda
    g = ffit(g,func);
    g = ordena(g);
    showpop(g);
    for (i=0; i<g.nger; i++) {
        g = fcross(g);
        g = ffit(g,func);
        g = ordena(g);
        showpop(g);
    }
//    getchar();
    g.best = g.pop + *g.ind*g.nbytes;
    return(g);
}

struct gaopt fcross(struct gaopt g) {
int i1, i2, i, j, indiv=0, iloop;
struct bytbit u;
unsigned char *auxpop;
unsigned char xbyte1, xbyte2;
unsigned char x1, x2, masc;
auxpop = malloc(g.nbytes*g.maxpop);
//printf("\n\n");
for (iloop=0; iloop<(g.maxpop/2); iloop++, indiv+=2) {
    //Seleção i1 e i2 devem ser diferentes
    i1 =i2 =0;
    while (i1==i2) {
        i1 = fselect(g);
        i2 = fselect(g);
        //RandomIntInRange(0,g.maxpop);  // Colocar função
    }

//    printf("\n%d %d ", i1,i2);

    u = fquebra(RandomIntInRange(0,g.nbits));
    for (i=0; i<g.nbytes; i++) {
        if (i<=u.nbyte) {
            xbyte1 = byte_ij(g,i1,i);
            xbyte2 = byte_ij(g,i2,i);
        }
        else {
            xbyte2 = byte_ij(g,i1,i);
            xbyte1 = byte_ij(g,i2,i);
        }
        if(i==u.nbyte) {
            masc = (char) (1<<u.nbit)-1; //Potencia de 2 menos 1 para fazer a mascara
            x1 = ((xbyte1&masc) | (xbyte2&(~masc)));
            x2 = ((xbyte1&(~masc)) | (xbyte2&masc));
            xbyte1 = x1;
            xbyte2 = x2;
        }
        *(auxpop + g.nbytes*(indiv)+i) = xbyte1;
        *(auxpop + g.nbytes*(indiv+1)+i) = xbyte2;
    }
}
//Mutação
g = fmut(g);
// Colocar elitismo aqui
for (j=0; j<g.maxpop; j++)
    *(auxpop + j) = *(g.pop + *(g.ind+0)*g.nbytes + j);
//Ok
free(g.pop);
g.pop = auxpop;
//getchar();
return(g);
}

int fselect(struct gaopt g) {
unsigned int mret, isort;
float xnorm;
isort = RandomIntInRange(0,1000000);
xnorm =  pow((float) isort/(1000001), 1.5);
mret = g.maxpop * xnorm;
mret = *(g.ind+mret);
return(mret);
}

struct gaopt ffit(struct gaopt g, float (*func)()) {
int i;
for (i=0; i<g.maxpop; i++) {
    *(g.fit+i) = (*func)((g.pop + g.nbytes*i), g.nbytes);
}
}

struct gaopt fmut(struct gaopt g) {
int nmut, i;
unsigned char bytemut, bitmut, xbyte;
nmut = (float) g.maxpop * g.pmut / 100.;
for (i=0; i<nmut; i++) {
    bytemut = RandomIntInRange(0,g.maxpop*g.nbytes);
    bitmut = RandomIntInRange(0,8);
    xbyte = *(g.pop+bytemut);
    *(g.pop+bytemut) = xbyte&(1<<bitmut) ? xbyte & ~(1<<bitmut) : xbyte | (1<<bitmut);
}
return(g);
}




#endif // GA__H_INCLUDED
