#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void main()
{
    getchar();
    char a[60][60];
    int radius = 30;
    int i,j;
    for (i=0; i<=2*radius; i++)
    {
        for (j=0; j<=2*radius; j++)
        {
            double distance = sqrt((double)(i-radius)*(i-radius) + (j-radius)*(j-radius));
            if (distance<radius)
            {
                a[i][j] = '*';
            }
            else
            { a[i][j] = ' ';}
        }
    }
    for (i=0; i<2*radius; i++)
    {
        for (j=0; j<2*radius; j++)
        {
            printf("%c ", a[i][j]);
        }
        printf("\n");
    }
    getchar();



}
