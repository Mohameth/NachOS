#include "syscall.h"


int main()
{
    char s[10];
    char ss[5];

    GetString(s,10);
    
    PutString(s);
    PutChar('\n');

     GetString(ss,5);
    
    PutString(ss);
    PutChar('\n');

    Halt();
}