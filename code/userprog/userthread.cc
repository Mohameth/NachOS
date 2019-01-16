#include "thread.h"
#include "system.h"
#include "userthread.h"

using namespace std;


Semaphore * mutex_thread = new Semaphore("mutex_thread",1);

static void StartUserThread(int f) {
    mutex_thread->P();

    DEBUG('a', "Start user thread\n");
    args* a = (args*) f;
    
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    machine->WriteRegister(PCReg,a->fonction);
    machine->WriteRegister(NextPCReg,a->fonction+4);
    machine->WriteRegister(StackReg,currentThread->space->infos->at(currentThread->getTID()).sp );

    machine->WriteRegister(4,a->arg);
    mutex_thread->V();

    machine->Run(); //execute la fonction dans PCREG
    ASSERT(false);
}

int do_UserThreadCreate(int f, int arg) {
    mutex_thread->P(); // critical section start

    args* a = new args; //contient les arguments: fonction à exectuer et arguments de cette fonction pour le thread
    a->fonction = f;
    a->arg = arg;
    int newSP = currentThread->space->GetSPnewThread();
    if (newSP == -1) { //création impossible pas assez de mémoire
        mutex_thread->V();
        return -1;
    }

    DEBUG('a', "Create user thread\n");
    Thread *newThread = new Thread("user thread");
    if (newThread == 0) {
        mutex_thread->V();
        return -1;
    }
    printf("Size : %d\n\n", currentThread->space->infos->size());
    ASSERT(currentThread->space->infos->size() < (UserStackSize / SectorSize)/UserThreadStackSize );
    newThread->setTID(currentThread->space->getCompteurTID()); //id du thread crée
    currentThread->space->setCompteurTID(currentThread->space->getCompteurTID()+1);
    ThreadInfo t;
    t.sp = newSP;
    t.s = new Semaphore("sem Thread",0);
    currentThread->space->infos->insert(pair<int,ThreadInfo>(newThread->getTID(),t)); //ajout dans la hashmap les données
   
    mutex_thread->V();

    newThread->Fork(StartUserThread,(int) a);  //execute StartUserThread
    return newThread->getTID();
}

void do_UserThreadExit() {
    currentThread->space->ClearSPThread(currentThread->space->infos->at(currentThread->getTID()).sp);

    currentThread->space->infos->at(currentThread->getTID()).s->V();
    currentThread->Finish();
}

void do_UserThreadJoin(int tid) {
    //gestion du cas ou on réalise un join sur un tid non valide ou d'un deuxième appel de threadjoin.
    if (currentThread->space->infos->find(tid) == currentThread->space->infos->end()) //si les données du thread n'exite plus dans la hashmap, ne rien faire
        return;
    currentThread->space->infos->at(tid).s->P(); //l'aquisition du sémaphore indique que le thread est terminé
    currentThread->space->infos->erase(tid); //supprime du hashmap les infos du thread (libèration de la mémoire)

}

void attendreThread(){
    mutex_thread->P();
    int compteur=currentThread->space->getCompteurTID();
    mutex_thread->V();
    for(int i=0;i<compteur;i++){
        do_UserThreadJoin(i);
    }
}
