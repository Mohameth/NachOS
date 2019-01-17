#include "syscall.h"


int main()
{
    ForkExec("upages1");
    ForkExec("upages0");
    ForkExec("putchar");
    PutChar('L');
    PutChar('O');
    PutChar('L');
    PutChar('\n');
    Exit(0);    
}