
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
};
