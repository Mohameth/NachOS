#include "syscall.h"
#include "test_utility.h"

int main()
{
    char data[14];
    Receive(data);
    PutString("Valeur: ");
    PutString(data);

    char longData[40*4];
    Receive(longData);
    PutString("Long text: ");
    PutString(longData);
    PutString(longData+50);
    PutString(longData+100);
    PutString(longData+150);

    Exit(0);
}