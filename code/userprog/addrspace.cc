// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"

#include <strings.h>		/* for bzero */



void
ReadAtVirtual (OpenFile * executable, int virtualaddr, int numBytes, int position, TranslationEntry *pageTable, unsigned int numPages) {

    ASSERT((unsigned int) numBytes < numPages*PageSize);

    TranslationEntry * backPageTable = machine->pageTable;
    unsigned int backNumPages = machine->pageTableSize;

    int currentByte = 0;
    int vAddr = virtualaddr;
    char buff[numBytes];
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
    executable->ReadAt (buff,numBytes,position);
    while (numBytes - currentByte > 0) {
        machine->WriteMem(vAddr, 1, (int) buff[currentByte]);
        currentByte++;
        vAddr++;
    }
    machine->pageTable = backPageTable;
    machine->pageTableSize = backNumPages;

}

//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
	WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{
    NoffHeader noffH;
    unsigned int i, size;

    executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
	(WordToHost (noffH.noffMagic) == NOFFMAGIC))
	SwapHeader (&noffH);
    ASSERT (noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;

    ASSERT (numPages <= frameProvider->NumAvailFrame());	// check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory

    DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
	   numPages, size);
// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++)
      {
	  pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
      int freePhysPage = frameProvider->GetEmptyFrame();
      if (freePhysPage == -1)
        ASSERT(freePhysPage != -1);
	  pageTable[i].physicalPage = freePhysPage;
	  pageTable[i].valid = TRUE;
	  pageTable[i].use = FALSE;
	  pageTable[i].dirty = FALSE;
	  pageTable[i].readOnly = FALSE;	// if the code segment was entirely on 
	  // a separate page, we could set its 
	  // pages to be read-only
      }
    this->pageTable = pageTable;
    this->numPages = numPages;
// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
    //bzero (machine->mainMemory, size); //Useless while we have working virtual memory

// then, copy in the code and data segments into memory
    if (noffH.code.size > 0)
      {
	  DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
		 noffH.code.virtualAddr, noffH.code.size);
      ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size, noffH.code.inFileAddr, pageTable, numPages);
	//   executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
			    //   noffH.code.size, noffH.code.inFileAddr);
      }
    if (noffH.initData.size > 0)
      {
	  DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
		 noffH.initData.virtualAddr, noffH.initData.size);
      ReadAtVirtual(executable, noffH.initData.virtualAddr, noffH.initData.size, noffH.initData.inFileAddr, pageTable, numPages);
	//   executable->ReadAt (&
	// 		      (machine->mainMemory
	// 		       [noffH.initData.virtualAddr]),
	// 		      noffH.initData.size, noffH.initData.inFileAddr);
      }

    this->stackBitMap = new BitMap(divRoundUp(UserStackSize,PageSize*3));
    this->stackBitMap->Mark(0); // 3 pages for the main thread; already use.


    this->compteurTID = 0;
    this->infos = new map<int,ThreadInfo>();
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
    unsigned int i;
    for (i = 0; i < numPages; i++) {
        frameProvider->ReleaseFrame(pageTable[i].physicalPage);
    }
    // LB: Missing [] for delete
    // delete pageTable;
    delete [] pageTable;
    // End of modification
    delete stackBitMap;
    delete infos;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to %d\n",
	   numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------
void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}

int AddrSpace::GetSPnewThread() {
    int numPageSP = stackBitMap->Find(); //recherche 3 pages libre dans la pile et le marque "1" si trouve un groupe de 3 pages libre
    if (numPageSP == -1)  //pas de groupe de 3 pages libre
        return -1; //création impossible du thread
    else {
        int SPMain = numPages * PageSize - 16;  //stack pointeur du thread principal
        int SP = SPMain - (UserThreadStackSize*numPageSP*PageSize); //stack pointeur du thread en cours de création
        return SP;
    }
}


void AddrSpace::ClearSPThread(int SP) {
    int SPMain = numPages * PageSize - 16;
    int numPageSP = (( (SPMain - SP) / UserThreadStackSize)/PageSize); //retrouve le numéro du groupe de 3 pages, a partir du stack pointeur associé au thread
    stackBitMap->Clear(numPageSP);
}

// map<int,ThreadInfo> *
// AddrSpace::getInfos() {
//     return this->infos;
// }

int
AddrSpace::getCompteurTID() {
    return this->compteurTID;
}

void
AddrSpace::setCompteurTID(int newTID) {
    this->compteurTID = newTID;
}

