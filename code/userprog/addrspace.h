// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "bitmap.h"
#include <map>


#define UserStackSize		1024	// increase this as necessary!

class Semaphore;

//contient les informations pour un thread: stack pointeur et semaphore assurant la synchronisation (thread join)
typedef struct {
    int sp;
    Semaphore * s;
} ThreadInfo; 


class AddrSpace
{
  public:

    //Hashmap reliant le tid du thread a la structure contenant les informations du thread
    map<int,ThreadInfo> * infos;

    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 

    int GetSPnewThread(); // get the SP of a new user thread created. -1 if not possible
    void ClearSPThread(int SP); // clear (Mark as free) the 3 pages identify by the addr SP

    // map<int,ThreadInfo> * getInfos();

    int getCompteurTID();

    void setCompteurTID(int newTID);

  
  private:
    TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space
    BitMap * stackBitMap; // bitMap for the userstack, now the group of 3 page use by user thread. at the beginning the last 3 page are use by the main thread.


    int compteurTID;

};

#endif // ADDRSPACE_H
