#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "frameprovider.h"

FrameProvider::FrameProvider(int numPage, TranslationEntry * entry) {
    frameBitMap = new BitMap(numPage);
    entries = entry;
    numPage = numPages;

    for (int i = 0; i < numPages; i++) {
        if (entries[i].valid)
            frameBitMap->Mark(i);
    }
}

FrameProvider::~FrameProvider() {
    delete frameBitMap;    
}

FrameProvider::GetEmptyFrame() {
    int freeFrame = frameBitMap->Find();
    TranslationEntry * freeEntry = entries[i];
    
}

#endif // CHANGED