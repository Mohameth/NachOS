#include "syscall.h"


int main()
{
    ForkExec("userp1");
    ForkExec("userp0");
    ForkExec("putchar");
    PutChar('L');
    PutChar('O');
    PutChar('L');
    PutChar('\n');
    Exit(0);    
}