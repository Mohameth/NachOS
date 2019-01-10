// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userthread.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}

void
do_ForkExec (char *filename)
{
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL)
      {
	  printf ("Unable to open file %s\n", filename);
	  return;
      }
    Thread * t = new Thread("forkexec");
    space = new AddrSpace (executable);
    t->space = space;

    delete executable;		// close file

    t->space->InitRegisters ();	// set the initial register values
    // t->space->RestoreState ();	// load page table register
    scheduler->ReadyToRun(t);
    // the address space exits
    // by doing the syscall "exit"
    printf("Fork done !\n");
}

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which) {
  int type = machine->ReadRegister (2);

#ifndef CHANGED // Noter le if*n*def

  if ((which == SyscallException) && (type == SC_Halt)) {
    DEBUG ('a', "Shutdown, initiated by user program.\n");
    interrupt->Halt ();
  }
  else {
    printf ("Unexpected user mode exception %d %d\n", which, type);
    ASSERT (FALSE);
  }

#else //CHANGED

  if (which == SyscallException) {
    switch(type) {

      case SC_Halt: {
        //printf("HALT");
        attendreThread(); // attente des threads en cour d'execution
        DEBUG('a', "Shutdown, initiated by user program.\n");
        
        interrupt->Halt ();
        break;
      }

      case SC_Exit: {
        //printf("EXIT");
        attendreThread(); // attente des threads en cour d'execution
        DEBUG('a', "User Program terminate\n");
        printf("exit with status %d\n", machine->ReadRegister(4));
        //interrupt->Halt();
        break;
      }

      case SC_PutChar: {
        synchconsole->SynchPutChar((char) machine->ReadRegister (4));
        break;
      }

      case SC_PutString: {
        int saddr =  machine->ReadRegister (4);
        //int size =  machine->ReadRegister (5);
        char s[MAX_STRING_SIZE];
        synchconsole->copyStringFromMachine(saddr,s,MAX_STRING_SIZE-1);
        synchconsole->SynchPutString(s);
        break;
      }

      case SC_GetChar: {
        char c = synchconsole->SynchGetChar();
        machine->WriteRegister(2,(int) c);
        break;
      }

      case SC_GetString: {
        int addr = machine->ReadRegister(4);
        int size = machine->ReadRegister(5);
        // char * s = (char *) addr;
        // synchconsole->SynchGetString(s,size);
        char s [size];
        synchconsole->SynchGetString(s,size);

        for (int i =0; i < size; i++) { 
          machine->WriteMem((int) (addr+i),1,(int) s[i]);
        }
        break;
      }

      case SC_PutInt: {
        int val = machine->ReadRegister(4);
        synchconsole->SynchPutInt(val);
        break;
      }

      case SC_GetInt: {
        int val = synchconsole->SynchGetInt();
        machine->WriteRegister(2,val);
        break;
      }

      case SC_ForkExec: {
        int saddr =  machine->ReadRegister (4);
        char s[MAX_STRING_SIZE];
        synchconsole->copyStringFromMachine(saddr,s,MAX_STRING_SIZE-1);
        do_ForkExec(s);
        break;
      }

      case SC_UserThreadCreate: {
        int f = machine->ReadRegister(4);
        int arg = machine->ReadRegister(5);
        int res = do_UserThreadCreate(f,arg);
        machine->WriteRegister(2,res);
        break;
      }

      case SC_UserThreadExit: {
        do_UserThreadExit();
        break;
      }

      case SC_UserThreadJoin: {
        int tid = machine->ReadRegister(4);
        do_UserThreadJoin(tid);
        break;
      }

      case SC_Open: {
        char name[MAX_STRING_SIZE];
        int addr = machine->ReadRegister(4);
        synchconsole->copyStringFromMachine(addr, name, MAX_STRING_SIZE-1);

        OpenFile* file = fileSystem->Open(name);
        if (file == NULL) printf("Erreur \n"); //TODO: Sortir
        int id = fileSystem->getUnusedId();

        fileSystem->addOpenFile(id, file);

        machine->WriteRegister(2, id);
        printf("fichier %s ouvert : %d",name, id);
        break;
      }

      case SC_Create: {
        char name[MAX_STRING_SIZE];
        int addr = machine->ReadRegister(4);
        synchconsole->copyStringFromMachine(addr, name, MAX_STRING_SIZE-1);

        bool res = fileSystem->Create(name, 0);//TODO: gerer initialSize
        if (res)  printf("Creation de fichier vide reussi\n");
        else      printf("ERREUR DE CREATION \n");
        break;
      }

      case SC_Read: {
        //int bufAddr = machine->ReadRegister(4);
        int size    = machine->ReadRegister(5);
        int fileId  = machine->ReadRegister(6);

        char value[MAX_STRING_SIZE];
        
        printf("id = %d \n", fileId);

        OpenFile* f = fileSystem->getOpenFile(fileId);
        
        f->Read(value, size);
        //TODO: mettre dans le buffer pointé par bufAddre la valeur
        //TODO: retourner nb octets lues
        printf("Lecture de %d octets = %s, id = %d \n", size, value, fileId);

        break;
      }

      case SC_Write: {
        int valueAddr = machine->ReadRegister(4);
        int size    = machine->ReadRegister(5);
        int fileId  = machine->ReadRegister(6);

        char value[MAX_STRING_SIZE];
        synchconsole->copyStringFromMachine(valueAddr, value, MAX_STRING_SIZE-1);
        printf("id = %d \n", fileId);

        OpenFile* f = fileSystem->getOpenFile(fileId);
        
        f->Write(value, size);

        printf("Ecriture de %d octets = %s, id = %d \n", size, value, fileId);
        break;
      }

      default: {
        printf ("Unexpected user mode exception %d %d\n", which, type);
        ASSERT (FALSE);
      }

    }
    // LB: Do not forget to increment the pc before returning!
    UpdatePC ();
    // End of addition
  }

}

#endif //CHANGED