#include "thread.h"
#include "system.h"
#include "string"
#include "userprocess.h"

Semaphore * mutex_process = new Semaphore("mutex_process",1);
// int pid = 1; //Censé être utilisé pour nommer les threads 

using namespace std;

void StartForkExec(int addrSpace) {

    AddrSpace * space = (AddrSpace *) addrSpace;


    currentThread->space = space;
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    machine->Run();
    ASSERT(false);

}

int
do_ForkExec (char * filename) //Crée
{
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;
    if (executable == NULL)
        {
    printf ("Unable to open file %s\n", filename);
    return -1;
        }

    IntStatus oldLevel = interrupt->SetLevel (IntOff);
    space = new AddrSpace (executable);
    (void) interrupt->SetLevel (oldLevel);
    
    if (space == NULL)
        return -1;
        
    delete executable;		// close file

    mutex_process->P();
    CurrentProcessNumber++;
    mutex_process->V();


    Thread * t = new Thread("forkexec");
    void (*f)(int) = StartForkExec;
    t->Fork(f, (int) space);
    return 1;

}

void exitProcess() {
    mutex_process->P();
    CurrentProcessNumber--;
    mutex_process->V();

    if(CurrentProcessNumber == 0) {
        interrupt->Halt ();
    }
    currentThread->Finish();
}


