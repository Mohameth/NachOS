#include "syscall.h"

void f(void * arg) {
    int * v = (int *) arg;
    PutInt(*v);
    PutChar('\n');
    UserThreadExit();
}

int main()
{
    int arg =320;
    int arg2=420;
    int tid = UserThreadCreate(f,&arg);
    tid = UserThreadCreate(f,&arg2);
    PutChar('\n');

    for(int i=0;i<=tid;i++){
        UserThreadJoin(i);
    }  
    return 0;
}