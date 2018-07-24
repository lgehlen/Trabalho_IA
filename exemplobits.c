#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblga.h"  //Usando direto no diretorio
//Par�metros:
/*extern struct gaopt {int maxpop;
                     int nger;
                     float pmut;
                     int nbits;
                     unsigned char *best;
                     int nbytes;
                     float *fit;
                     unsigned int *ind;
                     unsigned char *pop;
                     unsigned nshow;     };
*/
struct gaopt g = {5, //Tamanho da popoulacao
                  2, //Numero de geracoes
                  20,  //Percentual de mutacao
                  32,  //Numero de bits a otimizar
                  0,   //Ponteiro para a melhor solucao
                  0,   //Numero debyte (calcula automaticamente)
                  0,   //Nao usar
                  0,   //Nao usar
                  0,   //Ponteiro para a populacao
                  1    //Numero de solucoes mostradas a cada geracao (acumula na tela)
                   };
float func1 (unsigned char *p, int n); //Fun��o de fitness para o exemplo
char isbit(char xbyte, char nbit);
void wait_for_char()
{
    int in = 0;

    while (in == 0) {
        in = getchar();
    }
}

int main(void) {
    int i, j;
    g = ga(g, func1);

    printf("Usando %d bytes.\n\n",g.nbytes);
    for (i=0; i<g.nbytes; i++)
        printf("%d ",g.best[i]);
    printf("\n\n");
    printf("\n Bitarada \n\n");
    for (i=0; i<g.nbytes; i++) {
        for (j=0; j<8; j++)
            printf("%d ",(g.best[i]&(1<<j))>0);
        printf("\n");
    }

    getchar();
    return 0;
}

//Escrever qualquer fun��o de fitness *p s�o os bytes a serem otimizados e n (autom�tico)
//o numero de bytes usados
float func1(unsigned char *p, int n) {
int j;
char x1, x2, x3, x4;
float vcont[8] = {0.,0.,0.,0.,0.,0.,0.,0.};
float verr[8] = {1.,2.,3.,1.,1.,1.,2.,3.};
float serr = 0;
int i;
unsigned int ix;
x1 = *(p+0);
x2 = *(p+1);
x3 = *(p+2);
x4 = *(p+3);
for(i=0; i<8; i++) {
    vcont[i] += ((x1&(1<<i))>0);
    vcont[i] += ((x2&(1<<i))>0);
    vcont[i] += ((x3&(1<<i))>0);
    vcont[i] += ((x4&(1<<i))>0);
}
for(i=0; i<8; i++) {
    serr += abs(vcont[i]-verr[i]);
}
serr += (x1==0) + (x2==0) + (x3==0) + (x4==0); // nenhuma linha zerada
printf("\n ERRO: %f - %f",serr,1./(1+serr));

//wait_for_char();
return 1./(1+serr);
}

char isbit(char xbyte, char nbit) {
char i;
return((xbyte & 1<<i) > 0) ;
}
