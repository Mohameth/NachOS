#include "syscall.h"
int main()
{
    PutInt(0);
    PutInt(1);
    PutInt(41);
    PutInt(42);
    PutInt(007);
    PutInt(333);
    PutInt(-1);
    PutInt(1111*2);
    PutInt(47);

    int v = 77;
    PutInt(v);
    PutInt(v + 11);
}