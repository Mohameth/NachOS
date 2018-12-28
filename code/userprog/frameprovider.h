#ifdef CHANGED

#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "utility.h"
#include "bitmap.h"
#include "synch.h"
#include <strings.h>		/* for bzero */


class FrameProvider {
    public:
        FrameProvider(int numPage);
        
        ~FrameProvider();        //clean up

        unsigned int GetEmptyFrame();

        void ReleaseFrame(unsigned int physicalFrame);

        int NumAvailFrame();
    private:
        Semaphore * mutex;
        BitMap * frameBitMap;
        int numPages;

};

#endif // FRAMEPROVIDER_H

#endif // CHANGED