#ifndef FILETRANSFERT_H
#define FILETRANSFERT_H

#include "network.h"
#include "synchlist.h"
#include "unlimitedPost.h"

class FileTransfert {
  public:
    FileTransfert(NetworkAddress addr, double reliability, int nBoxes); // allocate a file transfert with creating an unlimited post office
    FileTransfert(UnlimitedPostOffice *p); // allocate a file transfert with an existing unlimited post office
    ~FileTransfert();		// De-allocate data
    
    void Serveur();  // The Serveur of the file transfer wait commande and respond to the client
    int ClientGet(int farAddr, const char* fileName); // the Get cmd of the client : 
                                                       // - Get demande un fichier présent sur le serveur distant
                                                       // Retour : 0 si le transfert à réussi -1 si il à echoué
    int ClientPut(int farAddr, const char* fileName); // The Put cmd of the client
                                                       // - Put envoie un fichier présent sur le client sur le serveur.
                                                       // Retour : 0 si le transfert à réussi -1 si il à echoué

  private:
    UnlimitedPostOffice * unlimitedPost;
    
};

#endif