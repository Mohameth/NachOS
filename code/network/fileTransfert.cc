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
    postOffice->Receive(0,&inPktHdr,&inMailHdr,cmd);

    outPktHdr.from = unlimitedPost->GetAddr();
    outPktHdr.to = inPktHdr.from;
    outMailHdr.from = inMailHdr.to;
    outMailHdr.to = inMailHdr.from;

    printf("commande reçu %s\n",cmd);

    if (!strcmp(cmd,"Get")) { // si cmd == Get
    
    //  reception du nom de fichier.
    char fileName[35];
    postOffice->Receive(0,&inPktHdr,&inMailHdr,fileName);
    printf("nom de fichier reçu %s\n",fileName);
    
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
        printf("%s not found\n",fileName);
        outMailHdr.length = sizeof(int);
        postOffice->Send(outPktHdr,outMailHdr,(char *) &errorSize);
        return;
    }
    int size = file->Length();
    outMailHdr.length = sizeof(int);
    printf("envoi de la taille\n");
    postOffice->Send(outPktHdr,outMailHdr,(char *) &size);

    //  lecture du fichier
    char donne[size];
    file->Read(donne,size);

    //  envoi du fichier
    printf("envoi du fichier\n");
    outMailHdr.length = size;
    postOffice->Send(outPktHdr,outMailHdr,donne);

    delete file;

    } else if (!strcmp(cmd,"Put")) { // si cmd == Put
    
    //  reception du nom du fichier
    //  reception de la taille du fichier
    //  creation du fichier sur le disque.
    //  reception du fichier (via UnlimitedPost)
    //  ecriture dans le fichier

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
    printf("envoi de Get\n");
    postOffice->Send(outPktHdr,outMailHdr,cmd);

    // envoi du nom de fichier au serveur.
    printf("envoi de nom de ficjier\n");
    outMailHdr.length = strlen(fileName);
    postOffice->Send(outPktHdr,outMailHdr,fileName);

    // (void) inMailHdr;
    // (void) inPktHdr;

    // // reception de la taille du fichier
    int taille;
    postOffice->Receive(1,&inPktHdr,&inMailHdr,(char *) &taille); //unlimited post inutilisable pour un int
    printf("taille reçu %d\n",taille);
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
    postOffice->Receive(1,&inPktHdr,&inMailHdr,donnee);
    printf("reçu %s\n",donnee);


    // ecriture dans le fichier
    file->Write(donnee,taille);

    delete file;
    return 0;

}

int FileTransfert::ClientPut(int farAddr, const char* fileName) {


    // ouverture du fichier
    // envoi commande Put.
    // envoi du nom du fichier
    // envoi de la taille
    // envoi du fichier (via UnlimitedPost)
    return -1;

}

