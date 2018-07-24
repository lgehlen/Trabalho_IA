#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include "biblga.h"

struct gaopt g = {5, //Tamanho da popoulacao
                  3, //Numero de geracoes //300
                  15,  //Percentual de mutacao
                  600,//(k*9)+(k*10)+k,//(x e y)+(on/off)//Numero de bits a otimizar
                  0,   //Ponteiro para a melhor solucao
                  0,   //Numero debyte (calcula automaticamente)
                  0,   //Nao usar
                  0,   //Nao usar
                  0,   //Ponteiro para a populacao
                  0    //Numero de solucoes mostradas a cada geracao (acumula na tela)
};

unsigned int root(unsigned int x);

void wait_for_char();

float func1 (unsigned char *p, int n); //Função de fitness

int tamanhoBuffer, linha = 0, coluna = 0, bytes=75, i, j, cont, var=0; //((k*9)+(k*10)+k)/8

char **mapa, **mapaAux, *stringBits;

float *vcontador;

int k=30,raio=60;

int onoff[30], coordenadas[30][2];

void main()
{
    int contaColuna = 0, contaLinha = 0;

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

    free(file);

    free(buffer);

    //verr = malloc (tamanhoBuffer * sizeof (int *));

    //bytes=(((k*9)+(k*10)+k)/8);

/// CRIA OS PONTOS DE DISTRIBUIÇÃO ===================================================================================================================

    srand((int)time(NULL));

    g = ga(g, func1);

    for(i = 0; i < linha; i++)
    {
        for(j = 0; j < coluna; j++)
        {
            mapa[i][j]=mapaAux[i][j];
        }
    }

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
                        putpixel(contaColuna,contaLinha,LIGHTBLUE);
                        contaColuna++;
                    }
                    else if(key=='9')
                    {
                        putpixel(contaColuna,contaLinha,CYAN);
                        contaColuna++;
                    }
                }
            }
        }
    }

    printf("\n\n");

    wait_for_char();

    delay(500);

    closegraph();

    for (int i = 0; i < linha; i++)
    {
        free(mapa[i]);
        free(mapaAux[i]);
    }
    //free(buffer);
    //free(vcontador);

}

