#include <stdio.h>
#include <stdlib.h>
#include "biblga.h"  //Usando direto no diretorio
#include <math.h>
//Par�metros:
/*extern struct gaopt {int maxpop;
                     int nger;
                     float pmut;
                     int nbits;
                     unsigned char *best;
                     int nbytes;file:///home/sa/Downloads/ia/exemplobits.c
                     float *fit;
                     unsigned int *ind;
                     unsigned char *pop;
                     unsigned nshow;     };
*/
struct gaopt g = {100, //Tamanho da popoulacao
                  100, //Numero de geracoes //300
                  10,  //Percentual de mutacao
                  32,  //Numero de bits a otimizar
                  0,   //Ponteiro para a melhor solucao
                  0,   //Numero debyte (calcula automaticamente)
                  0,   //Nao usar
                  0,   //Nao usar
                  0,   //Ponteiro para a populacao
                  1    //Numero de solucoes mostradas a cada geracao (acumula na tela)
                   };
float func1 (unsigned char *p, int n); //Fun��o de fitness para o exemplo

int main(void) {
    int i, j;
    g = ga(g, func1);

    printf("Usando %d bytes.\n\n",g.nbytes);

    printf("\n\n");

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
int i;
char x1, x2, x3, x4;
float err=0;
float vcont[8] = {0.,0.,0.,0.,0.,0.,0.,0.};
float verr[8] = {1.,1.,1.,1.,1.,1.,1.,1.};
unsigned int ix;
x1 = *(p+0);
x2 = *(p+1);
x3 = *(p+2);
x4 = *(p+3);

for (i=0; i<8; i++) {
    vcont[i]+=((x1&(1<<i))>0);
    vcont[i]+=((x2&(1<<i))>0);
    vcont[i]+=((x3&(1<<i))>0);
    vcont[i]+=((x4&(1<<i))>0);
}
for(i=0; i<8; i++) {
    err += abs(vcont[i]-verr[i]);
}
//return(1.0/(1+abs(abs(x2-x1)-11.)+ abs(abs(x3-x2)-13.)+abs(abs(x4-x3)-17. )));
return (1./(1+err));
//return 1./(1+abs(x1+x2 - abs(x3-x4)));   //  Maximiza a soma dos bytes (o resultado deve ser 255 255 255 255
}
