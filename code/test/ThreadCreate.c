#include "syscall.h"

void f(void * arg) {
    //PutChar('c');
    //PutChar('\n');
    int * v = (int *) arg;
    //int i = 32;
    PutInt(*v);
    //PutChar('\n');
    //PutInt(i);
    //PutChar('\n');
    UserThreadExit();
}

int main()
{
    int arg = 320;
    UserThreadCreate(f,&arg);
    PutChar('m');
    PutChar('\n');
    int i = 0;
    while(i < 100000) {
        i++;
    }
    return 0;
}