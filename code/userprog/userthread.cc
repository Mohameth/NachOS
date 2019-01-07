#include "thread.h"
#include "system.h"
#include "userthread.h"
#include <map>

using namespace std;

//int sps[divRoundUp(UserStackSize,PageSize*3)-1]; // +1 is not nessecary because the main thread is the number 0

//Semaphore * sems[divRoundUp(UserStackSize,PageSize*3)-1];


//Hashmap reliant le tid du thread a la structure contenant les informations du thread
map<int,ThreadInfo> infos;

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

    machine->Run(); //execute la fonction dans PCREG
}

int do_UserThreadCreate(int f, int arg) {
    mutex->P(); // critical section start

    args* a = new args; //contient les arguments: fonction à exectuer et arguments de cette fonction pour le thread
    a->fonction = f;
    a->arg = arg;
    int newSP = currentThread->space->GetSPnewThread();
    if (newSP == -1) { //création impossible pas assez de mémoire
        mutex->V();
        return -1;
    }

    DEBUG('a', "Create user thread\n");
    Thread *newThread = new Thread("user thread");
    if (newThread == 0) {
        mutex->V();
        return -1;
    }
    newThread->setTid(compteurTID++); //id du thread crée
    ThreadInfo t;
    t.sp = newSP;
    t.s = new Semaphore("sem Thread",0);
    infos.insert(pair<int,ThreadInfo>(newThread->getTid(),t)); //ajout dans la hashmap les données
    // sps[newThread->getTid()] = newSP;
    // sems[newThread->getTid()] = new Semaphore("sem Thread",0);
    
    mutex->V();

    newThread->Fork(StartUserThread,(int) a);  //execute StartUserThread
    return newThread->getTid();
}

void do_UserThreadExit() {
    currentThread->space->ClearSPThread(infos.at(currentThread->getTid()).sp);
    //sems[currentThread->getTid()]->V();
    infos.at(currentThread->getTid()).s->V();
    currentThread->Finish();
}

void do_UserThreadJoin(int tid) {
    if (infos.find(tid) == infos.end())
        return;
    infos.at(tid).s->P();
    infos.erase(tid);

}

void attendreThread(){
    mutex->P();
    int compteur=compteurTID;
    mutex->V();
    for(int i=0;i<compteur;i++){
        do_UserThreadJoin(i);
    }
}
