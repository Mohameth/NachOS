#ifndef USERPROCESS_H
#define USERPROCESS_H

#include "system.h"
#include "addrspace.h"
#include "synch.h"


extern void StartForkExec(int arg);

extern void do_ForkExec (char * filename);

extern void exitProcess();


#endif // USERPROCESS_H