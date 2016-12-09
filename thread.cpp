#include <string.h>
#include <stdio.h>
#include <iostream>
#include "ThreadClass.h"
#include "thread.h"

#define ROW 0
#define COL 1

extern SynOneToOneChannel *rightChannels[100][100];
extern SynOneToOneChannel *downChannels[100][100];
extern int matrixA[100][100];
extern int matrixB[100][100];
extern int matrixC[100][100];
extern int Arows;
extern int Acols;
extern int Brows;
extern int Bcols;
extern int Crows;
extern int Ccols;

OuterProcessor::OuterProcessor(int id, int t)
{
    numID = id; // holds the row or column it is responsible for
    type = t;

    ThreadName.seekp(0, ios::beg);
    ThreadName << "Outer" << numID << ends;

    UserDefinedThreadID = numID;

}

OuterProcessor::~OuterProcessor()
{
}

void OuterProcessor::ThreadFunc()
{
    Thread::ThreadFunc();
    Thread_t self = GetID();

    if (type == ROW)
    {
        // cycle through all the cols in this matrixA ROW to pass data on
        for (k = 1; k <= Acols; k++)
        {
            number = matrixA[numID - 1][k - 1];
            rightChannels[numID][1]->Send(&number, sizeof(int));
        }
        printf("Outer Row %d done\n", numID);
    }
    else
    {
        // cycle through all the rows in this matrixB COL to pass data on
        for (k = 1; k <= Brows; k++)
        {
            number = matrixB[k - 1][numID - 1];
            printf("Outer Col sending to 1,%d\n", numID);
            downChannels[1][numID]->Send(&number, sizeof(int));
        }

        // send the end of data bit
        number = END_OF_DATA;
        downChannels[1][numID]->Send(&number, sizeof(int));

        printf("Outer Col %d done\n", numID);
    }

    Exit();
}

InnerProcessor::InnerProcessor(int rID, int cID)
{
    rowID = rID;
    colID = cID;
    myNum = 0;

    ThreadName.seekp(0, ios::beg);
    ThreadName << "Inner" << rID << cID << ends;

    UserDefinedThreadID = rID + cID;

}

InnerProcessor::~InnerProcessor()
{
}

void InnerProcessor::ThreadFunc()
{
    Thread::ThreadFunc();
    Thread_t self = GetID();

    while (true)
    {
        // Receive from above
        printf("Inner %d%d looking above, (%d,%d)\n", rowID, colID, rowID - 1, colID);
        downChannels[rowID][colID]->Receive(&above, sizeof(int));

        printf("Inner %d%d got %d from above, (%d,%d)\n", rowID, colID, above, rowID - 1, colID);

        // if it is end of data, return results and exit
        if (above == END_OF_DATA)
        {
            // send END down
            // send the number down if possible
            if (rowID < Crows)
            {
                printf("Inner %d%d sending %d down to (%d, %d)\n", rowID, colID, above, rowID + 1, colID);
                downChannels[rowID + 1][colID]->Send(&above, sizeof(int));
                printf("Inner %d%d sent %d down to (%d, %d)\n", rowID, colID, above, rowID + 1, colID);
            }
            printf("Inner %d%d got here\n", rowID, colID);
            matrixC[rowID][colID] = myNum;
            Exit();
            printf("NEVER GET HERE\n");
        }

        // Receive from left
        printf("Inner %d%d looking left, (%d,%d)\n", rowID, colID, rowID, colID - 1);
        rightChannels[rowID][colID]->Receive(&left, sizeof(int));

        printf("Inner %d%d got %d from left, (%d,%d)\n", rowID, colID, left, rowID, colID - 1);

        // send the number down if possible
        if (rowID < Crows)
        {
            printf("Inner %d%d sending %d down to (%d, %d)\n", rowID, colID, above, rowID + 1, colID);
            downChannels[rowID + 1][colID]->Send(&above, sizeof(int));
            printf("Inner %d%d sent %d down to (%d, %d)\n", rowID, colID, above, rowID + 1, colID);
        }

        // send the number to the right if possible
        if (colID < Ccols)
        {
            printf("Inner %d%d sending %d right to (%d, %d)\n", rowID, colID, left, rowID, colID + 1);
            rightChannels[rowID][colID + 1]->Send(&left, sizeof(int));
            printf("Inner %d%d sent %d right to (%d, %d)\n", rowID, colID, left, rowID, colID + 1);
        }


        // compute the results and add them to your number
        myNum = myNum + (above * left);

    }

}
