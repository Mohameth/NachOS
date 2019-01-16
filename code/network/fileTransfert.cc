#include "fileTransfert.h"
#include "system.h"



FileTransfert::FileTransfert(NetworkAddress addr, double reliability, int nBoxes)
{
    unlimitedPost = new UnlimitedPostOffice(addr,reliability,nBoxes);
}

FileTransfert::FileTransfert(UnlimitedPostOffice* p) {
    unlimitedPost = p;
}

//----------------------------------------------------------------------
// FiablePostOffice::~FiablePostOffice
// 	De-allocate the fiable post office data structures.
//----------------------------------------------------------------------

FileTransfert::~FileTransfert()
{
    delete unlimitedPost;
}


void FileTransfert::Serveur() {
    PacketHeader outPktHdr, inPktHdr;
    MailHeader outMailHdr, inMailHdr;


    // attente d'une commande.
    char cmd[4];
    unlimitedPostOffice->Receive(0,&inPktHdr,&inMailHdr,cmd);

    outPktHdr.from = unlimitedPost->GetAddr();
    outPktHdr.to = inPktHdr.from;
    outMailHdr.from = inMailHdr.to;
    outMailHdr.to = inMailHdr.from;

    if (DebugIsEnabled('n')) printf("commande reçu %s\n",cmd);

    if (!strcmp(cmd,"Get")) { // si cmd == Get
    
    //  reception du nom de fichier.
    char fileName[35];
    unlimitedPostOffice->Receive(0,&inPktHdr,&inMailHdr,fileName);
    if (DebugIsEnabled('n')) printf("nom de fichier reçu %s\n",fileName);
    
    // (void) inMailHdr;
    // (void) inPktHdr;
    // (void) outMailHdr;
    // (void) outPktHdr;
    
    //  ouverture du fichier.
    OpenFile* file;
    file = fileSystem->Open(fileName);

    //  envoi de la taille ou -1 si fichier inexistant
    if (file == NULL) {
        int errorSize = -1;
        if (DebugIsEnabled('n')) printf("%s not found\n",fileName);
        outMailHdr.length = sizeof(int);
        unlimitedPostOffice->Send(outPktHdr,outMailHdr,(char *) &errorSize);
        return;
    }
    int size = file->Length();
    outMailHdr.length = sizeof(int);
    DEBUG('n',"envoi de la taille\n");
    unlimitedPostOffice->Send(outPktHdr,outMailHdr,(char *) &size);

    //  lecture du fichier
    char donne[size];
    file->Read(donne,size);

    //  envoi du fichier
    DEBUG('n',"envoi du fichier\n");
    outPktHdr.from = unlimitedPost->GetAddr();
    outPktHdr.to = inPktHdr.from;
    outMailHdr.length = size;
    unlimitedPostOffice->Send(outPktHdr,outMailHdr,donne);

    delete file;

    } else if (!strcmp(cmd,"Put")) { // si cmd == Put
    
    //  reception du nom du fichier
     char fileName[35];
    unlimitedPostOffice->Receive(0,&inPktHdr,&inMailHdr,fileName);
    if (DebugIsEnabled('n')) printf("nom de fichier reçu %s\n",fileName);

    //  reception de la taille du fichier
     int taille;
    unlimitedPostOffice->Receive(0,&inPktHdr,&inMailHdr,(char *) &taille); //unlimited post inutilisable pour un int
    if (DebugIsEnabled('n')) printf("taille reçu %d\n",taille);
    if (taille == -1)
        return;

    //  creation du fichier sur le disque.
    OpenFile* file;
    fileSystem->Create(fileName,taille);
    file = fileSystem->Open(fileName);
    if (file == NULL)
        return;

    //  reception du fichier (via UnlimitedPost)
    char donnee[taille];
    unlimitedPostOffice->Receive(0,&inPktHdr,&inMailHdr,donnee);
    if (DebugIsEnabled('n')) printf("reçu %s\n",donnee);


    // ecriture dans le fichier
    file->Write(donnee,taille);

    }
    

    
}

