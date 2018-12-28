#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "frameprovider.h"

FrameProvider::FrameProvider(int numPage, TranslationEntry * entry) {
    this->frameBitMap = new BitMap(numPage);
    this->mutex = new Semaphore("FrameProvider mutex", 1);
    this->entries = entry;
    this->numPages = numPage;

    for (int i = 0; i < numPages; i++) {
        if (entries[i].valid)
            frameBitMap->Mark(i);
    }
}

FrameProvider::~FrameProvider() {
    delete this->frameBitMap;  
    delete this->mutex;  
}

TranslationEntry *
FrameProvider::GetEmptyFrame() { //On doit peut-être modifier la page à retourner pour signifier qu'elle est maintenant utiisée ?
    mutex->P();
    int freeFrame = frameBitMap->Find();
    TranslationEntry * freeEntry = &entries[freeFrame];
    bzero((void *) freeEntry->physicalPage, PageSize);
    mutex->V();
    return freeEntry;
    
}


void
FrameProvider::ReleaseFrame(TranslationEntry * virtualPage) { //On doit peut-être modifier la page à retourner pour signifier qu'elle est maintenant libre ?
    mutex->P();

    int i = 0;
    while(i < numPages && entries[i].virtualPage != virtualPage->virtualPage) {
        i++;
    }
    
    if (i == numPages) {
        printf("This page does not exist : %x", virtualPage->virtualPage);
    }

    frameBitMap->Clear(i);

    mutex->V();
}

int
FrameProvider::NumAvailFrame() {
    mutex->P();
    int numAvail = 0;
    for (int i = 0; i < numPages; i++) {
        if (frameBitMap->Test(i))
            numAvail++;
    }
    return numAvail;
    mutex->V();
}

#endif // CHANGED