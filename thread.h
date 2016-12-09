
#include "ThreadClass.h"

const int NOT_DEFINED = -2;
const int END_OF_DATA = -1;

class OuterProcessor : public Thread
{
    public:
        OuterProcessor(int id);   // constructor
        ~OuterProcessor();                  // destructor
        SynOneToOneChannel *channel;

    private:
        int numID;
        int number;
        char buf[100];
        void ThreadFunc();
};

class InnerProcessor : public Thread
{
    public:
        InnerProcessor(int id);   // constructor
        ~InnerProcessor();                  // destructor
        SynOneToOneChannel *down;
        SynOneToOneChannel *right;

    private:
        int numID;
        int myNum;
        char buf[100];
        void ThreadFunc();
};
