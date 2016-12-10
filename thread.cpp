// -----------------------------------------------------------
// NAME : Zachary Hill                         User ID: zahill
// DUE DATE : 12/09/2016
// PROGRAM ASSIGNMENT #6
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//     Holds class implementations for the OuterProcessor
//     and InnerProcessor classes
// -----------------------------------------------------------
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

// -----------------------------------------------------------
// FUNCTION OuterProcessor :
//    Constructor for the OuterProcessor class
// PARAMETER USAGE :
//    id - the id for the processor
//    t  - identifies if the processor is for rows or cols
// FUNCTION CALLED :
//    N/A
// -----------------------------------------------------------
OuterProcessor::OuterProcessor(int id, int t)
{
    numID = id; // holds the row or column it is responsible for
    type = t;

    ThreadName.seekp(0, ios::beg);
    ThreadName << "Outer" << numID << ends;

    UserDefinedThreadID = numID;

}

// -----------------------------------------------------------
// FUNCTION ~OuterProcessor :
//    Deconstructor for the OuterProcessor class
// PARAMETER USAGE :
// FUNCTION CALLED :
// -----------------------------------------------------------
OuterProcessor::~OuterProcessor()
{
}

// -----------------------------------------------------------
// FUNCTION printWrap :
//    A wrapper method for printing using write()
// PARAMETER USAGE :
//    buf - A character array of size 100 containing
//          the print statement
// FUNCTION CALLED :
//    write()
// -----------------------------------------------------------
void OuterProcessor::printWrap(char buf[100]) {
    write(1, buf, strlen(buf));
}

// -----------------------------------------------------------
// FUNCTION OuterProcessor::ThreadFunc :
//    Contains functionality for the OuterProcessor thread.
//    Passes data from the input matricies to the
//    InnerProcessor threads for calculation.
// PARAMETER USAGE :
//    N/A
// FUNCTION CALLED :
//    N/A
// -----------------------------------------------------------
void OuterProcessor::ThreadFunc()
{
    Thread::ThreadFunc();
    Thread_t self = GetID();

    if (type == ROW)
    {
        sprintf(buf, "Row thread r[%d] started\n", numID);
        printWrap(buf);

        // cycle through all the cols in this matrixA ROW to pass data on
        for (k = 1; k <= Acols; k++)
        {
            number = matrixA[numID - 1][k - 1];
            rightChannels[numID][1]->Send(&number, sizeof(int));
            sprintf(buf, "Row thread r[%d] sent %d to P[%d,%d]\n"
                       , numID, number, numID, 1);
            printWrap(buf);
        }
    }
    else
    {
        // cycle through all the rows in this matrixB COL to pass data on
        for (k = 1; k <= Brows; k++)
        {
            number = matrixB[k - 1][numID - 1];
            downChannels[1][numID]->Send(&number, sizeof(int));
            sprintf(buf, "    Columm thread c[%d] sent %d to P[%d,%d]\n"
                       , numID, number, 1, numID);
            printWrap(buf);
        }

        // send the end of data bit
        number = END_OF_DATA;
        downChannels[1][numID]->Send(&number, sizeof(int));
        sprintf(buf, "    Column thread c[%d] sent EOD to P[%d,%d] and"
                     " terminated\n"
                   , numID, 1, numID);
        printWrap(buf);
    }

    Exit();
}

// -----------------------------------------------------------
// FUNCTION InnerProcessor :
//    Constructor for the InnerProcessor class
// PARAMETER USAGE :
//    rID - the row ID of the processor
//    cID - the column ID of the processor
// FUNCTION CALLED :
//    N/A
// -----------------------------------------------------------
InnerProcessor::InnerProcessor(int rID, int cID)
{
    rowID = rID;
    colID = cID;
    myNum = 0;

    ThreadName.seekp(0, ios::beg);
    ThreadName << "Inner" << rID << cID << ends;

    UserDefinedThreadID = rID + cID;

}

// -----------------------------------------------------------
// FUNCTION ~InnerProcessor :
//    Deconstructor for the InnerProcessor class
// PARAMETER USAGE :
// FUNCTION CALLED :
// -----------------------------------------------------------
InnerProcessor::~InnerProcessor()
{
}

// -----------------------------------------------------------
// FUNCTION printWrap :
//    A wrapper method for printing using write()
// PARAMETER USAGE :
//    buf - A character array of size 100 containing
//          the print statement
// FUNCTION CALLED :
//    write()
// -----------------------------------------------------------
void InnerProcessor::printWrap(char buf[100]) {
    write(1, buf, strlen(buf));
}

// -----------------------------------------------------------
// FUNCTION InnerProcessor::ThreadFunc :
//    Contains functionality for the InnerProcessor thread.
//    Retrieves a msg from above and to the left, passes them
//    down and to the right respectively, then multiplies the
//    2 numbers and adds it to it's total.
// PARAMETER USAGE :
//    N/A
// FUNCTION CALLED :
//    N/A
// -----------------------------------------------------------
void InnerProcessor::ThreadFunc()
{
    Thread::ThreadFunc();
    Thread_t self = GetID();

    while (true)
    {
        // Receive from above
        downChannels[rowID][colID]->Receive(&above, sizeof(int));


        // if it is end of data, return results and exit
        if (above == END_OF_DATA)
        {
            // send END down
            // send the number down if possible
            if (rowID < Crows)
            {
                downChannels[rowID + 1][colID]->Send(&above, sizeof(int));
            }
            matrixC[rowID][colID] = myNum;
            sprintf(buf, "       Thread P[%d,%d] received EOD, saved result"
                         "%d and terminated\n", rowID, colID, myNum);
            printWrap(buf);
            Exit();
        }

        // Receive from left
        rightChannels[rowID][colID]->Receive(&left, sizeof(int));

        sprintf(buf, "       Thread P[%d,%d] received %d from above and"
                     " %d from left\n", rowID, colID, above, left);
        printWrap(buf);

        // send the number down if possible
        if (rowID < Crows)
        {
            downChannels[rowID + 1][colID]->Send(&above, sizeof(int));
            sprintf(buf, "       Thread P[%d,%d] sent %d down to P[%d,%d]\n"
                       , rowID, colID, above, rowID + 1, colID);
            printWrap(buf);
        }

        // send the number to the right if possible
        if (colID < Ccols)
        {
            rightChannels[rowID][colID + 1]->Send(&left, sizeof(int));
            sprintf(buf, "       Thread P[%d,%d] sent %d right to P[%d,%d]\n"
                       , rowID, colID, left, rowID, colID + 1);
            printWrap(buf);
        }


        // compute the results and add them to your number
        myNum = myNum + (above * left);

    }

}
