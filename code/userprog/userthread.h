#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "system.h"
#include "addrspace.h"
#include "synch.h"

typedef struct {
    int fonction;
    int arg;
} args;

//contient les informations pour un thread: stack pointeur et semaphore assurant la synchronisation (thread join)
typedef struct {
    int sp;
    Semaphore * s;
} ThreadInfo; 

extern int do_UserThreadCreate(int f, int arg);

extern void do_UserThreadExit();

extern void do_UserThreadJoin(int tid);


#endif // USERTHREAD_H