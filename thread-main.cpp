#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void getDimensions(int* x, int* y)
{
   int status = 0;

   status = scanf("%d", x);
   if (status < 0)
   {
        perror("scanf");
   }
   status = scanf("%d", y);
   if (status < 0)
   {
        perror("scanf");
   }
   return;
}

void getData(int rows, int cols, int * matrix)
{
    int status = 0;
    int i = 0;
    int j = 0;

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            status = scanf("%d", ((matrix + (i*cols)) + j) );
            if (status < 0)
            {
                perror("scanf");
            }
        }
    }
    return;
}

int main (void)
{

    int Arows;
    int Acols;
    int Brows;
    int Bcols;
    int i;
    int j;

    // get the dimensions for A
    getDimensions(&Arows, &Acols);

    // get the data for A
    int matrixA[Arows][Acols];
    getData(Arows, Acols, (int *)matrixA);

    // get the dimensions for B
    getDimensions(&Brows, &Bcols);

    // if the cols of A don't match rows of B, can't do matrix multiplication
    // so error out
    if (Acols != Brows)
    {
        printf("ERROR: Cols of A do not match Rows of B.\nExiting...\n");
        exit(1);
    }

    // get the data for B
    int matrixB[Brows][Bcols];
    getData(Brows, Bcols, (int *)matrixB);




    // print A
    for (i = 0; i < Arows; i++)
    {
        for (j = 0; j < Acols; j++)
        {
            printf("%d ", matrixA[i][j]);
        }
        printf("\n");
    }

    // print B
    for (i = 0; i < Brows; i++)
    {
        for (j = 0; j < Bcols; j++)
        {
            printf("%d ", matrixB[i][j]);
        }
        printf("\n");
    }

}
