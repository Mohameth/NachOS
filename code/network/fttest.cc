// nettest.cc 
//	Test out message delivery between two "Nachos" machines,
//	using the Post Office to coordinate delivery.
//
//	Two caveats:
//	  1. Two copies of Nachos must be running, with machine ID's 0 and 1:
//		./nachos -m 0 -o 1 &
//		./nachos -m 1 -o 0 &
//
//	  2. You need an implementation of condition variables,
//	     which is *not* provided as part of the baseline threads 
//	     implementation.  The Post Office won't work without
//	     a correct implementation of condition variables.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "network.h"
#include "post.h"
#include "interrupt.h"

// Test out message delivery, by doing the following:
//	1. send a message to the machine with ID "farAddr", at mail box #0
//	2. wait for the other machine's message to arrive (in our mailbox #0)
//	3. send an acknowledgment for the other machine's message
//	4. wait for an acknowledgement from the other machine to our 
//	    original message

void
FileTest(int farAddr)
{
    if (farAddr != 0) { //client
        int ret = fileTransfert->ClientGet(farAddr,"placeholder");
        printf("code ret : %d\n",ret);
        OpenFile* file;
        file = fileSystem->Open("placeholder");
        if (file == NULL) {
            printf("not found\n");
        } else {
            char mess[248];
            file->Read(mess,248);
            printf("reçu %s\n",mess);
        }
    } else {
        OpenFile* file;
        fileSystem->Create("placeholder",248);
        file = fileSystem->Open("placeholder");
        if (file == NULL) {
            printf("not found\n");
        } else {
            const char* mess = "Hello There !!!\nNope le coup du Ack de ses morts\nD'après le monde de l'internet c'est bien ce que vous avez fait qu'il faut faire pour instancier la map\nC'est cppreferencies qui l'a dit donc ça doit être bonJe comprends pas c'est ouf, posdkjksq";
            file->Write(mess,247);
            delete file;
            fileTransfert->Serveur();
        }
    }
    interrupt->Halt();
}
