#include "syscall.h"


int main()
{
    char s[20];
    //char ss[2048];

    GetString(s,20);
    
    PutString(s);
    //PutChar('\n');

    //GetString(ss,2048);
    
    //PutString(ss);
    //PutChar('\n');

    Halt();
}