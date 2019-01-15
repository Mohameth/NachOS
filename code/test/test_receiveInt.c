#include "syscall.h"
#include "test_utility.h"

int main()
{
    char data[64];
    Receive(data);
    PutString("Valeur: ");
    int val = (int)*data;
    PutInt(val);
    PutString("\n");
    Exit(0);
}