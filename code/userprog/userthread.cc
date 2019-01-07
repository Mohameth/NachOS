#include "thread.h"
#include "system.h"
#include "userthread.h"

int sps[divRoundUp(UserStackSize,PageSize*3)-1]; // +1 is not nessecary because the main thread is the number 0

Semaphore * sems[divRoundUp(UserStackSize,PageSize*3)-1];

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
    machine->WriteRegister(StackReg,sps[currentThread->getTid()]);

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
    sps[newThread->getTid()] = newSP;
    sems[newThread->getTid()] = new Semaphore("sem Thread",0);
    
    mutex->V();

    newThread->Fork(StartUserThread,(int) a);
    return newThread->getTid();
}

void do_UserThreadExit() {
    currentThread->space->ClearSPThread(sps[currentThread->getTid()]);
    sems[currentThread->getTid()]->V();
    currentThread->Finish();
}

void do_UserThreadJoin(int tid) {
    sems[tid]->P();
    sems[tid]->V();
}
