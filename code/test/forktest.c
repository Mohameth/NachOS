#include "syscall.h"


int main()
{
    ForkExec("userpages1");
    ForkExec("userpages0");
    ForkExec("putchar");
    PutChar('L');
    PutChar('O');
    PutChar('L');
    PutChar('\n');
    Exit(0);    
}