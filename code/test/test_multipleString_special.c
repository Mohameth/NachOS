#include "syscall.h"
int main()
{
    PutString("ABC");
    PutString(" ");
    PutString("ABC\n");
    PutString("ABC\nDEF");
    PutString("ÿ");
    PutString("1");
    Exit(0);
}