float func1(unsigned char *p, int n)
{
    int i, j, l, xRand, yRand, xRandRaio, yRandRaio, aux = 0, cont, cont2;
    int cima, esquerda, retoX, retoY, posX, posY, posX2, posY2;

    float err=0;

    char x1;

    unsigned int ix;

    cont = 0;

    vcontador = malloc (k * sizeof (float *));

    stringBits = malloc ( (bytes*8) * sizeof (char *));

    mapaAux = malloc (linha * sizeof (int *));
    for (i = 0; i < linha; ++i)
        mapaAux[i] = malloc (coluna * sizeof (int));

    var++;

/// PREENCHE O MAPA AUXILIAR =============================================

    for(i = 0; i < linha; i++)
    {
        for(j = 0; j < coluna; j++)
        {
            mapaAux[i][j]=mapa[i][j];
        }
    }

    cont = 0;

/// PREENCHE O ARRAY DE BITES COM OS BYTES =============================================

    for(i=0; i<bytes; i++)
    {
        x1 = *(p+i);
        for(j=cont2; j<(cont2+8); j++)
        {
            if(((x1&(1<<j))>0)==1.)
            {
                stringBits[j]='0';
            }
            else
            {
                stringBits[j]='1';
            }
            cont++;
        }

        cont2=cont;
    }

/// SEPARA NAS MATRIZES on/off E coordenadas (x, y) =============================================

    cont = 0;

    for(i=0;i<k;i++)
    {
        onoff[cont] = 0;

        if(stringBits[i]=='1')
        {
            onoff[cont] = 1;
        }

        cont++;
    }

    cont2 = cont; //29

    for(i=0;i<k;i++)
    {
        coordenadas[i][0] = 0;

        for(j=cont2;j<cont2+9;j++)
        {
            if(stringBits[j] == '1')
            {
                if((j-cont2) == 0)
                {
                    coordenadas[i][0] = 1;
                }
                else
                {
                    coordenadas[i][0]+=coordenadas[i][0]+(2*abs(j-cont2));
                }
            }
            cont++;
        }
        cont2 = cont;
    }

    for(i=0;i<k;i++)
    {
        coordenadas[i][1] = 0;

        for(j=cont2;j<cont2+10;j++)
        {
            if(stringBits[j] == '1')
            {
                if((j-cont2) == 0)
                {
                    coordenadas[i][1] = 1;
                }
                else
                {
                    coordenadas[i][1]+=coordenadas[i][1]+(2*abs(j-cont2));
                }
            }
            cont++;
        }
        cont2 = cont;
    }

    for(l=0; l<k; l++)
    {
        aux = 0;
        xRand = coordenadas[l][0];
        yRand = coordenadas[l][1];

        if( (xRand > raio) && (xRand < linha-raio) && (yRand > raio) && (yRand < coluna-raio))
        {
            vcontador[l] = 0.;

            if(onoff[l] == 1)
            {
                xRandRaio = xRand-raio;
                yRandRaio = yRand-raio;

                for (i=0; i<(2*raio); i++)
                {
                    for (j=0; j<(2*raio); j++)
                    {
                        double distance = root((double)(i-raio)*(i-raio) + (j-raio)*(j-raio));

                        if (distance < raio)
                        {
                            if(mapaAux[xRandRaio+i][yRandRaio+j] == '1')
                            {
                                mapaAux[xRandRaio+i][yRandRaio+j] = '2';
                                vcontador[l]+=0.0;
                                aux++;
                            }
                            else if(mapaAux[xRandRaio+i][yRandRaio+j] == '2')
                            {
                                mapaAux[xRandRaio+i][yRandRaio+j] = '3';
                                vcontador[l]+=0.5;
                                aux++;
                            }
                            else if(mapaAux[xRandRaio+i][yRandRaio+j] == '3')
                            {
                                mapaAux[xRandRaio+i][yRandRaio+j] = '4';
                                vcontador[l]+=0.8;
                                aux++;
                            }
                            else if(mapaAux[xRandRaio+i][yRandRaio+j] == '4')
                            {
                                mapaAux[xRandRaio+i][yRandRaio+j] = '5';
                                vcontador[l]+=1.0;
                                aux++;
                            }
                            else if(mapaAux[xRandRaio+i][yRandRaio+j] == '0')
                            {
                                mapaAux[xRandRaio+i][yRandRaio+j] = '6';
                                vcontador[l]+=0.7;
                                aux++;
                            }
                            else if(mapaAux[xRandRaio+i][yRandRaio+j] == '6')
                            {
                                mapaAux[xRandRaio+i][yRandRaio+j] = '7';
                                vcontador[l]+=0.9;
                                aux++;
                            }
                        }
                    }
                }
            }
            aux = 0;
        }
        else
        {
            for (i=0; i<(2*raio); i++)
            {
                for (j=0; j<(2*raio); j++)
                {
                    double distance = root((double)(i-raio)*(i-raio) + (j-raio)*(j-raio));

                    if (distance < raio)
                    {
                        vcontador[l] += 1.;
                    }
                }
            }
        }
    }


    for(l=0; l<k; l++)
    {
        if(onoff[l] == 1)
        {
            posX = coordenadas[l][0];
            posY = coordenadas[l][1];

            if(l+1 == k)
            {
                posX2 = coordenadas[0][0];
                posY2 = coordenadas[0][1];
            }
            else
            {
                posX2 = coordenadas[l+1][0];
                posY2 = coordenadas[l+1][1];
            }

            //printf("INICIAL X=%d PARA X2=%d === Y=%d PARA Y2=%d \n",posX,posX2,posY,posY2);

            while((posX != posX2) || (posY != posY2))
            {
                cima=1, esquerda=1, retoX=0, retoY=0;

                if(posX == posX2)
                {
                    retoX = 1;

                }
                else if(posX < posX2)
                {
                    cima = 0;
                }

                if(posY == posY2)
                {
                    retoY = 1;
                }
                else if(posY < posY2)
                {
                    esquerda = 0;
                }

                if(retoX == 1)
                {
                    if(esquerda==1)
                    {
                        //Reto esquerda
                        posY--;
                    }
                    else
                    {
                        //Reto direita
                        posY++;
                    }
                }

                else if(retoY == 1)
                {
                    if(cima==1)
                    {
                        //Reto cima
                        posX--;
                    }
                    else
                    {
                        //Reto baixo
                        posX++;
                    }
                }
                else if(cima == 1)
                {
                    if(esquerda==1)
                    {
                        //cima esquerda
                        posX--;
                        posY--;
                    }
                    else
                    {
                        //cima direita
                        posX--;
                        posY++;
                    }
                }
                else
                {
                    if(esquerda==1)
                    {
                        //baixo esquerda
                        posX++;
                        posY--;
                    }
                    else
                    {
                        //baixo direita
                        posX++;
                        posY++;
                    }
                }

                if(posX < linha && posX > 0 && posY < coluna && posY > 0)
                {
                    mapaAux[posX][posY] == '9';
                }
            }
        }
    }



    for(i=0; i<k; i++)
    {
        err += abs(vcontador[i]-1);
    }

    free(stringBits);

    free(vcontador);

    if(var != 8)
    {

        for (int i = 0; i < linha; i++)
        {
            free(mapaAux[i]);
        }
    }

    return (1./(err-1));

}

void wait_for_char()
{
    int in = 0;

    while (in == 0) {
        in = getchar();
    }
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
