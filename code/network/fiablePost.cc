
#include "fiablePost.h"
#include "system.h"

#define TEMPO 	90000000   	// time to wait an Ackitment in machine tick
#define NBMAXREEMISSIONS 3  // nombre maximum de tentative réemissions


static void AckTimer(int f) 
{ FiablePostOffice *fi = (FiablePostOffice *)f; fi->CheckAckArrive(); }


FiablePostOffice::FiablePostOffice(NetworkAddress addr, double reliability, int nBoxes)
{
    post = new PostOffice(addr,reliability,nBoxes);
    nbReemis = 0;
    ack = false;
    
    myAddr = addr;
}

FiablePostOffice::FiablePostOffice(PostOffice* p) {
    post = p;
    nbReemis = 0;
    ack = false;
    
    myAddr = p->GetAddr();
}

//----------------------------------------------------------------------
// FiablePostOffice::~FiablePostOffice
// 	De-allocate the fiable post office data structures.
//----------------------------------------------------------------------

FiablePostOffice::~FiablePostOffice()
{
    delete post;
}

//----------------------------------------------------------------------
// FiablePostOffice::PostalDelivery
// 	Wait for incoming messages, and put them in the right mailbox.
//
//      Incoming messages have had the PacketHeader stripped off,
//	but the MailHeader is still tacked on the front of the data.
//----------------------------------------------------------------------

void
FiablePostOffice::PostalDelivery()
{
    post->PostalDelivery();
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
FiablePostOffice::Send(PacketHeader pktHdr, MailHeader mailHdr, const char* data)
{
    post->Send(pktHdr,mailHdr,data);
    lpktHdr = pktHdr;
    lmailHdr = mailHdr;
    ldata = data;

    interrupt->Schedule(AckTimer, (int)this, TEMPO, TimerInt);

    PacketHeader inPktHdr;
    MailHeader inMailHdr;
    char buffer[MaxMailSize];

    DEBUG('n',"try to recived ack \n");
    post->Receive(1, &inPktHdr, &inMailHdr, buffer);
    if (!strcmp(buffer,"ack")) {
        ack=true;
        DEBUG('n',"ack reçu \n");
        // const char *ackmess = "ack";
        // PacketHeader outPktHdr;
        // MailHeader outMailHdr;
        // outPktHdr.to = inPktHdr.from;
        // outMailHdr.to = 1;
        // outMailHdr.length = strlen(ackmess) + 1;
    
        //post->Send(outPktHdr,outMailHdr,ackmess);
    }
    else
        if (DebugIsEnabled('n')) printf("message non reçu : %s reçu\n",buffer);
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
FiablePostOffice::Receive(int box, PacketHeader *pktHdr, 
				MailHeader *mailHdr, char* data)
{
    post->Receive(box,pktHdr,mailHdr,data);
    if (DebugIsEnabled('n')) printf("paquet %s reçu : envoie du ack\n",data);
    const char *ackmess = "ack";
    PacketHeader outPktHdr;
    MailHeader outMailHdr;
    outPktHdr.to = pktHdr->from;
    outPktHdr.from = GetAddr();
    outMailHdr.to = 1;
    outMailHdr.from = mailHdr->to;
    outMailHdr.length = strlen(ackmess) + 1;
    fflush(stdout);
    post->Send(outPktHdr,outMailHdr,ackmess);    
}

void FiablePostOffice::CheckAckArrive() {
    if (ack == false) {
        if (nbReemis < NBMAXREEMISSIONS) {
            if (DebugIsEnabled('n')) printf("reemission n° %d\n",nbReemis);
            ReSend();
        }
        else {
            ack = false;
            nbReemis = 0;
            DEBUG('n',"paquet Perdu\n");

            //prevenir le thread qui à appelé send que le paquet a été perdu
            PacketHeader outPktHdr;
            MailHeader outMailHdr;
            const char *data = "Mess perdu";

            outPktHdr.to = myAddr; // send mess to my machine		
            outMailHdr.to = 1; // in box 1
            outMailHdr.from = 0; // from box 0
            outMailHdr.length = strlen(data) + 1;
            post->Send(outPktHdr,outMailHdr,data);
        }
    } else {
        ack = false;
        nbReemis = 0;
    }
}

void FiablePostOffice::ReSend() {
    nbReemis++;
    post->Send(lpktHdr,lmailHdr,ldata);
    interrupt->Schedule(AckTimer, (int)this, TEMPO, TimerInt);
}

int FiablePostOffice::GetAddr() {
    return myAddr;
}