#include "syscall.h"

int main()
{
    int val = GetInt();

    PutInt(val);
    PutChar('\n');

    //PutInt(val*100000);
    //PutChar('\n');

    Halt();
}