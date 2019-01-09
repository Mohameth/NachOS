#ifndef FIABLEPOST_H
#define FIABLEPOST_H

#include "network.h"
#include "synchlist.h"
#include "post.h"

class FiablePostOffice {
  public:
    FiablePostOffice(NetworkAddress addr, double reliability, int nBoxes);
				// Allocate and initialize Post Office
				//   "reliability" is how many packets
				//   get dropped by the underlying network
    ~FiablePostOffice();		// De-allocate Post Office data
    
    void Send(PacketHeader pktHdr, MailHeader mailHdr, const char *data);
    				// Send a message to a mailbox on a remote 
				// machine.  The fromBox in the MailHeader is 
				// the return box for ack's.
    
    void SendAck(PacketHeader pktHdr, MailHeader mailHdr, const char *data);

    void Receive(int box, PacketHeader *pktHdr, 
		MailHeader *mailHdr, char *data);
    				// Retrieve a message from "box".  Wait if
				// there is no message in the box.

    void PostalDelivery();	// Wait for incoming messages, 
				// and then put them in the correct mailbox
    void CheckAckArrive();

    void ReSend();

  private:
    PostOffice * post;
    PacketHeader lpktHdr;
    MailHeader lmailHdr;
    const char* ldata;
    int nbReemis;
    bool ack;
    int myAddr;
};

#endif