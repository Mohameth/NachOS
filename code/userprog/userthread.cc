#include "thread.h"
#include "system.h"
#include "userthread.h"
#include <vector> 
  
using namespace std; 

//int sps[divRoundUp(UserStackSize,PageSize*3)-1]; // +1 is not nessecary because the main thread is the number 0

//Semaphore * sems[divRoundUp(UserStackSize,PageSize*3)-1];

vector<ThreadInfo> infos;

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
    machine->WriteRegister(StackReg,infos.at(currentThread->getTid()).sp );

    machine->WriteRegister(4,a->arg);
    mutex->V();

    machine->Run();
}

int do_UserThreadCreate(int f, int arg) {
    mutex->P(); // critical section start

    args* a = new args;
    a->fonction = f;
    a->arg = arg;
    int newSP = currentThread->space->GetSPnewThread();
    if (newSP == -1) {
        mutex->V();
        return -1;
    }

    DEBUG('a', "Create user thread\n");
    Thread *newThread = new Thread("user thread");
    if (newThread == 0) {
        mutex->V();
        return -1;
    }
    newThread->setTid(compteurTID++);
    ThreadInfo t;
    t.sp = newSP;
    t.s = new Semaphore("sem Thread",0);
    infos.push_back(t);
    // sps[newThread->getTid()] = newSP;
    // sems[newThread->getTid()] = new Semaphore("sem Thread",0);
    
    mutex->V();

    newThread->Fork(StartUserThread,(int) a);
    return newThread->getTid();
}

void do_UserThreadExit() {
    currentThread->space->ClearSPThread(infos.at(currentThread->getTid()).sp);
    //sems[currentThread->getTid()]->V();
    infos.at(currentThread->getTid()).s->V();
    currentThread->Finish();
}

void do_UserThreadJoin(int tid) {
    infos.at(tid).s->P();
    infos.at(tid).s->V();
}
