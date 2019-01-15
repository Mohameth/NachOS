#include "syscall.h"

int
main ()
{
    //PutString("Test d'ouverture\n");
    Create("f1.txt",10);
    Create("f2.txt",10);
    OpenFileId f1 = Open("f1.txt");
    OpenFileId f2 = Open("f2.txt");

    char buf[10] = "testlol";
    PutString("creation terminé, tentative de lecture ...\n");
    Write(buf, 6, f1);
    Write(buf, 4, f2);

    Seek(1, f1);
    Seek(0, f2);

    char bufReadF1[10]; char bufReadF2[10];
    Read(bufReadF1, 4, f1);
    Read(bufReadF2, 4, f2); 

    PutString("Lu pour f1 : "); PutString(bufReadF1); PutChar('\n');
    PutString("Lu pour f2 : "); PutString(bufReadF2); PutChar('\n');

    return 0;
}