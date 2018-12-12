#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvial;
static Semaphore *writeDone;

static void ReadAvial(int arg) { readAvial->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile,char *writeFile) {
    readAvial = new Semaphore("read avial",0);
    writeDone = new Semaphore("write done",0);
    console = new Console(readFile,writeFile,ReadAvial,WriteDone,0);
}

SynchConsole::~SynchConsole() {
    delete console;
    delete writeDone;
    delete readAvial;
}

void SynchConsole::SynchPutChar(const char ch) {
    console->PutChar(ch);
    writeDone->P(); // wait for the write to be done
}

char SynchConsole::SynchGetChar() {
    readAvial->P();	// wait for character to arrive  
    return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[]) {
    //
}

void SynchConsole::SynchGetString(char *s, int n) {
    //
}

#endif // CHANGED