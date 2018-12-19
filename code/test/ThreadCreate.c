#include "syscall.h"

void f(void * arg) {
    //PutChar('c');
    int * v = (int *) arg;
    PutInt(*v);
    while(1);
}

int main()
{
    int arg = 320;
    UserThreadCreate(f,&arg);
    //PutChar('m');
    while(1);
    return 4;
}