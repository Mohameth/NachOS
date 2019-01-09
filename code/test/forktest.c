#include "syscall.h"


int main()
{
    ForkExec("putchar");
    // ForkExec("userpages1");
    PutChar('L');
    PutChar('\n');
    while(1);
}