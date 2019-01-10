#include "syscall.h"

int
main ()
{
    PutString("Test d'ouverture\n");
    Create("f1.txt");
    Create("f2.txt");
    OpenFileId f1 = Open("f1.txt");
    OpenFileId f2 = Open("f2.txt");

    char buf[10] = "test"; 
    PutString("creation terminé, tentative de lecture ...\n");
    Write(buf, 4, f1);
    Write(buf, 4, f2);
    /*Read(buf, 1, f1); //Lecture d'un caractere
    Read(buf, 1, f2); //Lecture d'un caractere*/

    PutString("Lu : ");
    char lu = buf[0];
    PutChar(lu);

    return 0;
}