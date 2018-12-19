#include "thread.h"
#include "system.h"
#include "userthread.h"

static void StartUserThread(int f) {
    DEBUG('a', "Start user thread\n");
    args* a = (args*) f;
    
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    //currentThread->stack += (PageSize*2);
    //stackallocate
    machine->WriteRegister(PCReg,a->fonction);
    machine->WriteRegister(NextPCReg,a->fonction+4);
    
    machine->WriteRegister(4,a->arg);
    machine->Run();
}

int do_UserThreadCreate(int f, int arg) {
    args* a = new args;
    a->fonction = f;
    a->arg = arg;
    DEBUG('a', "Create user thread\n");
    Thread *newThread = new Thread("user thread");
    if (newThread == 0) {
        return -1;
    }
    newThread->Fork(StartUserThread,(int) a);
    return 0;
}

