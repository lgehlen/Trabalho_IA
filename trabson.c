/*  demo.c*/
#include <graphics.h>
#include <stdio.h>
#include <string.h>

void main()
{
    FILE *file;
    char *buffer;
    unsigned long fileLen;

    file = fopen("mapa.dat", "rb");

    if (!file)
    {
        fprintf(stderr, "Não foi possível encontrar o arquivo %s\n", "mapa.dat");
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

    int coluna = 0, linha = 0, contaLinha = 0;

    for(int i = 0; i < 100000000; i++)
    {
        if(buffer[i]=='\0')
            break;

        if((contaLinha%4) == 0)
        {
            if(buffer[i] == '\n')
            {
                contaLinha++;
                //linha++;
                //coluna=0;
            }
        }
        else if((i%4) != 1)
        {
            if(buffer[i] == '0')
            {
                putpixel(coluna,linha,BLACK);
                coluna++;
            }
            else if(buffer[i]=='1')
            {
                putpixel(coluna,linha,GREEN);
                coluna++;
            }
            else if(buffer[i] == '\n')
            {
                linha++;
                contaLinha++;
                coluna=0;
            }
        }
        else if(buffer[i]=='\n')
        {
            linha++;
            contaLinha++;
            coluna=0;
        }
    }

    //outtextxy(25,240,"Texto");

    getchar();
    delay(500);

    closegraph();

}
