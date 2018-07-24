/*  demo.c*/
//#include <math.h>

#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include "biblga.h"

struct gaopt g = {70, //Tamanho da popoulacao
                  30, //Numero de geracoes //300
                  3,  //Percentual de mutacao
                  16,//19,  //Numero de bits a otimizar
                  0,   //Ponteiro para a melhor solucao
                  0,   //Numero debyte (calcula automaticamente)
                  0,   //Nao usar
                  0,   //Nao usar
                  0,   //Ponteiro para a populacao
                  0    //Numero de solucoes mostradas a cada geracao (acumula na tela)
                   };

unsigned int root(unsigned int x);

int hasGroundUnmapped();

float func1 (unsigned char *p, int n); //Fun��o de fitness para o exemplo

int tamanhoBuffer, contaParana, bola=0, linha = 0, coluna = 0;

char **mapa, **mapaAux;

float *vcont;//, *verr;

void main()
{

    int contaColuna = 0, contaLinha = 0, boolean = 0, i = 0, j = 0, cont = 0;

/// ABRE O ARQUIVO .DAT ===================================================================================================================

    FILE *file;
    char *buffer;
    unsigned long fileLen;

    file = fopen("mapa.dat", "rb");

    if (!file)
    {
        fprintf(stderr, "Não foi possível encontrar o arquivo %s\n", "mapa.dat");
        exit(1);
    }

/// PREPARA O BUFFER ===================================================================================================================

    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer=(char *)malloc(fileLen+1);

    if (!buffer)
    {
        fprintf(stderr, "Erro de memória!");
        fclose(file);
        exit(1);
    }

    fread(buffer, fileLen, 1, file);

/// PREPARA O CANVAS DE IMPRESSÃO ===================================================================================================================

    int gd = 0,gm,left=100,top=100,right=200,bottom=200,x=600,y=600,radius=50;

    initgraph(&gd,&gm,NULL);

/// PERCORRE O BUFFER RECEBENDO DADOS IMPORTANTES  ================================================================================================

    tamanhoBuffer = strlen(buffer);

    for(i = 0; i < tamanhoBuffer; i++)
    {
        if(buffer[i]=='\n')
        {
            linha++;
        }
        else if (linha == 0)
        {
            coluna++;
        }
    }

/// PREPARA A MATRIZ A PARTIR DO BUFFER =========================================================================================================

    mapa = malloc (linha * sizeof (int *));
    for (i = 0; i < linha; ++i)
        mapa[i] = malloc (coluna * sizeof (int));

    for(i = 0; i < linha; i++)
    {
        for(j = 0; j < coluna+1; j++)
        {
            if(buffer[cont]=='0'||buffer[cont]=='1')
            {
                mapa[i][j] = buffer[cont];
            }
            cont++;
        }
    }

    free(buffer);

    //FOR SEPARADO PARA LIBERAR MEMÓRIA

    vcont = malloc (tamanhoBuffer * sizeof (int *));

    //verr = malloc (tamanhoBuffer * sizeof (int *));

    for(i = 0; i < linha; i++)
    {
        for(j = 0; j < coluna+1; j++)
        {
            vcont[cont] = 1.;
            cont++;
        }
    }

    mapaAux = malloc (linha * sizeof (int *));
    for (i = 0; i < linha; ++i)
        mapaAux[i] = malloc (coluna * sizeof (int));


/// CRIA OS PONTOS DE DISTRIBUIÇÃO ===================================================================================================================

    srand((int)time(NULL));

    do
    {

        g = ga(g, func1);

        for(i = 0; i < linha; i++)
        {
            for(j = 0; j < coluna; j++)
            {
                mapa[i][j]=mapaAux[i][j];
            }
        }

    }while(hasGroundUnmapped()==1);



/// IMPRIME ===================================================================================================================

    for(i = 0; i < linha; i++)
    {
        if((i%4) != 1){
            contaLinha++;
            contaColuna = 0;
            for(j = 0; j < coluna; j++)
            {
                if(j <= coluna && (j%4) != 1)
                {
                    char key = mapa[i][j];

                    if(key == '0')
                    {
                        putpixel(contaColuna,contaLinha,BLACK);
                        contaColuna++;
                    }
                    else if(key=='1')
                    {
                        putpixel(contaColuna,contaLinha,GREEN);
                        contaColuna++;
                    }
                    else if(key=='2')
                    {
                        putpixel(contaColuna,contaLinha,BLUE);
                        contaColuna++;
                    }
                    else if(key=='3')
                    {
                        putpixel(contaColuna,contaLinha,RED);
                        contaColuna++;
                    }
                    else if(key=='4')
                    {
                        putpixel(contaColuna,contaLinha,YELLOW);
                        contaColuna++;
                    }
                    else if(key=='5')
                    {
                        putpixel(contaColuna,contaLinha,WHITE);
                        contaColuna++;
                    }
                    else if(key=='6')
                    {
                        putpixel(contaColuna,contaLinha,MAGENTA);
                        contaColuna++;
                    }
                    else if(key=='7')
                    {
                        putpixel(contaColuna,contaLinha,CYAN);
                        contaColuna++;
                    }
                }
            }
        }
    }

    printf("\n\n");

    int calc[2];
    //outtextxy(calc[0],calc[1],"esse");
    getchar();
    delay(500);

    closegraph();

    for (int i = 0; i < linha; i++)
    {
        free(mapa[i]);
        free(mapaAux[i]);
    }
    //free(buffer);
    free(vcont);
    //free(verr);
}

