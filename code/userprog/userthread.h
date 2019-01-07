#ifndef USERTHREAD_H
#define USERTHREAD_H

#include "system.h"
#include "addrspace.h"
#include "synch.h"

typedef struct {
    int fonction;
    int arg;
} args;

typedef struct {
    int sp;
    Semaphore * s;
} ThreadInfo;

extern int do_UserThreadCreate(int f, int arg);

extern void do_UserThreadExit();

extern void do_UserThreadJoin(int tid);

#endif // USERTHREAD_H