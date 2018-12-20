#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvial;
static Semaphore *writeDone;
static Semaphore *getStringSem; // basicly it's a mutex, it take for a Get String operation
static Semaphore *putStringSem; // basicly it's a mutex, it take for a Put String operation 
static Semaphore *putBusy; // basicly it's a mutex, it take for a PutChar operation, for protect multithreading access to console (2 write request at same time make an error)

static void ReadAvial(int arg) { readAvial->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
    readAvial = new Semaphore("read avial", 0);
    writeDone = new Semaphore("write done", 0);
    getStringSem = new Semaphore("get string",1);
    putBusy = new Semaphore("put busy",1); 
    putStringSem = new Semaphore("put string",1);
    console = new Console(readFile, writeFile, ReadAvial, WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
    delete console;
    delete putStringSem;
    delete putBusy;
    delete getStringSem;
    delete writeDone;
    delete readAvial;
}

void SynchConsole::SynchPutChar(const char ch)
{
    putBusy->P(); // critical section 1 thread addmited
    console->PutChar(ch);
    writeDone->P(); // wait for the write to be done
    putBusy->V();
}

char SynchConsole::SynchGetChar()
{
    readAvial->P(); // wait for character to arrive
    return console->GetChar();
}

void SynchConsole::copyStringFromMachine(int from, char *to, unsigned int size)
{
    for (unsigned int i = 0; i < size-1; i++)
    {
        machine->ReadMem(from + i, 1, (int *)(to + i));
    }
    *(to + (size-1)) = '\0';
}

void SynchConsole::SynchPutString(const char s[])
{
    putStringSem->P();
    int i = 0;
    while (i < MAX_STRING_SIZE && s[i] != '\0')
    {
        SynchPutChar(s[i]);
        i++;
    }
    putStringSem->V();
}

void SynchConsole::SynchGetString(char *s, int n)
{
    getStringSem->P(); // critical section. exclusif access for read the String
    char c;
    int i;
    for (i =0; i < n-1; i++) {
        c = SynchGetChar();
        
        if (c == EOF || c == '\n')
            break;

        *(s+i) = c;
        //machine->WriteMem((int) (s+i),1,(int) c); // first solution : obsolete
    }

    char fin = '\0';

    if (c == '\n') {
        *(s+i) = c;
        //machine->WriteMem((int) (s+i),1,(int) c);
        i++;
    }

    for (; i < n-1; i++) {
        *(s+i) = fin;
        //machine->WriteMem((int) (s+i),1,(int) fin);
    }

    *(s+i) = fin;
    //machine->WriteMem((int) (s+i),1,(int) fin);

    getStringSem->V();
}

void SynchConsole::SynchPutInt(int n) {
    char s[20];

    snprintf(s,20,"%d",n);

    SynchPutString(s);
}

int SynchConsole::SynchGetInt() {
    char s[20];
    int val = 0;

    SynchGetString(s,20);
    sscanf(s,"%d",&val);

    return val;
}

#endif // CHANGED