int FileTransfert::ClientGet(int farAddr, const char* fileName) {
    PacketHeader outPktHdr,inPktHdr;
    MailHeader outMailHdr, inMailHdr;

    const char * cmd ="Get";
    outPktHdr.from = unlimitedPost->GetAddr();
    outPktHdr.to = farAddr;		
    outMailHdr.to = 0;
    outMailHdr.from = 1;
    outMailHdr.length = strlen(cmd) + 1;

    // envoi commande Get au serveur.
    if (DebugIsEnabled('n')) printf("envoi de Get\n");
    unlimitedPostOffice->Send(outPktHdr,outMailHdr,cmd);

    // envoi du nom de fichier au serveur.
    if (DebugIsEnabled('n')) printf("envoi de nom de ficjier\n");
    outMailHdr.length = strlen(fileName);
    unlimitedPostOffice->Send(outPktHdr,outMailHdr,fileName);

    // (void) inMailHdr;
    // (void) inPktHdr;

    // // reception de la taille du fichier
    int taille;
    unlimitedPostOffice->Receive(1,&inPktHdr,&inMailHdr,(char *) &taille); //unlimited post inutilisable pour un int
    if (DebugIsEnabled('n')) printf("taille reçu %d\n",taille);
    if (taille == -1)
        return -1;

    // creation du fichier sur le disque.
    OpenFile* file;
    fileSystem->Create(fileName,taille);
    file = fileSystem->Open(fileName);
    if (file == NULL)
        return -1;

    // reception du fichier (via UnlimitedPost)
    char donnee[taille];
    outPktHdr.from = unlimitedPost->GetAddr();
    outPktHdr.to = farAddr;	
    unlimitedPostOffice->Receive(1,&inPktHdr,&inMailHdr,donnee);
    if (DebugIsEnabled('n')) printf("reçu %s\n",donnee);


    // ecriture dans le fichier
    file->Write(donnee,taille);

    delete file;
    return 0;

}

int FileTransfert::ClientPut(int farAddr, const char* fileName) {
    PacketHeader outPktHdr;
    MailHeader outMailHdr;

    const char * cmd ="Put";
    outPktHdr.from = unlimitedPost->GetAddr();
    outPktHdr.to = farAddr;		
    outMailHdr.to = 0;
    outMailHdr.from = 1;
    outMailHdr.length = strlen(cmd) + 1;

    // envoi commande Put au serveur.
    DEBUG('n',"envoi de Put\n");
    unlimitedPostOffice->Send(outPktHdr,outMailHdr,cmd);

    // envoi du nom de fichier au serveur.
    DEBUG('n',"envoi de nom de ficjier\n");
    outMailHdr.length = strlen(fileName);
    unlimitedPostOffice->Send(outPktHdr,outMailHdr,fileName);

    // ouverture du fichier
    OpenFile* file;
    file = fileSystem->Open(fileName);

    //  envoi de la taille ou -1 si fichier inexistant
    if (file == NULL) {
        int errorSize = -1;
        if (DebugIsEnabled('n')) printf("%s not found\n",fileName);
        outMailHdr.length = sizeof(int);
        unlimitedPostOffice->Send(outPktHdr,outMailHdr,(char *) &errorSize);
        return -1;
    }
    int size = file->Length();
    outMailHdr.length = sizeof(int);
    DEBUG('n',"envoi de la taille\n");
    unlimitedPostOffice->Send(outPktHdr,outMailHdr,(char *) &size);


    //  lecture du fichier
    char donne[size];
    file->Read(donne,size);

    //  envoi du fichier
    DEBUG('n',"envoi du fichier\n");
    outMailHdr.length = size;
    unlimitedPostOffice->Send(outPktHdr,outMailHdr,donne);

    delete file;
    return 0;
    //return -1;

}

