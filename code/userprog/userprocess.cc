#include "thread.h"
#include "system.h"
#include "string"
#include "userprocess.h"

Semaphore * mutex_process = new Semaphore("mutex_process",1);
int pid = 1;

using namespace std;

void StartForkExec(int arg) {

    char * filename = (char *) arg;
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;
    if (executable == NULL)
        {
    printf ("Unable to open file %s\n", filename);
    return;
        }

    space = new AddrSpace (executable);
    mutex_process->P();
    CurrentProcessNumber++;
    mutex_process->V();

    delete executable;		// close file

    currentThread->space = space;
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    machine->Run();
}

void
do_ForkExec (char * filename)
{
    mutex_process->P();
    string threadname = "forkexec" + pid;
    pid++;
    mutex_process->V();
    Thread * t = new Thread(threadname.c_str());
    void (*f)(int) = StartForkExec;
    t->Fork(f, (int) filename);
}

void exitProcess() {
    mutex_process->P();
    CurrentProcessNumber--;
    mutex_process->V();

    if(CurrentProcessNumber == 0) {
        interrupt->Halt ();
    }
}


