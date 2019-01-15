#include "unlimitedPost.h"
#include "system.h"



UnlimitedPostOffice::UnlimitedPostOffice(NetworkAddress addr, double reliability, int nBoxes)
{
    fiablepost = new FiablePostOffice(addr,reliability,nBoxes);
}

UnlimitedPostOffice::UnlimitedPostOffice(FiablePostOffice* p) {
    fiablepost = p;
}

//----------------------------------------------------------------------
// FiablePostOffice::~FiablePostOffice
// 	De-allocate the fiable post office data structures.
//----------------------------------------------------------------------

UnlimitedPostOffice::~UnlimitedPostOffice()
{
    delete fiablepost;
}

//----------------------------------------------------------------------
// FiablePostOffice::PostalDelivery
// 	Wait for incoming messages, and put them in the right mailbox.
//
//      Incoming messages have had the PacketHeader stripped off,
//	but the MailHeader is still tacked on the front of the data.
//----------------------------------------------------------------------

void
UnlimitedPostOffice::PostalDelivery()
{
    fiablepost->PostalDelivery();
}

//----------------------------------------------------------------------
// FiablePostOffice::Send
// 	Concatenate the MailHeader to the front of the data, and pass 
//	the result to the Network for delivery to the destination machine.
//
//	Note that the MailHeader + data looks just like normal payload
//	data to the Network.
//
//	"pktHdr" -- source, destination machine ID's
//	"mailHdr" -- source, destination mailbox ID's
//	"data" -- payload message data
//----------------------------------------------------------------------

void
UnlimitedPostOffice::Send(PacketHeader pktHdr, MailHeader mailHdr, const char* data)
{
    int nbMail = divRoundUp(mailHdr.length, MaxMailSize); 

    MailHeader outMailHdr;
    outMailHdr.from = mailHdr.from;
    outMailHdr.to = mailHdr.to;
    outMailHdr.length = sizeof(int);


    fiablepost->Send(pktHdr,outMailHdr,(char *) &nbMail); // envoie du nombre de mail du message 

    char outdata[MaxMailSize];
    outMailHdr.length = MaxMailSize;

    for (int i = 0; i < nbMail; i++) {
        if (i == nbMail-1) {
            int sizeRest = mailHdr.length % MaxMailSize;
            memcpy(outdata,data+(i*MaxMailSize),sizeRest+1);
            outMailHdr.length = sizeRest+1;
        } else {
            memcpy(outdata,data+(i*MaxMailSize),MaxMailSize);
        }
        

        fiablepost->Send(pktHdr,outMailHdr,outdata);
    }
}

//----------------------------------------------------------------------
// FiablePostOffice::Send
// 	Retrieve a message from a specific box if one is available, 
//	otherwise wait for a message to arrive in the box.
//
//	Note that the MailHeader + data looks just like normal payload
//	data to the Network.
//
//
//	"box" -- mailbox ID in which to look for message
//	"pktHdr" -- address to put: source, destination machine ID's
//	"mailHdr" -- address to put: source, destination mailbox ID's
//	"data" -- address to put: payload message data
//----------------------------------------------------------------------

void
UnlimitedPostOffice::Receive(int box, PacketHeader *pktHdr, 
				MailHeader *mailHdr, char* data)
{
    int nbMail;
    fiablepost->Receive(box,pktHdr,mailHdr,(char *) &nbMail); // reception du nombre de mail à recevoire
    
    // if (nbMail == 1) {
    //     fiablepost->Receive(box,pktHdr,mailHdr,data);
    //     return;
    // }
    if (DebugIsEnabled('n')) printf("nbMail %d\n",nbMail);


    char outdata[MaxMailSize]; // buffer pour chaque mail


    for (int i = 0; i < nbMail; i++) {
        fiablepost->Receive(box,pktHdr,mailHdr,outdata); // reception du message

        memcpy(data+(i*MaxMailSize),outdata,mailHdr->length); // copie des données reçu dans le relutat final
    }
}

int UnlimitedPostOffice::GetAddr() {
    return fiablepost->GetAddr();
}