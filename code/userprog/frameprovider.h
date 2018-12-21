#ifdef CHANGED

#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "utility.h"
#include "bitmap.h"

class FrameProvider {
    public:
        FrameProvider(int numPage, TranslationEntry * entries);
        
        ~FrameProvider();        //clean up

        TranslationEntry * GetEmptyFrame();

        void ReleaseFrame(TranslationEntry * pageTable);

        int NumAvailFrame();
    private:
        Semaphore * mutex;
        BitMap * frameBitMap;
        TranslationEntry * entries;
        int numPages;

};

#endif // FRAMEPROVIDER_H

#endif // CHANGED