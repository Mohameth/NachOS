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
    int tid = UserThreadCreate(f,&arg);
    PutChar('m');
    PutChar('\n');
    PutInt(tid);
    UserThreadJoin(tid);
    return 0;
}