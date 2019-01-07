#include "thread.h"
#include "system.h"
#include "userthread.h"

int sps[divRoundUp(UserStackSize,PageSize*3)-1]; // +1 is not nessecary because the main thread is the number 0

Semaphore * sems[divRoundUp(UserStackSize,PageSize*3)-1];
int numTid[divRoundUp(UserStackSize,PageSize*3)-1];

int compteurTID =0;

Semaphore * mutex = new Semaphore("mutex",1);

static void StartUserThread(int f) {
    mutex->P();

    DEBUG('a', "Start user thread\n");
    args* a = (args*) f;
    
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    machine->WriteRegister(PCReg,a->fonction);
    machine->WriteRegister(NextPCReg,a->fonction+4);
    int SP = currentThread->space->GetSpWithBitMapNumPage( sps[currentThread->getTid()-1] );
    machine->WriteRegister(StackReg,SP);

    machine->WriteRegister(4,a->arg);
    mutex->V();

    machine->Run();
}

int do_UserThreadCreate(int f, int arg) {
    mutex->P(); // critical section start

    args* a = new args;
    a->fonction = f;
    a->arg = arg;
    int numT = currentThread->space->GetNewThread();
    if (numT == -1) {
        mutex->V();
        return -1;
    }

    DEBUG('a', "Create user thread\n");
    Thread *newThread = new Thread("user thread");
    if (newThread == 0) {
        mutex->V();
        return -1;
    }
    newThread->setNumPage(numT);
    //compteurTID = (compteurTID+1)% ( divRoundUp(UserStackSize,PageSize*3)-1 );
    newThread->setTid(compteurTID++);
    sps[newThread->getNumPage()-1] = numT;
    sems[newThread->getNumPage()-1] = new Semaphore("sem Thread",0);
    numTid[newThread->getNumPage()-1]=newThread->getTid();
    mutex->V();

    newThread->Fork(StartUserThread,(int) a);
    return newThread->getTid();
}

void do_UserThreadExit() {
    currentThread->space->ClearThread(sps[currentThread->getTid()-1]);
    sems[currentThread->getTid()-1]->V();
    currentThread->Finish();
}

void do_UserThreadJoin(int tid) {
    int i=0;

    while(i<divRoundUp(UserStackSize,PageSize*3)-1 && numTid[i]!=tid){
        i++;
    }

    if(i==divRoundUp(UserStackSize,PageSize*3)-1)
    return;

    sems[i]->P();
    sems[i]->V();
    
}
