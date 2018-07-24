/*  demo.c*/
#include <graphics.h>
#include <stdio.h>
#include <string.h>

void main()
{
    FILE *file;
    char *buffer;
    unsigned long fileLen;

    file = fopen("top.dat", "rb");

    if (!file)
    {
        fprintf(stderr, "Não foi possível encontrar o arquivo %s\n", "top.dat");
        exit(1);
    }

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

    int gd = DETECT,gm,left=100,top=100,right=200,bottom=200,x=600,y=600,radius=50;

    initgraph(&gd,&gm,NULL);

    int coluna = 0, linha = 0, contaLinha = 0, tam = strlen(buffer);

    for(int i = 0; i < tam; i++)
    {

        if(buffer[i] == ' ')
        {
            putpixel(coluna,linha,BLACK);
            coluna++;
        }
        else if(buffer[i]=='\n')
        {
            linha+=2;
            coluna=0;
        }
        else if (buffer[i]==',')
        {
            putpixel(coluna,linha,WHITE);
            coluna++;
        }
        else if (buffer[i]=='/')
        {
            putpixel(coluna,linha,DARKGRAY);
            coluna++;
        }
        else
        {
            putpixel(coluna,linha,LIGHTGRAY);
            coluna++;
        }



    }

    //outtextxy(25,240,"Texto");

    getchar();
    delay(500);

    closegraph();

}
