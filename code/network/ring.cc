#include "copyright.h"

#include "system.h"
#include "network.h"
#include "post.h"
#include "interrupt.h"

void
RingTest(int m,int n) // le num de la machine qui doit envoyer le jeton et le nombre n de machine dans l'anneau
{
    PacketHeader outPktHdr, inPktHdr;
    MailHeader outMailHdr, inMailHdr;
    char buffer[MaxMailSize];
    // construct packet, mail header for original message
    // To: destination machine, mailbox 0
    // From: our machine, reply to: mailbox 1
    if (m == n-1)
        outPktHdr.to = 0;
    else
        outPktHdr.to = m+1;		
    outMailHdr.to = 0;
    outMailHdr.from = 1;
    

    if (m == 0) { // la machine 0 doit initialiser le jeton l'envoyer et attendre son retour
        const char *data = "Jeton!";
        outMailHdr.length = strlen(data) + 1;
        // Envoi du jeton
        fiablePostOffice->Send(outPktHdr, outMailHdr, data);

        // Attente que le jeton revienne
        fiablePostOffice->Receive(0, &inPktHdr, &inMailHdr, buffer);
        printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
        fflush(stdout);
    }
    else {
    // Attente du jeton
    fiablePostOffice->Receive(0, &inPktHdr, &inMailHdr, buffer);
    printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
    fflush(stdout);

    //Delay(1);

    // envoi du jeton à la machine suivante

    outMailHdr.length = strlen(buffer) + 1;
    fiablePostOffice->Send(outPktHdr, outMailHdr, buffer); 

    }

    

    // Then we're done!
    interrupt->Halt();
}