#include "syscall.h"


int main()
{
    char s[50];
    //char ss[2048];

    GetString(s,50);
    
    PutString(s);
    //PutChar('\n');

    //GetString(ss,2048);
    
    //PutString(ss);
    //PutChar('\n');

    Halt();
}