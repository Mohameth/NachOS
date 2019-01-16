#include "syscall.h"
int main()
{
    PutString("Hello");
    PutString("World");
    char *s = "house";
    PutString(s);
    PutString(s + 3);
    Exit(0);
}
