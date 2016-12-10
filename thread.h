// -----------------------------------------------------------
// NAME : Zachary Hill                         User ID: zahill
// DUE DATE : 12/09/2016
// PROGRAM ASSIGNMENT #6
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//    Contains thread class definitions
// -----------------------------------------------------------

#include "ThreadClass.h"

const int NOT_DEFINED = -2;
const int END_OF_DATA = -1;

class OuterProcessor : public Thread
{
    public:
        OuterProcessor(int id, int t);   // constructor
        ~OuterProcessor();                  // destructor
        SynOneToOneChannel *channel;

    private:
        int numID;
        int number;
        int type;
        int k;
        char buf[100];
        void ThreadFunc();
        void printWrap(char*);
};

class InnerProcessor : public Thread
{
    public:
        InnerProcessor(int rID, int cID);   // constructor
        ~InnerProcessor();                  // destructor
        SynOneToOneChannel *down;
        SynOneToOneChannel *right;

    private:
        int numID;
        int rowID;
        int colID;
        int myNum;
        int above;
        int left;
        char buf[100];
        void ThreadFunc();
        void printWrap(char*);
};
