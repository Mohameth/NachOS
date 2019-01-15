#include "syscall.h"


int main()
{
    //ForkExec("userpages1");
    ForkExec("putchar");
    ForkExec("putchar");
    PutChar('L');
    PutChar('O');
    PutChar('L');
    PutChar('\n');
    Exit(0);    
}