#include <string.h>
#include <stdio.h>
#include <iostream>
#include "ThreadClass.h"
#include "thread.h"

extern SynOneToOneChannel *aChannel;

OuterProcessor::OuterProcessor(int id)
{
    numID = id;


    ThreadName.seekp(0, ios::beg);
    ThreadName << "Outer" << numID << ends;

    UserDefinedThreadID = numID;

    strstream ChannelName;
    ChannelName << "Channel" << id - 1 << "-" << id << ends;
    channel = new SynOneToOneChannel(ChannelName.str(), 1, 2);
}

OuterProcessor::~OuterProcessor()
{
    delete channel;
}

void OuterProcessor::ThreadFunc()
{
    Thread_t self = GetID();

    if (numID == 1)
    {
        int testNum = 45;
        int k = 0;
        for (k = 0; k < 10; k++)
        {
            printf("Thread %d sending 45\n", numID);
            printf("channel pointer %d\n", channel);
            aChannel->Send(&testNum, sizeof(int));

        }
    }
    else
    {

         int count = 0;
         while(true)
         {
             printf("Attempting to Receive\n");
             printf("channel pointer %d\n", channel);
             aChannel->Receive(&number, sizeof(int));
             if (number == END_OF_DATA)
             {
                 printf("Found END\n");
                 Exit();
             }
             printf("Thread %d got num %d\n", numID, number);
             //printf("Sending num + 1 to other Thread\n");
             count++;
             if (count == 9)
             {
                 Exit();
             }
         }
    }
}

InnerProcessor::InnerProcessor(int id)
{
    numID = id;
    myNum = NOT_DEFINED;

    strstream ChannelName1;
    ChannelName1 << "Down" << id - 1 << "-" << id << ends;
    strstream ChannelName2;
    ChannelName2 << "Right" << id - 1 << "-" << id << ends;
    down = new SynOneToOneChannel(ChannelName1.str(), id - 1, id);
    right = new SynOneToOneChannel(ChannelName2.str(), id - 1, id);
}

InnerProcessor::~InnerProcessor()
{
    delete down;
    delete right;
}

void InnerProcessor::ThreadFunc()
{

}
