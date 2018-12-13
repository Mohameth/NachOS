#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvial;
static Semaphore *writeDone;
static Semaphore *getStringSem; // basicly it's a mutex 

static void ReadAvial(int arg) { readAvial->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
    readAvial = new Semaphore("read avial", 0);
    writeDone = new Semaphore("write done", 0);
    getStringSem = new Semaphore("get string",1); 
    console = new Console(readFile, writeFile, ReadAvial, WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
    delete console;
    delete getStringSem;
    delete writeDone;
    delete readAvial;
}

void SynchConsole::SynchPutChar(const char ch)
{
    console->PutChar(ch);
    writeDone->P(); // wait for the write to be done
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
    int i = 0;
    while (i < MAX_STRING_SIZE && s[i] != '\0')
    {
        SynchPutChar(s[i]);
        i++;
    }
}

void SynchConsole::SynchGetString(char *s, int n)
{
    getStringSem->P(); // critical section. exclusif access for read the String
    char c;
    int i;
    for (i =0; i < n-1; i++) {
        c = SynchGetChar();
        
        if (c == EOF)
            break;
    }

    *(s+(i-1)) = '\0';

    getStringSem->V();
}

#endif // CHANGED