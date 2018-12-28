#include "thread.h"
#include "system.h"
#include "userthread.h"

static void StartUserThread(int f) {
    DEBUG('a', "Start user thread\n");
    args* a = (args*) f;
    
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    machine->WriteRegister(PCReg,a->fonction);
    machine->WriteRegister(NextPCReg,a->fonction+4);
    machine->WriteRegister(StackReg,a->sp);
    
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
    a->sp = newSP;
    sp = newSP;
    DEBUG('a', "Create user thread\n");
    Thread *newThread = new Thread("user thread");
    if (newThread == 0) {
        return -1;
    }
    newThread->space = currentThread->space;
    newThread->Fork(StartUserThread,(int) a);
    return 0;
}

void do_UserThreadExit() {
    currentThread->space->ClearSPThread(sp);
    currentThread->Finish();
}

