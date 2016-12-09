#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include "thread.h"
#include "ThreadClass.h"

#define ROW 0
#define COL 1

SynOneToOneChannel* rightChannels[100][100];
SynOneToOneChannel* downChannels[100][100];
int matrixA[100][100];
int matrixB[100][100];
int matrixC[100][100];
int Arows;
int Acols;
int Brows;
int Bcols;
int Crows;
int Ccols;

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

void getData(int rows, int cols, int mat)
{
    int status = 0;
    int m = 0;
    int n = 0;

    for (m = 0; m < rows; m++)
    {
        for (n = 0; n < cols; n++)
        {
            if (mat == 1)
            {
                status = scanf("%d", &matrixA[m][n]);
            }
            else
            {
                status = scanf("%d", &matrixB[m][n]);
            }
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

    int i;
    int j;

    // get the dimensions for A
    getDimensions(&Arows, &Acols);

    // get the data for A
    getData(Arows, Acols, 1);

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
    getData(Brows, Bcols, 0);

    // set size for C matrix
    Crows = Arows;
    Ccols = Bcols;

    printf("Crows: %d Ccols: %d\n", Crows, Ccols);

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

    // create all the channels
    // the 2D array will have size Arows by Bcols based on matrix mult
    // add one to size for outer processors
    for(i = 0; i < Crows + 1; i++)
    {
        for (j = 0; j < Ccols + 1; j++)
        {
            rightChannels[i][j] = new SynOneToOneChannel("Channel",0,0);
        }
    }
    for(i = 0; i < Crows + 1; i++)
    {
        for (j = 0; j < Ccols + 1; j++)
        {
            downChannels[i][j] = new SynOneToOneChannel("Channel",0,0);
        }
    }

    // create all row processors
    OuterProcessor* outerRows[Crows + 1];
    for (i = 1; i < Crows + 1; i++)
    {
        outerRows[i] = new OuterProcessor(i, ROW);
        outerRows[i]->Begin();
    }

    // create all col processors
    OuterProcessor* outerCols[Ccols + 1];
    for (i = 1; i < Ccols + 1; i++)
    {
        outerCols[i] = new OuterProcessor(i, COL);
        outerCols[i]->Begin();
    }

    // create all inner processors
    InnerProcessor* inners[Crows + 1][Ccols + 1];
    for (i = 1; i < Crows + 1; i++)
    {
        for (j = 1; j < Ccols + 1; j++)
        {
            inners[i][j] = new InnerProcessor(i, j);
            inners[i][j]->Begin();
        }
    }

    // join all threads
    for (i = 1; i < Arows + 1; i++)
    {
        outerRows[i]->Join();
    }
    for (i = 1; i < Bcols + 1; i++)
    {
        outerCols[i]->Join();
    }
    for (i = 1; i < Crows + 1; i++)
    {
        for (j = 1; j < Ccols + 1; j++)
        {
            inners[i][j]->Join();
        }
    }

    // print C
    printf("printing C...\n");
    for (i = 1; i < Crows + 1; i++)
    {
        for (j = 1; j < Ccols + 1; j++)
        {
            printf("%d ", matrixC[i][j]);
        }
        printf("\n");
    }

}
