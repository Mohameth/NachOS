#include "syscall.h"

int
main ()
{
    //PutString("Test d'ouverture\n");
    Create("f1.txt");
    int success = CreateRepository("rep1");
    if (!success) PutString("ERREUR");
    success = ChangeRepository("rep1");
    if (!success) PutString("ERREUR");
    
     

    return 0;
}