float func1(unsigned char *p, int n)
{
    int i, j, xRand=1, yRand=1, xRandRaio, yRandRaio, raio = 30, aux = 0;
    float err=0;
    char x1, x2;

    float contX[9] = {0.,0.,0.,0.,0.,0.,0.,0.,0.};
    float contY[10] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};

    unsigned int ix;

    x1 = *(p+0);
    x2 = *(p+1);

    for(i = 0; i < linha; i++)
    {
        for(j = 0; j < coluna; j++)
        {
            mapaAux[i][j]=mapa[i][j];
        }
    }

    for(i=0;i<9; i++)
    {
        contX[i]+=((x1&(1<<i))>0);

        if(contX[i] == 1.)
        {
            if(i == 0)
            {
                xRand = 1;
            }
            else
            {
                xRand+=xRand+(2*i);
            }
        }
    }


    for(i=0;i<10; i++)
    {
        contY[i]+=((x1&(1<<i))>0);

        if(contY[i] == 1.)
        {
            if(i == 0)
            {
                yRand = 1;
            }
            else
            {
                yRand+=yRand+(2*i);
            }
        }
    }

    //printf("%d %d", xRand,yRand);

    if( (xRand > raio) && (xRand < linha-raio) && (yRand > raio) && (yRand < coluna-raio)){

        //printf("x: %d",yRand);
        //printf("y: %d",xRand);

        //getchar();

        if(mapaAux[xRand][yRand]=='1')
        {
            xRandRaio = xRand-raio;
            yRandRaio = yRand-raio;

            for (i=0; i<=2*raio; i++)
            {
                for (j=0; j<=2*raio; j++)
                {
                    double distance = root((double)(i-raio)*(i-raio) + (j-raio)*(j-raio));

                    if (distance < raio)
                    {
                        if(mapaAux[xRandRaio+i][yRandRaio+j] == '1')
                        {
                            mapaAux[xRandRaio+i][yRandRaio+j] = '2';
                        }
                        else if(mapaAux[xRandRaio+i][yRandRaio+j] == '2')
                        {
                            mapaAux[xRandRaio+i][yRandRaio+j] = '3';
                        }
                        else if(mapaAux[xRandRaio+i][yRandRaio+j] == '3')
                        {
                            mapaAux[xRandRaio+i][yRandRaio+j] = '4';
                        }
                        else if(mapaAux[xRandRaio+i][yRandRaio+j] == '4')
                        {
                            mapaAux[xRandRaio+i][yRandRaio+j] = '5';
                        }
                        else if(mapaAux[xRandRaio+i][yRandRaio+j] == '0')
                        {
                            mapaAux[xRandRaio+i][yRandRaio+j] = '6';
                        }
                        else if(mapaAux[xRandRaio+i][yRandRaio+j] == '6')
                        {
                            mapaAux[xRandRaio+i][yRandRaio+j] = '7';
                        }

                    }
                }
            }

            for(i = 0; i < linha; i++)
            {
                for(j = 0; j < coluna; j++)
                {
                    if(mapa[i][j]=='2')
                    {
                        vcont[aux] = 0.;
                    }
                    else if(mapa[i][j]=='3')
                    {
                        vcont[aux] = 0.5;
                    }
                    else if(mapa[i][j]=='4')
                    {
                        vcont[aux] = 0.8;
                    }
                    else if(mapa[i][j]=='5')
                    {
                        vcont[aux] = 1.;
                    }
                    else if(mapa[i][j]=='6')
                    {
                        vcont[aux] = 1.;
                    }
                    else if(mapa[i][j]=='7')
                    {
                        vcont[aux] = 1.;
                    }
                    aux++;
                }
            }
            for(i=0; i<tamanhoBuffer; i++) {
                err += abs(1.-vcont[i]);
            }
            return (1./(1+err));
        }
    }

    for(i=0; i<tamanhoBuffer; i++) {
        err += abs(1.);
    }

    return (1./(1+err));

}

unsigned int root(unsigned int x){
    unsigned int a,b;
    b = x;
    a = x = 0x3f;
    x = b/x;
    a = x = (x+a)>>1;
    x = b/x;
    a = x = (x+a)>>1;
    x = b/x;
    x = (x+a)>>1;
    return(x);
}

int hasGroundUnmapped()
{
    int i, j;

    for(i = 0; i < linha; i++)
    {
        for(j = 0; j < coluna; j++)
        {
            if(mapa[i][j] == '1')
                return 1;
        }
    }
    return 0;
}
