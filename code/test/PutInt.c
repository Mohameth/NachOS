#include "syscall.h"

int main()
{
    int val = 12345;

    PutInt(val);
    PutChar('\n');

    PutInt(val*100000);
    PutChar('\n');

    return 7890;
}