#include "syscall.h"
#include "test_utility.h"

int main()
{
    int data;
    data = ReceiveInt();
    PutString("Valeur: ");
    PutInt(data);
    PutString("\n");
    Exit(0);
}