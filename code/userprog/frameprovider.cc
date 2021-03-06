#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "frameprovider.h"

FrameProvider::FrameProvider(int numPage) {
    this->frameBitMap = new BitMap(numPage);
    this->mutex = new Semaphore("FrameProvider mutex", 1);
    this->numPages = numPage;
}

FrameProvider::~FrameProvider() {
    delete this->frameBitMap;  
    delete this->mutex;  
}

unsigned int
FrameProvider::GetEmptyFrame() {
    mutex->P();
    int freeFrameNum = frameBitMap->Find();
    unsigned int physicalFrame = (unsigned int) freeFrameNum;

    int physAddr = physicalFrame * PageSize;
    bzero(&machine->mainMemory[physAddr], PageSize);
    mutex->V();
    return physicalFrame;
    
}


void
FrameProvider::ReleaseFrame(unsigned int physicalFrame) {
    mutex->P();

    if (physicalFrame < 0 || physicalFrame > (unsigned int) this->numPages) {
        printf("This page does not exist : %ud", physicalFrame);
        exit(-1);
    }

    frameBitMap->Clear(physicalFrame);

    mutex->V();
}

unsigned int
FrameProvider::NumAvailFrame() {
    mutex->P();
     unsigned int numAvail = (unsigned int) frameBitMap->NumClear();
    mutex->V();
    return numAvail;
}

#endif // CHANGED