#include "thread.h"
#include "system.h"
#include "userthread.h"

int sps[( (UserStackSize/PageSize) /3 )]; // +1 is not nessecary because the main thread is the number 0

Semaphore * sems[( (UserStackSize/PageSize) /3 )];

int compteurTID = 0;

static void StartUserThread(int f) {
    DEBUG('a', "Start user thread\n");
    args* a = (args*) f;
    
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    machine->WriteRegister(PCReg,a->fonction);
    machine->WriteRegister(NextPCReg,a->fonction+4);
    machine->WriteRegister(StackReg,sps[currentThread->getTid()]);
    
    //int val = 0;
    //machine->ReadMem(a->arg,4,&val); // temp TODO : trouver un moyen de passer l'argument sous forme de pointeur c
    machine->WriteRegister(4,a->arg);
    machine->Run();
}

int do_UserThreadCreate(int f, int arg) {
    args* a = new args;
    a->fonction = f;
    a->arg = arg;
    int newSP = currentThread->space->GetSPnewThread();
    if (newSP == -1) {
        return -1;
    }

    DEBUG('a', "Create user thread\n");
    Thread *newThread = new Thread("user thread");
    if (newThread == 0) {
        return -1;
    }
    newThread->setTid(compteurTID);
    compteurTID++;
    sps[newThread->getTid()] = newSP;
    sems[newThread->getTid()] = new Semaphore("sem Thread",0);
    newThread->space = currentThread->space;
    newThread->Fork(StartUserThread,(int) a);
    return newThread->getTid();
}

void do_UserThreadExit() {
    currentThread->space->ClearSPThread(sps[currentThread->getTid()]);
    sems[currentThread->getTid()]->V();
    currentThread->Finish();
}

void do_UserThreadJoin(int tid) {
    Semaphore * s = sems[tid];
    if (s != NULL) {
        s->P(); // wait the thread to do V on his semaphore
        s->V(); // do V for the other call to userThreadJoin with the same tid.
    }
}
