#include "syscall.h"

int main()
{
    PutString("azertyuiop\n");
    PutString("hello world\n");


    char* s = "hello world v2";
    PutString(s);
    PutChar('\n');


    PutString(s+2);
    PutChar('\n');
    return 4;
}