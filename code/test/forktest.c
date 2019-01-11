#include "syscall.h"


int main()
{
    // ForkExec("putchar");
    ForkExec("userpages0");
    PutChar('L');
    PutChar('O');
    PutChar('L');
    PutChar('\n');
    PutChar('L');
    PutChar('O');
    PutChar('L');
    PutChar('\n');
    PutChar('L');
    PutChar('O');
    PutChar('L');
    PutChar('\n');
    Exit(0);    
}