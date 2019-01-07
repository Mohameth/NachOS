#include "syscall.h"


int main()
{
    ForkExec("userpages0");
    ForkExec("userpages1");
    PutChar('L');
    PutChar('\n');
    while(1);
    Halt();
}